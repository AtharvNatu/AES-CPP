#include "../include/CPU/AES-CPU.hpp"

int main(int argc, char** argv)
{
    // Variable Declarations
    AesCpu *aes_cpu = nullptr;
    string enc_key, dec_key;

    // Code
    aes_cpu = new AesCpu();

    aes_cpu->initialize(argv[1]);

    cout << endl << "Enter Encryption Key = ";
    cin >> enc_key;
    aes_cpu->set_key(enc_key, aes_cpu->aes_encryption_key, &aes_cpu->expanded_key_length);

    sdkStartTimer(&aes_cpu->aes_cpu_timer);
    aes_cpu->encrypt(argv[2], aes_cpu->aes_block_array, aes_cpu->aes_encryption_key, aes_cpu->expanded_key_length, aes_cpu->block_number);
    sdkStopTimer(&aes_cpu->aes_cpu_timer);
    
    if (DEBUG)
        cout << endl << "Time required for Encryption : " << sdkGetTimerValue(&aes_cpu->aes_cpu_timer) << " ms " << endl;
    
    cout << endl << "Enter Decryption Key = ";
    cin >> dec_key;

    aes_cpu->verify_key(enc_key, dec_key);

    aes_cpu->set_key(enc_key, aes_cpu->aes_decryption_key, &aes_cpu->expanded_key_length);

    sdkStartTimer(&aes_cpu->aes_cpu_timer);
    aes_cpu->decrypt(argv[3], aes_cpu->aes_block_array, aes_cpu->aes_decryption_key, aes_cpu->expanded_key_length, aes_cpu->block_number);
    sdkStopTimer(&aes_cpu->aes_cpu_timer);

    if (DEBUG)
        cout << endl << "Time required for Decryption : " << sdkGetTimerValue(&aes_cpu->aes_cpu_timer) << " ms " << endl;
    
    aes_cpu->uninitialize(aes_cpu);

    return AES_CPU_SUCCESS;
}
