/**
 * 
 */
package hash;

import java.math.BigInteger;
import java.nio.charset.Charset;
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

	private MessageDigest digest = null;
	
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
		init();
	}

	private void init() {
		this.digest = null;
		try {
			this.digest = MessageDigest.getInstance("SHA-256");
		} catch (NoSuchAlgorithmException e) {
			e.printStackTrace();
		}
	}
	
	public BigInteger getHash(String o ) {

		this.iteration   = 0;

		double avg = 0;
		double promedio = 0;

		String eval = o;
		byte[] bites = o.getBytes(StandardCharsets.UTF_8);
		for( ; iteration<numIterations; ) {
			iteration++;
			bites = getHashEval( bites );
			if( DEBUG_INTERMIDIATE_HASH ) {
				eval = new BigInteger(bites).toString();
				avg = Statistics.getAverage( eval );
				//System.out.println( "**** ["+iteration+"] HASH ("+eval.length()+") chars = "+eval );
				//System.out.println( "**** OUTPUT ["+bites.length+"] BYTES" );
				//System.out.println( "===> avg="+Statistics.getAverage( eval ) );
				promedio += avg;
			}
		}
		
		if( DEBUG_INTERMIDIATE_HASH ) {
			promedio = promedio/(double)numIterations;
			System.out.println( "===> promedio ["+numIterations+"]="+promedio );
		}

		
		return new BigInteger(bites);
	}
	
	public byte[] getHashEval(byte[] bites ) {
		digest.reset();
		return digest.digest( bites );
	}

	private static void itera() {
		double total = 0d;
		
		int length = 1000000;
		double avg = 0;
		
		SHA hash = null;
		BigInteger eval = null;
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
		System.out.println( "===> avg="+(float)avg );
	}
	
	public String toString() {
		return "HashSHA "+numIterations+"x";
	}


	/**
	 * @param args
	 */
	public static void main(String[] args) {

		SHA hash = new SHA();

		BigInteger out = hash.getHash("");

		String cad = out.toString();
		System.out.println( "===> hashEval="+cad );
		
		double avg = Statistics.getAverage( cad );
		System.out.println( "===> avg="+(float)avg );
		
		//itera();
	}

}
