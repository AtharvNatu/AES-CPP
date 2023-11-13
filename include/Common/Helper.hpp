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
uintmax_t read_file(const char *input_file, byte_t *output_buffer, uintmax_t input_data_size);
size_t write_file(const char *output_file, byte_t *data, uintmax_t output_data_size);

status_t read_crypt_data(const char *input_file);
void write_crypt_data(const char *input_file);

// Password Related Functions
byte_t *hash_password(const char *password);


                                                                        

