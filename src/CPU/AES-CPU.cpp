#include "../../include/CPU/AES-CPU.hpp"

// Global Variables
byte_arr_t aes_key;
vector<byte_arr_t> aes_sub_keys(SUB_KEYS);
StopWatchInterface *aes_cpu_timer = nullptr;
float aes_cpu_encryption_time = 0.0, aes_cpu_decryption_time = 0.0;

// Function Definitions
void aes_cpu_key_schedule(const byte_arr_t &key)
{
    // Code
    for (int i = 0; i < SUB_KEYS; i++)
    {
        if (i == 0)
            aes_sub_keys[i] = key;
        else
            aes_sub_keys[i] = aes_cpu_sub_key(aes_sub_keys[i - 1], i - 1);
    }
}

byte_arr_t aes_cpu_sub_key(byte_arr_t &prev_sub_key, const int rounds)
{
    // Variable Declarations
    byte_arr_t result(AES_LENGTH);

    // Code
    result[0] = (prev_sub_key[0] ^ (sbox[prev_sub_key[13]] ^ round_constant[rounds]));
    result[1] = (prev_sub_key[1] ^ sbox[prev_sub_key[14]]);
    result[2] = (prev_sub_key[2] ^ sbox[prev_sub_key[15]]);
    result[3] = (prev_sub_key[3] ^ sbox[prev_sub_key[12]]);

    for (int i = 4; i < AES_LENGTH; i += 4)
    {
        result[i + 0] = (result[i - 4] ^ prev_sub_key[i + 0]);
        result[i + 1] = (result[i - 3] ^ prev_sub_key[i + 1]);
        result[i + 2] = (result[i - 2] ^ prev_sub_key[i + 2]);
        result[i + 3] = (result[i - 1] ^ prev_sub_key[i + 3]);
    }

    return result;
}

void aes_cpu_byte_sub(byte_arr_t &data)
{
    // Code
    for (int i = 0; i < AES_LENGTH; i++)
        data[i] = sbox[data[i]];
}

void aes_cpu_byte_sub_inverse(byte_arr_t &data)
{
    // Code
    for (int i = 0; i < AES_LENGTH; i++)
        data[i] = sbox_inverse[data[i]];
}

void aes_cpu_shift_rows(byte_arr_t &data)
{
    // Code
    unsigned char aux_array[4];

    for (int i = 1; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
            aux_array[j] = data[i + 4 * j];

        for (int j = 0; j < 4; j++)
            data[i + 4 * j] = aux_array[(j + i) % 4];
    }
}

void aes_cpu_shift_rows_inverse(byte_arr_t &data)
{
    // Code
    unsigned char aux_array[4];

    for (int i = 1; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
            aux_array[j] = data[i + 4 * j];

        for (int j = 0; j < 4; j++)
            data[i + 4 * j] = aux_array[(j + 4 - i) % 4];
    }
}

void aes_cpu_mix_columns(byte_arr_t &data)
{
    // Code
    for (int i = 0; i < AES_LENGTH; i += 4)
    {
        unsigned char b0 = data[i + 0];
        unsigned char b1 = data[i + 1];
        unsigned char b2 = data[i + 2];
        unsigned char b3 = data[i + 3];

        data[i + 0] = mul[b0][0] ^ mul[b1][1] ^ b2 ^ b3;
        data[i + 1] = b0 ^ mul[b1][0] ^ mul[b2][1] ^ b3;
        data[i + 2] = b0 ^ b1 ^ mul[b2][0] ^ mul[b3][1];
        data[i + 3] = mul[b0][1] ^ b1 ^ b2 ^ mul[b3][0];
    }
}

void aes_cpu_mix_columns_inverse(byte_arr_t &data)
{
    // Code
    for (int i = 0; i < AES_LENGTH; i += 4)
    {
        unsigned char c0 = data[i + 0];
        unsigned char c1 = data[i + 1];
        unsigned char c2 = data[i + 2];
        unsigned char c3 = data[i + 3];

        data[i + 0] = mul[c0][5] ^ mul[c1][3] ^ mul[c2][4] ^ mul[c3][2];
        data[i + 1] = mul[c0][2] ^ mul[c1][5] ^ mul[c2][3] ^ mul[c3][4];
        data[i + 2] = mul[c0][4] ^ mul[c1][2] ^ mul[c2][5] ^ mul[c3][3];
        data[i + 3] = mul[c0][3] ^ mul[c1][4] ^ mul[c2][2] ^ mul[c3][5];
    }
}

void aes_cpu_add_round_key(byte_arr_t &data, const int round_key)
{
    // Code
    XOR(data, aes_sub_keys[round_key], AES_LENGTH);
}

byte_arr_t aes_cpu_encrypt_data(const byte_arr_t &data)
{
    // Variable Declarations
    int round = 0;
    byte_arr_t enc_data = data;

    // Code

    // Key Addition Prior to Round 1 (R0)
    aes_cpu_add_round_key(enc_data, round);
    round = 1;

    // Round 1 (R1) to AES_ROUNDS - 1 (R9)
    for (round = 1; round < AES_ROUNDS; round++)
    {
        aes_cpu_byte_sub(enc_data);
        aes_cpu_shift_rows(enc_data);
        aes_cpu_mix_columns(enc_data);
        aes_cpu_add_round_key(enc_data, round);
    }

    // Final Round Without Column Mixing
    round = AES_ROUNDS;
    aes_cpu_byte_sub(enc_data);
    aes_cpu_shift_rows(enc_data);
    aes_cpu_add_round_key(enc_data, round);

    return enc_data;
}

byte_arr_t aes_cpu_decrypt_data(const byte_arr_t &data)
{
    // Variable Declrations
    int round = AES_ROUNDS;
    byte_arr_t dec_data = data;

    // Code

    // Key Addition before Round
    aes_cpu_add_round_key(dec_data, round);
    aes_cpu_shift_rows_inverse(dec_data);
    aes_cpu_byte_sub_inverse(dec_data);
    aes_cpu_byte_sub_inverse(dec_data);
    round = AES_ROUNDS - 1;

    // Round AES_ROUNDS - 1 (R9) to 1 (R1)
    for (round = 9; round > 0; round--)
    {
        aes_cpu_add_round_key(dec_data, round);
        aes_cpu_mix_columns_inverse(dec_data);
        aes_cpu_shift_rows_inverse(dec_data);
        aes_cpu_byte_sub_inverse(dec_data);
    }

    // Final Round without Column Mixing (R0)
    round = 0;
    aes_cpu_add_round_key(dec_data, round);

    return dec_data;
}

const vector<byte_arr_t> aes_cpu_cipher(
    const vector<byte_arr_t> &data,
    const byte_arr_t &key,
    const byte_arr_t &iv)
{
    // Code
    aes_cpu_key_schedule(key);

    vector<byte_arr_t> ciphertext(data.size(), vector<unsigned char>(AES_LENGTH, 0x00));
    vector<byte_arr_t> counters(data.size(), vector<unsigned char>(AES_LENGTH, 0x00));

    generate_counters(counters, iv);

    sdkCreateTimer(&aes_cpu_timer);
    sdkStartTimer(&aes_cpu_timer);
    for (int i = 0; i < AES_ROUNDS; i++)
    {
        for (size_t j = 0; j < data.size(); j++)
            ciphertext[j] = XOR(aes_cpu_encrypt_data(counters[j]), data[j]);
    }
    sdkStopTimer(&aes_cpu_timer);
    aes_cpu_encryption_time = sdkGetTimerValue(&aes_cpu_timer);
    if (aes_cpu_timer)
    {   
        sdkDeleteTimer(&aes_cpu_timer);
        aes_cpu_timer = nullptr;
    }

    cout << endl << "Time For Encryption : " << aes_cpu_encryption_time / 1000.0f << " seconds" << endl;

    return ciphertext;
}

const vector<byte_arr_t> aes_cpu_decipher(
    const vector<byte_arr_t> &data,
    const byte_arr_t &key,
    const byte_arr_t &iv)
{
    // Code
    aes_cpu_key_schedule(key);

    vector<byte_arr_t> plaintext(data.size(), vector<unsigned char>(AES_LENGTH, 0x00));
    vector<byte_arr_t> counters(data.size(), vector<unsigned char>(AES_LENGTH, 0x00));

    generate_counters(counters, iv);

    sdkCreateTimer(&aes_cpu_timer);
    sdkStartTimer(&aes_cpu_timer);
    for (int i = 0; i < AES_ROUNDS; i++)
    {
        for (size_t j = 0; j < data.size(); j++)
            plaintext[j] = XOR(aes_cpu_encrypt_data(counters[j]), data[j]);
    }
    sdkStopTimer(&aes_cpu_timer);
    aes_cpu_encryption_time = sdkGetTimerValue(&aes_cpu_timer);
    if (aes_cpu_timer)
    {   
        sdkDeleteTimer(&aes_cpu_timer);
        aes_cpu_timer = nullptr;
    }

    cout << endl << "Time For Decryption : " << aes_cpu_decryption_time << endl;

    return plaintext;
}
