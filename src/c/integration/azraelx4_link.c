/*
 * Copyright (C) 2007-2020 Jorge Medina - <medinarosas.jorgealberto@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#include <string.h>
#include <stdint.h>

#define C0(x1,x2,x3,x4,x5) ((( x1 + x2 ) ^ ( x3 ^ x4 )) ^ x5)
#define C1(x1,x2,x3,x4,x5) ((( x1 & x2 ) ^ ( x3 + x4 )) ^ x5)
#define C2(x1,x2,x3,x4,x5) ((( x1 ^ x2 ) + ( x3 + x4 )) ^ x5)
#define C3(x1,x2,x3,x4,x5) ((( x1 ^ x2 ) ^ ( x3 + x4 )) ^ x5)
#define C4(x1,x2,x3,x4,x5) ((( x1 & x2 ) + ( x3 + x4 )) + x5)
#define C5(x1,x2,x3,x4,x5) ((( x1 & x2 ) + ( x3 + x4 )) ^ x5)
#define C6(x1,x2,x3,x4,x5) ((( x1 ^ x2 ) ^ ( x3 + x4 )) ^ x5)
#define C7(x1,x2,x3,x4,x5) ((( x1 | x2 ) ^ ( x3 + x4 )) ^ x5)
#define C8(x1,x2,x3,x4,x5) ((  x1 | x2 ) | ((x3 + x4 )  ^ x5))
#define C9(x1,x2,x3,x4,x5) ((( x1 + x2 ) + ( x3 + x4 )) ^ x5)
#define CA(x1,x2,x3,x4,x5) ((( x1 + x2 ) & ( x3 + x4 )) ^ x5)
#define CB(x1,x2,x3,x4,x5) ((( x1 ^ x2 ) + ( x3 ^ x4 )) ^ x5)
#define CC(x1,x2,x3,x4,x5) ((( x1 | x2 ) ^ ( x3 ^ x4 )) ^ x5)
#define CD(x1,x2,x3,x4,x5) ((( x1 + x2 ) + ( x3 ^ x4 )) ^ x5)
#define CE(x1,x2,x3,x4,x5) ((( x1 + x2 ) ^ ( x3 + x4 )) ^ x5)
#define CF(x1,x2,x3,x4,x5) ((  x1 + x2 ) & ((x3 + x4 )  + x5))

#define COMPRESS_320(x1,x2,x3,x4,x5) \
  + C0(x1,x2,x3,x4,x5) + C1(x1,x2,x3,x4,x5) + C2(x1,x2,x3,x4,x5) + C3(x1,x2,x3,x4,x5) \
  + C4(x1,x2,x3,x4,x5) + C5(x1,x2,x3,x4,x5) + C6(x1,x2,x3,x4,x5) + C7(x1,x2,x3,x4,x5) \
  + C8(x1,x2,x3,x4,x5) + C9(x1,x2,x3,x4,x5) + CA(x1,x2,x3,x4,x5) + CB(x1,x2,x3,x4,x5) \
  + CC(x1,x2,x3,x4,x5) + CD(x1,x2,x3,x4,x5) + CE(x1,x2,x3,x4,x5) + CF(x1,x2,x3,x4,x5)

// Fractional part of square roots..
static const uint64_t IV[10] = {
  0x6a09e667bb67ae85,
  0x3c6ef372a54ff53a,
  0x510e527f9b05688c,
  0x1f83d9ab5be0cd19,
  0x428a2f9871374491,
  0xb5c0fbcfe9b5dba5,
  0x3956c25b59f111f1,
  0x923f82a4ab1c5ed5,
  0xd807aa9812835b01,
  0x243185be550c7dc3
};

inline char* pad( const char* data, int length, int padding, char* output ) {
  int i;
  const uint64_t bits = length * 8;
  char pad[padding];
  pad[0] = (char) 0x80;
  for( i=1; i<=padding-1-8; i++ ) {
	 pad[i] = (char)0;
  }
  for( i=0; i < 8; i++ ) {
	 pad[padding - 1 - i] = (char) ((bits >> (8 * i)) & 0xFF);
  }
  memcpy(output,data,length);
  memcpy(output+length,pad,padding);
  return output;
}

void eval_hash( char* input, uint64_t* hash, int inputLength ) {
  // Do padding..
  const int tail = inputLength % 64;
  int padding = 64 - tail >= 9? 64 - tail: 128 - tail;
  char output[inputLength+padding];
  input = pad(input,inputLength,padding,output);
  inputLength += padding;

  // Begin hash calculation..
  int i;
  uint64_t cha[5] = {IV[0],IV[1],IV[2],IV[3],IV[4]};
  uint64_t carrier[8] = {IV[5],IV[6],IV[7],IV[8],IV[9],IV[0],IV[2],IV[7]};

  // First iteration..
  cha[0] += (uint64_t)input[ inputLength-2 ];
  cha[1] += (uint64_t)input[ inputLength-1 ];
  cha[2] += (uint64_t)input[ 0 ];
  cha[3] += (uint64_t)input[ 1 ];
  cha[4] += (uint64_t)input[ 2 ];
  carrier[7] += carrier[6];
  carrier[6] += carrier[5];
  carrier[5] += carrier[4];
  carrier[4] += carrier[3];
  carrier[3] += carrier[2];
  carrier[2] += carrier[1];
  carrier[1] += carrier[0];
  carrier[0] += COMPRESS_320( cha[0],cha[1],cha[2],cha[3],cha[4]);
  
  // Main Loop for hash streaming..
  for( i=1; i<inputLength-1; i++ ) {
    cha[0] += carrier[0];
    cha[1] += cha[2];
    cha[2] += cha[3];
    cha[3] += (uint64_t)input[i+1];
    cha[4] += carrier[1];
    carrier[7] += carrier[6];
    carrier[6] += carrier[5];
    carrier[5] += carrier[4];
    carrier[4] += carrier[3];
    carrier[3] += carrier[2];
    carrier[2] += carrier[1];
    carrier[1] += carrier[0];
    carrier[0] += COMPRESS_320( cha[0],cha[1],cha[2],cha[3],cha[4]);
  }

  // Last iteration..
  cha[0] += carrier[0];
  cha[1] += cha[2];
  cha[2] += cha[3];
  cha[3] += (uint64_t)input[ 0 ];
  cha[4] += carrier[1];
  carrier[7] += carrier[6];
  carrier[6] += carrier[5];
  carrier[5] += carrier[4];
  carrier[4] += carrier[3];
  carrier[3] += carrier[2];
  carrier[2] += carrier[1];
  carrier[1] += carrier[0];
  carrier[0] += COMPRESS_320( cha[0],cha[1],cha[2],cha[3],cha[4]);

  // Doing dispersion of bits.. (horizontal 'avalanche' effect (1))
  carrier[0] += COMPRESS_320( carrier[0],carrier[0],carrier[0],carrier[0],carrier[0]) + IV[7];
  carrier[1] += COMPRESS_320( carrier[1],carrier[1],carrier[1],carrier[1],carrier[1]) + IV[6];
  carrier[2] += COMPRESS_320( carrier[2],carrier[2],carrier[2],carrier[2],carrier[2]) + IV[5];
  carrier[3] += COMPRESS_320( carrier[3],carrier[3],carrier[3],carrier[3],carrier[3]) + IV[4];
  carrier[4] += COMPRESS_320( carrier[4],carrier[4],carrier[4],carrier[4],carrier[4]) + IV[3];
  carrier[5] += COMPRESS_320( carrier[5],carrier[5],carrier[5],carrier[5],carrier[5]) + IV[2];
  carrier[6] += COMPRESS_320( carrier[6],carrier[6],carrier[6],carrier[6],carrier[6]) + IV[1];
  carrier[7] += COMPRESS_320( carrier[7],carrier[7],carrier[7],carrier[7],carrier[7]) + IV[0];

  // Doing pile of bits.. (vertical 'avalanche' effect (2))
  hash[0] = ((carrier[0] << 48) & 0xffffffffffffffffL ) |
            (((carrier[0]+carrier[1]) << 32) & 0xffffffffffffffffL ) |
            (((carrier[0]+carrier[1]+carrier[2]) << 16) & 0xffffffffL) |
            ((carrier[2]+carrier[3]+carrier[4]+carrier[5]) & 0xffffffffL);
  hash[1] = ((carrier[0] << 48) & 0xffffffffffffffffL ) |
            (((carrier[0]+carrier[2]) << 32) & 0xffffffffffffffffL ) |
            (((carrier[1]+carrier[2]+carrier[3]) << 16) & 0xffffffffL) |
            ((carrier[3]+carrier[4]+carrier[0]+carrier[6]) & 0xffffffffL);
  hash[2] = ((carrier[2] << 48) & 0xffffffffffffffffL ) |
            (((carrier[0]+carrier[3]) << 32) & 0xffffffffffffffffL ) |
            (((carrier[2]+carrier[3]+carrier[4]) << 16) & 0xffffffffL) |
            ((carrier[4]+carrier[0]+carrier[1]+carrier[7]) & 0xffffffffL);
  hash[3] = ((carrier[1] << 48) & 0xffffffffffffffffL ) |
            (((carrier[0]+carrier[4]) << 32) & 0xffffffffffffffffL ) |
            (((carrier[3]+carrier[4]+carrier[0]) << 16) & 0xffffffffL) |
            ((carrier[0]+carrier[1]+carrier[2]+carrier[5]) & 0xffffffffL);
  hash[4] = ((carrier[1] << 48) & 0xffffffffffffffffL ) |
            (((carrier[0]+carrier[0]) << 32) & 0xffffffffffffffffL ) |
            (((carrier[4]+carrier[0]+carrier[1]) << 16) & 0xffffffffL) |
            ((carrier[1]+carrier[2]+carrier[3]+carrier[6]) & 0xffffffffL);
  hash[5] = ((carrier[0] << 48) & 0xffffffffffffffffL ) |
            (((carrier[0]+carrier[3]) << 32) & 0xffffffffffffffffL ) |
            (((carrier[1]+carrier[3]+carrier[4]) << 16) & 0xffffffffL) |
            ((carrier[2]+carrier[5]+carrier[6]+carrier[7]) & 0xffffffffL);
  hash[6] = ((carrier[2] << 48) & 0xffffffffffffffffL ) |
            (((carrier[0]+carrier[4]) << 32) & 0xffffffffffffffffL ) |
            (((carrier[3]+carrier[4]+carrier[5]) << 16) & 0xffffffffL) |
            ((carrier[6]+carrier[7]+carrier[0]+carrier[1]) & 0xffffffffL);
  hash[7] = ((carrier[2] << 48) & 0xffffffffffffffffL ) |
            (((carrier[0]+carrier[5]) << 32) & 0xffffffffffffffffL ) |
            (((carrier[6]+carrier[0]+carrier[1]) << 16) & 0xffffffffL) |
            ((carrier[4]+carrier[5]+carrier[6]+carrier[7]) & 0xffffffffL);
  hash[8] = ((carrier[1] << 48) & 0xffffffffffffffffL ) |
            (((carrier[0]+carrier[2]) << 32) & 0xffffffffffffffffL ) |
            (((carrier[6]+carrier[1]+carrier[3]) << 16) & 0xffffffffL) |
            ((carrier[3]+carrier[4]+carrier[6]+carrier[7]) & 0xffffffffL);
  hash[9] = ((carrier[0] << 48) & 0xffffffffffffffffL ) |
            (((carrier[1]+carrier[4]) << 32) & 0xffffffffffffffffL ) |
            (((carrier[5]+carrier[6]+carrier[7]) << 16) & 0xffffffffL) |
            ((carrier[0]+carrier[3]+carrier[4]+carrier[7]) & 0xffffffffL);
  hash[10]= ((carrier[0] << 48) & 0xffffffffffffffffL ) |
            (((carrier[2]+carrier[6]) << 32) & 0xffffffffffffffffL ) |
            (((carrier[3]+carrier[4]+carrier[7]) << 16) & 0xffffffffL) |
            ((carrier[1]+carrier[3]+carrier[5]+carrier[7]) & 0xffffffffL);
  hash[11]= ((carrier[1] << 48) & 0xffffffffffffffffL ) |
            (((carrier[0]+carrier[2]) << 32) & 0xffffffffffffffffL ) |
            (((carrier[3]+carrier[6]+carrier[7]) << 16) & 0xffffffffL) |
            ((carrier[2]+carrier[4]+carrier[5]+carrier[7]) & 0xffffffffL);

  // Doing dispersion of bits.. (horizontal 'avalanche' effect (3))
  hash[0]  += COMPRESS_320( hash[0],hash[0],hash[0],hash[0],hash[0]) + IV[5];
  hash[1]  += COMPRESS_320( hash[1],hash[1],hash[1],hash[1],hash[1]) + IV[6];
  hash[2]  += COMPRESS_320( hash[2],hash[2],hash[2],hash[2],hash[2]) + IV[7];
  hash[3]  += COMPRESS_320( hash[3],hash[3],hash[3],hash[3],hash[3]) + IV[8];
  hash[4]  += COMPRESS_320( hash[4],hash[4],hash[4],hash[4],hash[4]) + IV[9];
  hash[5]  += COMPRESS_320( hash[5],hash[5],hash[5],hash[5],hash[5]) + IV[0];
  hash[6]  += COMPRESS_320( hash[6],hash[6],hash[6],hash[6],hash[6]) + IV[2];
  hash[7]  += COMPRESS_320( hash[7],hash[7],hash[7],hash[7],hash[7]) + IV[7];
  hash[8]  += COMPRESS_320( hash[8],hash[8],hash[8],hash[8],hash[8]) + IV[4];
  hash[9]  += COMPRESS_320( hash[9],hash[9],hash[9],hash[9],hash[9]) + IV[6];
  hash[10] += COMPRESS_320( hash[10],hash[10],hash[10],hash[10],hash[10]) + IV[9];
  hash[11] += COMPRESS_320( hash[11],hash[11],hash[11],hash[11],hash[11]) + IV[3];

  // Finally, we add the number of rounds to output..
  hash[11] += inputLength + 8 + 12;
}
