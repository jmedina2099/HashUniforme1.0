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
#include <stdint.h>
#include <string.h>

// Fractional part of square roots..
const uint64_t IV[10] = {
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

#define IVA(i) IV[i]

static inline uint64_t ROTATE_AZR( uint64_t x, int o ) {
	o %= 64;
	return (uint64_t)( x >> o );
}

static inline char* azr_pad( const char* data, int length, int padding, char* output ) {
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
