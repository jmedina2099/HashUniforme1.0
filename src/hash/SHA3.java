/**
 * 
 */
package hash;

import static org.apache.commons.codec.digest.MessageDigestAlgorithms.SHA3_512;

import java.io.RandomAccessFile;
import java.math.BigInteger;
import java.nio.ByteBuffer;
import java.nio.charset.StandardCharsets;
import java.util.Random;

import org.apache.commons.codec.binary.Hex;
import org.apache.commons.codec.digest.DigestUtils;

import misc.Statistics;

/**
 * @author jmedina
 * 
 */
public class SHA3 implements FuncionHash {

	private static final boolean DEBUG_INTERMIDIATE_HASH = false;
	
	private int numIterations = 1;
	private int iteration = 0;

	private RandomAccessFile fileToPersist;
	
	/**
	 * 
	 */
	public SHA3() {
		this(1);
	}
	
	/**
	 * 
	 */
	public SHA3( int numIterations ) {
		this.numIterations = numIterations;
		init();
	}
	
	public int nextInt(Random rng, int n)
    {
        return rng.nextInt(n);
    }

	private void init() {
	}
	
	@Override
	public BigInteger getHash(byte[] input) {
		this.iteration  = 0;

		int promedioPro = 0;
		for( ; iteration<numIterations; ) {
			iteration++;
			input = getHashEval( input );
			
			/*
			String value = Hex.encodeHexString(input);
			
			if( this.fileToPersist == null ) {
				try {
					String filename = "/home/jmedina/eclipse-workspace/HashUniforme/src/siphash-500millions-hashes.txt";
					this.fileToPersist = new RandomAccessFile( new File(filename), "rw");
				} catch (FileNotFoundException e) {
					e.printStackTrace();
				}
			}
			try {
				this.fileToPersist.write( (value+"\n").getBytes(StandardCharsets.UTF_8) );
			} catch (IOException e) {
				e.printStackTrace();
			}
			*/
			
			if( DEBUG_INTERMIDIATE_HASH ) {
				
				String value = Hex.encodeHexString(input);
				
				int min=256, max=-1;
				
				int promedioHex = 0;
				int byteNum = 0;
				for( int i=0; i<input.length; i++ ) {
					byteNum = input[i]<0? 127-input[i]: input[i];
					promedioHex += byteNum;
					
					if( byteNum < min ) {
						min = byteNum;
					}
					if( max < byteNum ) {
						max = byteNum;
					}
				}
				double promedio = promedioHex/(double)input.length;
				promedio = (promedio*100)/255;
				promedio = ((int)(promedio*1000000))/1000000d;
				
				StringBuffer sb = new StringBuffer();
				//sb.append( promedio );
				
				if( iteration > this.numIterations - 100 ) {
					promedioPro += promedio;
					
					sb.append( "[" )
					  .append( value )
					  .append( "]-[bits]=[" )
					  .append( input.length*8 )
					  .append( "]-[avg]=[" )
					  .append( promedio )
					  .append( "]-[min,max]=[" )
					  .append( min )
					  .append( "," )
					  .append( max )
					  .append( "]-[iter]=[" )
					  .append( iteration )
					  .append( "]" );					
					 
					//System.out.println( iteration +"---"+ sb.toString() );
					System.out.println( sb.toString() );
				}
			}
		}
		
		//System.out.println( promedioPro/100d );

		return new BigInteger( input );
	}

	public byte[] getHashEval(byte[] bites ) {
		return new DigestUtils(SHA3_512).digest( bites );
	}
	
	public byte[] longToBytes(long x) {
	    ByteBuffer buffer = ByteBuffer.allocate(Long.BYTES);
	    buffer.putLong(x);
	    return buffer.array();
	}

	private static void itera() {
		double total = 0d;
		
		int length = 1000000;
		double avg = 0;
		
		SHA3 hash = null;
		BigInteger eval = null;
		for( int i=1; i<=length; i++ ) {
			hash = new SHA3(i);
			eval = hash.getHash( "".getBytes(StandardCharsets.UTF_8) );
	
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
		return "SHA3 "+numIterations+"x";
	}
	
	public static void main2(String[] args) {
		
		int tope = 500000000;
		SHA3 hash = new SHA3(tope);
		
		long timeIni = System.currentTimeMillis();
		System.out.println( "=====> INIT ("+tope+")-"+timeIni );

		hash.getHash( "".getBytes(StandardCharsets.UTF_8) );
		
		long timeNow = System.currentTimeMillis() - timeIni;
		System.out.println( "TIME = "+
				"["+(timeNow/(1000.0))+"] secs,"+
				"["+(timeNow/(1000.0*60.0))+"] mins." );
	}


	/**
	 * @param args
	 */
	public static void main(String[] args) {

		SHA3 hash = new SHA3(1);

		byte[] cript = hash.getHashEval("".getBytes(StandardCharsets.UTF_8));
		System.out.println( Hex.encodeHexString(cript) );
		
	}

}
