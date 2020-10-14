/* Copyright (C) 2007-2020 - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential (eyes-only) (only-military)
 * Written by Jorge Medina <medinarosas.jorgealberto@gmail.com>, October 14th, 2020
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

static const uint64_t IV[10] = {
		0x6a09e667bb67ae85LL,
		0x3c6ef372a54ff53aLL,
		0x510e527f9b05688cLL,
		0x1f83d9ab5be0cd19LL,
		0x428a2f9871374491LL,
		0xb5c0fbcfe9b5dba5LL,
		0x3956c25b59f111f1LL,
		0x923f82a4ab1c5ed5LL,
		0xd807aa9812835b01LL,
		0x243185be550c7dc3LL
};

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
  uint64_t cha[5] = {IV[0],IV[1],IV[2],IV[3],IV[4]};
  uint64_t carrier[8] = {IV[5],IV[6],IV[7],IV[8],IV[9],IV[0],IV[2],IV[7]};
  uint64_t hash[12];

  // First iteration.
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
  carrier[0] += evaluaFuncBool( cha[0],cha[1],cha[2],cha[3],cha[4]);
  
  // Main Loop.
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
    carrier[0] += evaluaFuncBool( cha[0],cha[1],cha[2],cha[3],cha[4]);
  }

  // Last iteration.
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
  carrier[0] += evaluaFuncBool( cha[0],cha[1],cha[2],cha[3],cha[4]);

#ifdef DEBUG_PARTIAL_HASH
    printf("(1) END ACUMULACION (%d)\n", rounds );
    printf("carrier[7] = [%016" PRId64 "]!\n",carrier[7] );
    printf("carrier[6] = [%016" PRId64 "]!\n",carrier[6] );
    printf("carrier[5] = [%016" PRId64 "]!\n",carrier[5] );
    printf("carrier[4] = [%016" PRId64 "]!\n",carrier[4] );
    printf("carrier[3] = [%016" PRId64 "]!\n",carrier[3] );
    printf("carrier[2] = [%016" PRId64 "]!\n",carrier[2] );
    printf("carrier[1] = [%016" PRId64 "]!\n",carrier[1] );
    printf("carrier[0] = [%016" PRId64 "]!\n",carrier[0] );
#endif

  carrier[0] += evaluaFuncBool( carrier[0],carrier[0],carrier[0],carrier[0],carrier[0]) + IV[7];
  carrier[1] += evaluaFuncBool( carrier[1],carrier[1],carrier[1],carrier[1],carrier[1]) + IV[6];
  carrier[2] += evaluaFuncBool( carrier[2],carrier[2],carrier[2],carrier[2],carrier[2]) + IV[5];
  carrier[3] += evaluaFuncBool( carrier[3],carrier[3],carrier[3],carrier[3],carrier[3]) + IV[4];
  carrier[4] += evaluaFuncBool( carrier[4],carrier[4],carrier[4],carrier[4],carrier[4]) + IV[3];
  carrier[5] += evaluaFuncBool( carrier[5],carrier[5],carrier[5],carrier[5],carrier[5]) + IV[2];
  carrier[6] += evaluaFuncBool( carrier[6],carrier[6],carrier[6],carrier[6],carrier[6]) + IV[1];
  carrier[7] += evaluaFuncBool( carrier[7],carrier[7],carrier[7],carrier[7],carrier[7]) + IV[0];

#ifdef DEBUG_PARTIAL_HASH
    printf("(2) END DISPERSION (%d)\n", rounds );
    printf("carrier[7] = [%016" PRId64 "]!\n",carrier[7] );
    printf("carrier[6] = [%016" PRId64 "]!\n",carrier[6] );
    printf("carrier[5] = [%016" PRId64 "]!\n",carrier[5] );
    printf("carrier[4] = [%016" PRId64 "]!\n",carrier[4] );
    printf("carrier[3] = [%016" PRId64 "]!\n",carrier[3] );
    printf("carrier[2] = [%016" PRId64 "]!\n",carrier[2] );
    printf("carrier[1] = [%016" PRId64 "]!\n",carrier[1] );
    printf("carrier[0] = [%016" PRId64 "]!\n",carrier[0] );
#endif

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

#ifdef DEBUG_PARTIAL_HASH
    printf("(3) END APILACION (%d)\n", rounds );
    printf("hash[11]= [%016" PRId64 "]!\n",hash[11] );
    printf("hash[10]= [%016" PRId64 "]!\n",hash[10] );
    printf("hash[9] = [%016" PRId64 "]!\n",hash[9] );
    printf("hash[8] = [%016" PRId64 "]!\n",hash[8] );
    printf("hash[7] = [%016" PRId64 "]!\n",hash[7] );
    printf("hash[6] = [%016" PRId64 "]!\n",hash[6] );
    printf("hash[5] = [%016" PRId64 "]!\n",hash[5] );
    printf("hash[4] = [%016" PRId64 "]!\n",hash[4] );
    printf("hash[3] = [%016" PRId64 "]!\n",hash[3] );
    printf("hash[2] = [%016" PRId64 "]!\n",hash[2] );
    printf("hash[1] = [%016" PRId64 "]!\n",hash[1] );
    printf("hash[0] = [%016" PRId64 "]!\n",hash[0] );
#endif

  hash[0] += evaluaFuncBool( hash[0],hash[0],hash[0],hash[0],hash[0]) + IV[5];
  hash[1] += evaluaFuncBool( hash[1],hash[1],hash[1],hash[1],hash[1]) + IV[6];
  hash[2] += evaluaFuncBool( hash[2],hash[2],hash[2],hash[2],hash[2]) + IV[7];
  hash[3] += evaluaFuncBool( hash[3],hash[3],hash[3],hash[3],hash[3]) + IV[8];
  hash[4] += evaluaFuncBool( hash[4],hash[4],hash[4],hash[4],hash[4]) + IV[9];
  hash[5] += evaluaFuncBool( hash[5],hash[5],hash[5],hash[5],hash[5]) + IV[0];
  hash[6] += evaluaFuncBool( hash[6],hash[6],hash[6],hash[6],hash[6]) + IV[2];
  hash[7] += evaluaFuncBool( hash[7],hash[7],hash[7],hash[7],hash[7]) + IV[7];
  hash[8] += evaluaFuncBool( hash[8],hash[8],hash[8],hash[8],hash[8]) + IV[4];
  hash[9] += evaluaFuncBool( hash[9],hash[9],hash[9],hash[9],hash[9]) + IV[6];
  hash[10] += evaluaFuncBool( hash[10],hash[10],hash[10],hash[10],hash[10]) + IV[9];
  hash[11] += evaluaFuncBool( hash[11],hash[11],hash[11],hash[11],hash[11]) + IV[3];

#ifdef DEBUG_PARTIAL_HASH
    printf("(4) END DISPERSION FINAL (%d) \n",rounds );
    printf("hash[11]= [%016" PRId64 "]!\n",hash[11] );
    printf("hash[10]= [%016" PRId64 "]!\n",hash[10] );
    printf("hash[9] = [%016" PRId64 "]!\n",hash[9] );
    printf("hash[8] = [%016" PRId64 "]!\n",hash[8] );
    printf("hash[7] = [%016" PRId64 "]!\n",hash[7] );
    printf("hash[6] = [%016" PRId64 "]!\n",hash[6] );
    printf("hash[5] = [%016" PRId64 "]!\n",hash[5] );
    printf("hash[4] = [%016" PRId64 "]!\n",hash[4] );
    printf("hash[3] = [%016" PRId64 "]!\n",hash[3] );
    printf("hash[2] = [%016" PRId64 "]!\n",hash[2] );
    printf("hash[1] = [%016" PRId64 "]!\n",hash[1] );
    printf("hash[0] = [%016" PRId64 "]!\n",hash[0] );
#endif

  // We add the number of rounds to output.
  hash[11] += rounds;

#ifdef PRINT_HASH
  if( iteration >= iteraciones - numMostrar ) {
    char hex[193];
    sprintf(hex,"%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64,hash[0],hash[1],hash[2],hash[3],hash[4],hash[5],hash[6],hash[7],hash[8],hash[9],hash[10],hash[11] );
    printf( "%s\n", hex );
  }
#endif

  // Do output val.
  for(i=0; i<12; i++ ) {
    for(j=0; j<8; j++ ) {
      val[8*(i+1)-1-j] = hash[i] >> 8*j;
    }
  }

  return val;
}

static inline char* getHash( int numIterations, char* val1, char* val2, int size ) {

  iteraciones = numIterations;
  rounds  = 0;

  val1 = eval_hash( val1, val2, size );
  for( iteration = 1; iteration<numIterations; iteration++ ) {
    val1 = eval_hash( val1, val2, 96 );
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

  char val2[96]; // output 96 bytes/768 bits.
  char* val1 = argv[1]; // input
  int size = strlen(val1);

  getHash( iter, val1, val2, size );

  return 0;
}
