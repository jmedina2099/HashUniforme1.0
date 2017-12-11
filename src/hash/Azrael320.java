/* Copyright (C) 2007-2017 - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Jorge Medina <medinarosas.jorge@gmail.com>, December 2017
 */
package hash;

import java.math.BigInteger;
import java.nio.ByteBuffer;
import java.nio.charset.StandardCharsets;

import misc.Statistics;

/**
 * @author jmedina
 *
 */ 
public class Azrael320 implements FuncionHash {

	private static final boolean DEBUG_PARTIAL_HASH = false;
	private static final boolean DEBUG_INTERMIDIATE_HASH = false;

	private int rounds = 0;

	private int numIterations = 2;
	private int iteration = 0;


	private static final String EMPTY_STRING_1_IT = 
			"-627277315191390235940789440215439439324534235220350993358900696794980670260475751337677187537924";

	private static final String EMPTY_STRING_2_IT = 
			"-430582002577382478205708536629329591455672661905183236698410908785849449112827509800523924172481";

	/**
	 * 
	 */
	public Azrael320() {
		this(2);
	}

	/**
	 *
	 */
	public Azrael320( int numIterations ) {
		this.numIterations  = numIterations;
	}
	
	@Override
	public BigInteger getHash(String o) {
		this.rounds  = 0;
		this.iteration  = 0;

		double avg = 0;
		double promedio = 0;
		
		String eval = null;
		byte[] bites = o.getBytes( StandardCharsets.UTF_8 );

		for( ; iteration<numIterations; ) {
			iteration++;
			bites = getHashEval( bites );
			if( DEBUG_INTERMIDIATE_HASH ) {
				eval = new BigInteger( bites ).toString();
				avg = Statistics.getAverage( eval );
				//System.out.println( "**** ["+iteration+"] HASH ("+eval.length()+") chars = "+eval );
				//System.out.println( "**** OUTPUT ["+bites.length+"] BYTES" );
				//System.out.println( "===> avg="+avg );
				promedio += avg;
			}
		}

		if( DEBUG_INTERMIDIATE_HASH ) {
			promedio = promedio/(double)numIterations;
			System.out.println( "===> promedio ["+numIterations+"]="+promedio );
		}
		
		return new BigInteger( bites );
	}

	/**
	 * 
	 * @param o
	 * @param inmediate
	 * @return 40 bytes / 320 bits
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

		char1 += (long)input[ input.length-2 ];
		char2 += (long)input[ input.length-1 ];
		char3 += (long)input[ 0 ];
		char4 += (long)input[ 1 ];
		char5 += (long)input[ 2 ];
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
		sumaAnt5 += sumaAnt4;
		sumaAnt4 += sumaAnt3;
		sumaAnt3 += sumaAnt2;
		sumaAnt2 += sumaAnt1;
		sumaAnt1 += evaluaFuncBool( char1,char2,char3,char4,char5);
		
		if( DEBUG_PARTIAL_HASH ) {
			System.out.println( "**** END ACUMULACION 5x64: ("+rounds+") rounds" );
			System.out.println( "**** sumAnt5 = "+sumaAnt5 );
			System.out.println( "**** sumAnt4 = "+sumaAnt4 );
			System.out.println( "**** sumAnt3 = "+sumaAnt3 );
			System.out.println( "**** sumAnt2 = "+sumaAnt2 );
			System.out.println( "**** sumAnt1 = "+sumaAnt1 );
		}
		
		sumaAnt1 += evaluaFuncBool( sumaAnt1,sumaAnt1,sumaAnt1,sumaAnt1,sumaAnt1) + IV1;
		sumaAnt2 += evaluaFuncBool( sumaAnt2,sumaAnt2,sumaAnt2,sumaAnt2,sumaAnt2) + IV2;
		sumaAnt3 += evaluaFuncBool( sumaAnt3,sumaAnt3,sumaAnt3,sumaAnt3,sumaAnt3) + IV3;
		sumaAnt4 += evaluaFuncBool( sumaAnt4,sumaAnt4,sumaAnt4,sumaAnt4,sumaAnt4) + IV4;
		sumaAnt5 += evaluaFuncBool( sumaAnt5,sumaAnt5,sumaAnt5,sumaAnt5,sumaAnt5) + IV5;

		if( DEBUG_PARTIAL_HASH ) {
			System.out.println( "**** END DISPERSION 5x64: ("+rounds+") rounds" );
			System.out.println( "**** sumAnt5 = "+sumaAnt5 );
			System.out.println( "**** sumAnt4 = "+sumaAnt4 );
			System.out.println( "**** sumAnt3 = "+sumaAnt3 );
			System.out.println( "**** sumAnt2 = "+sumaAnt2 );
			System.out.println( "**** sumAnt1 = "+sumaAnt1 );
		}

			long hash1 = ((sumaAnt1 << 48) & 0xffffffffffffffffL ) |
	               ((sumaAnt1+sumaAnt2 << 32) & 0xffffffffffffffffL ) |
	               ((sumaAnt1+sumaAnt2+sumaAnt3 << 16) & 0xffffffffL) |
	               ((sumaAnt3+sumaAnt4+sumaAnt5) & 0xffffffffL);
			long hash2 = ((sumaAnt1 << 48) & 0xffffffffffffffffL ) |
	               ((sumaAnt1+sumaAnt3 << 32) & 0xffffffffffffffffL ) |
	               ((sumaAnt2+sumaAnt3+sumaAnt4 << 16) & 0xffffffffL) |
	               ((sumaAnt4+sumaAnt5+sumaAnt1) & 0xffffffffL);
			long hash3 = ((sumaAnt3 << 48) & 0xffffffffffffffffL ) |
	               ((sumaAnt1+sumaAnt4 << 32) & 0xffffffffffffffffL ) |
	               ((sumaAnt3+sumaAnt4+sumaAnt5 << 16) & 0xffffffffL) |
	               ((sumaAnt5+sumaAnt1+sumaAnt2) & 0xffffffffL);
			long hash4 = ((sumaAnt2 << 48) & 0xffffffffffffffffL ) |
	               ((sumaAnt1+sumaAnt5 << 32) & 0xffffffffffffffffL ) |
	               ((sumaAnt4+sumaAnt5+sumaAnt1 << 16) & 0xffffffffL) |
	               ((sumaAnt1+sumaAnt2+sumaAnt3) & 0xffffffffL);
			long hash5 = ((sumaAnt2 << 48) & 0xffffffffffffffffL ) |
	               ((sumaAnt1+sumaAnt1 << 32) & 0xffffffffffffffffL ) |
	               ((sumaAnt5+sumaAnt1+sumaAnt2 << 16) & 0xffffffffL) |
	               ((sumaAnt2+sumaAnt3+sumaAnt4) & 0xffffffffL);

			if( DEBUG_PARTIAL_HASH ) {
				System.out.println( "**** END APILACION 5x64:" );
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
		
			if( DEBUG_PARTIAL_HASH ) {
				System.out.println( "**** END DISPERSION FINAL 5x64: ("+rounds+") rounds" );
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
										hash5+rounds} );
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
	
	public String toString() {
		return "Azrael320 "+numIterations+"x";
	}

	private static void itera() {
		double total = 0d;
		
		int length = 1000000;
		double avg = 0;
		
		Azrael320 hash = null;
		BigInteger eval = null;
		for( int i=1; i<=length; i++ ) {
			hash = new Azrael320(i);
			eval = hash.getHash( "" );
	
			//System.out.println( "===> hashEval="+eval );
			avg = Statistics.getAverage( eval.toString() );
			total += avg;
			if( i % 500 == 0) {
				System.out.println( "===> ["+i+"]="+(float)avg+"\t promedio="+total/(double)i );
			}
		}
		
		total = total/(double)length;
		System.out.println( "===> AVG="+total );

		System.out.println( "===> hashEval="+eval );
		avg = Statistics.getAverage( eval.toString() );
		System.out.println( "===> avg="+(float)avg );
	}
	
	/**
	 * 
	 * @param args
	 */
	public static void main(String[] args) {

		Azrael320 hash = new Azrael320(1000000);

		BigInteger out = hash.getHash("");

		String cad = out.toString();
		System.out.println( "===> hashEval="+cad );
		
		double avg = Statistics.getAverage( cad );
		System.out.println( "===> avg="+(float)avg );
		
		//itera();
		
	}
}
