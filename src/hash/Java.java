/**
 * 
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
public class Java implements FuncionHash {
	
	private static final boolean DEBUG_INTERMIDIATE_HASH = false;

	private int numIterations = 1;
	private int iteration = 0;

	/**
	 * 
	 */
	public Java() {
		this(1);
	}
	
	/**
	 * 
	 */
	public Java( int numIterations ) {
		this.numIterations = numIterations;
	}
	
	@Override
	public BigInteger getHash(byte[] input) {
		this.iteration  = 0;
		
		double avg = 0;
		double promedio = 0;
		
		String eval = null;
		for( ; iteration<numIterations; ) {
			iteration++;
			input = getHashEval( input );
			if( DEBUG_INTERMIDIATE_HASH ) {
				eval = new BigInteger( input ).toString();
				avg = Statistics.getAverage( eval );
				System.out.println( "**** ["+iteration+"] HASH ("+eval.length()+") chars = "+eval );
				System.out.println( "**** OUTPUT ["+input.length+"] BYTES" );
				System.out.println( "===> avg="+Statistics.getAverage( eval ) );
				promedio += avg;
			}
		}
		
		if( DEBUG_INTERMIDIATE_HASH ) {
			promedio = promedio/(double)numIterations;
			System.out.println( "===> promedio ["+numIterations+"]="+promedio );
		}
		
		return new BigInteger( input );
	}
	
	public byte[] getHashEval( byte[] bites ) {
		//bites = pad(bites);
		return intToBytes( new String(bites,StandardCharsets.UTF_8).hashCode() );
	}
	
	public byte[] intToBytes(int x) {
	    ByteBuffer buffer = ByteBuffer.allocate(Integer.BYTES);
	    buffer.putInt(x);
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
	public String toString() {
		return "Java "+numIterations+"x";
	}
	
	/**
	 * @param args
	 */
	public static void main(String[] args) {
		Java hash = new Java();

		BigInteger out = hash.getHash("".getBytes(StandardCharsets.UTF_8));

		String cad = out.toString();
		System.out.println( "===> hashEval="+cad );
		
		double avg = Statistics.getAverage( cad );
		System.out.println( "===> avg="+(float)avg );
		
		//itera();
	}

}
