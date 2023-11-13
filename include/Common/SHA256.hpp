#ifndef __SHA256_HPP__
#define __SHA256_HPP__

#include <iostream>
#include <cstring>
#include <cstdlib>

using namespace std;

#define SHA256_FAILURE EXIT_FAILURE
#define SHA256_SUCCESS EXIT_SUCCESS

#if defined (_WIN32) || defined (_WIN64) || defined (WIN32) || defined (WIN64)
    #include <stdint.h>
#elif defined(__linux)
    #include <sys/types.h>
    typedef u_int32_t uint32_t;
#endif

typedef struct 
{
    uint32_t h0, h1, h2, h3, h4, h5, h6, h7;
    uint32_t num_blocks;
    unsigned char buffer[64];
    int count;

} SHA256_CONTEXT;

typedef int sha_status_t;

// Function Declarations

/// @brief Initializes the SHA256 context; it must be called before doing any hashing.
/// @param handle 
void sha256_init(SHA256_CONTEXT *handle);

/// @brief Updates the message digest with the contents of input buffer
/// @param handle 
/// @param input_buffer 
/// @param input_buffer_length 
sha_status_t sha256_write(SHA256_CONTEXT *handle, unsigned char *input_buffer, size_t input_buffer_length);


/// @brief Reads the content of the hash computed with the given context
/// @param handle 
/// @return 
unsigned char *sha256_read(SHA256_CONTEXT *handle);


/// @brief Terminates the computation of the hash associated to the given context. 
// The handle is prepared for a new cycle, but adding unsigned chars to the
// context will the destroy the returned buffer.
/// @param handle 
void sha256_final(SHA256_CONTEXT *handle);

#endif