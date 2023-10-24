#pragma once

#include <cstdlib>
#include <cstring>
#include <filesystem>

#include "../Common/Tables.hpp"
#include "../Common/Helper.hpp"
#include "../Common/Logger.hpp"

#ifndef _HELPER_TIMER_H
    #define _HELPER_TIMER_H
    #include "../Common/helper_timer.h"
#endif

// Global Variables
byte_arr_t aes_key;
vector<byte_arr_t> aes_sub_keys;
StopWatchInterface *aes_cpu_timer = nullptr;

// ifstream input_file_stream;
// FILE *encryption_file = nullptr;
// FILE *decryption_file = nullptr;

// int block_number;
// int num_zero_pending;
// uintmax_t input_file_size;
// string input_file_extension, enc_file_with_extension, dec_file_with_extension;

// byte_t aes_encryption_key[AES_LENGTH * (14 + 1)];
// byte_t aes_decryption_key[AES_LENGTH * (14 + 1)];
// char auxillary_buffer[AES_LENGTH];

// int round_constant = 1;
// int key_length = 0;
// int expanded_key_length = 0;
// int block_length = AES_LENGTH;
// float aes_cpu_encryption_time = 0.0, aes_cpu_decryption_time = 0.0;

// Function Prototypes
void aes_cpu_initialize(void);
void aes_cpu_cleanup(void);

// Key Schedule Functions
void aes_cpu_key_schedule(void);
byte_arr_t aes_cpu_sub_key(byte_arr_t &prev_sub_key, const int r);

// AES Operations
// -> Byte Substitution
// -> Shifting Rows
// -> Mixing Columns
// -> Adding Round Key
void aes_cpu_byte_sub(byte_arr_t &data);
void aes_cpu_byte_sub_inverse(byte_arr_t &data);

void aes_cpu_shift_rows(byte_arr_t &data);
void saes_cpu_hift_rows_inverse(byte_arr_t &data);

void aes_cpu_mix_columns(byte_arr_t &data);
void aes_cpu_mix_columns_inverse(byte_arr_t &data);

void add_round_key(byte_arr_t &data, const int round_key);

// AES Encryption and Decryption Functions
byte_arr_t aes_cpu_encrypt_data(const byte_arr_t &data);
byte_arr_t aes_cpu_decrypt_data(const byte_arr_t &data);

// Wrapper For Counter Mode and Counter Mode Inverse Functions
const vector<byte_arr_t> aes_cpu_encrypt(
    const vector<byte_arr_t> &data,
    const byte_arr_t &key,
    const byte_arr_t &iv
);

const vector<byte_arr_t> aes_cpu_decrypt(
    const vector<byte_arr_t> &data,
    const byte_arr_t &key,
    const byte_arr_t &iv
);