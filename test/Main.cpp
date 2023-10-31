#include "../include/CPU/AES-CPU.hpp"

int main(int argc, char** argv)
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
                byte_arr_t enc_key, dec_key, iv;
                string input_file, output_file, output_file_name;

                input_file = argv[1];
                output_file_name = filesystem::path(input_file).filename();
                output_file = output_file_name + ".enc";

                vector<byte_arr_t> file_data = read_data_file(argv[1]);

                cout << endl << "Enter Encryption Key = ";
                cin >> user_key;
                enc_key = set_key(user_key);

                iv = generate_iv(IV_LENGTH);

                vector<byte_arr_t> encrypted_data = aes_cpu_cipher(file_data, enc_key, iv);

                write_encrypted_data(encrypted_data, enc_key, output_file);
            }
            break;

            case 2:
            {
                // Variable Declarations
                string user_key, file_key;
                byte_arr_t enc_key, dec_key, iv;
                string input_file, output_file, output_file_name;

                input_file = argv[2];
                // output_file_name = filesystem::path(input_file).filename();
                // output_file = output_file_name - ".enc";

                cout << endl << "Enter Decryption Key = ";
                cin >> user_key;
                dec_key = set_key(user_key);

                file_key = get_key(input_file);
                verify_key(file_key, dec_key.data());

                iv = generate_iv(IV_LENGTH);

                // vector<byte_arr_t> encrypted_data = aes_cpu_cipher(file_data, enc_key, iv);

                // write_encrypted_data(encrypted_data, enc_key, output_file);

                // aes_cpu->decrypt(argv[3], aes_cpu->aes_block_array, aes_cpu->aes_decryption_key, aes_cpu->expanded_key_length, aes_cpu->block_number);
                // if (DEBUG)
                //     cout << endl << "Time required for AES 128-bit Decryption : " << aes_cpu->aes_cpu_decryption_time << " ms " << endl;
                // elses
                //     aes_cpu->logger->print_log("Time required for AES 128-bit Decryption : %f", aes_cpu->aes_cpu_decryption_time, " ms ");
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
