using System;
using System.Text;

class Azrael64 {

	public long evaluaFuncBool(long char1, long char2, long char3, long char4, long char5) {
		return (( char1 + char2 ) ^ ( char3 ^ char4 ) ^ char5) +
			   (( char1 & char2 ) ^ ( char3 + char4 ) ^ char5) +
			   (( char1 ^ char2 ) + ( char3 + char4 ) ^ char5) +
			   (( char1 ^ char2 ) ^ ( char3 + char4 ) ^ char5) +
			   (( char1 & char2 ) + ( char3 + char4 ) + char5) +
			   (( char1 & char2 ) + ( char3 + char4 ) ^ char5) +
			   (( char1 ^ char2 ) ^ ( char3 + char4 ) ^ char5) +
			   (( char1 | char2 ) ^ ( char3 + char4 ) ^ char5) +
			   (( char1 | char2 ) | ( char3 + char4 ) ^ char5) +
			   (( char1 + char2 ) + ( char3 + char4 ) ^ char5) +
			   (( char1 + char2 ) & ( char3 + char4 ) ^ char5) +
			   (( char1 ^ char2 ) + ( char3 ^ char4 ) ^ char5) +
			   (( char1 | char2 ) ^ ( char3 ^ char4 ) ^ char5) +
			   (( char1 + char2 ) + ( char3 ^ char4 ) ^ char5) +
			   (( char1 + char2 ) ^ ( char3 + char4 ) ^ char5) +
			   (( char1 + char2 ) & ( char3 + char4 ) + char5);
	}
	
	private long rotate( long x, int o ) {
		o %= 64;
		return (long)( (ulong)x >> o );
	}
	
	private int[] pad(byte[] data) {
		int length = data.Length;
        int tail = length % 64;
        int padding;

        if ((64 - tail >= 9)) {
            padding = 64 - tail;
        } else {
            padding = 128 - tail;
        }

        int[] pad = new int[padding];
        pad[0] = -0x80;
        long bits = length * 8;
        for (int i = 0; i < 8; i++) {
            pad[pad.Length - 1 - i] = (int)((bits >> (8 * i)) & 0xFF);
        }

        int[] output = new int[length + padding];
		for(int i = 0; i < length; i++) {
			output[i] = (sbyte)data[i];
		}
        //Array.Copy(data, 0, output, 0, length);
        Array.Copy(pad, 0, output, length, pad.Length);

        return output;
    }

	
  public byte[] getHashEval( byte[] inputB ) {
	
		int[] input = pad(inputB);
		
		long hash = 0;
		
		// fractional part of sqrt(2) and sqrt(3)
		long IV1 = 0x6a09e667bb67ae85L;
		long IV2 = 0x3c6ef372a54ff53aL;
		long IV3 = 0x510e527f9b05688cL;
		long IV4 = 0x1f83d9ab5be0cd19L;
		long IV5 = 0x428a2f9871374491L;
		long IV6 = -0x4A3F0430164A245BL; //0xb5c0fbcfe9b5dba5;
		long IV7 = 0x3956c25b59f111f1L;
		long IV8 = -0x6DC07D5B54E3A12BL; //0x923f82a4ab1c5ed5L;
		long IV9 = -0x27F85567ED7CA4FFL; //0xd807aa9812835b01L;
		long IV10 = 0x243185be550c7dc3L;
		
		long char1=IV1;
		long char2=IV2;
		long char3=IV3;
		long char4=IV4;
		long char5=IV5;
		
		long sumaAnt1 = IV6;
		long sumaAnt2 = IV7;
		long sumaAnt3 = IV8;
		long sumaAnt4 = IV9;
		long sumaAnt5 = IV10;
		
		int a =2;
		int b =3;
		int c =4;
		int d =5;

		char1 += (long)input[ input.Length-2 ];
		char2 += (long)input[ input.Length-1 ];
		char3 += (long)((sbyte)input[ 0 ]);
		char4 += (long)input[ 1 ];
		char5 += (long)input[ 2 ];
		sumaAnt5 += rotate( sumaAnt4, d);
		sumaAnt4 += rotate( sumaAnt3, c);
		sumaAnt3 += rotate( sumaAnt2, b);
		sumaAnt2 += rotate( sumaAnt1, a);
		sumaAnt1 += evaluaFuncBool( char1,char2,char3,char4,char5);

		// Main Loop.
		for( int i=1; i<input.Length-1; i++ ) {
			char1 += sumaAnt1;
			char2 += char3;
			char3 += char4;
			char4 += (long)input[ i+1 ];
			char5 += sumaAnt2;
			sumaAnt5 += rotate( sumaAnt4, (8*(i+3)+d));
			sumaAnt4 += rotate( sumaAnt3, (8*(i+2)+c));
			sumaAnt3 += rotate( sumaAnt2, (8*(i+1)+b));
			sumaAnt2 += rotate( sumaAnt1, (8*i+a));
			sumaAnt1 += evaluaFuncBool( char1,char2,char3,char4,char5);
		}

		char1 += sumaAnt1;
		char2 += char3;
		char3 += char4;
		char4 += (long)input[ 0 ];
		char5 += sumaAnt2;
		sumaAnt5 += rotate( sumaAnt4,(32+d) );
		sumaAnt4 += rotate( sumaAnt3,(24+c) );
		sumaAnt3 += rotate( sumaAnt2,(16+b) );
		sumaAnt2 += rotate( sumaAnt1,(8+a) );
		sumaAnt1 += evaluaFuncBool( char1,char2,char3,char4,char5);

		sumaAnt1 += evaluaFuncBool( sumaAnt1,sumaAnt1,sumaAnt1,sumaAnt1,sumaAnt1) + IV1;
		sumaAnt2 += evaluaFuncBool( sumaAnt2,sumaAnt2,sumaAnt2,sumaAnt2,sumaAnt2) + IV2;
		sumaAnt3 += evaluaFuncBool( sumaAnt3,sumaAnt3,sumaAnt3,sumaAnt3,sumaAnt3) + IV3;
		sumaAnt4 += evaluaFuncBool( sumaAnt4,sumaAnt4,sumaAnt4,sumaAnt4,sumaAnt4) + IV4;
		sumaAnt5 += evaluaFuncBool( sumaAnt5,sumaAnt5,sumaAnt5,sumaAnt5,sumaAnt5) + IV4;

		long mask1 = 0xffffffffL;
		long mask2 = -0x1;
		 
		hash = ((sumaAnt1 << 48) & mask2 ) |
	               ((sumaAnt1+sumaAnt2 << 32) & mask2 ) |
	               ((sumaAnt1+sumaAnt2+sumaAnt3 << 16) & mask1) |
	               ((sumaAnt3+sumaAnt4+sumaAnt5) & mask1);

		hash += evaluaFuncBool( hash,hash,hash,hash,hash) + IV3;
		hash += evaluaFuncBool( hash,hash,hash,hash,hash) + IV8 + input.Length+5+2+1;
		
		return longToBytes(hash);
  }

	public static byte[] longToBytes(long l) {
	    byte[] result = new byte[8];
	    for (int i = 7; i >= 0; i--) {
	        result[i] = (byte)(l & 0xFF);
	        l >>= 8;
	    }
	    return result;
	}

	public static string ToHexString(byte[] bytes) // 0xae00cf => "AE00CF "
	{
		string hexString = string.Empty;
		if (bytes != null)
		{
			StringBuilder strB = new StringBuilder();
 			for (int i = 0; i < bytes.Length; i++)
			{
				strB.Append(bytes[i].ToString("X2"));
			}
			hexString = strB.ToString();
		}
		return hexString;
	}
	
  static void Main() {
    Azrael64 hash = new Azrael64();
	
	byte[] hash1 = hash.getHashEval( Encoding.UTF8.GetBytes("") );
	string hex1 = BitConverter.ToString(hash1).Replace("-", string.Empty).ToLower();
	Console.WriteLine( hex1 );
	
  }
}
