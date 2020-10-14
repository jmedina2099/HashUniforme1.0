/* Copyright (C) 2007-2020 - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential (eyes-only)
 * Written by Jorge Medina <medinarosas.jorgealberto@gmail.com>, October 13th, 2020
 */
package hash;

import java.math.BigInteger;
import java.nio.ByteBuffer;
import java.nio.charset.StandardCharsets;

import org.apache.commons.codec.binary.Hex;

/**
 * @author jmedina
 *
 */ 
public class AzraelX2 implements FuncionHash {

	private static final boolean DEBUG_PARTIAL_HASH = false;
	private static final boolean DEBUG_INTERMIDIATE_HASH = false;

	private int rounds = 0;

	private int numIterations = 1;
	private int iteration = 0;


	private static final String EMPTY_STRING_1_IT = 
			"f554aaf304b84b547b43c583aa4c0cba2130a61ab1001a1e30c0d8e6f2d62d44f84af258e16c696480d609df3ac941d7b42780d82a0aa7b5a197b299c892b2b2049432c1c76931ee501368337eec18aa";

	private static final String EMPTY_STRING_2_IT = 
			"f656616f9d295f76916eba1c07528cc52c9d042300a87d71a81c7f7e75840c6da4e206ed55d0c82fc9a167e677d3e9baeb652764b41d868983432995ab36c5e2a2a2276630c0860b98ee0c91081677e8";
	
	/**
	 * 
	 */
	public AzraelX2() {
		this(1);
	}

	/**
	 *
	 */
	public AzraelX2( int numIterations ) {
		this.numIterations  = numIterations;
	}
	
	@Override
	public BigInteger getHash(byte[] input) {
		this.rounds  = 0;
		this.iteration  = 0;

		int promedioPro = 0;
		for( ; iteration<numIterations; ) {
			iteration++;
			input = getHashEval( input );
			if( DEBUG_INTERMIDIATE_HASH ) {
				
				String value = Hex.encodeHexString(input);
				
				int min=256, max=-1;
				
				int promedioHex = 0;
				int byteNum = 0;
				for( int i=0; i<input.length; i++ ) {
					byteNum = input[i]<0? 127-input[i]: input[i];
					promedioHex += byteNum;
					
					if( byteNum < min ) {
						min = byteNum;
					}
					if( max < byteNum ) {
						max = byteNum;
					}
				}
				double promedio = promedioHex/(double)input.length;
				promedio = (promedio*100)/255;
				promedio = ((int)(promedio*1000000))/1000000d;
				
				StringBuffer sb = new StringBuffer();
				//sb.append( promedio );
				
				if( iteration > this.numIterations - 100 ) {
					promedioPro += promedio;
					
					sb.append( "[" )
					  .append( value )
					  .append( "]-[bits]=[" )
					  .append( input.length*8 )
					  .append( "]-[avg]=[" )
					  .append( promedio )
					  .append( "]-[min,max]=[" )
					  .append( min )
					  .append( "," )
					  .append( max )
					  .append( "]-[iter]=[" )
					  .append( iteration )
					  .append( "]" );					
					 
					//System.out.println( iteration +"---"+ sb.toString() );
					System.out.println( sb.toString() );
				}
			}
		}
		
		//System.out.println( promedioPro/100d );

		return new BigInteger( input );
	}

	/**
	 * 
	 * @param input
	 * @return 64 bytes / 512 bits
	 */
	public byte[] getHashEval( byte[] input ) {
		
		input = pad(input);

		// fractional part of sqrt(2) and sqrt(3)
		long IV1 = 0x6a09e667bb67ae85l;
		long IV2 = 0x3c6ef372a54ff53al;
		long IV3 = 0x510e527f9b05688cl;
		long IV4 = 0x1f83d9ab5be0cd19l;
		long IV5 = 0x428a2f9871374491l;
		long IV6 = 0xb5c0fbcfe9b5dba5l;
		long IV7 = 0x3956c25b59f111f1l;
		long IV8 = 0x923f82a4ab1c5ed5l;
		long IV9 = 0xd807aa9812835b01l;
		long IV10 = 0x243185be550c7dc3l;

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
		long sumaAnt6 = IV1;
		long sumaAnt7 = IV3;
		long sumaAnt8 = IV8;
		
		char1 += (long)input[ input.length-2 ];
		char2 += (long)input[ input.length-1 ];
		char3 += (long)input[ 0 ];
		char4 += (long)input[ 1 ];
		char5 += (long)input[ 2 ];

		sumaAnt8 += sumaAnt7;
		sumaAnt7 += sumaAnt6;
		sumaAnt6 += sumaAnt5;
		sumaAnt5 += sumaAnt4;
		sumaAnt4 += sumaAnt3;
		sumaAnt3 += sumaAnt2;
		sumaAnt2 += sumaAnt1;
		sumaAnt1 += evaluaFuncBool( char1,char2,char3,char4,char5);

		// Main Loop.
		for( int i=1; i<input.length-1; i++ ) {
			char1 += sumaAnt1;
			char2 += char3;
			char3 += char4;
			char4 += (long)input[ i+1 ];
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
		char4 += (long)input[ 0 ];
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
			System.out.println( "**** END ACUMULACION 8x64: ("+rounds+") rounds" );
			System.out.println( "**** sumAnt8 = "+sumaAnt8 );
			System.out.println( "**** sumAnt7 = "+sumaAnt7 );
			System.out.println( "**** sumAnt6 = "+sumaAnt6 );
			System.out.println( "**** sumAnt5 = "+sumaAnt5 );
			System.out.println( "**** sumAnt4 = "+sumaAnt4 );
			System.out.println( "**** sumAnt3 = "+sumaAnt3 );
			System.out.println( "**** sumAnt2 = "+sumaAnt2 );
			System.out.println( "**** sumAnt1 = "+sumaAnt1 );
		}
		
		sumaAnt1 += evaluaFuncBool( sumaAnt1,sumaAnt1,sumaAnt1,sumaAnt1,sumaAnt1) + IV2;
		sumaAnt2 += evaluaFuncBool( sumaAnt2,sumaAnt2,sumaAnt2,sumaAnt2,sumaAnt2) + IV4;
		sumaAnt3 += evaluaFuncBool( sumaAnt3,sumaAnt3,sumaAnt3,sumaAnt3,sumaAnt3) + IV6;
		sumaAnt4 += evaluaFuncBool( sumaAnt4,sumaAnt4,sumaAnt4,sumaAnt4,sumaAnt4) + IV8;
		sumaAnt5 += evaluaFuncBool( sumaAnt5,sumaAnt5,sumaAnt5,sumaAnt5,sumaAnt5) + IV1;
		sumaAnt6 += evaluaFuncBool( sumaAnt6,sumaAnt6,sumaAnt6,sumaAnt6,sumaAnt6) + IV3;
		sumaAnt7 += evaluaFuncBool( sumaAnt7,sumaAnt7,sumaAnt7,sumaAnt7,sumaAnt7) + IV5;
		sumaAnt8 += evaluaFuncBool( sumaAnt8,sumaAnt8,sumaAnt8,sumaAnt8,sumaAnt8) + IV9;

		if( DEBUG_PARTIAL_HASH ) {
			System.out.println( "**** END DISPERSION 8x64: ("+rounds+") rounds" );
			System.out.println( "**** sumAnt8 = "+sumaAnt8 );
			System.out.println( "**** sumAnt7 = "+sumaAnt7 );
			System.out.println( "**** sumAnt6 = "+sumaAnt6 );
			System.out.println( "**** sumAnt5 = "+sumaAnt5 );
			System.out.println( "**** sumAnt4 = "+sumaAnt4 );
			System.out.println( "**** sumAnt3 = "+sumaAnt3 );
			System.out.println( "**** sumAnt2 = "+sumaAnt2 );
			System.out.println( "**** sumAnt1 = "+sumaAnt1 );
		}

		// (1/1,2/1,2,3/3,4,5,6/
			long hash1 = ((sumaAnt1 << 48) & 0xffffffffffffffffL ) |
	               ((sumaAnt1+sumaAnt2 << 32) & 0xffffffffffffffffL ) |
	               ((sumaAnt1+sumaAnt2+sumaAnt3 << 16) & 0xffffffffL) |
	               ((sumaAnt3+sumaAnt4+sumaAnt5+sumaAnt6) & 0xffffffffL);
		// (1/1,3/2,3,4/4,5,2,7/
			long hash2 = ((sumaAnt1 << 48) & 0xffffffffffffffffL ) |
	               ((sumaAnt1+sumaAnt3 << 32) & 0xffffffffffffffffL ) |
	               ((sumaAnt2+sumaAnt3+sumaAnt4 << 16) & 0xffffffffL) |
	               ((sumaAnt4+sumaAnt5+sumaAnt1+sumaAnt7) & 0xffffffffL);
		// (3/1,4/3,4,5/5,1,2,8/
			long hash3 = ((sumaAnt3 << 48) & 0xffffffffffffffffL ) |
	               ((sumaAnt1+sumaAnt4 << 32) & 0xffffffffffffffffL ) |
	               ((sumaAnt3+sumaAnt4+sumaAnt5 << 16) & 0xffffffffL) |
	               ((sumaAnt5+sumaAnt1+sumaAnt2+sumaAnt8) & 0xffffffffL);
		// (2/1,5/4,5,1/1,2,3,6/
			long hash4 = ((sumaAnt2 << 48) & 0xffffffffffffffffL ) |
	               ((sumaAnt1+sumaAnt5 << 32) & 0xffffffffffffffffL ) |
	               ((sumaAnt4+sumaAnt5+sumaAnt1 << 16) & 0xffffffffL) |
	               ((sumaAnt1+sumaAnt2+sumaAnt3+sumaAnt6) & 0xffffffffL);
		// (2/1,1/5,1,2/2,3,4,7/
			long hash5 = ((sumaAnt2 << 48) & 0xffffffffffffffffL ) |
	               ((sumaAnt1+sumaAnt1 << 32) & 0xffffffffffffffffL ) |
	               ((sumaAnt5+sumaAnt1+sumaAnt2 << 16) & 0xffffffffL) |
	               ((sumaAnt2+sumaAnt3+sumaAnt4+sumaAnt7) & 0xffffffffL);
		// (1/1,4/2,4,5/3,6,7,8/
			long hash6 = ((sumaAnt1 << 48) & 0xffffffffffffffffL ) |
		           ((sumaAnt1+sumaAnt4 << 32) & 0xffffffffffffffffL ) |
		           ((sumaAnt2+sumaAnt4+sumaAnt5 << 16) & 0xffffffffL) |
		           ((sumaAnt3+sumaAnt6+sumaAnt7+sumaAnt8) & 0xffffffffL);
		// (3/1,5/4,5,6/7,8,1,2/
			long hash7 = ((sumaAnt3 << 48) & 0xffffffffffffffffL ) |
		           ((sumaAnt1+sumaAnt5 << 32) & 0xffffffffffffffffL ) |
		           ((sumaAnt4+sumaAnt5+sumaAnt6 << 16) & 0xffffffffL) |
		           ((sumaAnt7+sumaAnt8+sumaAnt1+sumaAnt2) & 0xffffffffL);
		// (3/1,6/7,1,2/5,6,7,8/
			long hash8 = ((sumaAnt3 << 48) & 0xffffffffffffffffL ) |
		           ((sumaAnt1+sumaAnt6 << 32) & 0xffffffffffffffffL ) |
		           ((sumaAnt7+sumaAnt1+sumaAnt2 << 16) & 0xffffffffL) |
		           ((sumaAnt5+sumaAnt6+sumaAnt7+sumaAnt8) & 0xffffffffL);
		// (2/1,3/7,2,4/4,5,7,8/
			long hash9 = ((sumaAnt2 << 48) & 0xffffffffffffffffL ) |
		           ((sumaAnt1+sumaAnt3 << 32) & 0xffffffffffffffffL ) |
		           ((sumaAnt7+sumaAnt2+sumaAnt4 << 16) & 0xffffffffL) |
		           ((sumaAnt4+sumaAnt5+sumaAnt7+sumaAnt8) & 0xffffffffL);
		// (1/2,5/6,7,8/1,4,5,8/
			long hashA = ((sumaAnt1 << 48) & 0xffffffffffffffffL ) |
		           ((sumaAnt2+sumaAnt5 << 32) & 0xffffffffffffffffL ) |
		           ((sumaAnt6+sumaAnt7+sumaAnt8 << 16) & 0xffffffffL) |
		           ((sumaAnt1+sumaAnt4+sumaAnt5+sumaAnt8) & 0xffffffffL);

			if( DEBUG_PARTIAL_HASH ) {
				System.out.println( "**** END APILACION 8x64: ("+rounds+") rounds" );
				System.out.println( "**** hashA = "+hashA );
				System.out.println( "**** hash9 = "+hash9 );
				System.out.println( "**** hash8 = "+hash8 );
				System.out.println( "**** hash7 = "+hash7 );
				System.out.println( "**** hash6 = "+hash6 );
				System.out.println( "**** hash5 = "+hash5 );
				System.out.println( "**** hash4 = "+hash4 );
				System.out.println( "**** hash3 = "+hash3 );
				System.out.println( "**** hash2 = "+hash2 );
				System.out.println( "**** hash1 = "+hash1 );
			}

			hash1 += evaluaFuncBool( hash1,hash1,hash1,hash1,hash1) + IV6;
			hash2 += evaluaFuncBool( hash2,hash2,hash2,hash2,hash2) + IV7;
			hash3 += evaluaFuncBool( hash3,hash3,hash3,hash3,hash3) + IV8;
			hash4 += evaluaFuncBool( hash4,hash4,hash4,hash4,hash4) + IV9;
			hash5 += evaluaFuncBool( hash5,hash5,hash5,hash5,hash5) + IV10;
			hash6 += evaluaFuncBool( hash6,hash6,hash6,hash6,hash6) + IV1;
			hash7 += evaluaFuncBool( hash7,hash7,hash7,hash7,hash7) + IV3;
			hash8 += evaluaFuncBool( hash8,hash8,hash8,hash8,hash8) + IV8;
			hash9 += evaluaFuncBool( hash9,hash9,hash9,hash9,hash9) + IV5;
			hashA += evaluaFuncBool( hashA,hashA,hashA,hashA,hashA) + IV7;
		
			if( DEBUG_PARTIAL_HASH ) {
				System.out.println( "**** END DISPERSION FINAL 8x64: ("+rounds+") rounds" );
				System.out.println( "**** hashA END = "+hashA );
				System.out.println( "**** hash9 END = "+hash9 );
				System.out.println( "**** hash8 END = "+hash8 );
				System.out.println( "**** hash7 END = "+hash7 );
				System.out.println( "**** hash6 END = "+hash6 );
				System.out.println( "**** hash5 END = "+hash5 );
				System.out.println( "**** hash4 END = "+hash4 );
				System.out.println( "**** hash3 END = "+hash3 );
				System.out.println( "**** hash2 END = "+hash2 );
				System.out.println( "**** hash1 END = "+hash1 );
			}
			
		return longToBytes( new long[]{ hash1,
										hash2,
										hash3,
										hash4,
										hash5,
										hash6,
										hash7,
										hash8,
										hash9,
										hashA +rounds } );
	}
	
	public byte[] longToBytes(long[] x) {
	    ByteBuffer buffer = ByteBuffer.allocate(Long.BYTES*x.length);
	    for( int i=0; i<x.length; i++ ) {
	    	buffer.putLong(x[i]);
	    }
	    return buffer.array();
	}
	
	private byte[] pad(byte[] data) {
        int length = data.length;
        int tail = length % 64;
        int padding;

        if ((64 - tail >= 9)) {
            padding = 64 - tail;
        } else {
            padding = 128 - tail;
        }

        byte[] pad = new byte[padding];
        pad[0] = (byte) 0x80;
        long bits = length * 8;
        for (int i = 0; i < 8; i++) {
            pad[pad.length - 1 - i] = (byte) ((bits >>> (8 * i)) & 0xFF);
        }

        byte[] output = new byte[length + padding];
        System.arraycopy(data, 0, output, 0, length);
        System.arraycopy(pad, 0, output, length, pad.length);

        return output;
    }
	
	public long evaluaFuncBool(Long char1, Long char2, Long char3, Long char4, Long char5) {
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
	
	public String toString() {
		return "AzraelX2 "+numIterations+"x";
	}
	
	/**
	 * 
	 * @param args
	 */
	public static void main(String[] args) {
		
		selfTest();
		
		int tope = 2;
		AzraelX2 hash = new AzraelX2(tope);
		
		long timeIni = System.currentTimeMillis();
		System.out.println( "=====> INIT ("+tope+")-"+timeIni );

		hash.getHash( "".getBytes(StandardCharsets.UTF_8) );
		
		long timeNow = System.currentTimeMillis() - timeIni;
		System.out.println( "TIME = "+
				"["+(timeNow/(1000.0))+"] secs,"+
				"["+(timeNow/(1000.0*60.0))+"] mins." );

		/*
		String cadena = "";
		
		Azrael512 hash = new Azrael512(1);
		byte[] hash1 = hash.getHashEval( cadena.getBytes(StandardCharsets.UTF_8) );
		hex1 = Hex.encodeHexString( hash1 );
		System.out.println( "["+cadena+"]=["+hex1+"]" );
		*/

		/*
		String input = null;
		input = "Azrael512";
		System.out.println( getHash(input,hash)+" == "+input );
		input = "Jorge Alberto Medina Rosas";
		System.out.println( getHash(input,hash)+" == "+input );
		*/
	}
	
	public static void selfTest() {
		AzraelX2 hash = new AzraelX2(1);
		byte[] hash1 = hash.getHashEval( "".getBytes(StandardCharsets.UTF_8) );
		byte[] hash2 = hash.getHashEval( hash1 );
		String hex1 = Hex.encodeHexString( hash1 );
		String hex2 = Hex.encodeHexString( hash2 );
		
		System.out.println( hex1+" == EMPTY (1) ="+hex1.equals(EMPTY_STRING_1_IT) );
		System.out.println( hex2+" == EMPTY (2) ="+hex2.equals(EMPTY_STRING_2_IT) );
	}
	
	public static String getHash( String input, AzraelX2 hash ) {
		byte[] hashEval = hash.getHashEval( input.getBytes(StandardCharsets.UTF_8) );
		String hexVal = Hex.encodeHexString( hashEval );
		return hexVal;
	}
}
