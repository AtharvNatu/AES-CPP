#include "../../include/CPU/AES-CPU.hpp"
#include "../../include/Common/Tables.hpp"

// Global Variables
state_t *state;
const byte_t *aes_round_key;

StopWatchInterface *aes_cpu_timer = nullptr;
float aes_cpu_encryption_time = 0.0, aes_cpu_decryption_time = 0.0;

// Function Definitions
byte_t xtime(byte_t x)
{
	return ((x << 1) ^ (((x >> 7) & 1) * 0x1b));
}

byte_t multiply(byte_t x, byte_t y)
{
    return (((y & 1) * x) ^
		((y >> 1 & 1) * xtime(x)) ^
		((y >> 2 & 1) * xtime(xtime(x))) ^
		((y >> 3 & 1) * xtime(xtime(xtime(x)))) ^
		((y >> 4 & 1) * xtime(xtime(xtime(xtime(x))))));
}

void aes_cpu_expand_key(byte_t *round_key, byte_t *key)
{
    // Variable Declrations
    unsigned char aux[4], k;
    size_t i, j;

    // Code

    // 1st round is the key itself
    for (i = 0; i < AES_COLS; i++)
    {
        round_key[(i * 4) + 0] = key[(i * 4) + 0];
        round_key[(i * 4) + 1] = key[(i * 4) + 1];
        round_key[(i * 4) + 2] = key[(i * 4) + 2];
        round_key[(i * 4) + 3] = key[(i * 4) + 3];
    }

    // All other round keys are derived from previous round keys
    while (i < (AES_COLS * (AES_ROUNDS + 1)))
    {
        for (j = 0; j < 4; j++)
            aux[j] = round_key[(i - 1) * AES_COLS + j];
        
        if (i % AES_COLS == 0)
        {
            // Rotate Word
            k = aux[0];
            aux[0] = aux[1];
            aux[1] = aux[2];
            aux[2] = aux[3];
            aux[3] = k;

            // Substitute
            aux[0] = sbox[aux[0]];
            aux[1] = sbox[aux[1]];
            aux[2] = sbox[aux[2]];
            aux[3] = sbox[aux[3]];

            aux[0] = aux[0] ^ round_constants[i / AES_COLS];
        }

        round_key[i * 4 + 0] = round_key[(i - AES_COLS) * 4 + 0] ^ aux[0];
        round_key[i * 4 + 1] = round_key[(i - AES_COLS) * 4 + 1] ^ aux[1];
        round_key[i * 4 + 2] = round_key[(i - AES_COLS) * 4 + 2] ^ aux[2];
        round_key[i * 4 + 3] = round_key[(i - AES_COLS) * 4 + 3] ^ aux[3];

        i++;
    }
}

void aes_cpu_add_round_key(byte_t round)
{
	// Code
    for (byte_t i = 0; i < 4; i++)
    {
        for (byte_t j = 0; j < 4; j++)
            (*state)[i][j] ^= aes_round_key[round * AES_COLS * 4 + i * AES_COLS + j];
    }
}

void aes_cpu_byte_sub(void)
{
    // Code
    for (byte_t i = 0; i < 4; i++)
    {
        for (byte_t j = 0; j < 4; j++)
            (*state)[j][i] = sbox[(*state)[j][i]];
    }
}

void aes_cpu_byte_sub_inverse(void)
{
    // Code
    for (byte_t i = 0; i < 4; i++)
    {
        for (byte_t j = 0; j < 4; j++)
            (*state)[j][i] = sbox_inverse[(*state)[j][i]];
    }
}

void aes_cpu_shift_rows(void)
{
    // Code
    byte_t temp;

    temp           = (*state)[0][1];
	(*state)[0][1] = (*state)[1][1];
	(*state)[1][1] = (*state)[2][1];
	(*state)[2][1] = (*state)[3][1];
	(*state)[3][1] = temp;

	temp           = (*state)[0][2];
	(*state)[0][2] = (*state)[2][2];
	(*state)[2][2] = temp;

	temp           = (*state)[1][2];
	(*state)[1][2] = (*state)[3][2];
	(*state)[3][2] = temp;

	temp           = (*state)[0][3];
	(*state)[0][3] = (*state)[3][3];
	(*state)[3][3] = (*state)[2][3];
	(*state)[2][3] = (*state)[1][3];
	(*state)[1][3] = temp;
}

void aes_cpu_shift_rows_inverse(void)
{
    // Code
    byte_t temp;

    // Inverse row shift operation
    temp           = (*state)[3][1];
    (*state)[3][1] = (*state)[2][1];
    (*state)[2][1] = (*state)[1][1];
    (*state)[1][1] = (*state)[0][1];
    (*state)[0][1] = temp;

    temp           = (*state)[0][2];
    (*state)[0][2] = (*state)[2][2];
    (*state)[2][2] = temp;

    temp           = (*state)[1][2];
    (*state)[1][2] = (*state)[3][2];
    (*state)[3][2] = temp;

    temp           = (*state)[0][3];
    (*state)[0][3] = (*state)[1][3];
    (*state)[1][3] = (*state)[2][3];
    (*state)[2][3] = (*state)[3][3];
    (*state)[3][3] = temp;
}

void aes_cpu_mix_columns(void)
{
    // Variable Declarations
    unsigned char tmp, tm, t;

    // Code
    for (int i = 0; i < 4; i++)
    {
        t = (*state)[i][0];
		tmp = (*state)[i][0] ^ (*state)[i][1] ^ (*state)[i][2] ^ (*state)[i][3];
		
        tm = (*state)[i][0] ^ (*state)[i][1]; 
        tm = xtime(tm);  
        (*state)[i][0] ^= tm ^ tmp;

		tm = (*state)[i][1] ^ (*state)[i][2]; 
        tm = xtime(tm);  
        (*state)[i][1] ^= tm ^ tmp;

		tm = (*state)[i][2] ^ (*state)[i][3]; 
        tm = xtime(tm);  
        (*state)[i][2] ^= tm ^ tmp;

		tm = (*state)[i][3] ^ t;                
        tm = xtime(tm);  
        (*state)[i][3] ^= tm ^ tmp;
    }
}

void aes_cpu_mix_columns_inverse(void)
{
    // Variable Declarations
    int i;
	unsigned char a, b, c, d;

    // Code
	for (i = 0; i < 4; i++) 
    {
		a = (*state)[i][0];
		b = (*state)[i][1];
		c = (*state)[i][2];
		d = (*state)[i][3];

		(*state)[i][0] = multiply(a, 0x0e) ^ multiply(b, 0x0b) ^ multiply(c, 0x0d) ^ multiply(d, 0x09);
		(*state)[i][1] = multiply(a, 0x09) ^ multiply(b, 0x0e) ^ multiply(c, 0x0b) ^ multiply(d, 0x0d);
		(*state)[i][2] = multiply(a, 0x0d) ^ multiply(b, 0x09) ^ multiply(c, 0x0e) ^ multiply(d, 0x0b);
		(*state)[i][3] = multiply(a, 0x0b) ^ multiply(b, 0x0d) ^ multiply(c, 0x09) ^ multiply(d, 0x0e);
	}
}

string cipher(int offset, char input_data[4][4], int file_size)
{
	// Variable Declarations
	int round = 0;
    char *output_data = NULL;
    string ret;

    int mem_size = file_size;

    output_data = (char*)malloc(mem_size * sizeof(char));
    if (output_data == NULL)
    {
        cerr << endl << "Failed to allocate memory to output_data !!!" << endl;
        exit(AES_FAILURE);
    }

	// Copy plaintext to (*state) array
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
        {
            (*state)[i][j] = input_data[i][j];
        }
	}

	// Key Addition Prior to Round 1 (R0)
	aes_cpu_add_round_key(0);
	// round = 1;

	// Round 1 (R1) to AES_ROUNDS - 1 (R9)
    for (round = 1; round < AES_ROUNDS; round++)
    {
        aes_cpu_byte_sub();
        aes_cpu_shift_rows();
        aes_cpu_mix_columns();
        aes_cpu_add_round_key(round);
    }

	// Final Round Without Column Mixing
	// round = AES_ROUNDS;
	aes_cpu_byte_sub();
	aes_cpu_shift_rows();
	aes_cpu_add_round_key(AES_ROUNDS);

	// Copy (*state) array to output array
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
        {
            output_data[offset + i * 4 + j] = (*state)[i][j];
        }
	}

    for (int i = 0; i < mem_size; i++)
        ret.push_back(output_data[i]);

    write_file("Novel.txt.enc", ret);

    free(output_data);
    output_data = NULL;

    return ret;
}

static void copy_block(unsigned char *input, unsigned char* output)
{
    // Code
    for (unsigned char i = 0; i < AES_LENGTH; i++)
        output[i] = input[i];
}

void cipher(void)
{
    // Variable Declarations
    byte_t round = 0;

    // Code
    aes_cpu_add_round_key(0);

	// Round 1 (R1) to AES_ROUNDS - 1 (R9)
    for (round = 1; round < AES_ROUNDS; round++)
    {
        aes_cpu_byte_sub();
        aes_cpu_shift_rows();
        aes_cpu_mix_columns();
        aes_cpu_add_round_key(round);
    }

	// Final Round Without Column Mixing
	aes_cpu_byte_sub();
	aes_cpu_shift_rows();
	aes_cpu_add_round_key(AES_ROUNDS);
}

void decipher(void)
{
    // Code
    // Variable Declarations
    byte_t round = 0;

    // Code
    aes_cpu_add_round_key(AES_ROUNDS);

	// Round AES_ROUNDS - 1 (R9) to Round 0
    for (round = AES_ROUNDS - 1; round > 0; round--)
    {
        aes_cpu_shift_rows_inverse();
        aes_cpu_byte_sub_inverse();
        aes_cpu_add_round_key(round);
        aes_cpu_mix_columns_inverse(); 
    }

	// Final Round Without Column Mixing
	aes_cpu_shift_rows_inverse();
    aes_cpu_byte_sub_inverse();
    aes_cpu_add_round_key(0);
}

void aes_cpu_encrypt(byte_t* input, const byte_t* round_key, byte_t* output)
{
    // Code
    copy_block(input, output);
    state = (state_t*)output;
    aes_round_key = round_key;

    cipher();
}

void aes_cpu_decrypt(byte_t* input, const byte_t* round_key, byte_t* output)
{
    // Code
    copy_block(input, output);
    state = (state_t*)output;
    aes_round_key = round_key;

    decipher();
}


