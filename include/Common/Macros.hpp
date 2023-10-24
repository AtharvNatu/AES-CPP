#pragma once

// Headers
#include <iostream>
#include <vector>
using namespace std;

// Macros
#define DEBUG           true

#define AES_LENGTH      16
#define AES_ROUNDS      10
#define AES_BITS        128
#define SUB_KEYS        (AES_ROUNDS + 1)
#define AES_SUCCESS     EXIT_SUCCESS
#define AES_FAILURE     EXIT_FAILURE

// Typedefs
typedef unsigned char byte_t;
typedef vector<unsigned char> byte_arr_t;
typedef long long hash_t;
typedef int status_t;
