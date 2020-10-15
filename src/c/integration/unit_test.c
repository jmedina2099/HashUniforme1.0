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

#include "azrael_base.c"
#include "azrael64_link.c"
#include "azrael320_link.c"
#include "azrael512_link.c"
#include "azraelx4_link.c"
#include "azraelxM_link.c"
#include "azraelxMM_link.c"

void test_xMM( char* val1 ) {

  uint64_t hash[32]; // output 256 bytes/2048 bits.
  eval_hash_xMM( val1, hash, strlen(val1) );

  char hex[513];
  sprintf(hex,"%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64,hash[0],hash[1],hash[2],hash[3],hash[4],hash[5],hash[6],hash[7],hash[8],hash[9],hash[10],hash[11],hash[12],hash[13],hash[14],hash[15],hash[16],hash[17],hash[18],hash[19],hash[20],hash[21],hash[22],hash[23],hash[24],hash[25],hash[26],hash[27],hash[28],hash[29],hash[30],hash[31] );

  char* empty_hash = "e9b90e715bd879a1847732c649632381cbdd7ef7ab8ba076579b9f66a03228e068882ef485f29061d3a598c4aad1ae88ae3b7c80604d6a492a635f9f816e33bedab9adc5954419617b08a7874c9b6a54d7ebd8652469e78b2235dcd3ac7fa2501d0f580357ec96e72cb65b2ccc1683bc113d761a686c351454c207bd5ce62116d29eb349746ac9b96907647a940881b0377fd5cf08726ed92f7e8b539a9203bd8299df3fb512283cf1b6ea5cac8818f984b1a6d36b14ffb088a2d2efcf3e2f808202f86438e801a1a5a03532a54a18ef3cccc5b7bee4407c0fa76d5afd9f4f0b793bcd151b6391b7cf90fd473e58403fae3fb7a0118d81ab09cddf8a964ded84";
  if( strcmp(hex,empty_hash) == 0 ) {
    printf( "TEST 2048 bits of output OK!!!\n%s\n", hex );
  } else {
    printf( "TEST FAILED!\n%s\n", hex );
  }
}

void test_xM( char* val1 ) {

  uint64_t hash[16]; // output 128 bytes/1024 bits.
  eval_hash_xM( val1, hash, strlen(val1) );

  char hex[257];
  sprintf(hex,"%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64,hash[0],hash[1],hash[2],hash[3],hash[4],hash[5],hash[6],hash[7],hash[8],hash[9],hash[10],hash[11],hash[12],hash[13],hash[14],hash[15] );

  char* empty_hash = "0d3a879c9a71f6717b43c589321cf0e13e9927f0fac73232de94f77f17b01b555362f257810dfc3301408bb9c0e967d3bcbf9f678a1b8e445298fbdc7122e40e453c32c5f3f802f1fe9e6e12bcd6a0a069dddb7ece5a06792235dcd3ac7fa250354cc5b7a8300e5e13bac744abb43dc375f01f6ab71ca82d825cfab272fdda51";
  if( strcmp(hex,empty_hash) == 0 ) {
    printf( "TEST 1024 bits of output OK!!!\n%s\n", hex );
  } else {
    printf( "TEST FAILED!\n%s\n", hex );
  }
}

void test_x4( char* val1 ) {

  uint64_t hash[12]; // output 96 bytes/768 bits.
  eval_hash_x4( val1, hash, strlen(val1) );

  char hex[193];
  sprintf(hex,"%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64,hash[0],hash[1],hash[2],hash[3],hash[4],hash[5],hash[6],hash[7],hash[8],hash[9],hash[10],hash[11] );

  char* empty_hash = "cded98730118b7a4a15597430c9d75863d9532329540cab109ea26ab72a96d1b30c264e213beef31700795f4bb881bf0fd10ce945b15a8557dac5d970b49d75b6e8904776c7d3b5b975228ead94f06dbeb97574581bf53ac4b82ae93ec4f77c7";
  if( strcmp(hex,empty_hash) == 0 ) {
    printf( "TEST  768 bits of output OK!!!\n%s\n", hex );
  } else {
    printf( "TEST FAILED!\n%s\n", hex );
  }
}

void test_512( char* val1 ) {

  uint64_t hash[8]; // output 64 bytes/512 bits.
  eval_hash_512( val1, hash, strlen(val1) );

  char hex[129];
  sprintf(hex,"%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64,hash[0],hash[1],hash[2],hash[3],hash[4],hash[5],hash[6],hash[7] );

  char* empty_hash = "b4d1fb3fffb8b7a496677285b03e5c7dfaf26eda834dab12040a81923eb1081086a03ff930128bc8c2cbd29e3637fd13b911297c89d5a855fadd4393eb442683";
  if( strcmp(hex,empty_hash) == 0 ) {
    printf( "TEST  512 bits of output OK!!!\n%s\n", hex );
  } else {
    printf( "TEST FAILED!\n%s\n", hex );
  }
}

void test_320( char* val1 ) {

  uint64_t hash[5]; // output 40 bytes/320 bits.
  eval_hash_320( val1, hash, strlen(val1) );

  char hex[81];
  sprintf(hex,"%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64,hash[0],hash[1],hash[2],hash[3],hash[4] );

  char* empty_hash = "f5aeee6b7ff1ddf24484f584b3f7ce72e30d418fb28c7e3cea737b7f3abd27f7518dcc041e291c0a";
  if( strcmp(hex,empty_hash) == 0 ) {
    printf( "TEST  320 bits of output OK!!!\n%s\n", hex );
  } else {
    printf( "TEST FAILED!\n%s\n", hex );
  }
}

void test_64( char* val1 ) {

  uint64_t hash[1]; // output 8 bytes/64 bits.
  eval_hash_64( val1, hash, strlen(val1) );

  char hex[17];
  sprintf(hex,"%016" PRIx64,hash[0] );

  char* empty_hash = "ffdb3d80fed96840";
  if( strcmp(hex,empty_hash) == 0 ) {
    printf( "TEST   64 bits of output OK!!!\n%s\n", hex );
  } else {
    printf( "TEST FAILED!\n%s\n", hex );
  }
}

void itera_64( char* val1, int n ) {

  char* first = "ffdb3d80fed96840";

  int i,j;
  char hex[17];
  int size = strlen(val1);
  uint64_t hash[1] = {0}; // output 8 bytes/64 bits.
  char val2[8];

  for( i=0; i<n; i++ ) {
    eval_hash_64( val1, hash, size );
    sprintf(hex,"%016" PRIx64,hash[0] );
    if( strcmp(first,hex) == 0 ) {
    	printf( "EQUALS %d\n",(i+1) );
    }
    for(j=0; j<8; j++ ) {
      val2[7-j] = hash[0] >> 8*j;
    }
    val1 = val2;
    size = 8;
  }

  sprintf(hex,"%016" PRIx64,hash[0] );
  printf( "%s\n", hex );
}

int main(int argc, char *argv[]) {

  char* val1 = ""; // input the empty string.
  int n;
  if( argc < 2 ) {
    n = 1;
  } else {
    n = atoi(argv[1]);
  }

  test_xMM( val1 );
  test_xM( val1 );
  test_x4( val1 );
  test_512( val1 );
  test_320( val1 );
  test_64( val1 );

  itera_64( val1, n );

  return 0;
}
