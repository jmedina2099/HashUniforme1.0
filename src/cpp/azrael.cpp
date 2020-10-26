#include <iostream>
#include <cstring>
#include <iomanip>
#include <bits/stdc++.h>

using namespace std;

class Azrael64 {

	public:
		// Fractional part of square roots..
		const uint64_t IV[10] = {
		  0x6a09e667bb67ae85,
		  0x3c6ef372a54ff53a,
		  0x510e527f9b05688c,
		  0x1f83d9ab5be0cd19,
		  0x428a2f9871374491,
		  0xb5c0fbcfe9b5dba5,
		  0x3956c25b59f111f1,
		  0x923f82a4ab1c5ed5,
		  0xd807aa9812835b01,
		  0x243185be550c7dc3
		};
		inline uint64_t ROTATE_AZR( uint64_t x, int o ) {
			o %= 64;
			return (uint64_t)( x >> o );
		}
		inline uint64_t COMPRESS_320( register const uint64_t char1,
				register const uint64_t char2,
				register const uint64_t char3,
				register const uint64_t char4,
				register const uint64_t char5 ) {
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
		char* azr_pad( const char* data, int length, int padding, char* output ) {

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
		  memcpy(output,data,length);
		  memcpy(output+length,pad,padding);

		  return output;
		}
		void eval_hash_64( char* input, uint64_t* hash, int inputLength ) {
		  // Do padding..
		  const int tail = inputLength % 64;
		  int padding = 64 - tail >= 9? 64 - tail: 128 - tail;
		  char output[inputLength+padding];
		  input = azr_pad(input,inputLength,padding,output);
		  inputLength += padding;

		  // Begin hash calculation..
		  int i;
		  uint64_t cha[5] = {IV[0],IV[1],IV[2],IV[3],IV[4]};
		  uint64_t carrier[5] = {IV[5],IV[6],IV[7],IV[8],IV[9]};

		  int a =2;
		  int b =3;
		  int c =4;
		  int d =5;

		  // First iteration..
		  cha[0] += (uint64_t)input[ inputLength-2 ];
		  cha[1] += (uint64_t)input[ inputLength-1 ];
		  cha[2] += (uint64_t)input[ 0 ];
		  cha[3] += (uint64_t)input[ 1 ];
		  cha[4] += (uint64_t)input[ 2 ];
		  carrier[4] += ROTATE_AZR( carrier[3], d);
		  carrier[3] += ROTATE_AZR( carrier[2], c);
		  carrier[2] += ROTATE_AZR( carrier[1], b);
		  carrier[1] += ROTATE_AZR( carrier[0], a);
		  carrier[0] += COMPRESS_320( cha[0],cha[1],cha[2],cha[3],cha[4]);

		  // Main Loop for hash streaming..
		  for( i=1; i<inputLength-1; i++ ) {
		    cha[0] += carrier[0];
		    cha[1] += cha[2];
		    cha[2] += cha[3];
		    cha[3] += (uint64_t)input[i+1];
		    cha[4] += carrier[1];
		    carrier[4] += ROTATE_AZR( carrier[3], (8*(i+3)+d));
		    carrier[3] += ROTATE_AZR( carrier[2], (8*(i+2)+c));
		    carrier[2] += ROTATE_AZR( carrier[1], (8*(i+1)+b));
		    carrier[1] += ROTATE_AZR( carrier[0], (8*i+a));
		    carrier[0] += COMPRESS_320( cha[0],cha[1],cha[2],cha[3],cha[4]);
		  }

		  // Last iteration..
		  cha[0] += carrier[0];
		  cha[1] += cha[2];
		  cha[2] += cha[3];
		  cha[3] += (uint64_t)input[ 0 ];
		  cha[4] += carrier[1];
		  carrier[4] += ROTATE_AZR( carrier[3],(32+d) );
		  carrier[3] += ROTATE_AZR( carrier[2],(24+c) );
		  carrier[2] += ROTATE_AZR( carrier[1],(16+b) );
		  carrier[1] += ROTATE_AZR( carrier[0],(8+a) );
		  carrier[0] += COMPRESS_320( cha[0],cha[1],cha[2],cha[3],cha[4]);

		  // Doing dispersion of bits..
		  carrier[0] += COMPRESS_320( carrier[0],carrier[0],carrier[0],carrier[0],carrier[0]) + IV[0];
		  carrier[1] += COMPRESS_320( carrier[1],carrier[1],carrier[1],carrier[1],carrier[1]) + IV[1];
		  carrier[2] += COMPRESS_320( carrier[2],carrier[2],carrier[2],carrier[2],carrier[2]) + IV[2];
		  carrier[3] += COMPRESS_320( carrier[3],carrier[3],carrier[3],carrier[3],carrier[3]) + IV[3];
		  carrier[4] += COMPRESS_320( carrier[4],carrier[4],carrier[4],carrier[4],carrier[4]) + IV[3];

		  // Doing pile of bits..
		  hash[0] = ((carrier[0] << 48) & 0xffffffffffffffffL ) |
		            (((carrier[0]+carrier[1]) << 32) & 0xffffffffffffffffL ) |
		            (((carrier[0]+carrier[1]+carrier[2]) << 16) & 0xffffffffL) |
		            ((carrier[2]+carrier[3]+carrier[4]) & 0xffffffffL);

		  // Doing dispersion of bits
		  hash[0]  += COMPRESS_320( hash[0],hash[0],hash[0],hash[0],hash[0]) + IV[2];
		  hash[0]  += COMPRESS_320( hash[0],hash[0],hash[0],hash[0],hash[0]) + IV[7];

		  // Finally, we add the number of rounds to output..
		  hash[0] += inputLength + 5 + 1 + 2;
		}
};

int main(int argc, char *argv[]) {

	if( argc > 1 )  {
		string val(argv[1]); // input the empty string.
		int size = val.length();

		Azrael64 azrael64;

		char val1[size+1];
		strcpy(val1, val.c_str());

		uint64_t hash[1]; // output 8 bytes/64 bits.
		azrael64.eval_hash_64(val1,hash,size);

		stringstream sstream;
		sstream << std::hex << hash[0];
		string result = sstream.str();

		std::cout << result << "\n";
	}

    return 0;
}
