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

#include "../sha/sha3.h"
#include "../siphash/csiphash.c"

void test_64( char* val1 ) {

  uint64_t hash[1]; // output 8 bytes/64 bits.
  eval_hash_64( val1, hash, strlen(val1), 5, 7 );

  char hex[17];
  sprintf(hex,"%016" PRIx64,hash[0] );

  char* empty_hash = "a041afe55d33c2cf";
  if( strcmp(hex,empty_hash) == 0 ) {
    printf( "TEST   64 bits of output OK!!!\n%s\n", hex );
  } else {
    printf( "TEST FAILED!\n%s\n", hex );
  }
}

void itera_64( char* val1, long n, int flag, char* hex, uint64_t* hash, int a, int b) {

  long i;
  int j;
  int size = strlen(val1);
  char val2[8];

  for( i=0; i<n; i++ ) {
    eval_hash_64( val1, hash, size, a, b );
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

void itera( char* val1, long n, int flag, char* hex, char* spice, uint64_t* hash, int a, int b ) {

	if( strcmp(spice,"64") == 0 ) {
		itera_64( val1, n, flag, hex, hash, a, b );
	}
}

int getNumHex(char* spice) {
  int sizeHex = 17;
  if( strcmp(spice,"64") == 0 ) {
	sizeHex = 17;
  }
  return sizeHex;
}

int getOutSize(char* spice) {
  int size = 1;
  if( strcmp(spice,"64") == 0 ) {
	  size = 1;
  }
  return size;
}


int getOutSize2(char* spice) {
  int size = 8;
  if( strcmp(spice,"64") == 0 ) {
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
    	itera(line,n,flag,hex,spice,hash,5,7);
    }

    fclose(fp);
    if (line)
        free(line);
    exit(EXIT_SUCCESS);
}

int doFileWrite( long n, int flag, char* spice, char* filein, char* fileout, uint64_t* hash, int size, int a, int b ) {
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
    	itera(line,n,flag,hex,spice,hash,a,b);
    	fwrite(hash, sizeof(hash[0]), size, fo);
    }

    fclose(fp);
    fclose(fo);
    if (line)
        free(line);
    exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[]) {

  char* val1 = ""; // input the empty string.
  long n;
  int t;
  if( argc == 1 ) { // Sin argumentos.
    test_64( val1 );
  } else if( argc == 2 ) { // Cadena a hashear.
	val1 = argv[1];
	char hex[17];
	uint64_t hash[getOutSize("")]; // output 8 bytes/64 bits.
	itera_64( val1, 1, 1, hex, hash, 5,7 );
  } else if( argc == 3 ) { // Num. de iteracion.
	val1 = argv[1];
	n = atol(argv[2]);
	char hex[17];
	uint64_t hash[getOutSize("")]; // output 8 bytes/64 bits.
	itera_64( val1, n, 1, hex, hash, 5,7 );
  } else if( argc == 4 ) { // Bandera para imprimir el hash.
	val1 = argv[1];
	n = atol(argv[2]);
	t = atoi(argv[3]);
	char hex[17];
	uint64_t hash[getOutSize("")]; // output 8 bytes/64 bits.
	itera_64( val1, n, t, hex, hash, 5,7 );
  } else if( argc == 5 ) { // Tipo de algoritmo.
	val1 = argv[1];
	n = atol(argv[2]);
	t = atoi(argv[3]);
	int sizeHex = getNumHex(argv[4]);
	char hex[sizeHex];
	uint64_t hash[getOutSize(argv[4])];
    itera( val1, n, t, hex, argv[4], hash, 5,7 );
  } else if( argc == 6 ) { // Archivo a hashear entrada.
    n = atol(argv[2]);
    t = atoi(argv[3]);
    doFileRead( n, t, argv[4], argv[5] );
  } else if( argc == 7 ) { // Archivo de salida.
    n = atol(argv[2]);
    t = atoi(argv[3]);
	uint64_t hash[getOutSize(argv[4])];
	int sizeBytes = (int)((getOutSize2(argv[4]))/8.0);
    doFileWrite( n, t, argv[4], argv[5], argv[6], hash, sizeBytes, 5, 7 );
  } else if( argc == 9 ) {
	int a = atoi(argv[7]);
	int b = atoi(argv[8]);
	n = atol(argv[2]);
	t = atoi(argv[3]);
	uint64_t hash[getOutSize(argv[4])];
	int sizeBytes = (int)((getOutSize2(argv[4]))/8.0);
	doFileWrite( n, t, argv[4], argv[5], argv[6], hash, sizeBytes, a,b );
  }

  return 0;
}
