/* Copyright (C) 2007-2020 - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Jorge Medina <medinarosas.jorgealberto@gmail.com>, October 7th, 2020
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdint.h>
#include <inttypes.h>

static int rounds = 0;
static int iteration = 0;
static int iteraciones = 0;

#ifdef PRINT_HASH
  static const int numMostrar = 20;
#endif

static inline uint64_t evaluaFuncBool( register const uint64_t char1,
		register const uint64_t char2,
		register const uint64_t char3,
		register const uint64_t char4,
		register const uint64_t char5 ) {
	rounds++;
  	return ((( char1 + char2 ) ^ ( char3 ^ char4 )) ^ char5) +
	       ((( char1 & char2 ) ^ ( char3 + char4 ))	^ char5) +
	       ((( char1 ^ char2 ) + ( char3 + char4 )) ^ char5) +
	       ((( char1 ^ char2 ) ^ ( char3 + char4 )) ^ char5) +
	       ((( char1 & char2 ) + ( char3 + char4 )) + char5) +
	       ((( char1 & char2 ) + ( char3 + char4 )) ^ char5) +
	       ((( char1 ^ char2 ) ^ ( char3 + char4 )) ^ char5) +
	       ((( char1 | char2 ) ^ ( char3 + char4 )) ^ char5) +
	       (( char1 | char2 ) | (( char3 + char4 ) ^ char5)) +
	       ((( char1 + char2 ) + ( char3 + char4 )) ^ char5) +
	       ((( char1 + char2 ) & ( char3 + char4 )) ^ char5) +
	       ((( char1 ^ char2 ) + ( char3 ^ char4 )) ^ char5) +
	       ((( char1 | char2 ) ^ ( char3 ^ char4 )) ^ char5) +
	       ((( char1 + char2 ) + ( char3 ^ char4 )) ^ char5) +
	       ((( char1 + char2 ) ^ ( char3 + char4 )) ^ char5) +
	       (( char1 + char2 ) & (( char3 + char4 ) + char5));
 }

static inline char* pad( const char* data, int length, int padding, char* output ) {

  memcpy(output,data,length);

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
  memcpy(output+length,pad,padding);

  return output;
}

char* eval_hash( char* input, char* val, int inputLength ) {

  // Do padding.
  const int tail = inputLength % 64;
  int padding = 64 - tail >= 9? 64 - tail: 128 - tail;

  char output[inputLength+padding];
  input = pad(input,inputLength,padding,output);
  inputLength += padding;

  // Begin calculate..
  int i,j;
  uint64_t cha[5] = {0,0,0,0,0};
  uint64_t carrier[5] = {(uint64_t)input[inputLength-2],(uint64_t)input[2],0,0,0};
  register uint64_t hash1;

    // First iteration.
  cha[0] += carrier[0];
  cha[1] += (uint64_t)input[ inputLength-1 ];
  cha[2] += (uint64_t)input[ 0 ];
  cha[3] += (uint64_t)input[ 1 ];
  cha[4] += carrier[1];
  carrier[4] += carrier[3];
  carrier[3] += carrier[2];
  carrier[2] += carrier[1];
  carrier[1] += carrier[0];
  carrier[0] += evaluaFuncBool( cha[0],cha[1],cha[2],cha[3],cha[4]);
  
  // Main Loop.
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
    carrier[0] += evaluaFuncBool( cha[0],cha[1],cha[2],cha[3],cha[4]);
  }

  // Last iteration.
  cha[0] += carrier[0];
  cha[1] += cha[2];
  cha[2] += cha[3];
  cha[3] += (uint64_t)input[ 0 ];
  cha[4] += carrier[1];
  carrier[4] += carrier[3];
  carrier[3] += carrier[2];
  carrier[2] += carrier[1];
  carrier[1] += carrier[0];
  carrier[0] += evaluaFuncBool( cha[0],cha[1],cha[2],cha[3],cha[4]);

#ifdef DEBUG_PARTIAL_HASH
    printf("(1) END ACUMULACION (%d)\n", rounds );
    printf("carrier[4] = [%016" PRId64 "]!\n",carrier[4] );
    printf("carrier[3] = [%016" PRId64 "]!\n",carrier[3] );
    printf("carrier[2] = [%016" PRId64 "]!\n",carrier[2] );
    printf("carrier[1] = [%016" PRId64 "]!\n",carrier[1] );
    printf("carrier[0] = [%016" PRId64 "]!\n",carrier[0] );
#endif


  hash1 = ((carrier[0] << 48) & 0xffffffffffffffffL ) |
          (((carrier[0]+carrier[1]) << 32) & 0xffffffffffffffffL ) |
          (((carrier[0]+carrier[1]+carrier[2]) << 16) & 0xffffffffL) |
          ((carrier[2]+carrier[3]+carrier[4]) & 0xffffffffL);

#ifdef DEBUG_PARTIAL_HASH
    printf("(3) END APILACION (%d)\n", rounds );
    printf("hash[0] = [%016" PRId64 "] !\n",hash[0] );
#endif

  hash1 += rounds;

#ifdef PRINT_HASH
  if( iteration >= iteraciones - numMostrar ) {
    char hex[17];
    sprintf(hex,"%016" PRIx64,hash1 );
    printf( "%s\n", hex );
  }
#endif

  // Do output val.
  for(j=0; j<8; j++ ) {
    val[7-j] = hash1 >> 8*j;
  }

  return val;
}

static inline char* getHash( int numIterations, char* val1, char* val2, int size ) {

  iteraciones = numIterations;
  rounds  = 0;

  val1 = eval_hash( val1, val2, size );
  for( iteration = 1; iteration<numIterations; iteration++ ) {
    val1 = eval_hash( val1, val2, 8 );
  }

  return val2;
}

int main(int argc, char *argv[]) {

  int iter = 0;
  if( argc < 2 ) {
    printf( "Error: need one argument to hash!\n" );
    return 1;
  } else if( argc == 2 ) {
    iter = 1;
  } else {
    iter = atoi(argv[2]);
    if( iter <= 0 ) {
        printf( "Error: second argument need to be positive!\n" );
        return 1;
    }
  }

  char val2[64]; // output 64 bytes/512 bits.
  char* val1 = argv[1]; // input
  int size = strlen(val1);

  getHash( iter, val1, val2, size );

  return 0;
}
