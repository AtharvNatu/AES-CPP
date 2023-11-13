#include "../include/CPU/AES-CPU.hpp"
#include <vector>

byte_t *hash_password(const char *password, size_t size)
{
    byte_t *hash = (byte_t*)malloc(size * sizeof(byte_t));

    SHA256_CONTEXT context;
    sha256_init(&context);
    sha256_write(&context, (byte_t *)password, strlen(password));
    sha256_final(&context);

    memcpy(hash, sha256_read(&context), size);

    return hash;
}

int main(int argc, char **argv)
{
    int choice = -1;

    // Code
    while (true)
    {
        cout << endl << "AES 128-bit Encryption and Decryption" << endl;
        cout << "--------------------------------------------------------------------" << endl;
        cout << "1. Encrypt File" << endl << "2. Decrypt File" << endl << "3. Exit" << endl << "\nEnter your choice : ";
        cin >> choice;

        switch(choice)
        {
            case 1:
            {
                // Variable Declarations
                string user_key, file_key;
                string input_file, output_file, output_file_name;
                string file_data;
                byte_t *plaintext = NULL;
                byte_t ciphertext_block[AES_BLOCK_SIZE];
                byte_t *ciphertext = NULL;
                byte_t round_key[176];

                // Reading input and output file paths
                input_file = argv[1];
                output_file_name = filesystem::path(input_file).filename();
                output_file = output_file_name + ".enc";

                // Allocate memory to input buffer
                int file_length = filesystem::file_size(input_file);
                plaintext = (byte_t *)malloc(sizeof(byte_t) * file_length);
                if (plaintext == NULL)
                {
                    cerr << endl << "Error :  Failed To Allocate Memory To Input File Buffer ... Exiting !!!" << endl;
                    exit(AES_FAILURE);
                }

                uintmax_t bytes_read = read_file(input_file.c_str(), plaintext, file_length);
                cout << endl << bytes_read << " Bytes Read From File " << input_file << endl;

                uintmax_t ciphertext_blocks = (bytes_read + AES_BLOCK_SIZE - 1) / AES_BLOCK_SIZE;
                ciphertext = (byte_t*)malloc(ciphertext_blocks * AES_BLOCK_SIZE);
                if (ciphertext == NULL)
                {
                    cerr << endl << "Error :  Failed To Allocate Memory To Output File Buffer ciphertext ... Exiting !!!" << endl;
                    exit(AES_FAILURE);
                }
            
                cout << endl << "Enter Encryption Key = ";
                cin >> user_key;

                unsigned char *key = hash_password(user_key.c_str(), AES_LENGTH);

                cout << endl << "Key (" << user_key.length() << ") ";
                for (size_t i = 0; i < user_key.length(); ++i)
                    printf("%X ", (unsigned char)user_key[i]);
                printf("\n");

                cout << endl << "Key Hash (" << user_key.length() << ") ";
                for (size_t i = 0; i < AES_LENGTH; ++i)
                    printf("%X ", key[i]);
                printf("\n");

                aes_cpu_expand_key(round_key, key);

                StopWatchInterface *timer = NULL;
                sdkCreateTimer(&timer);
                sdkStartTimer(&timer);
                for (uintmax_t i = 0; i < ciphertext_blocks; i++)
                {
                    aes_cpu_encrypt(plaintext + i * AES_BLOCK_SIZE, round_key, ciphertext_block);

                    memcpy(ciphertext + i * AES_BLOCK_SIZE, ciphertext_block, sizeof(byte_t) * AES_BLOCK_SIZE);
                }
                sdkStopTimer(&timer);
                cout << endl << "Time taken to Encrypt : " << sdkGetTimerValue(&timer) / 1000.0f << " seconds" << endl;

                size_t bytes_written = write_file(argv[2], ciphertext, AES_BLOCK_SIZE * ciphertext_blocks);
                cout << endl << bytes_written << " Bytes Written To File " << argv[2] << endl;

                sdkDeleteTimer(&timer);
                timer = NULL;

                free(plaintext);
                plaintext = NULL;

                free(key);
                key = NULL;

            }
            break;

            case 2:
            {
                // Variable Declarations
                string user_key, file_key;
                string input_file, output_file, output_file_name;
                string file_data;
                byte_t *plaintext = NULL;
                byte_t plaintext_block[AES_BLOCK_SIZE];
                byte_t *ciphertext = NULL;
                byte_t round_key[176];

                // Reading input and output file paths
                input_file = argv[2];
                // output_file_name = filesystem::path(input_file).filename();
                // output_file = output_file_name + ".enc";

                // Allocate memory to input buffer
                int file_length = filesystem::file_size(input_file);
                ciphertext = (byte_t *)malloc(sizeof(byte_t) * file_length);
                if (ciphertext == NULL)
                {
                    cerr << endl << "Error :  Failed To Allocate Memory To Input File Buffer ... Exiting !!!" << endl;
                    exit(AES_FAILURE);
                }

                uintmax_t bytes_read = read_file(input_file.c_str(), ciphertext, file_length);
                cout << endl << bytes_read << " Bytes Read From File " << input_file << endl;

                uintmax_t ciphertext_blocks = (bytes_read + AES_BLOCK_SIZE - 1) / AES_BLOCK_SIZE;
                plaintext = (byte_t*)malloc(ciphertext_blocks * AES_BLOCK_SIZE);
                if (plaintext == NULL)
                {
                    cerr << endl << "Error :  Failed To Allocate Memory To Output File Buffer plaintext ... Exiting !!!" << endl;
                    exit(AES_FAILURE);
                }
            
                cout << endl << "Enter Encryption Key = ";
                cin >> user_key;

                unsigned char *key = hash_password(user_key.c_str(), AES_LENGTH);

                cout << endl << "Key (" << user_key.length() << ") ";
                for (size_t i = 0; i < user_key.length(); ++i)
                    printf("%X ", (unsigned char)user_key[i]);
                printf("\n");

                cout << endl << "Key Hash (" << user_key.length() << ") ";
                for (size_t i = 0; i < AES_LENGTH; ++i)
                    printf("%X ", key[i]);
                printf("\n");

                aes_cpu_expand_key(round_key, key);

                StopWatchInterface *timer = NULL;
                sdkCreateTimer(&timer);
                sdkStartTimer(&timer);
                for (uintmax_t i = 0; i < ciphertext_blocks; i++)
                {
                    aes_cpu_decrypt(ciphertext + i * AES_BLOCK_SIZE, round_key, plaintext_block);

                    memcpy(plaintext + i * AES_BLOCK_SIZE, plaintext_block, sizeof(byte_t) * AES_BLOCK_SIZE);
                }
                sdkStopTimer(&timer);
                cout << endl << "Time taken to Decrypt : " << sdkGetTimerValue(&timer) / 1000.0f << " seconds" << endl;

                size_t bytes_written = write_file(argv[3], plaintext, AES_BLOCK_SIZE * ciphertext_blocks);
                cout << endl << bytes_written << " Bytes Written To File " << argv[3] << endl;

                sdkDeleteTimer(&timer);
                timer = NULL;

                free(plaintext);
                plaintext = NULL;

                free(key);
                key = NULL;

            }
            break;

            case 3:
                cout << endl << "Exiting !!!" << endl;
                exit(AES_SUCCESS);
            break;

            default:
                cerr << endl << "Error: Please Choose Between Options 1-3 ... Exiting !!!" << endl;
                exit(AES_FAILURE);
            break;
        }

        cout << "--------------------------------------------------------------------" << endl;
    }

    return AES_SUCCESS;
}
