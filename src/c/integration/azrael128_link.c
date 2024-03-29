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
#ifndef UNIT_TEST
  #include "azrael_base.c"
#endif

void eval_hash_128( char* input, uint64_t* hash, int inputLength ) {
  // Do padding..
  const int tail = inputLength % 64;
  int padding = 64 - tail >= 9? 64 - tail: 128 - tail;
  char output[inputLength+padding];
  input = azr_pad(input,inputLength,padding,output);
  inputLength += padding;

  // Begin hash calculation..
  int i;
  uint64_t cha[5] = {IV[0],IV[1],IV[2],IV[3],IV[4]};
  uint64_t carrier[5] = {IV[5],IV[6],IV[7],IV[8],IV[9]};

  // First iteration..
  cha[0] += (uint64_t)input[ inputLength-2 ];
  cha[1] += (uint64_t)input[ inputLength-1 ];
  cha[2] += (uint64_t)input[ 0 ];
  cha[3] += (uint64_t)input[ 1 ];
  cha[4] += (uint64_t)input[ 2 ];
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

  // Doing dispersion of bits.. (horizontal 'avalanche' effect (1))
  carrier[0] += COMPRESS_320( carrier[0],carrier[0],carrier[0],carrier[0],carrier[0]) + IV[6];
  carrier[1] += COMPRESS_320( carrier[1],carrier[1],carrier[1],carrier[1],carrier[1]) + IV[7];
  carrier[2] += COMPRESS_320( carrier[2],carrier[2],carrier[2],carrier[2],carrier[2]) + IV[8];
  carrier[3] += COMPRESS_320( carrier[3],carrier[3],carrier[3],carrier[3],carrier[3]) + IV[9];
  carrier[4] += COMPRESS_320( carrier[4],carrier[4],carrier[4],carrier[4],carrier[4]) + IV[0];

  // Doing pile of bits.. (vertical 'avalanche' effect (2))
  hash[0] = ((carrier[0] << 48) & 0xffffffffffffffffL ) |
            (((carrier[0]+carrier[1]) << 32) & 0xffffffffffffffffL ) |
            (((carrier[0]+carrier[1]+carrier[2]) << 16) & 0xffffffffL) |
            ((carrier[2]+carrier[3]+carrier[4]) & 0xffffffffL);
  hash[1] = ((carrier[1] << 48) & 0xffffffffffffffffL ) |
            (((carrier[0]+carrier[4]) << 32) & 0xffffffffffffffffL ) |
            (((carrier[3]+carrier[4]+carrier[0]) << 16) & 0xffffffffL) |
            ((carrier[0]+carrier[1]+carrier[2]) & 0xffffffffL);

  // Doing dispersion of bits.. (horizontal 'avalanche' effect (3))
  hash[0]  += COMPRESS_320( hash[0],hash[0],hash[0],hash[0],hash[0]) + IV[1];
  hash[1]  += COMPRESS_320( hash[1],hash[1],hash[1],hash[1],hash[1]) + IV[2];

  // Finally, we add the number of rounds to output..
  hash[1] += inputLength + 5 + 2 + 2;
}
