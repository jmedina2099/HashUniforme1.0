#include <stdio.h>
#include <string.h>

int DEBUG_PARTIAL_HASH = 0;

int rounds = 0;

signed long long evaluaFuncBool( signed long long char1,
								 signed long long char2,
								 signed long long char3,
								 signed long long char4,
								 signed long long char5 ) {

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

char* pad( char* data, unsigned long length, int padding, char* output ) {

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

char* eval_hash( char* input, char* val ) {

  unsigned long inputLength = strlen(input);
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

  signed long long IV1  = 0x6a09e667bb67ae85;
  signed long long IV2  = 0x3c6ef372a54ff53a;
  signed long long IV3  = 0x510e527f9b05688c;
  signed long long IV4  = 0x1f83d9ab5be0cd19;
  signed long long IV5  = 0x428a2f9871374491;
  signed long long IV6  = 0xb5c0fbcfe9b5dba5;
  signed long long IV7  = 0x3956c25b59f111f1;
  signed long long IV8  = 0x923f82a4ab1c5ed5;
  signed long long IV9  = 0xd807aa9812835b01;
  signed long long IV10 = 0x243185be550c7dc3;

  signed long long char1=IV1;
  signed long long char2=IV2;
  signed long long char3=IV3;
  signed long long char4=IV4;
  signed long long char5=IV5;
		
  signed long long sumaAnt1 = IV6;
  signed long long sumaAnt2 = IV7;
  signed long long sumaAnt3 = IV8;
  signed long long sumaAnt4 = IV9;
  signed long long sumaAnt5 = IV10;
  signed long long sumaAnt6 = IV1;
  signed long long sumaAnt7 = IV3;
  signed long long sumaAnt8 = IV8;
  
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
	  printf("END ACUMULACION\n" );
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
	  printf("END DISPERSION\n" );
	  printf("sumaAnt8 = [%lld]!\n",sumaAnt8 );
	  printf("sumaAnt7 = [%lld]!\n",sumaAnt7 );
	  printf("sumaAnt6 = [%lld]!\n",sumaAnt6 );
	  printf("sumaAnt5 = [%lld]!\n",sumaAnt5 );
	  printf("sumaAnt4 = [%lld]!\n",sumaAnt4 );
	  printf("sumaAnt3 = [%lld]!\n",sumaAnt3 );
	  printf("sumaAnt2 = [%lld]!\n",sumaAnt2 );
	  printf("sumaAnt1 = [%lld]!\n",sumaAnt1 );
  }

  signed long long hash1 = ((sumaAnt1 << 48) & 0xffffffffffffffffL ) |
         (((sumaAnt1+sumaAnt2) << 32) & 0xffffffffffffffffL ) |
         (((sumaAnt1+sumaAnt2+sumaAnt3) << 16) & 0xffffffffL) |
         ((sumaAnt3+sumaAnt4+sumaAnt5+sumaAnt6) & 0xffffffffL);
  signed long long hash2 = ((sumaAnt1 << 48) & 0xffffffffffffffffL ) |
         (((sumaAnt1+sumaAnt3) << 32) & 0xffffffffffffffffL ) |
         (((sumaAnt2+sumaAnt3+sumaAnt4) << 16) & 0xffffffffL) |
         ((sumaAnt4+sumaAnt5+sumaAnt1+sumaAnt7) & 0xffffffffL);
  signed long long hash3 = ((sumaAnt3 << 48) & 0xffffffffffffffffL ) |
         (((sumaAnt1+sumaAnt4) << 32) & 0xffffffffffffffffL ) |
         (((sumaAnt3+sumaAnt4+sumaAnt5) << 16) & 0xffffffffL) |
         ((sumaAnt5+sumaAnt1+sumaAnt2+sumaAnt8) & 0xffffffffL);
  signed long long hash4 = ((sumaAnt2 << 48) & 0xffffffffffffffffL ) |
         (((sumaAnt1+sumaAnt5) << 32) & 0xffffffffffffffffL ) |
         (((sumaAnt4+sumaAnt5+sumaAnt1) << 16) & 0xffffffffL) |
         ((sumaAnt1+sumaAnt2+sumaAnt3+sumaAnt6) & 0xffffffffL);
  signed long long hash5 = ((sumaAnt2 << 48) & 0xffffffffffffffffL ) |
         (((sumaAnt1+sumaAnt1) << 32) & 0xffffffffffffffffL ) |
         (((sumaAnt5+sumaAnt1+sumaAnt2) << 16) & 0xffffffffL) |
         ((sumaAnt2+sumaAnt3+sumaAnt4+sumaAnt7) & 0xffffffffL);
  signed long long hash6 = ((sumaAnt1 << 48) & 0xffffffffffffffffL ) |
         (((sumaAnt1+sumaAnt4) << 32) & 0xffffffffffffffffL ) |
         (((sumaAnt2+sumaAnt4+sumaAnt5) << 16) & 0xffffffffL) |
         ((sumaAnt3+sumaAnt6+sumaAnt7+sumaAnt8) & 0xffffffffL);
  signed long long hash7 = ((sumaAnt3 << 48) & 0xffffffffffffffffL ) |
         (((sumaAnt1+sumaAnt5) << 32) & 0xffffffffffffffffL ) |
         (((sumaAnt4+sumaAnt5+sumaAnt6) << 16) & 0xffffffffL) |
         ((sumaAnt7+sumaAnt8+sumaAnt1+sumaAnt2) & 0xffffffffL);
  signed long long hash8 = ((sumaAnt3 << 48) & 0xffffffffffffffffL ) |
         (((sumaAnt1+sumaAnt6) << 32) & 0xffffffffffffffffL ) |
         (((sumaAnt7+sumaAnt1+sumaAnt2) << 16) & 0xffffffffL) |
         ((sumaAnt5+sumaAnt6+sumaAnt7+sumaAnt8) & 0xffffffffL);

  if( DEBUG_PARTIAL_HASH ) {
	  printf("END APILACION\n" );
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


  sprintf(val,"%016llx%016llx%016llx%016llx%016llx%016llx%016llx%016llx",hash1,hash2,hash3,hash4,hash5,hash6,hash7,hash8+rounds );

  return val;
}

int main(int argc, char *argv[]) {

  // printf() displays the string inside quotation
  char val[128];
  printf("[%s]=[%s]\n",argv[1],eval_hash( argv[1], val ) );

  return 0;
}
