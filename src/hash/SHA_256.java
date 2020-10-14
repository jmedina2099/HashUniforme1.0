/**
 * 
 */
package hash;

import static org.apache.commons.codec.digest.MessageDigestAlgorithms.SHA3_512;
import static org.apache.commons.codec.digest.MessageDigestAlgorithms.SHA_256;

import java.math.BigInteger;
import java.nio.charset.StandardCharsets;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;

import org.apache.commons.codec.binary.Hex;
import org.apache.commons.codec.digest.DigestUtils;

import misc.Statistics;

/**
 * @author jmedina
 * 
 */
public class SHA_256 implements FuncionHash {

	private static final boolean DEBUG_INTERMIDIATE_HASH = false;
	
	private static final String EMPTY_STRING_1_IT =
			"-12804752987762098394035772686106585063470084017442529046078187006797464553387";

	private int numIterations = 1;
	private int iteration = 0;

	private DigestUtils digest = null;
	
	/**
	 * 
	 */
	public SHA_256() {
		this(1);
	}
	
	/**
	 * 
	 */
	public SHA_256( int numIterations ) {
		this.numIterations = numIterations;
		init();
	}

	private void init() {
		this.digest = null;
		//try {
			//this.digest = MessageDigest.getInstance("SHA-256");
		//} catch (NoSuchAlgorithmException e) {
			//e.printStackTrace();
		//}
	}
	
	public BigInteger getHash(byte[] input ) {

		this.iteration   = 0;

		double avg = 0;
		double promedio = 0;

		String eval = null;
		for( ; iteration<numIterations; ) {
			iteration++;
			input = getHashEval( input );
			if( DEBUG_INTERMIDIATE_HASH ) {
				eval = new BigInteger(input).toString();
				avg = Statistics.getAverage( eval );
				//System.out.println( "**** ["+iteration+"] HASH ("+eval.length()+") chars = "+eval );
				//System.out.println( "**** OUTPUT ["+input.length+"] BYTES" );
				//System.out.println( "===> avg="+Statistics.getAverage( eval ) );
				promedio += avg;
			}
		}
		
		if( DEBUG_INTERMIDIATE_HASH ) {
			promedio = promedio/(double)numIterations;
			System.out.println( "===> promedio ["+numIterations+"]="+promedio );
		}

		
		return new BigInteger(input);
	}
	
	public byte[] getHashEval(byte[] bites ) {
		return new DigestUtils(SHA_256).digest( bites );
//		digest.reset();
//		return digest.digest( bites );
	}

	public String toString() {
		return "SHA-256 "+numIterations+"x";
	}


	/**
	 * @param args
	 */
	public static void main(String[] args) {

		SHA_256 hash = new SHA_256();

		byte[] cript = hash.getHashEval("".getBytes(StandardCharsets.UTF_8));
		System.out.println( "===> hashEval="+Hex.encodeHexString(cript) );
		
		BigInteger out = hash.getHash("".getBytes(StandardCharsets.UTF_8));

		String cad = out.toString();
		System.out.println( "===> hashEval="+cad );
		
		double avg = Statistics.getAverage( cad );
		System.out.println( "===> avg="+(float)avg );
		
		//itera();
	}

}
