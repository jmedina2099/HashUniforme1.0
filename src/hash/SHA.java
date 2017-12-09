/**
 * 
 */
package hash;

import java.math.BigInteger;
import java.nio.charset.StandardCharsets;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;

/**
 * @author jmedina
 * 
 */
public class SHA implements FuncionHash {

	public SHA() {
	}

	public BigInteger getHash(String o ) {
		MessageDigest digest = null;
		try {
			digest = MessageDigest.getInstance("SHA-256");
		} catch (NoSuchAlgorithmException e) {
			e.printStackTrace();
		}
		byte[] hash = digest.digest(o.getBytes(StandardCharsets.UTF_8));
		return new BigInteger(hash);
	}
	
	public String toString() {
		return "HashSHA";
	}

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		SHA hash = new SHA(); 

		BigInteger eval = hash.getHash( "" );
		System.out.println( "===> hashEval="+eval );
		
		int promedio = 0;
		
		String cad = eval.toString();
		int digit;
		for( int i=0; i<cad.length(); i++ ) {
			digit = Integer.parseInt( ""+cad.charAt(i) );
			promedio += digit;
		}
		
		double avg = promedio/(double)cad.length();
		System.out.println( "===> avg="+avg );
	
	}

}
