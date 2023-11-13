#pragma once

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <cstdint>
#include <bitset>
#include <random>
#include <filesystem>
#include "Macros.hpp"
#include "SHA256.hpp"

using namespace std;

// I/O Releated Functions 
// void read_file(const char *input_file, char **output);
string read_file(string input_file);
int read_file(const char *input_file, char *output_buffer);
uintmax_t read_file(const char *input_file, byte_t *output_buffer, uintmax_t input_data_size);

size_t write_file(const char *output_file, byte_t *data, uintmax_t output_data_size);
size_t write_file(const char *output_file, char *data, int output_data_size);
void write_file(string output_file, string input);
void write_file(string output_file, vector<string> input);


// void write_encrypted_data(vector<byte_arr_t> data_vector, byte_arr_t enc_key, string output_file);
// void write_decrypted_data(vector<byte_arr_t> data_vector, string output_file);


// Key Related Functions
string get_key(string input_file);
// byte_arr_t set_key(string str_key);
status_t verify_key(string encryption_key, unsigned char* decryption_key);
string get_hash(string input);

                                                                        

