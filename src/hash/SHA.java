/**
 * 
 */
package hash;

import java.math.BigInteger;
import java.nio.charset.StandardCharsets;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;

import misc.Statistics;

/**
 * @author jmedina
 * 
 */
public class SHA implements FuncionHash {

	private static final boolean DEBUG_INTERMIDIATE_HASH = false;
	
	private static final String EMPTY_STRING_1_IT =
			"-12804752987762098394035772686106585063470084017442529046078187006797464553387";

	private int numIterations = 1;
	private int iteration = 0;
	
	/**
	 * 
	 */
	public SHA() {
		this(1);
	}
	
	/**
	 * 
	 */
	public SHA( int numIterations ) {
		this.numIterations = numIterations;
	}
	
	public BigInteger getHash(String o ) {

		this.iteration   = 0;
		
		String eval = o;
		BigInteger out = null;
		byte[] bites = null;
		for( ; iteration<numIterations; ) {
			iteration++;
			bites = getHashEval( eval );
			out = new BigInteger(bites);
			eval = out.toString();
			if( DEBUG_INTERMIDIATE_HASH ) {
				System.out.println( "**** ["+iteration+"] HASH ("+eval.length()+") chars = "+eval );
				System.out.println( "**** OUTPUT ["+bites.length+"] BYTES" );
				System.out.println( "===> avg="+Statistics.getAverage( eval ) );
			}
		}
		
		return out;
	}
	
	public byte[] getHashEval(String o ) {
		MessageDigest digest = null;
		try {
			digest = MessageDigest.getInstance("SHA-256");
		} catch (NoSuchAlgorithmException e) {
			e.printStackTrace();
		}
		return digest.digest(o.getBytes(StandardCharsets.UTF_8));
	}
	
	public String toString() {
		return "HashSHA "+numIterations+"x";
	}


	/**
	 * @param args
	 */
	public static void main(String[] args) {

		double total = 0d;
		double avg;
		
		int length = 100000;
		
		BigInteger eval = null;
		FuncionHash hash = null;
		for( int i=1; i<=length; i++ ) {
			hash = new SHA(i);
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
		System.out.println( "===> ["+length+"]="+(float)avg );
	}

}
