/**
 * 
 */
package hash;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.RandomAccessFile;
import java.math.BigInteger;
import java.nio.ByteBuffer;
import java.nio.charset.StandardCharsets;
import java.security.SecureRandom;
import java.util.HashSet;
import java.util.Random;

import org.apache.commons.codec.binary.Hex;
import org.bouncycastle.crypto.macs.SipHash;
import org.bouncycastle.crypto.params.KeyParameter;

import misc.Statistics;

/**
 * @author jmedina
 * 
 */
public class SipHashImpl implements FuncionHash {

	private static final boolean DEBUG_INTERMIDIATE_HASH = false;
	
	private int numIterations = 1;
	private int iteration = 0;

	private SipHash mac;

	private RandomAccessFile fileToPersist;
	
	/**
	 * 
	 */
	public SipHashImpl( byte[] key ) {
		this(1,key);
	}
	
	/**
	 * 
	 */
	public SipHashImpl( int numIterations, byte[] key ) {
		this.numIterations = numIterations;
		init(key);
	}
	
	public int nextInt(Random rng, int n)
    {
        return rng.nextInt(n);
    }

	private void init( byte[] key ) {
        mac = new SipHash();
        mac.init(new KeyParameter(key));

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
        mac.update(bites, 0, bites.length);
        return longToBytes( mac.doFinal() );
	}
	
	public byte[] longToBytes(long x) {
	    ByteBuffer buffer = ByteBuffer.allocate(Long.BYTES);
	    buffer.putLong(x);
	    return buffer.array();
	}

	public String toString() {
		return "SipHash "+numIterations+"x";
	}
	
	public static void main(String[] args) {
		
		byte[] key = new byte[16];
		new SecureRandom().nextBytes(key);
		
		int tope = 500000000;
		SipHashImpl hash = new SipHashImpl(tope,key);
		
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
	public static void main2(String[] args) {
		
		byte[] key = new byte[16];
		new SecureRandom().nextBytes(key);

		SipHashImpl hash = new SipHashImpl(1,key);

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
