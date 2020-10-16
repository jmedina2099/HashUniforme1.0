#!/usr/bin/env python

import struct
import io
import sys

def evaluaFuncBool(char1,char2,char3,char4,char5):
    return (( char1 + char2 ) ^ ( char3 ^ char4 ) ^ char5) + \
           (( char1 & char2 ) ^ ( char3 + char4 ) ^ char5) + \
           (( char1 ^ char2 ) + ( char3 + char4 ) ^ char5) + \
           (( char1 ^ char2 ) ^ ( char3 + char4 ) ^ char5) + \
           (( char1 & char2 ) + ( char3 + char4 ) + char5) + \
           (( char1 & char2 ) + ( char3 + char4 ) ^ char5) + \
           (( char1 ^ char2 ) ^ ( char3 + char4 ) ^ char5) + \
           (( char1 | char2 ) ^ ( char3 + char4 ) ^ char5) + \
           (( char1 | char2 ) | ( char3 + char4 ) ^ char5) + \
           (( char1 + char2 ) + ( char3 + char4 ) ^ char5) + \
           (( char1 + char2 ) & ( char3 + char4 ) ^ char5) + \
           (( char1 ^ char2 ) + ( char3 ^ char4 ) ^ char5) + \
           (( char1 | char2 ) ^ ( char3 ^ char4 ) ^ char5) + \
           (( char1 + char2 ) + ( char3 ^ char4 ) ^ char5) + \
           (( char1 + char2 ) ^ ( char3 + char4 ) ^ char5) + \
           (( char1 + char2 ) & ( char3 + char4 ) + char5);

def pad(data):

    length = len(data);
    tail = length % 64;
    if (64 - tail >= 9):
        padding = 64 - tail;
    else:
        padding = 128 - tail;

    pad = [0 for i in range(padding)];
    pad[0] = 0x80;
    bits = length * 8;
    for i in range(8):
        pad[padding - 1 - i] = ((bits >> (8 * i)) & 0xFF);

    return data + bytes(pad);


class Azrael(object):
    
    def __init__(self):
        # Initial digest variables
        self._IV = (
            0x6a09e667bb67ae85,
            0x3c6ef372a54ff53a,
            0x510e527f9b05688c,
            0x1f83d9ab5be0cd19,
            0x428a2f9871374491,
            0xb5c0fbcfe9b5dba5,
            0x3956c25b59f111f1,
            0x923f82a4ab1c5ed5,
            0xd807aa9812835b01,
            0x243185be550c7dc3,
        )

    def getHashEval(self,input):
        
        input = pad(input);
        length = len(input);
        
        char = [self._IV[0],self._IV[1],self._IV[2],self._IV[3],self._IV[4]];
        carrier = [self._IV[5],self._IV[6],self._IV[7],self._IV[8],self._IV[9]];
        
        char[0] += input[length-2];
        char[1] += input[length-1];
        char[2] += input[0];
        char[3] += input[1];
        char[4] += input[2];
        carrier[4] += carrier[3]; 
        carrier[3] += carrier[2];
        carrier[2] += carrier[1];
        carrier[1] += carrier[0];
        carrier[0] += evaluaFuncBool( char[0],char[1],char[2],char[3],char[4]);
        
        for i in range(1,length-1):
            char[0] += carrier[0];
            char[1] += char[2];
            char[2] += char[3];
            char[3] += input[i+1];
            char[4] += carrier[1];
            carrier[4] += carrier[3]; 
            carrier[3] += carrier[2];
            carrier[2] += carrier[1];
            carrier[1] += carrier[0];
            carrier[0] += evaluaFuncBool( char[0],char[1],char[2],char[3],char[4]);
            
        char[0] += carrier[0];
        char[1] += char[2];
        char[2] += char[3];
        char[3] += input[0];
        char[4] += carrier[1];
        carrier[4] += carrier[3]; 
        carrier[3] += carrier[2];
        carrier[2] += carrier[1];
        carrier[1] += carrier[0];
        carrier[0] += evaluaFuncBool( char[0],char[1],char[2],char[3],char[4]);
        
        hash = ((carrier[0] << 48) & 0xffffffffffffffff ) | \
            (((carrier[0]+carrier[1]) << 32) & 0xffffffffffffffff ) | \
            (((carrier[0]+carrier[1]+carrier[2]) << 16) & 0xffffffff) | \
            ((carrier[2]+carrier[3]+carrier[4]) & 0xffffffff);
        hash += length;
        
        return hash;


def main():
    print("OK")
    azrael = Azrael();
    output = azrael.getHashEval( bytearray(b'') );
    print(hex(output)); 
    
if __name__ == "__main__":
    main()
