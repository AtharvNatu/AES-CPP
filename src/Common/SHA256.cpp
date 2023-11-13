#include "../../include/Common/SHA256.hpp"

// Function Definitions
void sha256_init(SHA256_CONTEXT *handle)
{
    // Code
    handle->h0 = 0x6a09e667;
	handle->h1 = 0xbb67ae85;
	handle->h2 = 0x3c6ef372;
	handle->h3 = 0xa54ff53a;
	handle->h4 = 0x510e527f;
	handle->h5 = 0x9b05688c;
	handle->h6 = 0x1f83d9ab;
	handle->h7 = 0x5be0cd19;

	handle->num_blocks = 0;
	handle->count = 0;
}

static void transform(SHA256_CONTEXT *handle, unsigned char *data)
{
    // Variable Declarations
    uint32_t a, b, c, d, e, f, g, h;
	uint32_t w[64];
	int t;
	static const uint32_t k[] = 
    {
		0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1,
		0x923f82a4, 0xab1c5ed5, 0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
		0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174, 0xe49b69c1, 0xefbe4786,
		0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
		0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147,
		0x06ca6351, 0x14292967, 0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
		0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85, 0xa2bfe8a1, 0xa81a664b,
		0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
		0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a,
		0x5b9cca4f, 0x682e6ff3, 0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
		0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
	};

    // Code

    // Get the values from the chaining variables
    a = handle->h0;
    b = handle->h1;
	c = handle->h2;
	d = handle->h3;
	e = handle->h4;
	f = handle->h5;
	g = handle->h6;
	h = handle->h7;

// Copy data into w array with byte-order conversion if needed
#ifdef BIG_ENDIAN_HOST
    memcpy(w, data, 64);
#else
    {
		int i;
		unsigned char *p2;

		for (i = 0, p2 = (unsigned char *) w; i < 16; i++, p2 += 4) 
        {
			p2[3] = *data++;
			p2[2] = *data++;
			p2[1] = *data++;
			p2[0] = *data++;
		}
	}
#endif

#define ROTR(x,n) (((x)>>(n)) | ((x)<<(32-(n))))
#define Ch(x,y,z) (((x) & (y)) ^ ((~(x)) & (z)))
#define Maj(x,y,z) (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))
#define Sum0(x) (ROTR((x),2) ^ ROTR((x),13) ^ ROTR((x),22))
#define Sum1(x) (ROTR((x),6) ^ ROTR((x),11) ^ ROTR((x),25))
#define S0(x) (ROTR((x),7) ^ ROTR((x),18) ^ ((x)>>3))
#define S1(x) (ROTR((x),17) ^ ROTR((x),19) ^ ((x)>>10))

    for (t = 16; t < 64; t++)
		w[t] = S1(w[t - 2]) + w[t - 7] + S0(w[t - 15]) + w[t - 16];

	for (t = 0; t < 64; t++) 
    {
		uint32_t t1, t2;

		t1 = h + Sum1(e) + Ch(e, f, g) + k[t] + w[t];
		t2 = Sum0(a) + Maj(a, b, c);
		h = g;
		g = f;
		f = e;
		e = d + t1;
		d = c;
		c = b;
		b = a;
		a = t1 + t2;
	}

    // Update Chaining Variables
    handle->h0 += a;
	handle->h1 += b;
	handle->h2 += c;
	handle->h3 += d;
	handle->h4 += e;
	handle->h5 += f;
	handle->h6 += g;
	handle->h7 += h;
}

sha_status_t sha256_write(SHA256_CONTEXT *handle, unsigned char *input_buffer, size_t input_buffer_length)
{
    // Code

    // Flush the buffer
    if (handle->count == 64)
    {
        transform(handle, handle->buffer);
        handle->count = 0;
        handle->num_blocks++;
    }

    if (!input_buffer)
        return SHA256_FAILURE;

    if (handle->count)
    {
        for (; input_buffer_length && handle->count < 64; input_buffer_length--)
            handle->buffer[handle->count++] = *input_buffer++;
        
        sha256_write(handle, NULL, 0);

        if (!input_buffer_length)
            return SHA256_FAILURE;
    }

    while (input_buffer_length >= 64)
    {
        transform(handle, input_buffer);
        handle->count = 0;
        handle->num_blocks++;
        input_buffer_length -= 64;
        input_buffer += 64;
    }

    for (; input_buffer_length && handle->count < 64; input_buffer_length--)
        handle->buffer[handle->count++] = *input_buffer++;

    return SHA256_SUCCESS;
}

unsigned char *sha256_read(SHA256_CONTEXT *handle)
{
    // Code
    return handle->buffer;
}

void sha256_final(SHA256_CONTEXT *handle)
{
    // Variable Declarations
    uint32_t t, msb, lsb;
    unsigned char *p;

    // Code

    // Flush
    sha256_write(handle, NULL, 0);

    t = handle->num_blocks;
    // Multiply by 64 to make a unsigned char count
    lsb = t << 6;
    msb = t >> 26;
    // Add the count
    t = lsb;
    if ((lsb += handle->count) < t)
        msb++;
    // Multiply by 8 to make a bit count
	t = lsb;
	lsb <<= 3;
	msb <<= 3;
	msb |= t >> 29;

    if (handle->count < 56)
    {
        // Pad Character
        handle->buffer[handle->count++] = 0x80;
        while (handle->count < 56)
            handle->buffer[handle->count++] = 0;
    }
    // Need 1 extra block
    else
    {
        // Pad Character
        handle->buffer[handle->count++] = 0x80;
        while (handle->count < 64)
            handle->buffer[handle->count++] = 0;
        // Flush
        sha256_write(handle, NULL, 0);
        // Fill next block with 0s
        memset(handle->buffer, 0, 56);
    }

    // Append the 64 bit count
    handle->buffer[56] = msb >> 24;
    handle->buffer[57] = msb >> 16;
    handle->buffer[58] = msb >> 8;
    handle->buffer[59] = msb;
    handle->buffer[60] = lsb >> 24;
    handle->buffer[61] = lsb >> 16;
    handle->buffer[62] = lsb >> 8;
    handle->buffer[63] = lsb;
    transform(handle, handle->buffer);

    p = handle->buffer;

#ifdef BIG_ENDIAN_HOST
    #define X(a) do { *(u32*)p = handle->h##a ; p += 4; } while(0)
#else
    #define X(a) do { *p++ = handle->h##a >> 24; *p++ = handle->h##a >> 16;	 \
		      *p++ = handle->h##a >> 8; *p++ = handle->h##a; } while(0)
#endif
	X(0);
	X(1);
	X(2);
	X(3);
	X(4);
	X(5);
	X(6);
	X(7);
#undef X

}
