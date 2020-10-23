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
#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>


#define UNIT_TEST 1

#include "azrael_imp_base.c"
#include "azrael_imp_64_link.c"
#include "azrael_imp_512_link.c"
#include "azrael_imp_x4_link.c"

#include "../sha/sha3.h"
#include "../siphash/csiphash.c"

void itera_x4( char* val1, long n, int flag, char* hex, uint64_t* hash, int a, int b) {

  long i;
  int k,j;
  int size = strlen(val1);
  char val2[96];

  for( i=0; i<n; i++ ) {
    eval_hash_x4( val1, hash, size, a, b );
    for(k=0; k<12; k++ ) {
      for(j=0; j<8; j++ ) {
        val2[8*(k+1)-1-j] = hash[k] >> 8*j;
      }
    }
    val1 = val2;
    size = 96;
  }

  if( flag != 0 ) {
	sprintf(hex,"%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64,hash[0],hash[1],hash[2],hash[3],hash[4],hash[5],hash[6],hash[7],hash[8],hash[9],hash[10],hash[11] );
    printf( "%s\n", hex );
  }
}

void test_x4( char* val1 ) {

  uint64_t hash[12]; // output 96 bytes/768 bits.
  eval_hash_x4( val1, hash, strlen(val1), 7, 7 );

  char hex[193];
  sprintf(hex,"%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64,hash[0],hash[1],hash[2],hash[3],hash[4],hash[5],hash[6],hash[7],hash[8],hash[9],hash[10],hash[11] );

  char* empty_hash = "6219986a754b80526556d98a6f67e19c32e5c68c9b3321cd40cba83372e0eeef9774287a573d1c4b4a102a501e2a9a9c1ffe501c67efa8559d66d7d1aaf6e76f454a46c642cf1cf9aaafe17a9f1c277541991420f6cd914894ff99d537770487";
  if( strcmp(hex,empty_hash) == 0 ) {
    printf( "TEST  768 bits of output (x4) OK!!!\n%s\n", hex );
  } else {
    printf( "TEST FAILED!\n%s\n", hex );
  }

}

void itera_512( char* val1, long n, int flag, char* hex, uint64_t* hash, int a, int b, int c) {

  long i;
  int j,k;
  int size = strlen(val1);
  char val2[64];

  for( i=0; i<n; i++ ) {
    eval_hash_512( val1, hash, size, a, b, c );
    for(k=0; k<8; k++ ) {
      for(j=0; j<8; j++ ) {
        val2[8*(k+1)-1-j] = hash[k] >> 8*j;
      }
    }
    val1 = val2;
    size = 64;
  }

  if( flag != 0 ) {
	sprintf(hex,"%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64,hash[0],hash[1],hash[2],hash[3],hash[4],hash[5],hash[6],hash[7] );
    printf( "%s\n", hex );
  }
}

void test_512( char* val1 ) {

	int a=2;
	int b=3;
	int c=6;

	uint64_t hash[8]; // output 64 bytes/512 bits.
  eval_hash_512( val1, hash, strlen(val1), a,b,c );

  char hex[129];
  sprintf(hex,"%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64,hash[0],hash[1],hash[2],hash[3],hash[4],hash[5],hash[6],hash[7] );

  char* empty_hash = "62199873880effb36556d98ab20a2c4f32e5c68a4bc49bbd40cba833785e3c2c9774287ab18dffbe195d0642c2c741e81ffe501b86c53034ae85b5315be5a12b";
  if( strcmp(hex,empty_hash) == 0 ) {
    printf( "TEST  512 bits of output OK!!!\n%s\n", hex );
  } else {
    printf( "TEST FAILED!\n%s\n", hex );
  }

}

void test_64( char* val1 ) {

	int a=4;
	int b=6;
	int c=6;

  uint64_t hash[1]; // output 8 bytes/64 bits.
  eval_hash_64( val1, hash, strlen(val1), a, b, c );

  char hex[17];
  sprintf(hex,"%016" PRIx64,hash[0] );

  char* empty_hash = "ac242eaf9248c6df";
  if( strcmp(hex,empty_hash) == 0 ) {
    printf( "TEST   64 bits of output OK!!!\n%s\n", hex );
  } else {
    printf( "TEST FAILED!\n%s\n", hex );
  }
}

void itera_64( char* val1, long n, int flag, char* hex, uint64_t* hash, int a, int b, int c) {

  long i;
  int j;
  int size = strlen(val1);
  char val2[8];

  for( i=0; i<n; i++ ) {
    eval_hash_64( val1, hash, size, a, b, c );
    for(j=0; j<8; j++ ) {
      val2[7-j] = hash[0] >> 8*j;
    }
    val1 = val2;
    size = 8;
  }

  if( flag != 0 ) {
	sprintf(hex,"%016" PRIx64,hash[0] );
    printf( "%s\n", hex );
  }
}

void itera( char* val1, long n, int flag, char* hex, char* spice, uint64_t* hash, int a, int b, int c ) {

	if( strcmp(spice,"64") == 0 ) {
		a = a==-1? 4: a;
		b = b==-1? 6: b;
		c = c==-1? 6: c;
		itera_64( val1, n, flag, hex, hash, a, b, c );
	} else if( strcmp(spice,"512") == 0 ) {
		a = a==-1? 2: a;
		b = b==-1? 3: b;
		c = c==-1? 6: c;
		itera_512( val1, n, flag, hex, hash, a, b, c );
	} else if( strcmp(spice,"x4") == 0 ) {
		a = a==-1? 7: a;
		b = b==-1? 7: b;
		itera_x4( val1, n, flag, hex, hash, a, b );
	}
}

int getNumHex(char* spice) {
  int sizeHex = 17;
  if( strcmp(spice,"64") == 0 ) {
	sizeHex = 17;
  } else if( strcmp(spice,"512") == 0 ) {
	sizeHex = 129;
  } else if( strcmp(spice,"x4") == 0 ) {
	sizeHex = 193;
  }
  return sizeHex;
}

int getOutSize(char* spice) {
  int size = 1;
  if( strcmp(spice,"64") == 0 ) {
	  size = 1;
  } else if( strcmp(spice,"512") == 0 ) {
  	  size = 8;
  } else if( strcmp(spice,"x4") == 0 ) {
	  size = 12;
  }
  return size;
}


int getOutSize2(char* spice) {
  int size = 8;
  if( strcmp(spice,"64") == 0 ) {
	  size = 8;
  } else if( strcmp(spice,"512") == 0 ) {
  	  size = 64;
  } else if( strcmp(spice,"x4") == 0 ) {
	  size = 96;
  }
  return size;
}

int doFileRead( long n, int flag, char* spice, char* filein ) {
    FILE * fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;

    fp = fopen(filein, "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);

    int sizeHex = getNumHex(spice);
    char hex[sizeHex];
    uint64_t hash[getOutSize(spice)];
    while ((read = getline(&line, &len, fp)) != -1) {
    	itera(line,n,flag,hex,spice,hash,-1,-1,-1);
    }

    fclose(fp);
    if (line)
        free(line);
    exit(EXIT_SUCCESS);
}

int doFileWrite( long n, int flag, char* spice, char* filein, char* fileout, uint64_t* hash, int size, int a, int b, int c ) {
    FILE * fp;
    FILE * fo;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;

    fp = fopen(filein, "r");
    fo = fopen(fileout, "w");
    if (fp == NULL)
        exit(EXIT_FAILURE);
    if (fo == NULL)
        exit(EXIT_FAILURE);

    int sizeHex = getNumHex(spice);
    char hex[sizeHex];
    while ((read = getline(&line, &len, fp)) != -1) {
    	itera(line,n,flag,hex,spice,hash,a,b,c);
    	fwrite(hash, sizeof(hash[0]), size, fo);
    }

    fclose(fp);
    fclose(fo);
    if (line)
        free(line);
    exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[]) {

	int a=4; //7
	int b=6; //7
	int c=6; //4

  char* val1 = ""; // input the empty string.
  long n;
  int t;
  if( argc == 1 ) { // Sin argumentos.
    test_64( val1 );
    test_512( val1 );
    test_x4( val1 );
  } else if( argc == 2 ) { // Cadena a hashear.
	val1 = argv[1];
	char hex[17];
	uint64_t hash[getOutSize("")]; // output 8 bytes/64 bits.
	itera_64( val1, 1, 1, hex, hash, a,b,c );
  } else if( argc == 3 ) { // Num. de iteracion.
	val1 = argv[1];
	n = atol(argv[2]);
	char hex[17];
	uint64_t hash[getOutSize("")]; // output 8 bytes/64 bits.
	itera_64( val1, n, 1, hex, hash, a,b,c );
  } else if( argc == 4 ) { // Bandera para imprimir el hash.
	val1 = argv[1];
	n = atol(argv[2]);
	t = atoi(argv[3]);
	char hex[17];
	uint64_t hash[getOutSize("")]; // output 8 bytes/64 bits.
	itera_64( val1, n, t, hex, hash, a,b,c );
  } else if( argc == 5 ) { // Tipo de algoritmo.
	val1 = argv[1];
	n = atol(argv[2]);
	t = atoi(argv[3]);
	int sizeHex = getNumHex(argv[4]);
	char hex[sizeHex];
	uint64_t hash[getOutSize(argv[4])];
    itera( val1, n, t, hex, argv[4], hash, -1,-1,-1 );
  } else if( argc == 6 ) { // Archivo a hashear entrada.
    n = atol(argv[2]);
    t = atoi(argv[3]);
    doFileRead( n, t, argv[4], argv[5] );
  } else if( argc == 7 ) { // Archivo de salida.
    n = atol(argv[2]);
    t = atoi(argv[3]);
	uint64_t hash[getOutSize(argv[4])];
	int sizeBytes = (int)((getOutSize2(argv[4]))/8.0);
    doFileWrite( n, t, argv[4], argv[5], argv[6], hash, sizeBytes, -1,-1,-1 );
  } else if( argc == 9 ) { // (a,b) iv's perm.
	int a = atoi(argv[7]);
	int b = atoi(argv[8]);
	n = atol(argv[2]);
	t = atoi(argv[3]);
	uint64_t hash[getOutSize(argv[4])];
	int sizeBytes = (int)((getOutSize2(argv[4]))/8.0);
	doFileWrite( n, t, argv[4], argv[5], argv[6], hash, sizeBytes, a,b,-1 );
  } else if( argc == 10 ) { // c iv perm.
	int a = atoi(argv[7]);
	int b = atoi(argv[8]);
	int c = atoi(argv[9]);
	n = atol(argv[2]);
	t = atoi(argv[3]);
	uint64_t hash[getOutSize(argv[4])];
	int sizeBytes = (int)((getOutSize2(argv[4]))/8.0);
	doFileWrite( n, t, argv[4], argv[5], argv[6], hash, sizeBytes, a,b,c );
  }

  return 0;
}
