#!/usr/bin/env python

import struct
import io
import sys
import numpy as np

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

    pad = [np.int64(0) for i in range(padding)];
    pad[0] = np.int64(-0x80);
    bits = length * 8;
    for i in range(8):
        pad[padding - 1 - i] = ((bits >> (8 * i)) & 0xFF);

    out = [np.int64(0) for i in range(length+padding)];
    for i in range(length):
        out[i] = np.int64(data[i]);
    for i in range(padding):
        out[length+i] = pad[i];

    return out;

def longToBytes(l):
    result = [0 for i in range(8)];
    for i in range(7,-1,-1):
        result[i] = l & 0xFF;
        l >>= 8;
    return result;

class Azrael(object):
    
    def __init__(self):
        # Initial digest variables
        self._IV = (
            np.int64(0x6a09e667bb67ae85),
            np.int64(0x3c6ef372a54ff53a),
            np.int64(0x510e527f9b05688c),
            np.int64(0x1f83d9ab5be0cd19),
            np.int64(0x428a2f9871374491),
            np.int64(-0x4A3F0430164A245B),
            np.int64(0x3956c25b59f111f1),
            np.int64(-0x6DC07D5B54E3A12B),
            np.int64(-0x27F85567ED7CA4FF),
            np.int64(0x243185be550c7dc3),
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
        
        carrier[0] += evaluaFuncBool( carrier[0],carrier[0],carrier[0],carrier[0],carrier[0]) + self._IV[0];
        carrier[1] += evaluaFuncBool( carrier[1],carrier[1],carrier[1],carrier[1],carrier[1]) + self._IV[1];
        carrier[2] += evaluaFuncBool( carrier[2],carrier[2],carrier[2],carrier[2],carrier[2]) + self._IV[2];
        carrier[3] += evaluaFuncBool( carrier[3],carrier[3],carrier[3],carrier[3],carrier[3]) + self._IV[3];
        carrier[4] += evaluaFuncBool( carrier[4],carrier[4],carrier[4],carrier[4],carrier[4]) + self._IV[6];
        
        mask1 = np.int64(0xffffffff);
        mask2 = np.int64(-0x1);
         
        hash = np.int64(0); 
        hash = ((carrier[0] << 48) & mask2 ) | \
            (((carrier[0]+carrier[1]) << 32) & mask2 ) | \
            (((carrier[0]+carrier[1]+carrier[2]) << 16) & mask1) | \
            ((carrier[2]+carrier[3]+carrier[4]) & mask1);

        hash += evaluaFuncBool( hash,hash,hash,hash,hash) + self._IV[4];
        hash += evaluaFuncBool( hash,hash,hash,hash,hash) + self._IV[6];
        hash += length + 5 + 1 + 2;
        
        return longToBytes(hash);


def main():
    azrael = Azrael();
    output = azrael.getHashEval( bytearray(b'') );
    hex = '0x' + ''.join('{:02x}'.format(i) for i in output);
    print(hex);
    
    
if __name__ == "__main__":
    main()
