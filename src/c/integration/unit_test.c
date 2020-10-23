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
#include "azrael128_link.c"
#include "azrael192_link.c"
#include "azrael320_link.c"
#include "azrael512_link.c"
#include "azraelx4_link.c"
#include "azraelxM_link.c"
#include "azraelxMM_link.c"

#include "../sha/sha3.h"
#include "../siphash/csiphash.c"

void test_xMM( char* val1 ) {

  uint64_t hash[32]; // output 256 bytes/2048 bits.
  eval_hash_xMM( val1, hash, strlen(val1) );

  char hex[513];
  sprintf(hex,"%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64,hash[0],hash[1],hash[2],hash[3],hash[4],hash[5],hash[6],hash[7],hash[8],hash[9],hash[10],hash[11],hash[12],hash[13],hash[14],hash[15],hash[16],hash[17],hash[18],hash[19],hash[20],hash[21],hash[22],hash[23],hash[24],hash[25],hash[26],hash[27],hash[28],hash[29],hash[30],hash[31] );

  char* empty_hash = "e9b90e715bd879a1847732c649632381cbdd7ef7ab8ba076579b9f66a03228e068882ef485f29061d3a598c4aad1ae88ae3b7c80604d6a492a635f9f816e33bedab9adc5954419617b08a7874c9b6a54d7ebd8652469e78b2235dcd3ac7fa2501d0f580357ec96e72cb65b2ccc1683bc113d761a686c351454c207bd5ce62136d29eb349746ac9b96907647a940881b0377fd5cf08726ed92f7e8b539a9203bd8299df3fb512283cf1b6ea5cac8818f984b1a6d36b14ffb088a2d2efcf3e2f808202f86438e801a1a5a03532a54a18ef3cccc5b7bee4407c0fa76d5afd9f4f0b793bcd151b6391b7cf90fd473e58403fae3fb7a0118d81ab09cddf8a964ded84";
  if( strcmp(hex,empty_hash) == 0 ) {
    printf( "TEST 2048 bits of output (xMM) OK!!!\n%s\n", hex );
  } else {
    printf( "TEST FAILED!\n%s\n", hex );
  }
}

void itera_xMM( char* val1, long n, int flag, char* hex, uint64_t* hash ) {

  long i;
  int k,j;
  int size = strlen(val1);
  char val2[256];

  for( i=0; i<n; i++ ) {
    eval_hash_xMM( val1, hash, size );
    for(k=0; k<32; k++ ) {
      for(j=0; j<8; j++ ) {
        val2[8*(k+1)-1-j] = hash[k] >> 8*j;
      }
    }
    val1 = val2;
    size = 256;
  }

  if( flag != 0 ) {
	sprintf(hex,"%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64,hash[0],hash[1],hash[2],hash[3],hash[4],hash[5],hash[6],hash[7],hash[8],hash[9],hash[10],hash[11],hash[12],hash[13],hash[14],hash[15],hash[16],hash[17],hash[18],hash[19],hash[20],hash[21],hash[22],hash[23],hash[24],hash[25],hash[26],hash[27],hash[28],hash[29],hash[30],hash[31] );
    printf( "%s\n", hex );
  }
}

void test_xM( char* val1 ) {

  uint64_t hash[16]; // output 128 bytes/1024 bits.
  eval_hash_xM( val1, hash, strlen(val1) );

  char hex[257];
  sprintf(hex,"%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64,hash[0],hash[1],hash[2],hash[3],hash[4],hash[5],hash[6],hash[7],hash[8],hash[9],hash[10],hash[11],hash[12],hash[13],hash[14],hash[15] );

  char* empty_hash = "0d3a879c9a71f6717b43c589321cf0e13e9927f0fac73232de94f77f17b01b555362f257810dfc3301408bb9c0e967d3bcbf9f678a1b8e445298fbdc7122e40e453c32c5f3f802f1fe9e6e12bcd6a0a069dddb7ece5a06792235dcd3ac7fa250354cc5b7a8300e5e13bac744abb43dc375f01f6ab71ca82d825cfab272fdda61";
  if( strcmp(hex,empty_hash) == 0 ) {
    printf( "TEST 1024 bits of output (xM) OK!!!\n%s\n", hex );
  } else {
    printf( "TEST FAILED!\n%s\n", hex );
  }
}

void itera_xM( char* val1, long n, int flag, char* hex, uint64_t* hash ) {

  long i;
  int k,j;
  int size = strlen(val1);
  char val2[128];

  for( i=0; i<n; i++ ) {
    eval_hash_xM( val1, hash, size );
    for(k=0; k<16; k++ ) {
      for(j=0; j<8; j++ ) {
        val2[8*(k+1)-1-j] = hash[k] >> 8*j;
      }
    }
    val1 = val2;
    size = 128;
  }

  if( flag != 0 ) {
	sprintf(hex,"%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64,hash[0],hash[1],hash[2],hash[3],hash[4],hash[5],hash[6],hash[7],hash[8],hash[9],hash[10],hash[11],hash[12],hash[13],hash[14],hash[15] );
    printf( "%s\n", hex );
  }
}

void test_x4( char* val1 ) {

  uint64_t hash[12]; // output 96 bytes/768 bits.
  eval_hash_x4( val1, hash, strlen(val1) );

  char hex[193];
  sprintf(hex,"%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64,hash[0],hash[1],hash[2],hash[3],hash[4],hash[5],hash[6],hash[7],hash[8],hash[9],hash[10],hash[11] );

  char* empty_hash = "6219986a754b80526556d98a6f67e19c32e5c68c9b3321cd40cba83372e0eeef9774287a573d1c4b4a102a501e2a9a9c1ffe501c67efa8559d66d7d1aaf6e76f454a46c642cf1cf9aaafe17a9f1c277541991420f6cd914894ff99d537770487";
  if( strcmp(hex,empty_hash) == 0 ) {
    printf( "TEST  768 bits of output (x4) OK!!!\n%s\n", hex );
  } else {
    printf( "TEST FAILED!\n%s\n", hex );
  }
}

void itera_x4( char* val1, long n, int flag, char* hex, uint64_t* hash ) {

  long i;
  int k,j;
  int size = strlen(val1);
  char val2[96];

  for( i=0; i<n; i++ ) {
    eval_hash_x4( val1, hash, size );
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

void test_512( char* val1 ) {

  uint64_t hash[8]; // output 64 bytes/512 bits.
  eval_hash_512( val1, hash, strlen(val1) );

  char hex[129];
  sprintf(hex,"%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64,hash[0],hash[1],hash[2],hash[3],hash[4],hash[5],hash[6],hash[7] );

  char* empty_hash = "62199873880effb36556d98ab20a2c4f32e5c68a4bc49bbd40cba833785e3c2c9774287ab18dffbe195d0642c2c741e81ffe501b86c53034ae85b5315be5a12b";
  if( strcmp(hex,empty_hash) == 0 ) {
    printf( "TEST  512 bits of output OK!!!\n%s\n", hex );
  } else {
    printf( "TEST FAILED!\n%s\n", hex );
  }
}

void itera_512( char* val1, long n, int flag, char* hex, uint64_t* hash ) {

  long i;
  int k,j;
  int size = strlen(val1);
  char val2[64];

  for( i=0; i<n; i++ ) {
    eval_hash_512( val1, hash, size );
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

void test_320( char* val1 ) {

  uint64_t hash[5]; // output 40 bytes/320 bits.
  eval_hash_320( val1, hash, strlen(val1) );

  char hex[81];
  sprintf(hex,"%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64,hash[0],hash[1],hash[2],hash[3],hash[4] );

  char* empty_hash = "f5aeee6b7ff1ddf24484f584b3f7ce72e30d418fb28c7e3cea737b7f3abd27f7518dcc041e291c0f";
  if( strcmp(hex,empty_hash) == 0 ) {
    printf( "TEST  320 bits of output OK!!!\n%s\n", hex );
  } else {
    printf( "TEST FAILED!\n%s\n", hex );
  }
}

void itera_320( char* val1, long n, int flag, char* hex, uint64_t* hash ) {

  long i;
  int k,j;
  int size = strlen(val1);
  char val2[40];

  for( i=0; i<n; i++ ) {
    eval_hash_320( val1, hash, size );
    for(k=0; k<5; k++ ) {
      for(j=0; j<8; j++ ) {
        val2[8*(k+1)-1-j] = hash[k] >> 8*j;
      }
    }
    val1 = val2;
    size = 40;
  }

  if( flag != 0 ) {
	sprintf(hex,"%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64,hash[0],hash[1],hash[2],hash[3],hash[4] );
    printf( "%s\n", hex );
  }
}

void test_192( char* val1 ) {

  uint64_t hash[3]; // output 24 bytes/192 bits.
  eval_hash_192( val1, hash, strlen(val1) );

  char hex[49];
  sprintf(hex,"%016" PRIx64 "%016" PRIx64 "%016" PRIx64,hash[0],hash[1],hash[2] );

  char* empty_hash = "e8c790169510bfbab9d25019a4f786cd8847d74565d2eba5";
  if( strcmp(hex,empty_hash) == 0 ) {
    printf( "TEST  192 bits of output OK!!!\n%s\n", hex );
  } else {
    printf( "TEST FAILED!\n%s\n", hex );
  }
}

void itera_192( char* val1, long n, int flag, char* hex, uint64_t* hash ) {

  long i;
  int k,j;
  int size = strlen(val1);
  char val2[24];

  for( i=0; i<n; i++ ) {
    eval_hash_192( val1, hash, size );
    for(k=0; k<3; k++ ) {
      for(j=0; j<8; j++ ) {
        val2[8*(k+1)-1-j] = hash[k] >> 8*j;
      }
    }
    val1 = val2;
    size = 24;
  }

  if( flag != 0 ) {
    sprintf(hex,"%016" PRIx64 "%016" PRIx64 "%016" PRIx64,hash[0],hash[1],hash[2] );
    printf( "%s\n", hex );
  }
}

void test_128( char* val1 ) {

  uint64_t hash[2]; // output 16 bytes/128 bits.
  eval_hash_128( val1, hash, strlen(val1) );

  char hex[33];
  sprintf(hex,"%016" PRIx64 "%016" PRIx64,hash[0],hash[1] );

  char* empty_hash = "e8c790169510bfbab9d25019a4f78716";
  if( strcmp(hex,empty_hash) == 0 ) {
    printf( "TEST  128 bits of output OK!!!\n%s\n", hex );
  } else {
    printf( "TEST FAILED!\n%s\n", hex );
  }
}

void itera_128( char* val1, long n, int flag, char* hex, uint64_t* hash ) {

  long i;
  int k,j;
  int size = strlen(val1);
  char val2[16];

  for( i=0; i<n; i++ ) {
    eval_hash_128( val1, hash, size );
    for(k=0; k<2; k++ ) {
      for(j=0; j<8; j++ ) {
        val2[8*(k+1)-1-j] = hash[k] >> 8*j;
      }
    }
    val1 = val2;
    size = 16;
  }

  if( flag != 0 ) {
	sprintf(hex,"%016" PRIx64 "%016" PRIx64, hash[0],hash[1] );
    printf( "%s\n", hex );
  }
}


void test_64( char* val1 ) {

  uint64_t hash[1]; // output 8 bytes/64 bits.
  eval_hash_64( val1, hash, strlen(val1) );

  char hex[17];
  sprintf(hex,"%016" PRIx64,hash[0] );

  char* empty_hash = "ac242eaf9248c6df";
  if( strcmp(hex,empty_hash) == 0 ) {
    printf( "TEST   64 bits of output OK!!!\n%s\n", hex );
  } else {
    printf( "TEST FAILED!\n%s\n", hex );
  }
}

void itera_64( char* val1, long n, int flag, char* hex, uint64_t* hash ) {

  long i;
  int j;
  int size = strlen(val1);
  char val2[8];

  for( i=0; i<n; i++ ) {
    eval_hash_64( val1, hash, size );
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

static void byte_to_hex(uint8_t b, char s[23]) {
    unsigned i=1;
    s[0] = s[1] = '0';
    s[2] = '\0';
    while(b) {
        unsigned t = b & 0x0f;
        if( t < 10 ) {
            s[i] = '0' + t;
        } else {
            s[i] = 'a' + t - 10;
        }
        i--;
        b >>= 4;
    }
}

void itera_sha3( char* val1, long n, int flag, char* hex, uint64_t* hash ) {

  sha3_context c;

  long i;
  int k;
  int size = strlen(val1);
  char val2[64];

  const uint8_t* hash1 = {0};

  for( i=0; i<n; i++ ) {
	sha3_Init512(&c);
    sha3_Update(&c, val1, size);
    hash1 = sha3_Finalize(&c);
    for(k=0; k<64; k++ ) {
        val2[k] = hash1[k];
    }
    val1 = val2;
    size = 64;
  }

  for( int j=0; j<8; j++ ) {
	for( int i=0; i<8; i++ ) {
	  hash[j] = (hash[j] << 8) | hash1[j*8+i];
	}
  }

  if( flag != 0 ) {
	sprintf(hex,"%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64,hash[0],hash[1],hash[2],hash[3],hash[4],hash[5],hash[6],hash[7] );
    printf( "%s\n", hex );
  }
}

void test_sha3( char* val1 ) {

	sha3_context c;

	int i;
	int size = strlen(val1);
	const uint8_t *hash;
	char hex[129];
	char* ptr = hex;
	char s[3];

	sha3_Init512(&c);
	sha3_Update(&c, val1, size);
	hash = sha3_Finalize(&c);

	for(i=0; i<64; i++) {
		byte_to_hex(hash[i],s);
		sprintf(ptr,"%s", s);
		ptr += 2;
	}

	char* empty_hash = "a69f73cca23a9ac5c8b567dc185a756e97c982164fe25859e0d1dcc1475c80a615b2123af1f5f94c11e3e9402c3ac558f500199d95b6d3e301758586281dcd26";
	if( strcmp(hex,empty_hash) == 0 ) {
		printf( "SHA3  512 bits of output OK!!!\n%s\n", hex );
	} else {
		printf( "TEST FAILED!\n%s\n", hex );
	}

}

void itera_siphash( char* val1, long n, int flag, char* hex, uint64_t* hash ) {

  long i;
  int j;
  int size = strlen(val1);
  char val2[8];

  char key[16] = {0,1,2,3,4,5,6,7,8,9,0xa,0xb,0xc,0xd,0xe,0xf};

  for( i=0; i<n; i++ ) {
	hash[0] = siphash24(val1, size, key);
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

void test_siphash( char* val1 ) {

	int size = strlen(val1);
	char key[16] = {0,1,2,3,4,5,6,7,8,9,0xa,0xb,0xc,0xd,0xe,0xf};

	uint64_t hash = siphash24(val1, size, key);

	char hex[17];
	sprintf(hex,"%016" PRIx64,hash );

	char* empty_hash = "726fdb47dd0e0e31";
	if( strcmp(hex,empty_hash) == 0 ) {
		printf( "SIPHASH64 bits of output OK!!!\n%s\n", hex );
	} else {
		printf( "TEST FAILED!\n%s\n", hex );
	}
}

void itera( char* val1, long n, int flag, char* hex, char* spice, uint64_t* hash ) {

	if( strcmp(spice,"64") == 0 ) {
		itera_64( val1, n, flag, hex, hash );
	} else if( strcmp(spice,"128") == 0 ) {
		itera_128( val1, n, flag, hex, hash );
	} else if( strcmp(spice,"192") == 0 ) {
		itera_192( val1, n, flag, hex, hash );
	} else if( strcmp(spice,"320") == 0 ) {
		itera_320( val1, n, flag, hex, hash );
	} else if( strcmp(spice,"512") == 0 ) {
		itera_512( val1, n, flag, hex, hash );
	} else if( strcmp(spice,"x4") == 0 ) {
		itera_x4( val1, n, flag, hex, hash );
	} else if( strcmp(spice,"xM") == 0 ) {
		itera_xM( val1, n, flag, hex, hash );
	} else if( strcmp(spice,"xMM") == 0 ) {
		itera_xMM( val1, n, flag, hex, hash );
	} else if( strcmp(spice,"sha3") == 0 ) {
		itera_sha3( val1, n, flag, hex, hash );
	} else if( strcmp(spice,"siphash") == 0 ) {
		itera_siphash( val1, n, flag, hex, hash );
	}
}

int getNumHex(char* spice) {
  int sizeHex = 17;
  if( strcmp(spice,"64") == 0 || strcmp(spice,"siphash") == 0 ) {
	sizeHex = 17;
  } else if( strcmp(spice,"128") == 0 ) {
	sizeHex = 33;
  } else if( strcmp(spice,"192") == 0 ) {
	sizeHex = 49;
  } else if( strcmp(spice,"320") == 0 ) {
	sizeHex = 81;
  } else if( strcmp(spice,"512") == 0 || strcmp(spice,"sha3") == 0 ) {
	sizeHex = 129;
  } else if( strcmp(spice,"x4") == 0 ) {
	sizeHex = 193;
  } else if( strcmp(spice,"xM") == 0 ) {
	sizeHex = 257;
  } else if( strcmp(spice,"xMM") == 0 ) {
	sizeHex = 513;
  }
  return sizeHex;
}

int getOutSize(char* spice) {
  int size = 1;
  if( strcmp(spice,"64") == 0 || strcmp(spice,"siphash") == 0 ) {
	  size = 1;
  } else if( strcmp(spice,"128") == 0 ) {
	  size = 2;
  } else if( strcmp(spice,"192") == 0 ) {
	  size = 3;
  } else if( strcmp(spice,"320") == 0 ) {
	  size = 5;
  } else if( strcmp(spice,"512") == 0 || strcmp(spice,"sha3") == 0 ) {
	  size = 8;
  } else if( strcmp(spice,"x4") == 0 ) {
	  size = 12;
  } else if( strcmp(spice,"xM") == 0 ) {
	  size = 16;
  } else if( strcmp(spice,"xMM") == 0 ) {
	  size = 32;
  }
  return size;
}


int getOutSize2(char* spice) {
  int size = 8;
  if( strcmp(spice,"64") == 0 || strcmp(spice,"siphash") == 0 ) {
	  size = 8;
  } else if( strcmp(spice,"128") == 0 ) {
	  size = 16;
  } else if( strcmp(spice,"192") == 0 ) {
	  size = 24;
  } else if( strcmp(spice,"320") == 0 ) {
	  size = 40;
  } else if( strcmp(spice,"512") == 0 || strcmp(spice,"sha3") == 0 ) {
	  size = 64;
  } else if( strcmp(spice,"x4") == 0 ) {
	  size = 96;
  } else if( strcmp(spice,"xM") == 0 ) {
	  size = 128;
  } else if( strcmp(spice,"xMM") == 0 ) {
	  size = 256;
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
    	itera(line,n,flag,hex,spice,hash);
    }

    fclose(fp);
    if (line)
        free(line);
    exit(EXIT_SUCCESS);
}

int doFileWrite( long n, int flag, char* spice, char* filein, char* fileout, uint64_t* hash, int size ) {
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
    	itera(line,n,flag,hex,spice,hash);
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
    test_siphash( val1 );
    test_64( val1 );
    test_128( val1 );
    test_192( val1 );
    test_320( val1 );
    test_sha3( val1 );
    test_512( val1 );
    test_x4( val1 );
    test_xM( val1 );
    test_xMM( val1 );
  } else if( argc == 2 ) { // Cadena a hashear.
	val1 = argv[1];
	char hex[17];
	uint64_t hash[getOutSize("")]; // output 8 bytes/64 bits.
	itera_64( val1, 1, 1, hex, hash );
  } else if( argc == 3 ) { // Num. de iteracion.
	val1 = argv[1];
	n = atol(argv[2]);
	char hex[17];
	uint64_t hash[getOutSize("")]; // output 8 bytes/64 bits.
	itera_64( val1, n, 1, hex, hash );
  } else if( argc == 4 ) { // Bandera para imprimir el hash.
	val1 = argv[1];
	n = atol(argv[2]);
	t = atoi(argv[3]);
	char hex[17];
	uint64_t hash[getOutSize("")]; // output 8 bytes/64 bits.
	itera_64( val1, n, t, hex, hash );
  } else if( argc == 5 ) { // Tipo de algoritmo.
	val1 = argv[1];
	n = atol(argv[2]);
	t = atoi(argv[3]);
	int sizeHex = getNumHex(argv[4]);
	char hex[sizeHex];
	uint64_t hash[getOutSize(argv[4])];
    itera( val1, n, t, hex, argv[4], hash );
  } else if( argc == 6 ) { // Archivo a hashear entrada.
    n = atol(argv[2]);
    t = atoi(argv[3]);
    doFileRead( n, t, argv[4], argv[5] );
  } else if( argc == 7 ) { // Archivo de salida.
    n = atol(argv[2]);
    t = atoi(argv[3]);
	uint64_t hash[getOutSize(argv[4])];
	int sizeBytes = (int)((getOutSize2(argv[4]))/8.0);
    doFileWrite( n, t, argv[4], argv[5], argv[6], hash, sizeBytes );
  }

  return 0;
}
