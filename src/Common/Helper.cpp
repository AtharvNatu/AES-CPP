#include "../../include/Common/Helper.hpp"

// Function Definitions
string get_hash(string input)
{
    // Variable Declarations
    SHA256_CONTEXT sha256_context;
    unsigned char *hash = NULL;
    string str_hash;

    // Code
    hash = (unsigned char *)malloc(AES_LENGTH * sizeof(unsigned char));

    sha256_init(&sha256_context);
    sha256_write(&sha256_context, (unsigned char *)input.c_str(), input.length());
    sha256_final(&sha256_context);

    memcpy(hash, sha256_read(&sha256_context), AES_LENGTH);

    cout << endl << sha256_read(&sha256_context) << endl;

    str_hash = reinterpret_cast<char*>(hash);

    free(hash);

    return str_hash;
}

string get_key(string input_file)
{
    // Variable Declarations
    fstream data_file;
    string key_string;

    // Code
    data_file.open(input_file, ios::in);
    if (data_file.is_open())
    {
        getline(data_file, key_string);
        data_file.close();
    }

    return key_string;
}

// byte_arr_t set_key(string str_key)
// {
//     // Variable Declarations
//     byte_arr_t aes_key;

//     // Code
//     if (str_key.length() <= AES_LENGTH)
//     {
//         int padding_length = AES_LENGTH - str_key.length();

//         for (int i = 0; i < padding_length; i++)
//             str_key.insert(str_key.length(), "0");

//         for (int i = 0; i < AES_LENGTH; i++)
//             aes_key.push_back(str_key.at(i));
//     }
//     else
//     {
//         cerr << endl << "Key Length Should Not Be Greater Than 16 ... Exiting !!!" << endl;
//         exit(AES_FAILURE);
//     }
    
//     // memcpy(aes_key, str_key.c_str(), AES_LENGTH);

//     // *expanded_key_length = expand_key(aes_key);

//     return aes_key;

// }

// status_t verify_key(string encryption_key, unsigned char* decryption_key)
// {
//     // Code
//     if (encryption_key != to_string(get_hash(reinterpret_cast<char*>(decryption_key))))
//     {
//         if (DEBUG)
//             cerr << endl << "Invalid Key Entered ... Exiting !!!" << endl;
//         return AES_FAILURE;
//     }
//     else
//         cout << endl << "Valid Key ..." << endl;

//     return AES_SUCCESS;
// }


// void read_file(const char *input_file, char **output)
string read_file(string input_file)
{
    // Code
    ifstream input_file_stream;
    string output;
    char ch;

    input_file_stream.open(input_file, ifstream::binary);
    if (!input_file_stream)
    {
        if (DEBUG)
            cerr << endl << "Failed To Open Input File !!!" << endl;
        exit(AES_FAILURE);
    }

    while (!input_file_stream.eof())
    {
        input_file_stream.get(ch);
        output.push_back(ch);
    }

    input_file_stream.close();

    return output;
}

int read_file(const char *input_file, char *output_buffer)
{
    // Variable Declarations
    char ch;
    int offset = 0;

    // Code
    if (input_file == NULL)
    {
        cerr << endl << "Error :  Failed To Read Input File " << input_file << " ... Exiting !!!" << endl;
        exit(AES_FAILURE);
    }

    FILE *fp = NULL;

    #if (OS == 1)
        fopen_s(&fp, input_file, "rb");
    #else
        fp = fopen(input_file, "rb");
    #endif

    if (fp == NULL)
    {
        cerr << endl << "Error :  Failed To Read Input File " << input_file << " ... Exiting !!!" << endl;
        exit(AES_FAILURE);
    }

    while ((ch = fgetc(fp)) != EOF)
        output_buffer[offset++] = ch;
    *(output_buffer + offset) = '\0';

    fclose(fp);
    fp = NULL;

    return offset;
}

uintmax_t read_file(const char *input_file, byte_t *output_buffer, uintmax_t input_data_size)
{
    // Code
    if (input_file == NULL)
    {
        cerr << endl << "Error :  Failed To Read Input File " << input_file << " ... Exiting !!!" << endl;
        exit(AES_FAILURE);
    }

    FILE *fp = NULL;

    #if (OS == 1)
        fopen_s(&fp, input_file, "rb");
    #else
        fp = fopen(input_file, "rb");
    #endif

    if (fp == NULL)
    {
        cerr << endl << "Error :  Failed To Read Input File " << input_file << " ... Exiting !!!" << endl;
        exit(AES_FAILURE);
    }

    uintmax_t bytes_read = fread(output_buffer, sizeof(byte_t), input_data_size, fp);

    fclose(fp);
    fp = NULL;

    return bytes_read;
}

size_t write_file(const char *output_file, char *data, int output_data_size)
{
    // Code
    if (output_file == NULL)
    {
        cerr << endl << "Error :  Failed To Read Output File " << output_file << " ... Exiting !!!" << endl;
        exit(AES_FAILURE);
    }

    FILE *fp = NULL;

    #if (OS == 1)
        fopen_s(&fp, output_file, "wb+");
    #else
        fp = fopen(output_file, "wb+");
    #endif

    if (fp == NULL)
    {
        cerr << endl << "Error :  Failed To Read Output File " << output_file << " ... Exiting !!!" << endl;
        exit(AES_FAILURE);
    }

    size_t data_size = fwrite(data, sizeof(char), output_data_size, fp);

    fclose(fp);
    fp = NULL;

    return data_size;
}

size_t write_file(const char *output_file, byte_t *data, uintmax_t output_data_size)
{
    // Code
    if (output_file == NULL)
    {
        cerr << endl << "Error :  Failed To Read Output File " << output_file << " ... Exiting !!!" << endl;
        exit(AES_FAILURE);
    }

    FILE *fp = NULL;

    #if (OS == 1)
        fopen_s(&fp, output_file, "wb+");
    #else
        fp = fopen(output_file, "wb+");
    #endif

    if (fp == NULL)
    {
        cerr << endl << "Error :  Failed To Read Output File " << output_file << " ... Exiting !!!" << endl;
        exit(AES_FAILURE);
    }

    size_t data_size = fwrite(data, sizeof(byte_t), output_data_size, fp);

    fclose(fp);
    fp = NULL;

    return data_size;
}

void write_file(string output_file, string input)
{
    // Variable Declarations
    ofstream data_file;

    // Code
    data_file.open(output_file, ios::ate);
    if (data_file.is_open())
    {
        for (size_t i = 0; i < input.length(); i++)
            data_file << input.at(i);
        data_file.close();
    }
}

void write_file(string output_file, vector<string> input)
{
    // Variable Declarations
    ofstream data_file;

    // Code
    data_file.open(output_file, ios::ate);
    if (data_file.is_open())
    {
        for (size_t i = 0; i < input.size(); i++)
            data_file << input[i].data();
        data_file.close();
    }
}



