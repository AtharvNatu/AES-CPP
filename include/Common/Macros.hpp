#pragma once

// Headers
#include <iostream>
#include <vector>
#include <string>
using namespace std;

// Macros
#define DEBUG           true

#define AES_BLOCK_SIZE  16
#define AES_LENGTH      16
#define AES_COLS        4
#define AES_ROUNDS      10
#define AES_BITS        128
#define AES_STATE_SIDE  4
#define AES_SUCCESS     EXIT_SUCCESS
#define AES_FAILURE     EXIT_FAILURE

#if defined (_WIN32) || defined (_WIN64) || defined (WIN32) || defined (WIN64)
    #define OS 1
#elif defined(__linux)
    #define OS 2
#endif

// Typedefs
typedef unsigned char byte_t;
typedef byte_t state_t[4][4];

