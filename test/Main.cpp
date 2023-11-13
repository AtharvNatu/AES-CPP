#include "../include/CPU/AES-CPU.hpp"

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
                string user_key;
                string input_file, output_file, output_file_name;
                byte_t *plaintext = NULL;
                byte_t *ciphertext = NULL;
                byte_t ciphertext_block[AES_BLOCK_SIZE];
                byte_t round_key[176];

                // Reading input and output file paths
                input_file = argv[1];
                filesystem::path output_file_path = filesystem::path(input_file).filename();
                output_file_name = output_file_path.string();
                output_file = output_file_name + ".enc";

                // Allocate memory to input buffer
                int file_length = filesystem::file_size(input_file);
                plaintext = (byte_t *)malloc(sizeof(byte_t) * file_length);
                if (plaintext == NULL)
                {
                    cerr << endl << "Error : Failed To Allocate Memory To Input File Buffer ... Exiting !!!" << endl;
                    exit(AES_FAILURE);
                }

                // Read Input File
                uintmax_t bytes_read = read_file(input_file.c_str(), plaintext, file_length);
                uintmax_t ciphertext_blocks = (bytes_read + AES_BLOCK_SIZE - 1) / AES_BLOCK_SIZE;
                ciphertext = (byte_t*)malloc(ciphertext_blocks * AES_BLOCK_SIZE);
                if (ciphertext == NULL)
                {
                    cerr << endl << "Error : Failed To Allocate Memory To Output File Buffer ... Exiting !!!" << endl;
                    exit(AES_FAILURE);
                }

                // Get Password
                cout << endl << "Enter Password = ";
                cin >> user_key;
                unsigned char *key = hash_password(user_key.c_str());
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

                // Write Encrypted Data
                write_file(output_file.c_str(), ciphertext, AES_BLOCK_SIZE * ciphertext_blocks);

                sdkDeleteTimer(&timer);
                timer = NULL;

                free(key);
                key = NULL;

                free(ciphertext);
                ciphertext = NULL;

                free(plaintext);
                plaintext = NULL;
            }
            break;

            case 2:
            {
                // Variable Declarations
                string user_key;
                string input_file, output_file, output_file_name;
                int output_file_name_index;
                byte_t *plaintext = NULL;
                byte_t *ciphertext = NULL;
                byte_t plaintext_block[AES_BLOCK_SIZE];
                byte_t round_key[176];
                
                // Code

                // Reading input and output file paths
                input_file = argv[2];
                output_file_name_index = input_file.find("enc") - 1;
                output_file = input_file.substr(0, output_file_name_index);

                // Allocate memory to input buffer
                int file_length = filesystem::file_size(input_file);
                ciphertext = (byte_t *)malloc(sizeof(byte_t) * file_length);
                if (ciphertext == NULL)
                {
                    cerr << endl << "Error : Failed To Allocate Memory To Input File Buffer ... Exiting !!!" << endl;
                    exit(AES_FAILURE);
                }

                // Read Encrypted Input File
                uintmax_t bytes_read = read_file(input_file.c_str(), ciphertext, file_length);
                uintmax_t ciphertext_blocks = (bytes_read + AES_BLOCK_SIZE - 1) / AES_BLOCK_SIZE;
                plaintext = (byte_t*)malloc(ciphertext_blocks * AES_BLOCK_SIZE);
                if (plaintext == NULL)
                {
                    cerr << endl << "Error : Failed To Allocate Memory To Output File Buffer ... Exiting !!!" << endl;
                    exit(AES_FAILURE);
                }

                // Get Password
                cout << endl << "Enter Password = ";
                cin >> user_key;
                unsigned char *key = hash_password(user_key.c_str());
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

                write_file(output_file.c_str(), plaintext, AES_BLOCK_SIZE * ciphertext_blocks);

                sdkDeleteTimer(&timer);
                timer = NULL;

                free(key);
                key = NULL;

                free(plaintext);
                plaintext = NULL;

                free(ciphertext);
                ciphertext = NULL;

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
