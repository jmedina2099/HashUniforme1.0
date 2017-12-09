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
		return new BigInteger( ""+o.hashCode() );
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
