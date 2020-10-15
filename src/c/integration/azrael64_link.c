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

inline char* pad_64( const char* data, int length, int padding, char* output ) {
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

void eval_hash_64( char* input, uint64_t* hash, int inputLength ) {
  // Do padding..
  const int tail = inputLength % 64;
  int padding = 64 - tail >= 9? 64 - tail: 128 - tail;
  char output[inputLength+padding];
  input = pad_64(input,inputLength,padding,output);
  inputLength += padding;

  // Begin hash calculation..
  int i;
  uint64_t cha[5] = {0,0,0,0,0};
  uint64_t carrier[5] = {(uint64_t)input[ inputLength-2 ],(uint64_t)input[ 2 ],0,0,0};

  // First iteration..
  cha[0] += carrier[0];
  cha[1] += (uint64_t)input[ inputLength-1 ];
  cha[2] += (uint64_t)input[ 0 ];
  cha[3] += (uint64_t)input[ 1 ];
  cha[4] += carrier[1];
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
  carrier[4] += carrier[3];
  carrier[3] += carrier[2];
  carrier[2] += carrier[1];
  carrier[1] += carrier[0];
  carrier[0] += COMPRESS_320( cha[0],cha[1],cha[2],cha[3],cha[4]);

  // Doing pile of bits.. (vertical 'avalanche' effect (2))
  hash[0] = ((carrier[0] << 48) & 0xffffffffffffffffL ) |
            (((carrier[0]+carrier[1]) << 32) & 0xffffffffffffffffL ) |
            (((carrier[0]+carrier[1]+carrier[2]) << 16) & 0xffffffffL) |
            ((carrier[2]+carrier[3]+carrier[4]) & 0xffffffffL);

  // Finally, we add the number of rounds to output..
  hash[0] += inputLength;
}
