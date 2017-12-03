package hash;

import java.math.BigInteger;

/**
 * @author jmedina
 *
 */ 
public class HashIterativeBoolean implements FuncionHash {

	private static final boolean DEBUG_PARTIAL_HASH = false;
	private int rounds = 0;

	public HashIterativeBoolean() {
	}

	@Override
	public BigInteger getHash(String o) {
		rounds  = 0;
		long hash = getHashEval( o.toString() ) + rounds;
		//System.out.println( "===> getHash = "+hash);
		return new BigInteger(""+(hash < 0 ? -hash : hash)); // Return hash <int> without modulus size of table.
	}
	

	private long getHashEval( String o) {
		
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
		
		hash = ((sumaAnt1 << 48) & 0xffffffffffffffffL ) |
	               ((sumaAnt1+sumaAnt2 << 32) & 0xffffffffffffffffL ) |
	               ((sumaAnt1+sumaAnt2+sumaAnt3 << 16) & 0xffffffffL) |
	               ((sumaAnt3+sumaAnt4+sumaAnt5) & 0xffffffffL);
					
		if( DEBUG_PARTIAL_HASH ) {
			System.out.println( "**** sumAnt5 = "+sumaAnt5 );
			System.out.println( "**** sumAnt4 = "+sumaAnt4 );
			System.out.println( "**** sumAnt3 = "+sumaAnt3 );
			System.out.println( "**** sumAnt2 = "+sumaAnt2 );
			System.out.println( "**** sumAnt1 = "+sumaAnt1 );
			System.out.println( "**** hash  = "+hash );
			
			System.out.println( "**** hashFinal = "+hash );
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
		long hash = evaluaFuncBool1( char1,char2,char3,char4,char5 ) +
				   evaluaFuncBool2( char1,char2,char3,char4,char5 ) +
				   evaluaFuncBool3( char1,char2,char3,char4,char5 ) +
				   evaluaFuncBool4( char1,char2,char3,char4,char5 ) +
				   evaluaFuncBool5( char1,char2,char3,char4,char5 ) +
				   evaluaFuncBool6( char1,char2,char3,char4,char5 ) +
				   evaluaFuncBool7( char1,char2,char3,char4,char5 ) +
				   evaluaFuncBool8( char1,char2,char3,char4,char5 ) +
				   evaluaFuncBool9( char1,char2,char3,char4,char5 ) +
				   evaluaFuncBool10( char1,char2,char3,char4,char5 ) +
				   evaluaFuncBool11( char1,char2,char3,char4,char5 ) +
				   evaluaFuncBool12( char1,char2,char3,char4,char5 ) +
				   evaluaFuncBool13( char1,char2,char3,char4,char5 ) +
				   evaluaFuncBool14( char1,char2,char3,char4,char5 ) +
				   evaluaFuncBool15( char1,char2,char3,char4,char5 ) +
				   evaluaFuncBool16( char1,char2,char3,char4,char5 );
		
		if( DEBUG_PARTIAL_HASH ) {
			System.out.println( "**** hashParcial = "+hash );
		}
		
		rounds++;
		
		return hash;	
	}
	
	//2996-(0,1,1,1),2996-(2,1,0,1),2997-(1,0,0,1),2997-(1,1,0,1),2997-(2,0,0,0),2997-(2,0,0,1),2997-(3,0,1,1),2997-(3,1,0,1),
	
	// (0,1,1,1)=(+,^,^,^)
	public long evaluaFuncBool1(Long char1, Long char2, Long char3, Long char4, Long char5) {
		return ( char1 + char2 ) ^ ( char3 ^ char4 ) ^ char5;
	}

	// (2,1,0,1)=(&,^,+,^)
	public long evaluaFuncBool2(Long char1, Long char2, Long char3, Long char4, Long char5) {
		return ( char1 & char2 ) ^ ( char3 + char4 ) ^ char5;
	}	
	
	// (1,0,0,1)=(^,+,+,^)
	public long evaluaFuncBool3(Long char1, Long char2, Long char3, Long char4, Long char5) {
		return ( char1 ^ char2 ) + ( char3 + char4 ) ^ char5;
	}	
	
	// (1,1,0,1)=(^,^,+,^)
	public long evaluaFuncBool4(Long char1, Long char2, Long char3, Long char4, Long char5) {
		return ( char1 ^ char2 ) ^ ( char3 + char4 ) ^ char5;
	}
	
	// (2,0,0,0)=(&,+,+,+)
	public long evaluaFuncBool5(Long char1, Long char2, Long char3, Long char4, Long char5) {
		return ( char1 & char2 ) + ( char3 + char4 ) + char5;
	}

	// (2,0,0,1)=(&,+,+,^)
	public long evaluaFuncBool6(Long char1, Long char2, Long char3, Long char4, Long char5) {
		return ( char1 & char2 ) + ( char3 + char4 ) ^ char5;
	}

	// (3,0,1,1)=(^,^,+,^)
	public long evaluaFuncBool7(Long char1, Long char2, Long char3, Long char4, Long char5) {
		return ( char1 ^ char2 ) ^ ( char3 + char4 ) ^ char5;
	}

	// (3,1,0,1)=(|,^,+,^)
	public long evaluaFuncBool8(Long char1, Long char2, Long char3, Long char4, Long char5) {
		return ( char1 | char2 ) ^ ( char3 + char4 ) ^ char5;
	}

	//2997-(3,3,0,1),2998-(0,0,0,1),2998-(0,2,0,1),2998-(1,0,1,1),2998-(3,1,1,1),2999-(0,0,1,1),2999-(0,1,0,1),2999-(0,2,0,0),

	// (3,3,0,1)=(|,|,+,^)
	public long evaluaFuncBool9(Long char1, Long char2, Long char3, Long char4, Long char5) {
		return ( char1 | char2 ) | ( char3 + char4 ) ^ char5;
	}

	// (0,0,0,1)=(+,+,+,^)
	public long evaluaFuncBool10(Long char1, Long char2, Long char3, Long char4, Long char5) {
		return ( char1 + char2 ) + ( char3 + char4 ) ^ char5;
	}
	
	// (0,2,0,1)=(+,&,+,^)
	public long evaluaFuncBool11(Long char1, Long char2, Long char3, Long char4, Long char5) {
		return ( char1 + char2 ) & ( char3 + char4 ) ^ char5;
	}
	
	// (1,0,1,1)=(^,+,^,^)
	public long evaluaFuncBool12(Long char1, Long char2, Long char3, Long char4, Long char5) {
		return ( char1 ^ char2 ) + ( char3 ^ char4 ) ^ char5;
	}
	
	// (3,1,1,1)=(|,^,^,^)
	public long evaluaFuncBool13(Long char1, Long char2, Long char3, Long char4, Long char5) {
		return ( char1 | char2 ) ^ ( char3 ^ char4 ) ^ char5;
	}

	// (0,0,1,1)=(+,+,^,^)
	public long evaluaFuncBool14(Long char1, Long char2, Long char3, Long char4, Long char5) {
		return ( char1 + char2 ) + ( char3 ^ char4 ) ^ char5;
	}

	// (0,1,0,1)=(+,^,+,^)
	public long evaluaFuncBool15(Long char1, Long char2, Long char3, Long char4, Long char5) {
		return ( char1 + char2 ) ^ ( char3 + char4 ) ^ char5;
	}	
	
	// (0,2,0,0)=(+,&,+,+)
	public long evaluaFuncBool16(Long char1, Long char2, Long char3, Long char4, Long char5) {
		return ( char1 + char2 ) & ( char3 + char4 ) + char5;
	}
	
	public String toString() {
		return "HashIterativeBoolean";
	}

	
	/**
	 * 
	 * @param args
	 */
	public static void main(String[] args) {
		HashIterativeBoolean hash = new HashIterativeBoolean();
		System.out.println( "===> hashEval="+hash.getHash( "" ) );
	}
}
