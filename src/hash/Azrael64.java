/* Copyright (C) 2007-2017 - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Jorge Medina <medinarosas.jorge@gmail.com>, December 2017
 */
package hash;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.RandomAccessFile;
import java.math.BigInteger;
import java.nio.ByteBuffer;
import java.nio.charset.StandardCharsets;

import org.apache.commons.codec.binary.Hex;

/**
 * @author jmedina
 *
 */ 
public class Azrael64 implements FuncionHash {

	private static final boolean DEBUG_PARTIAL_HASH = false;
	private static final boolean DEBUG_INTERMIDIATE_HASH = false;

	//private int rounds = 0;

	private long numIterations = 2;
	private int iteration = 0;
	private RandomAccessFile fileToPersist;

	private static final String EMPTY_STRING_1_IT =
			"b4d1fb3fec8b0428";

	private static final String EMPTY_STRING_2_IT = 
			"507db4a04fa3fc27";

	/**
	 * 
	 */
	public Azrael64() {
		this(1);
	}
	
	/**
	 * 
	 */
	public Azrael64( long numIterations ) {
		this.numIterations = numIterations;
	}

	@Override
	public BigInteger getHash(byte[] input) {
		//this.rounds  = 0;
		this.iteration  = 0;

		int promedioPro = 0;
		for( ; iteration<numIterations; ) {
			iteration++;
			input = getHashEval( input );
			
			String value = Hex.encodeHexString(input);
			
			if( "ffdb3d80fed96840".equals(value) ) {
				System.out.println( iteration +"="+value );
			}
			
			/*
			if( this.fileToPersist == null ) {
				try {
					String filename = "/home/jmedina/eclipse-workspace/HashUniforme/src/azrael64-500millions-hashes.txt";
					this.fileToPersist = new RandomAccessFile( new File(filename), "rw");
				} catch (FileNotFoundException e) {
					e.printStackTrace();
				}
			}
			try {
				this.fileToPersist.write( (value+"\n").getBytes(StandardCharsets.UTF_8) );
			} catch (IOException e) {
				e.printStackTrace();
			}*/
			
			
			if( DEBUG_INTERMIDIATE_HASH ) {
				
				//String value = Hex.encodeHexString(input);
				
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
		/*
		try {
			this.fileToPersist.close();
		} catch (IOException e) {
			e.printStackTrace();
		}
		*/
		
		//System.out.println( promedioPro/100d );

		return new BigInteger( input );
	}

	public byte[] getHashEval( byte[] input ) {
		
		input = pad(input);

		long hash = 0;
		//long char1=0,char2=0,char3=0,char4=0,char5=0;

		// fractional part of sqrt(2) and sqrt(3)
		long IV1 = 0x6a09e667bb67ae85l;
		long IV2 = 0x3c6ef372a54ff53al;
		long IV3 = 0x510e527f9b05688cl;
		long IV4 = 0x1f83d9ab5be0cd19l;
		long IV5 = 0x428a2f9871374491l;
		long IV6 = 0xb5c0fbcfe9b5dba5l;
		long IV7 = 0x3956c25b59f111f1l;
		long IV8 = 0x923f82a4ab1c5ed5l;
		long IV9 = 0xd807aa9812835b01l;
		long IV10 = 0x243185be550c7dc3l;
		
		long char1=IV1;
		long char2=IV2;
		long char3=IV3;
		long char4=IV4;
		long char5=IV5;
		
		/*
		long sumaAnt1 = (long)input[ input.length-2 ];
		long sumaAnt2 = (long)input[ 2 ];
		long sumaAnt3 = 0;
		long sumaAnt4 = 0;
		long sumaAnt5 = 0;
		*/
		
		long sumaAnt1 = IV6;
		long sumaAnt2 = IV7;
		long sumaAnt3 = IV8;
		long sumaAnt4 = IV9;
		long sumaAnt5 = IV10;

		char1 += (long)input[ input.length-2 ];
		char2 += (long)input[ input.length-1 ];
		char3 += (long)input[ 0 ];
		char4 += (long)input[ 1 ];
		char5 += (long)input[ 2 ];
		sumaAnt5 += sumaAnt4;
		sumaAnt4 += sumaAnt3;
		sumaAnt3 += sumaAnt2;
		sumaAnt2 += sumaAnt1;
		sumaAnt1 += evaluaFuncBool( char1,char2,char3,char4,char5);

		// Main Loop.
		for( int i=1; i<input.length-1; i++ ) {
			char1 += sumaAnt1;
			char2 += char3;
			char3 += char4;
			char4 += (long)input[ i+1 ];
			char5 += sumaAnt2;
			sumaAnt5 += sumaAnt4;
			sumaAnt4 += sumaAnt3;
			sumaAnt3 += sumaAnt2;
			sumaAnt2 += sumaAnt1;
			sumaAnt1 += evaluaFuncBool( char1,char2,char3,char4,char5);
		}

		char1 += sumaAnt1;
		char2 += char3;
		char3 += char4;
		char4 += (long)input[ 0 ];
		char5 += sumaAnt2;
		sumaAnt5 += sumaAnt4;
		sumaAnt4 += sumaAnt3;
		sumaAnt3 += sumaAnt2;
		sumaAnt2 += sumaAnt1;
		sumaAnt1 += evaluaFuncBool( char1,char2,char3,char4,char5);
		
		if( DEBUG_PARTIAL_HASH ) {
			System.out.println( "**** END ACUMULACION 5x64: " );
			System.out.println( "**** sumAnt5 = "+sumaAnt5 );
			System.out.println( "**** sumAnt4 = "+sumaAnt4 );
			System.out.println( "**** sumAnt3 = "+sumaAnt3 );
			System.out.println( "**** sumAnt2 = "+sumaAnt2 );
			System.out.println( "**** sumAnt1 = "+sumaAnt1 );
		}

		sumaAnt1 += evaluaFuncBool( sumaAnt1,sumaAnt1,sumaAnt1,sumaAnt1,sumaAnt1) + IV1;
		sumaAnt2 += evaluaFuncBool( sumaAnt2,sumaAnt2,sumaAnt2,sumaAnt2,sumaAnt2) + IV2;
		sumaAnt3 += evaluaFuncBool( sumaAnt3,sumaAnt3,sumaAnt3,sumaAnt3,sumaAnt3) + IV3;
		sumaAnt4 += evaluaFuncBool( sumaAnt4,sumaAnt4,sumaAnt4,sumaAnt4,sumaAnt4) + IV4;
		sumaAnt5 += evaluaFuncBool( sumaAnt5,sumaAnt5,sumaAnt5,sumaAnt5,sumaAnt5) + IV5;

		if( DEBUG_PARTIAL_HASH ) {
			System.out.println( "**** END DISPERSION 5x64: " );
			System.out.println( "**** sumAnt5 = "+sumaAnt5 );
			System.out.println( "**** sumAnt4 = "+sumaAnt4 );
			System.out.println( "**** sumAnt3 = "+sumaAnt3 );
			System.out.println( "**** sumAnt2 = "+sumaAnt2 );
			System.out.println( "**** sumAnt1 = "+sumaAnt1 );
		}
		
		hash = ((sumaAnt1 << 48) & 0xffffffffffffffffL ) |
	               ((sumaAnt1+sumaAnt2 << 32) & 0xffffffffffffffffL ) |
	               ((sumaAnt1+sumaAnt2+sumaAnt3 << 16) & 0xffffffffL) |
	               ((sumaAnt3+sumaAnt4+sumaAnt5) & 0xffffffffL);

		hash += evaluaFuncBool( hash,hash,hash,hash,hash) + IV6;
		
		if( DEBUG_PARTIAL_HASH ) {
			System.out.println( "**** END APILACION 1x64:" );
			System.out.println( "**** hash = "+hash );
		}

		return longToBytes(hash+input.length+5+1);
	}
	
	public byte[] longToBytes(long x) {
	    ByteBuffer buffer = ByteBuffer.allocate(Long.BYTES);
	    buffer.putLong(x);
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
	
	public long evaluaFuncBool(Long char1, Long char2, Long char3, Long char4, Long char5) {
		//rounds++;
		return (( char1 + char2 ) ^ ( char3 ^ char4 ) ^ char5) +
			   (( char1 & char2 ) ^ ( char3 + char4 ) ^ char5) +
			   (( char1 ^ char2 ) + ( char3 + char4 ) ^ char5) +
			   (( char1 ^ char2 ) ^ ( char3 + char4 ) ^ char5) +
			   (( char1 & char2 ) + ( char3 + char4 ) + char5) +
			   (( char1 & char2 ) + ( char3 + char4 ) ^ char5) +
			   (( char1 ^ char2 ) ^ ( char3 + char4 ) ^ char5) +
			   (( char1 | char2 ) ^ ( char3 + char4 ) ^ char5) +
			   (( char1 | char2 ) | ( char3 + char4 ) ^ char5) +
			   (( char1 + char2 ) + ( char3 + char4 ) ^ char5) +
			   (( char1 + char2 ) & ( char3 + char4 ) ^ char5) +
			   (( char1 ^ char2 ) + ( char3 ^ char4 ) ^ char5) +
			   (( char1 | char2 ) ^ ( char3 ^ char4 ) ^ char5) +
			   (( char1 + char2 ) + ( char3 ^ char4 ) ^ char5) +
			   (( char1 + char2 ) ^ ( char3 + char4 ) ^ char5) +
			   (( char1 + char2 ) & ( char3 + char4 ) + char5);
	}

	public String toString() {
		return "Azrael64 "+numIterations+"x";
	}
	
	public static void main2(String[] args) {
		
		long tope = 10000000000l;
		Azrael64 hash = new Azrael64(tope);
		
		long timeIni = System.currentTimeMillis();
		System.out.println( "=====> INIT ("+tope+")-"+timeIni );

		hash.getHash( "".getBytes(StandardCharsets.UTF_8) );
		
		long timeNow = System.currentTimeMillis() - timeIni;
		System.out.println( "TIME = "+
				"["+(timeNow/(1000.0))+"] secs,"+
				"["+(timeNow/(1000.0*60.0))+"] mins." );
	}

	/**
	 * 
	 * @param args
	 */
	public static void main(String[] args) {
		
		Azrael64 hash = new Azrael64(1);
		
		byte[] hash1 = hash.getHashEval( "".getBytes(StandardCharsets.UTF_8) );
		byte[] hash2 = hash.getHashEval( hash1 );
		String hex1 = Hex.encodeHexString( hash1 );
		String hex2 = Hex.encodeHexString( hash2 );
		
		System.out.println( "1 => "+hex1 );
		System.out.println( "2 => "+hex2 );
		
		System.out.println( "EMPTY 1 => "+(hex1.equals(EMPTY_STRING_1_IT)) );
		System.out.println( "EMPTY 2 => "+(hex2.equals(EMPTY_STRING_2_IT)) );
	}
}
