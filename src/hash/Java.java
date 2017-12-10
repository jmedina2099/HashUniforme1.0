/**
 * 
 */
package hash;

import java.math.BigInteger;

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
	public BigInteger getHash(String o) {
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
	
	public int getHashEval(String o) {
		return o.hashCode();
	}
	
	public String toString() {
		return "HashJava "+numIterations+"x";
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
		for( int i=length; i<=length; i++ ) {
			hash = new Java(4);
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
		Statistics.getAverage( eval.toString() );
		
	}

}
