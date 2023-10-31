#pragma once

#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <omp.h>

#include "../Common/Tables.hpp"
#include "../Common/Helper.hpp"
#include "../Common/Logger.hpp"

#ifndef _HELPER_TIMER_H
    #define _HELPER_TIMER_H
    #include "../Common/helper_timer.h"
#endif

// Function Prototypes

// Key Schedule Functions
void aes_cpu_key_schedule(const byte_arr_t &key);
byte_arr_t aes_cpu_sub_key(byte_arr_t &prev_sub_key, const int rounds);

// AES Operations
// -> Byte Substitution
// -> Shifting Rows
// -> Mixing Columns
// -> Adding Round Key
void aes_cpu_byte_sub(byte_arr_t &data);
void aes_cpu_byte_sub_inverse(byte_arr_t &data);

void aes_cpu_shift_rows(byte_arr_t &data);
void aes_cpu_shift_rows_inverse(byte_arr_t &data);

void aes_cpu_mix_columns(byte_arr_t &data);
void aes_cpu_mix_columns_inverse(byte_arr_t &data);

void aes_cpu_add_round_key(byte_arr_t &data, const int round_key);

// AES Encryption and Decryption Algorithm Implementations
byte_arr_t aes_cpu_encrypt_data(const byte_arr_t &data);
byte_arr_t aes_cpu_decrypt_data(const byte_arr_t &data);

// AES Encryption and Decryption Wrapper Functions
const vector<byte_arr_t> aes_cpu_cipher(
    const vector<byte_arr_t> &data,
    const byte_arr_t &key,
    const byte_arr_t &iv
);

const vector<byte_arr_t> aes_cpu_decipher(
    const vector<byte_arr_t> &data,
    const byte_arr_t &key,
    const byte_arr_t &iv
);

const vector<byte_arr_t> aes_cpu_cipher_omp(
    const vector<byte_arr_t> &data,
    const byte_arr_t &key,
    const byte_arr_t &iv
);

const vector<byte_arr_t> aes_cpu_decipher_omp(
    const vector<byte_arr_t> &data,
    const byte_arr_t &key,
    const byte_arr_t &iv
);

// Library Export Wrappers
double aes_cpu_encrypt(const char *input_file_name);
double aes_cpu_decrypt(const char *input_file_name);
