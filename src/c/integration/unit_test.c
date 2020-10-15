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
#include <stdio.h>
#include <inttypes.h>

#define UNIT_TEST 1

#include "azrael_base.c"
#include "azrael64_link.c"
#include "azrael320_link.c"
#include "azrael512_link.c"
#include "azraelx4_link.c"

void test_x4( char* val1 ) {

  uint64_t hash[12]; // output 96 bytes/768 bits.
  eval_hash_x4( val1, hash, strlen(val1) );

  char hex[193];
  sprintf(hex,"%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64,hash[0],hash[1],hash[2],hash[3],hash[4],hash[5],hash[6],hash[7],hash[8],hash[9],hash[10],hash[11] );

  char* empty_hash = "cded98730118b7a4a15597430c9d75863d9532329540cab109ea26ab72a96d1b30c264e213beef31700795f4bb881bf0fd10ce945b15a8557dac5d970b49d75b6e8904776c7d3b5b975228ead94f06dbeb97574581bf53ac4b82ae93ec4f77c7";
  if( strcmp(hex,empty_hash) == 0 ) {
    printf( "TEST 768 bits of output OK!!!\n%s\n", hex );
  } else {
    printf( "TEST FAILED!\n" );
  }
}

void test_512( char* val1 ) {

  uint64_t hash[8]; // output 64 bytes/512 bits.
  eval_hash_512( val1, hash, strlen(val1) );

  char hex[129];
  sprintf(hex,"%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64,hash[0],hash[1],hash[2],hash[3],hash[4],hash[5],hash[6],hash[7] );

  char* empty_hash = "b4d1fb3fffb8b7a496677285b03e5c7dfaf26eda834dab12040a81923eb1081086a03ff930128bc8c2cbd29e3637fd13b911297c89d5a855fadd4393eb442683";
  if( strcmp(hex,empty_hash) == 0 ) {
    printf( "TEST 512 bits of output OK!!!\n%s\n", hex );
  } else {
    printf( "TEST FAILED!\n" );
  }
}

void test_320( char* val1 ) {

  uint64_t hash[5]; // output 40 bytes/320 bits.
  eval_hash_320( val1, hash, strlen(val1) );

  char hex[81];
  sprintf(hex,"%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64,hash[0],hash[1],hash[2],hash[3],hash[4] );

  char* empty_hash = "b4d1fb3fec8b03e2966772855feaa41bfaf26ed6d2a21452040a819378b5ccce86a0400155d2a3fc";
  if( strcmp(hex,empty_hash) == 0 ) {
    printf( "TEST 320 bits of output OK!!!\n%s\n", hex );
  } else {
    printf( "TEST FAILED!\n" );
  }
}

void test_64( char* val1 ) {

  uint64_t hash[1]; // output 8 bytes/64 bits.
  eval_hash_64( val1, hash, strlen(val1) );

  char hex[17];
  sprintf(hex,"%016" PRIx64,hash[0] );

  char* empty_hash = "ffdb3d80fed96840";
  if( strcmp(hex,empty_hash) == 0 ) {
    printf( "TEST  64 bits of output OK!!!\n%s\n", hex );
  } else {
    printf( "TEST FAILED!\n" );
  }
}

int main() {

  char* val1 = ""; // input the empty string.
  test_x4( val1 );
  test_512( val1 );
  test_320( val1 );
  test_64( val1 );

  return 0;
}
