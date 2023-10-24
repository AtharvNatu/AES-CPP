#pragma once

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <cstdint>
#include <bitset>
#include <random>
#include "Macros.hpp"

using namespace std;

// I/O Releated Functions 
const vector<byte_arr_t> read_data_file(string src_input_file);
void write_data_file(byte_t* data_arr, int length, int file);

// Debugging
void print_byte(byte_t byte);
void print_byte_array(byte_arr_t &arr);
status_t compare_byte_arrays(const byte_arr_t &arr1, const byte_arr_t &arr2);
status_t compare_vector_of_byte_arrays(const vector<byte_arr_t> &arr1, const vector<byte_arr_t> &arr2);

// XOR Operations
byte_arr_t XOR(const byte_arr_t &arr1, const byte_arr_t &arr2);
void XOR(byte_arr_t &arr1, byte_arr_t &arr2, const unsigned int length);

// Counter Related Functions
byte_arr_t increment_counter(const byte_arr_t &start_counter, const unsigned int round);
void generate_counters(vector<byte_arr_t> &counters, const byte_arr_t &iv);

// Key Related Functions
status_t set_key(string str_key, byte_t* aes_key, int *expanded_key_length);
status_t verify_key(string encryption_key, string decryption_key);
hash_t generate_hash(string input);
int expand_key(byte_t* key);

// AES IV Related Functions
byte_arr_t generate_iv(int iv_length);