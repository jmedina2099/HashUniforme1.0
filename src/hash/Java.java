/**
 * 
 */
package hash;

import java.math.BigInteger;

/**
 * @author jmedina
 *
 */
public class Java implements FuncionHash {

	/**
	 * 
	 */
	public Java() {
	}
	
	@Override
	public BigInteger getHash(String o) {
		int hashCode = o.hashCode();
		if( hashCode < 0 ) {
			hashCode = hashCode*-1;
		}
		return new BigInteger( ""+hashCode );
	}
	
	public String toString() {
		return "HashJava";
	}
	
	/**
	 * @param args
	 */
	public static void main(String[] args) {
		Java hash = new Java(); 
		System.out.println( "===> "+hash.getHash( "friabili" ) );
		System.out.println( "===> "+hash.getHash( "tensada" ) );
	}

}
