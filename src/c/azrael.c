/* Copyright (C) 2007-2020 - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Jorge Medina <medinarosas.jorgealberto@gmail.com>, October 7th, 2020
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

static int rounds = 0;
static int iteration = 0;
static int iteraciones = 0;

#ifdef PRINT_HASH
  static const int numMostrar = 20;
#endif

static const signed long long IV1  = 0x6a09e667bb67ae85LL;
static const signed long long IV2  = 0x3c6ef372a54ff53aLL;
static const signed long long IV3  = 0x510e527f9b05688cLL;
static const signed long long IV4  = 0x1f83d9ab5be0cd19LL;
static const signed long long IV5  = 0x428a2f9871374491LL;
static const signed long long IV6  = 0xb5c0fbcfe9b5dba5LL;
static const signed long long IV7  = 0x3956c25b59f111f1LL;
static const signed long long IV8  = 0x923f82a4ab1c5ed5LL;
static const signed long long IV9  = 0xd807aa9812835b01LL;
static const signed long long IV10 = 0x243185be550c7dc3LL;

static inline signed long long evaluaFuncBool( const signed long long char1,
		const signed long long char2,
		const signed long long char3,
		const signed long long char4,
		const signed long long char5 ) {
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
  const signed long long bits = length * 8;

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

  const int tail = inputLength % 64;
  int padding = 64 - tail >= 9? 64 - tail: 128 - tail;

  char output[inputLength+padding];
  input = pad(input,inputLength,padding,output);
  inputLength += padding;

  register signed long long char1=IV1;
  register signed long long char2=IV2;
  register signed long long char3=IV3;
  register signed long long char4=IV4;
  register signed long long char5=IV5;
		
  register signed long long sumaAnt1 = IV6;
  register signed long long sumaAnt2 = IV7;
  register signed long long sumaAnt3 = IV8;
  register signed long long sumaAnt4 = IV9;
  register signed long long sumaAnt5 = IV10;
  register signed long long sumaAnt6 = IV1;
  register signed long long sumaAnt7 = IV3;
  register signed long long sumaAnt8 = IV8;
  
  char1 += (signed long long)input[ inputLength-2 ];
  char2 += (signed long long)input[ inputLength-1 ];
  char3 += (signed long long)input[ 0 ];
  char4 += (signed long long)input[ 1 ]; 
  char5 += (signed long long)input[ 2 ];

  sumaAnt8 += sumaAnt7;
  sumaAnt7 += sumaAnt6;
  sumaAnt6 += sumaAnt5;
  sumaAnt5 += sumaAnt4;
  sumaAnt4 += sumaAnt3;
  sumaAnt3 += sumaAnt2;
  sumaAnt2 += sumaAnt1;
  sumaAnt1 += evaluaFuncBool( char1,char2,char3,char4,char5);
  
  // Main Loop.
  int i;
  for( i=1; i<inputLength-1; i++ ) {
    char1 += sumaAnt1;
    char2 += char3;
    char3 += char4;
    char4 += (signed long long)input[i+1];
    char5 += sumaAnt2;
    sumaAnt8 += sumaAnt7;
    sumaAnt7 += sumaAnt6;
    sumaAnt6 += sumaAnt5;
    sumaAnt5 += sumaAnt4;
    sumaAnt4 += sumaAnt3;
    sumaAnt3 += sumaAnt2;
    sumaAnt2 += sumaAnt1;
    sumaAnt1 += evaluaFuncBool( char1,char2,char3,char4,char5);
  }

  char1 += sumaAnt1;
  char2 += char3;
  char3 += char4;
  char4 += (signed long long)input[ 0 ];
  char5 += sumaAnt2;
  sumaAnt8 += sumaAnt7;
  sumaAnt7 += sumaAnt6;
  sumaAnt6 += sumaAnt5;
  sumaAnt5 += sumaAnt4;
  sumaAnt4 += sumaAnt3;
  sumaAnt3 += sumaAnt2;
  sumaAnt2 += sumaAnt1;
  sumaAnt1 += evaluaFuncBool( char1,char2,char3,char4,char5);

#ifdef DEBUG_PARTIAL_HASH
    printf("(1) END ACUMULACION (%d)\n", rounds );
    printf("sumaAnt8 = [%lld]!\n",sumaAnt8 );
    printf("sumaAnt7 = [%lld]!\n",sumaAnt7 );
    printf("sumaAnt6 = [%lld]!\n",sumaAnt6 );
    printf("sumaAnt5 = [%lld]!\n",sumaAnt5 );
    printf("sumaAnt4 = [%lld]!\n",sumaAnt4 );
    printf("sumaAnt3 = [%lld]!\n",sumaAnt3 );
    printf("sumaAnt2 = [%lld]!\n",sumaAnt2 );
    printf("sumaAnt1 = [%lld]!\n",sumaAnt1 );
#endif

  sumaAnt1 += evaluaFuncBool( sumaAnt1,sumaAnt1,sumaAnt1,sumaAnt1,sumaAnt1) + IV1;
  sumaAnt2 += evaluaFuncBool( sumaAnt2,sumaAnt2,sumaAnt2,sumaAnt2,sumaAnt2) + IV2;
  sumaAnt3 += evaluaFuncBool( sumaAnt3,sumaAnt3,sumaAnt3,sumaAnt3,sumaAnt3) + IV3;
  sumaAnt4 += evaluaFuncBool( sumaAnt4,sumaAnt4,sumaAnt4,sumaAnt4,sumaAnt4) + IV4;
  sumaAnt5 += evaluaFuncBool( sumaAnt5,sumaAnt5,sumaAnt5,sumaAnt5,sumaAnt5) + IV5;
  sumaAnt6 += evaluaFuncBool( sumaAnt6,sumaAnt6,sumaAnt6,sumaAnt6,sumaAnt6) + IV6;
  sumaAnt7 += evaluaFuncBool( sumaAnt7,sumaAnt7,sumaAnt7,sumaAnt7,sumaAnt7) + IV7;
  sumaAnt8 += evaluaFuncBool( sumaAnt8,sumaAnt8,sumaAnt8,sumaAnt8,sumaAnt8) + IV8;

#ifdef DEBUG_PARTIAL_HASH
    printf("(2) END DISPERSION (%d)\n", rounds );
    printf("sumaAnt8 = [%lld]!\n",sumaAnt8 );
    printf("sumaAnt7 = [%lld]!\n",sumaAnt7 );
    printf("sumaAnt6 = [%lld]!\n",sumaAnt6 );
    printf("sumaAnt5 = [%lld]!\n",sumaAnt5 );
    printf("sumaAnt4 = [%lld]!\n",sumaAnt4 );
    printf("sumaAnt3 = [%lld]!\n",sumaAnt3 );
    printf("sumaAnt2 = [%lld]!\n",sumaAnt2 );
    printf("sumaAnt1 = [%lld]!\n",sumaAnt1 );
#endif

  signed long long hash[8];
  hash[0] = ((sumaAnt1 << 48) & 0xffffffffffffffffL ) |
            (((sumaAnt1+sumaAnt2) << 32) & 0xffffffffffffffffL ) |
            (((sumaAnt1+sumaAnt2+sumaAnt3) << 16) & 0xffffffffL) |
            ((sumaAnt3+sumaAnt4+sumaAnt5+sumaAnt6) & 0xffffffffL);
  hash[1] = ((sumaAnt1 << 48) & 0xffffffffffffffffL ) |
            (((sumaAnt1+sumaAnt3) << 32) & 0xffffffffffffffffL ) |
            (((sumaAnt2+sumaAnt3+sumaAnt4) << 16) & 0xffffffffL) |
            ((sumaAnt4+sumaAnt5+sumaAnt1+sumaAnt7) & 0xffffffffL);
  hash[2] = ((sumaAnt3 << 48) & 0xffffffffffffffffL ) |
            (((sumaAnt1+sumaAnt4) << 32) & 0xffffffffffffffffL ) |
            (((sumaAnt3+sumaAnt4+sumaAnt5) << 16) & 0xffffffffL) |
            ((sumaAnt5+sumaAnt1+sumaAnt2+sumaAnt8) & 0xffffffffL);
  hash[3] = ((sumaAnt2 << 48) & 0xffffffffffffffffL ) |
            (((sumaAnt1+sumaAnt5) << 32) & 0xffffffffffffffffL ) |
            (((sumaAnt4+sumaAnt5+sumaAnt1) << 16) & 0xffffffffL) |
            ((sumaAnt1+sumaAnt2+sumaAnt3+sumaAnt6) & 0xffffffffL);
  hash[4] = ((sumaAnt2 << 48) & 0xffffffffffffffffL ) |
            (((sumaAnt1+sumaAnt1) << 32) & 0xffffffffffffffffL ) |
            (((sumaAnt5+sumaAnt1+sumaAnt2) << 16) & 0xffffffffL) |
            ((sumaAnt2+sumaAnt3+sumaAnt4+sumaAnt7) & 0xffffffffL);
  hash[5] = ((sumaAnt1 << 48) & 0xffffffffffffffffL ) |
            (((sumaAnt1+sumaAnt4) << 32) & 0xffffffffffffffffL ) |
            (((sumaAnt2+sumaAnt4+sumaAnt5) << 16) & 0xffffffffL) |
            ((sumaAnt3+sumaAnt6+sumaAnt7+sumaAnt8) & 0xffffffffL);
  hash[6] = ((sumaAnt3 << 48) & 0xffffffffffffffffL ) |
            (((sumaAnt1+sumaAnt5) << 32) & 0xffffffffffffffffL ) |
            (((sumaAnt4+sumaAnt5+sumaAnt6) << 16) & 0xffffffffL) |
            ((sumaAnt7+sumaAnt8+sumaAnt1+sumaAnt2) & 0xffffffffL);
  hash[7] = ((sumaAnt3 << 48) & 0xffffffffffffffffL ) |
            (((sumaAnt1+sumaAnt6) << 32) & 0xffffffffffffffffL ) |
            (((sumaAnt7+sumaAnt1+sumaAnt2) << 16) & 0xffffffffL) |
            ((sumaAnt5+sumaAnt6+sumaAnt7+sumaAnt8) & 0xffffffffL);

#ifdef DEBUG_PARTIAL_HASH
    printf("(3) END APILACION (%d)\n", rounds );
    printf("hash[7] = [%lld]!\n",hash[7] );
    printf("hash[6] = [%lld]!\n",hash[6] );
    printf("hash[5] = [%lld]!\n",hash[5] );
    printf("hash[4] = [%lld]!\n",hash[4] );
    printf("hash[3] = [%lld]!\n",hash[3] );
    printf("hash[2] = [%lld]!\n",hash[2] );
    printf("hash[1] = [%lld]!\n",hash[1] );
    printf("hash[0] = [%lld]!\n",hash[0] );
#endif

  hash[0] += evaluaFuncBool( hash[0],hash[0],hash[0],hash[0],hash[0]) + IV6;
  hash[1] += evaluaFuncBool( hash[1],hash[1],hash[1],hash[1],hash[1]) + IV7;
  hash[2] += evaluaFuncBool( hash[2],hash[2],hash[2],hash[2],hash[2]) + IV8;
  hash[3] += evaluaFuncBool( hash[3],hash[3],hash[3],hash[3],hash[3]) + IV9;
  hash[4] += evaluaFuncBool( hash[4],hash[4],hash[4],hash[4],hash[4]) + IV10;
  hash[5] += evaluaFuncBool( hash[5],hash[5],hash[5],hash[5],hash[5]) + IV1;
  hash[6] += evaluaFuncBool( hash[6],hash[6],hash[6],hash[6],hash[6]) + IV3;
  hash[7] += evaluaFuncBool( hash[7],hash[7],hash[7],hash[7],hash[7]) + IV8;

#ifdef DEBUG_PARTIAL_HASH
    printf("(4) END DISPERSION FINAL (%d) \n",rounds );
    printf("hash[7] = [%lld]!\n",hash[7] );
    printf("hash[6] = [%lld]!\n",hash[6] );
    printf("hash[5] = [%lld]!\n",hash[5] );
    printf("hash[4] = [%lld]!\n",hash[4] );
    printf("hash[3] = [%lld]!\n",hash[3] );
    printf("hash[2] = [%lld]!\n",hash[2] );
    printf("hash[1] = [%lld]!\n",hash[1] );
    printf("hash[0] = [%lld]!\n",hash[0] );
#endif

  // We add the number of rounds to output.
  hash[7] += rounds;

#ifdef PRINT_HASH
  if( iteration >= iteraciones - numMostrar ) {
    char hex[129];
    sprintf(hex,"%016llx%016llx%016llx%016llx%016llx%016llx%016llx%016llx",hash[0],hash[1],hash[2],hash[3],hash[4],hash[5],hash[6],hash[7] );
    printf( "%s\n", hex );
  }
#endif

  // Do output val.
  int j;
  for(i=0; i<8; i++ ) {
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
    val1 = eval_hash( val1, val2, 64 );
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
