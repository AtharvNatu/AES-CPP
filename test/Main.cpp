#include "../include/CPU/AES-CPU.hpp"

int main(int argc, char** argv)
{
    // Variable Declarations
    AesCpu *aes_cpu = nullptr;
    string enc_key, dec_key;
    int choice = -1;

    // Code
    while (true)
    {
        cout << endl << "AES 128-bit Encryption" << endl;
        cout << "--------------------------------------------------------------------" << endl;
        cout << "1. Encrypt File" << endl << "2. Decrypt File" << endl << "3. Exit" << endl << "\nEnter your choice : ";
        cin >> choice;

        switch(choice)
        {
            case 1:

                aes_cpu = new AesCpu();

                aes_cpu->initialize(argv[1]);

                cout << endl << "Enter Encryption Key = ";
                cin >> enc_key;
                aes_cpu->set_key(enc_key, aes_cpu->aes_encryption_key, &aes_cpu->expanded_key_length);

                aes_cpu->encrypt(argv[2], aes_cpu->aes_block_array, aes_cpu->aes_encryption_key, aes_cpu->expanded_key_length, aes_cpu->block_number);
                if (DEBUG)
                    cout << endl << "Time required for AES 128-bit Encryption : " << aes_cpu->aes_cpu_encryption_time << " ms " << endl;
                else
                    aes_cpu->logger->print_log("Time required for AES 128-bit Encryption : %f", aes_cpu->aes_cpu_encryption_time, " ms ");
                
                aes_cpu->uninitialize(aes_cpu);

            break;

            // case 2:

            //     cout << endl << "Enter Decryption Key = ";
            //     cin >> dec_key;

            //     aes_cpu->verify_key(enc_key, dec_key);

            //     aes_cpu->set_key(enc_key, aes_cpu->aes_decryption_key, &aes_cpu->expanded_key_length);

            //     aes_cpu->decrypt(argv[3], aes_cpu->aes_block_array, aes_cpu->aes_decryption_key, aes_cpu->expanded_key_length, aes_cpu->block_number);
            //     if (DEBUG)
            //         cout << endl << "Time required for AES 128-bit Decryption : " << aes_cpu->aes_cpu_decryption_time << " ms " << endl;
            //     else
            //         aes_cpu->logger->print_log("Time required for AES 128-bit Decryption : %f", aes_cpu->aes_cpu_decryption_time, " ms ");

            //     aes_cpu->uninitialize(aes_cpu);

            // break;

            case 3:
                cout << endl << "Exiting !!!" << endl;
                exit(AES_SUCCESS);
            break;
        }

        cout << "--------------------------------------------------------------------" << endl;
    }

    delete aes_cpu;
    aes_cpu = nullptr;

    return AES_SUCCESS;
}
