#include "../../include/Common/Helper.hpp"

// Function Definitions
byte_t *hash_password(const char *password)
{
    // Code
    byte_t *hash = (byte_t*)malloc(AES_LENGTH * sizeof(byte_t));

    SHA256_CONTEXT context;
    sha256_init(&context);
    sha256_write(&context, (byte_t *)password, strlen(password));
    sha256_final(&context);

    memcpy(hash, sha256_read(&context), AES_LENGTH);

    return hash;
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

status_t read_crypt_header(const char *input_file)
{
    // Variable Declarations
    fstream data_file;
    string file_signature;

    // Code
    data_file.open(input_file, ios::in);
    if (data_file.is_open())
    {
        getline(data_file, file_signature);
        data_file.close();
    }

    if (file_signature == AES_FILE)
        return AES_SUCCESS;
    else
        return AES_FAILURE;
}

void write_crypt_header(const char *input_file)
{
    // Variable Declarations
    fstream data_file;
    string file_signature;

    // Code
    data_file.open(input_file, ios::ate);
    if (data_file.is_open())
    {
        data_file << AES_FILE;
        data_file << endl;
        data_file.close();
    }
}

