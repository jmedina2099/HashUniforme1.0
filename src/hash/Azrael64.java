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
public class Azrael64 implements FuncionHash {

	private static final boolean DEBUG_PARTIAL_HASH = false;
	private static final boolean DEBUG_INTERMIDIATE_HASH = false;

	private int rounds = 0;

	private int numIterations = 2;
	private int iteration = 0;

	private static final String EMPTY_STRING_1_IT =
			"-10346949897525184";

	private static final String EMPTY_STRING_2_IT = 
			"-1613582014970097038";

	/**
	 * 
	 */
	public Azrael64() {
		this(1);
	}
	
	/**
	 * 
	 */
	public Azrael64( int numIterations ) {
		this.numIterations = numIterations;
	}

	@Override
	public BigInteger getHash(String o) {
		this.rounds  = 0;
		this.iteration  = 0;
		
		String eval = null;
		byte[] bites = o.getBytes( StandardCharsets.UTF_8 );
		for( ; iteration<numIterations; ) {
			iteration++;
			bites = getHashEval( bites );
			if( DEBUG_INTERMIDIATE_HASH ) {
				eval = new BigInteger( bites ).toString();
				System.out.println( "**** ["+iteration+"] HASH ("+eval.length()+") chars = "+eval );
				System.out.println( "**** OUTPUT ["+bites.length+"] BYTES" );
				System.out.println( "===> avg="+Statistics.getAverage( eval ) );
			}
		}
		
		return new BigInteger( bites );
	}
	

	private byte[] getHashEval( byte[] input ) {
		
		input = pad(input);

		long hash = 0;
		long char1=0,char2=0,char3=0,char4=0,char5=0;
		
		long sumaAnt1 = (long)input[ input.length-2 ];
		long sumaAnt2 = (long)input[ 2 ];
		long sumaAnt3 = 0;
		long sumaAnt4 = 0;
		long sumaAnt5 = 0;

		char1 += sumaAnt1;
		char2 += (long)input[ input.length-1 ];
		char3 += (long)input[ 0 ];
		char4 += (long)input[ 1 ];
		char5 += sumaAnt2;
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

		
		hash = ((sumaAnt1 << 48) & 0xffffffffffffffffL ) |
	               ((sumaAnt1+sumaAnt2 << 32) & 0xffffffffffffffffL ) |
	               ((sumaAnt1+sumaAnt2+sumaAnt3 << 16) & 0xffffffffL) |
	               ((sumaAnt3+sumaAnt4+sumaAnt5) & 0xffffffffL) +rounds;
					
		if( DEBUG_PARTIAL_HASH ) {
			System.out.println( "**** END APILACION 1x64:" );
			System.out.println( "**** hash = "+hash );
		}

		return longToBytes(hash);
	}
	
	public byte[] longToBytes(long x) {
	    ByteBuffer buffer = ByteBuffer.allocate(Long.BYTES);
	    buffer.putLong(x);
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

	private static void itera() {
		double total = 0d;
		
		int length = 2000000;
		double avg = 0;
		
		Azrael64 hash = null;
		BigInteger eval = null;
		for( int i=1; i<=length; i++ ) {
			hash = new Azrael64(i);
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

	public String toString() {
		return "Azrael64 "+numIterations+"x";
	}

	/**
	 * 
	 * @param args
	 */
	public static void main(String[] args) {
		Azrael64 hash = new Azrael64(2);

		BigInteger out = hash.getHash("");

		String cad = out.toString();
		System.out.println( "===> hashEval="+cad );
		
		double avg = Statistics.getAverage( cad );
		System.out.println( "===> avg="+(float)avg );
		
		itera();
	}
}
