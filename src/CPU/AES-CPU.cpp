#include "../../include/CPU/AES-CPU.hpp"

// Function Definitions
void aes_cpu_initialize(void)
{
    sdkCreateTimer(&aes_cpu_timer);
}

// void AesCpu::write_output(byte_t* data_arr, int length, int file)
// {
//     // Variable Declarations
//     int flag = 0;

//     // Code
//     switch(file)
// 	{
// 		case 1:
// 			for (int i = 0; i < length; i++)
// 				fprintf(encryption_file, "%c", data_arr[i]);
// 			fprintf(encryption_file, "\n");
// 		break;

// 		case 2:
// 			for (int i = 0; i < length; i++)
// 			{
// 				fprintf(decryption_file, "%c", data_arr[i]);
// 				if (data_arr[i] == '\n')
// 					flag++;
// 			}
// 		break;

// 		case 3:
// 			for (int i = 0; i < length; i++)
// 			{
// 				if (data_arr[i] == '\0')
// 					return;
// 				fprintf(decryption_file, "%c", data_arr[i]);
// 				if (data_arr[i] == '\n')
// 					flag++;
// 			}
// 		break;
// 	}
// }

void aes_cpu_cleanup(void)
{
    // Code
    if (aes_cpu_timer)
    {
        sdkDeleteTimer(&aes_cpu_timer);
        aes_cpu_timer = nullptr;
    }
}
