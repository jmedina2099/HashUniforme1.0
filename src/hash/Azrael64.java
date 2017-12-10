/* Copyright (C) 2007-2017 - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Jorge Medina <medinarosas.jorge@gmail.com>, December 2017
 */
package hash;

import java.math.BigInteger;

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
			"7709881500987057892";

	private static final String EMPTY_STRING_2_IT = 
			"9121543017975456842";

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
		
		String eval = o;
		for( ; iteration<numIterations; ) {
			iteration++;
			eval = ""+getHashEval( eval );
			if( DEBUG_INTERMIDIATE_HASH ) { 
				System.out.println( "**** ["+iteration+"] HASH ("+eval.length()+") chars = "+eval );
				System.out.println( "**** OUTPUT ["+Long.BYTES+"] BYTES" );
				System.out.println( "===> avg="+Statistics.getAverage( eval ) );
			}
		}
		
		return new BigInteger( eval );
	}
	

	private long getHashEval( String o ) {
		
		o = pad(o);

		long hash = 0;
		long char1=0,char2=0,char3=0,char4=0,char5=0;
		
		long sumaAnt1 = (long)o.charAt( o.length()-2 );
		long sumaAnt2 = (long)o.charAt( 2 );
		long sumaAnt3 = 0;
		long sumaAnt4 = 0;
		long sumaAnt5 = 0;

		char1 += sumaAnt1;
		char2 += (long)o.charAt( o.length()-1 );
		char3 += (long)o.charAt( 0 );
		char4 += (long)o.charAt( 1 );
		char5 += sumaAnt2;
		sumaAnt5 += sumaAnt4;
		sumaAnt4 += sumaAnt3;
		sumaAnt3 += sumaAnt2;
		sumaAnt2 += sumaAnt1;
		sumaAnt1 += evaluaFuncBool( char1,char2,char3,char4,char5);

		// Main Loop.
		for( int i=1; i<o.length()-1; i++ ) {
			char1 += sumaAnt1;
			char2 += char3;
			char3 += char4;
			char4 += (long)o.charAt( i+1 );
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
		char4 += (long)o.charAt( 0 );
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

		return hash;
	}
	
	private String pad(String o) {
		int size = o.length();
		
		if( size < 3 ) {
			for( int i=0; i<3-size; i++ ) {
				o += "0";
			}
		}
		
		return o+size;
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
		return "Azrael64 "+numIterations+"x";
	}

	/**
	 * 
	 * @param args
	 */
	public static void main(String[] args) {
		double total = 0d;
		double avg;
		
		int length = 100000;
		
		BigInteger eval = null;
		FuncionHash hash = null;
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
}
