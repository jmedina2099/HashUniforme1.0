/* Copyright (C) 2007-2017 - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Jorge Medina <medinarosas.jorge@gmail.com>, December 2017
 */
package hash;

import java.math.BigInteger;

/**
 * @author jmedina
 *
 */ 
public class Azrael64 implements FuncionHash {

	private static final boolean DEBUG_PARTIAL_HASH = false;
	private static final boolean DEBUG_FIRST_HASH = false;
	private static final boolean DEBUG_FINAL_HASH = false;
	private int rounds = 0;

	private boolean STRENGTH = false;

	private static final String EMPTY_STRING_SIMPLE =
			"7709881500987057892";

	private static final String EMPTY_STRING_STRENGTH = 
			"9121543017975456842";

	/**
	 * 
	 */
	public Azrael64( boolean twoIterations ) {
		STRENGTH = twoIterations;
	}

	@Override
	public BigInteger getHash(String o) {
		rounds  = 0;
		return new BigInteger(""+getHashEval( o.toString(), STRENGTH ));
	}
	

	private long getHashEval( String o, boolean strength ) {
		
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

		char1 += sumaAnt1;
		char2 += char3;
		char3 += char4;
		char4 += (long)o.charAt( 2 );
		char5 += sumaAnt2;
		sumaAnt5 += sumaAnt4;
		sumaAnt4 += sumaAnt3;
		sumaAnt3 += sumaAnt2;
		sumaAnt2 += sumaAnt1;
		sumaAnt1 += evaluaFuncBool( char1,char2,char3,char4,char5);

		// Main Loop.
		for( int i=2; i<o.length()-2; i++ ) {
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
		char4 += (long)o.charAt( o.length()-1 );
		char5 += sumaAnt2;
		sumaAnt5 += sumaAnt4;
		sumaAnt4 += sumaAnt3;
		sumaAnt3 += sumaAnt2;
		sumaAnt2 += sumaAnt1;
		sumaAnt1 += evaluaFuncBool( char1,char2,char3,char4,char5);

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
		}

		String input = ""+(hash<0?-hash:hash);
		if( strength ) { 
			if( DEBUG_FIRST_HASH ) {
				System.out.println( "**** FIRST HASH ("+input.length()+") chars = "+input );
				printAverage( input );
			}	
			return getHashEval( input, false );
		} else {
			if( DEBUG_FINAL_HASH ) {
				System.out.println( "**** FINAL HASH ("+input.length()+") chars = "+input );
				System.out.println( "**** OUTPUT [8] BYTES" );
				printAverage( input );
			}
			return hash;
		}
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
		return "Azrael64 "+(STRENGTH? "2x": "1x");
	}

	private static void printAverage( String cad ) {
		int promedio = 0;
		
		int digit;
		for( int i=0; i<cad.length(); i++ ) {
			digit = Integer.parseInt( ""+cad.charAt(i) );
			promedio += digit;
		}
		
		double avg = promedio/(double)cad.length();
		System.out.println( "===> avg="+avg );
	}

	/**
	 * 
	 * @param args
	 */
	public static void main(String[] args) {
		Azrael64 hash = new Azrael64( true );
		BigInteger eval = hash.getHash( "" );
		
		System.out.println( "===> hashEval="+eval );
		printAverage( eval.toString() );
	}
}
