/* Copyright (C) 2007-2020 - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Jorge Medina <medinarosas.jorgealberto@gmail.com>, October 7th, 2020
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int DEBUG_PARTIAL_HASH = 0;

int rounds = 0;
int iteration = 0;

static signed long long IV1  = 0x6a09e667bb67ae85;
static signed long long IV2  = 0x3c6ef372a54ff53a;
static signed long long IV3  = 0x510e527f9b05688c;
static signed long long IV4  = 0x1f83d9ab5be0cd19;
static signed long long IV5  = 0x428a2f9871374491;
static signed long long IV6  = 0xb5c0fbcfe9b5dba5;
static signed long long IV7  = 0x3956c25b59f111f1;
static signed long long IV8  = 0x923f82a4ab1c5ed5;
static signed long long IV9  = 0xd807aa9812835b01;
static signed long long IV10 = 0x243185be550c7dc3;

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

char* pad( const char* data, unsigned long length, int padding, char* output ) {

  char pad[padding];
  pad[0] = (char) 0x80;
  signed long long bits = length * 8;
  int i=0;
  for( ; i < 8; i++ ) {
	 pad[padding - 1 - i] = (char) ((bits >> (8 * i)) & 0xFF);
  }
  i=1;
  for( ; i<=padding-1-8; i++ ) {
	 pad[i] = (char)0;
  }

  i=0;
  for( ; i<length; i++ ) {
	  output[i] = data[i];
  }

  i=0;
  for( ; i<padding; i++ ) {
	  output[length+i] = pad[i];
  }

  return output;
}

char* eval_hash( char* input, char* val, unsigned long inputLength, int printHash ) {

  int tail = inputLength % 64;
  int padding;

  if ((64 - tail >= 9)) {
    padding = 64 - tail;
  } else {
    padding = 128 - tail;
  }

  char output[inputLength+padding];

  input = pad(input,inputLength,padding,output);
  inputLength = inputLength+padding;

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
  int i=1;
  for( ;  i<inputLength-1 ; i++ ) {
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

  if( DEBUG_PARTIAL_HASH ) {
	  printf("END ACUMULACION (%d)\n", rounds );
	  printf("sumaAnt8 = [%lld]!\n",sumaAnt8 );
	  printf("sumaAnt7 = [%lld]!\n",sumaAnt7 );
	  printf("sumaAnt6 = [%lld]!\n",sumaAnt6 );
	  printf("sumaAnt5 = [%lld]!\n",sumaAnt5 );
	  printf("sumaAnt4 = [%lld]!\n",sumaAnt4 );
	  printf("sumaAnt3 = [%lld]!\n",sumaAnt3 );
	  printf("sumaAnt2 = [%lld]!\n",sumaAnt2 );
	  printf("sumaAnt1 = [%lld]!\n",sumaAnt1 );
  }

  sumaAnt1 += evaluaFuncBool( sumaAnt1,sumaAnt1,sumaAnt1,sumaAnt1,sumaAnt1) + IV1;
  sumaAnt2 += evaluaFuncBool( sumaAnt2,sumaAnt2,sumaAnt2,sumaAnt2,sumaAnt2) + IV2;
  sumaAnt3 += evaluaFuncBool( sumaAnt3,sumaAnt3,sumaAnt3,sumaAnt3,sumaAnt3) + IV3;
  sumaAnt4 += evaluaFuncBool( sumaAnt4,sumaAnt4,sumaAnt4,sumaAnt4,sumaAnt4) + IV4;
  sumaAnt5 += evaluaFuncBool( sumaAnt5,sumaAnt5,sumaAnt5,sumaAnt5,sumaAnt5) + IV5;
  sumaAnt6 += evaluaFuncBool( sumaAnt6,sumaAnt6,sumaAnt6,sumaAnt6,sumaAnt6) + IV6;
  sumaAnt7 += evaluaFuncBool( sumaAnt7,sumaAnt7,sumaAnt7,sumaAnt7,sumaAnt7) + IV7;
  sumaAnt8 += evaluaFuncBool( sumaAnt8,sumaAnt8,sumaAnt8,sumaAnt8,sumaAnt8) + IV8;

  if( DEBUG_PARTIAL_HASH ) {
	  printf("END DISPERSION (%d)\n", rounds );
	  printf("sumaAnt8 = [%lld]!\n",sumaAnt8 );
	  printf("sumaAnt7 = [%lld]!\n",sumaAnt7 );
	  printf("sumaAnt6 = [%lld]!\n",sumaAnt6 );
	  printf("sumaAnt5 = [%lld]!\n",sumaAnt5 );
	  printf("sumaAnt4 = [%lld]!\n",sumaAnt4 );
	  printf("sumaAnt3 = [%lld]!\n",sumaAnt3 );
	  printf("sumaAnt2 = [%lld]!\n",sumaAnt2 );
	  printf("sumaAnt1 = [%lld]!\n",sumaAnt1 );
  }

  register signed long long hash1 = ((sumaAnt1 << 48) & 0xffffffffffffffffL ) |
         (((sumaAnt1+sumaAnt2) << 32) & 0xffffffffffffffffL ) |
         (((sumaAnt1+sumaAnt2+sumaAnt3) << 16) & 0xffffffffL) |
         ((sumaAnt3+sumaAnt4+sumaAnt5+sumaAnt6) & 0xffffffffL);
  register signed long long hash2 = ((sumaAnt1 << 48) & 0xffffffffffffffffL ) |
         (((sumaAnt1+sumaAnt3) << 32) & 0xffffffffffffffffL ) |
         (((sumaAnt2+sumaAnt3+sumaAnt4) << 16) & 0xffffffffL) |
         ((sumaAnt4+sumaAnt5+sumaAnt1+sumaAnt7) & 0xffffffffL);
  register signed long long hash3 = ((sumaAnt3 << 48) & 0xffffffffffffffffL ) |
         (((sumaAnt1+sumaAnt4) << 32) & 0xffffffffffffffffL ) |
         (((sumaAnt3+sumaAnt4+sumaAnt5) << 16) & 0xffffffffL) |
         ((sumaAnt5+sumaAnt1+sumaAnt2+sumaAnt8) & 0xffffffffL);
  register signed long long hash4 = ((sumaAnt2 << 48) & 0xffffffffffffffffL ) |
         (((sumaAnt1+sumaAnt5) << 32) & 0xffffffffffffffffL ) |
         (((sumaAnt4+sumaAnt5+sumaAnt1) << 16) & 0xffffffffL) |
         ((sumaAnt1+sumaAnt2+sumaAnt3+sumaAnt6) & 0xffffffffL);
  register signed long long hash5 = ((sumaAnt2 << 48) & 0xffffffffffffffffL ) |
         (((sumaAnt1+sumaAnt1) << 32) & 0xffffffffffffffffL ) |
         (((sumaAnt5+sumaAnt1+sumaAnt2) << 16) & 0xffffffffL) |
         ((sumaAnt2+sumaAnt3+sumaAnt4+sumaAnt7) & 0xffffffffL);
  register signed long long hash6 = ((sumaAnt1 << 48) & 0xffffffffffffffffL ) |
         (((sumaAnt1+sumaAnt4) << 32) & 0xffffffffffffffffL ) |
         (((sumaAnt2+sumaAnt4+sumaAnt5) << 16) & 0xffffffffL) |
         ((sumaAnt3+sumaAnt6+sumaAnt7+sumaAnt8) & 0xffffffffL);
  register signed long long hash7 = ((sumaAnt3 << 48) & 0xffffffffffffffffL ) |
         (((sumaAnt1+sumaAnt5) << 32) & 0xffffffffffffffffL ) |
         (((sumaAnt4+sumaAnt5+sumaAnt6) << 16) & 0xffffffffL) |
         ((sumaAnt7+sumaAnt8+sumaAnt1+sumaAnt2) & 0xffffffffL);
  register signed long long hash8 = ((sumaAnt3 << 48) & 0xffffffffffffffffL ) |
         (((sumaAnt1+sumaAnt6) << 32) & 0xffffffffffffffffL ) |
         (((sumaAnt7+sumaAnt1+sumaAnt2) << 16) & 0xffffffffL) |
         ((sumaAnt5+sumaAnt6+sumaAnt7+sumaAnt8) & 0xffffffffL);

  if( DEBUG_PARTIAL_HASH ) {
	  printf("END APILACION (%d)\n", rounds );
	  printf("hash8 = [%lld]!\n",hash8 );
	  printf("hash7 = [%lld]!\n",hash7 );
	  printf("hash6 = [%lld]!\n",hash6 );
	  printf("hash5 = [%lld]!\n",hash5 );
	  printf("hash4 = [%lld]!\n",hash4 );
	  printf("hash3 = [%lld]!\n",hash3 );
	  printf("hash2 = [%lld]!\n",hash2 );
	  printf("hash1 = [%lld]!\n",hash1 );
  }

  hash1 += evaluaFuncBool( hash1,hash1,hash1,hash1,hash1) + IV6;
  hash2 += evaluaFuncBool( hash2,hash2,hash2,hash2,hash2) + IV7;
  hash3 += evaluaFuncBool( hash3,hash3,hash3,hash3,hash3) + IV8;
  hash4 += evaluaFuncBool( hash4,hash4,hash4,hash4,hash4) + IV9;
  hash5 += evaluaFuncBool( hash5,hash5,hash5,hash5,hash5) + IV10;
  hash6 += evaluaFuncBool( hash6,hash6,hash6,hash6,hash6) + IV1;
  hash7 += evaluaFuncBool( hash7,hash7,hash7,hash7,hash7) + IV3;
  hash8 += evaluaFuncBool( hash8,hash8,hash8,hash8,hash8) + IV8;

  if( DEBUG_PARTIAL_HASH ) {
	  printf("END DISPERSION FINAL (%d) \n",rounds );
	  printf("hash8 = [%lld]!\n",hash8 );
	  printf("hash7 = [%lld]!\n",hash7 );
	  printf("hash6 = [%lld]!\n",hash6 );
	  printf("hash5 = [%lld]!\n",hash5 );
	  printf("hash4 = [%lld]!\n",hash4 );
	  printf("hash3 = [%lld]!\n",hash3 );
	  printf("hash2 = [%lld]!\n",hash2 );
	  printf("hash1 = [%lld]!\n\n",hash1 );
  }

  if( printHash ) {
	  char hex[129];
	  sprintf(hex,"%016llx%016llx%016llx%016llx%016llx%016llx%016llx%016llx",hash1,hash2,hash3,hash4,hash5,hash6,hash7,(hash8+rounds) );
	  printf( "%s\n", hex );
  }

  /*
  int str_len = 128;
  int j;
  for (j = 0; j < (str_len / 2); j++) {
    sscanf(hex + 2*j, "%02x", &val[j]);
    printf( "(val)[%d]=%d\n",j,val[j]);
  }
  */

  val[7] = hash1;
  val[6] = hash1 >> 8;
  val[5] = hash1 >> 16;
  val[4] = hash1 >> 24;
  val[3] = hash1 >> 32;
  val[2] = hash1 >> 40;
  val[1] = hash1 >> 48;
  val[0] = hash1 >> 56;

  val[15] = hash2;
  val[14] = hash2 >> 8;
  val[13] = hash2 >> 16;
  val[12] = hash2 >> 24;
  val[11] = hash2 >> 32;
  val[10] = hash2 >> 40;
  val[9] = hash2 >> 48;
  val[8] = hash2 >> 56;

  val[23] = hash3;
  val[22] = hash3 >> 8;
  val[21] = hash3 >> 16;
  val[20] = hash3 >> 24;
  val[19] = hash3 >> 32;
  val[18] = hash3 >> 40;
  val[17] = hash3 >> 48;
  val[16] = hash3 >> 56;

  val[31] = hash4;
  val[30] = hash4 >> 8;
  val[29] = hash4 >> 16;
  val[28] = hash4 >> 24;
  val[27] = hash4 >> 32;
  val[26] = hash4 >> 40;
  val[25] = hash4 >> 48;
  val[24] = hash4 >> 56;

  val[39] = hash5;
  val[38] = hash5 >> 8;
  val[37] = hash5 >> 16;
  val[36] = hash5 >> 24;
  val[35] = hash5 >> 32;
  val[34] = hash5 >> 40;
  val[33] = hash5 >> 48;
  val[32] = hash5 >> 56;

  val[47] = hash6;
  val[46] = hash6 >> 8;
  val[45] = hash6 >> 16;
  val[44] = hash6 >> 24;
  val[43] = hash6 >> 32;
  val[42] = hash6 >> 40;
  val[41] = hash6 >> 48;
  val[40] = hash6 >> 56;

  val[55] = hash7;
  val[54] = hash7 >> 8;
  val[53] = hash7 >> 16;
  val[52] = hash7 >> 24;
  val[51] = hash7 >> 32;
  val[50] = hash7 >> 40;
  val[49] = hash7 >> 48;
  val[48] = hash7 >> 56;

  hash8 += rounds;
  val[63] = hash8;
  val[62] = hash8 >> 8;
  val[61] = hash8 >> 16;
  val[60] = hash8 >> 24;
  val[59] = hash8 >> 32;
  val[58] = hash8 >> 40;
  val[57] = hash8 >> 48;
  val[56] = hash8 >> 56;

  return val;
}

void printValues( char* val1, char* val2 ) {
  printf("[%s]=[%s]\n",val1, val2 );
}

static inline char* getHash( int numIterations, char* val1, char* val2, unsigned int size, int printHash ) {

  rounds  = 0;
  iteration  = 0;

  for( ; iteration<numIterations; ) {
    iteration++;
    val2 = eval_hash( val1, val2, size, printHash );
    size = 64;

    int j=0;
    for( ;j<64 ;j++ ) {
    	val1[j] = val2[j];
    }

    j=0;
    for( ;j<64;j++ ) {
    	val2[j] = 0;
    }


  }

  return val2;
}

int main(int argc, char *argv[]) {


  char* val1 = argv[1];
  unsigned int size = strlen(val1);
  char val2[64];

  getHash( atoi(argv[2]), val1,val2, size, atoi(argv[3]) );
  //printValues( argv[1],val2 );

  return 0;
}
