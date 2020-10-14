/**
 * 
 */
package main;

import static org.apache.commons.codec.digest.MessageDigestAlgorithms.MD2;
import static org.apache.commons.codec.digest.MessageDigestAlgorithms.MD5;
import static org.apache.commons.codec.digest.MessageDigestAlgorithms.SHA_1;
import static org.apache.commons.codec.digest.MessageDigestAlgorithms.SHA_256;
import static org.apache.commons.codec.digest.MessageDigestAlgorithms.SHA_512;

import java.io.BufferedInputStream;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.UnsupportedEncodingException;
import java.nio.ByteBuffer;
import java.nio.charset.StandardCharsets;
import java.security.SecureRandom;
import java.util.Base64;
import java.util.Map;
import java.util.Random;
import java.util.TreeMap;

import org.apache.commons.codec.binary.Hex;
import org.apache.commons.codec.digest.DigestUtils;

import hash.Azrael320;
import hash.Azrael512;
import hash.Azrael64;
import hash.FuncionHash;
import hash.MD5;
import hash.SHA3;
import hash.SHA_256;
import hash.SipHashImpl;
import table.TablaHash;
import ui.PanelPrincipal;
import ui.VentanaPrincipal;

/**
 * @author jmedina
 *
 */
public class Main {
	
	private boolean usePrime = false;
	private boolean useFiles = false;
	private boolean useRockyou = false;
	private boolean useClean = false;
	private boolean oneBitDistinct = false;
	private boolean randomBites = true;
	private boolean withHistogram = true;
	
	static TreeMap<String,String> tree = new TreeMap<String,String>();


	private void test( FuncionHash funcionHash ) {
		String[] arrayNames = new String[]{ "/esp.txt", "/en.txt", "/words.txt", "/rockyou.txt", "/clean.txt" };
		String[] charsetNames = new String[]{ "ISO-8859-1", "UTF-8", "UTF-8", "UTF-8", "UTF-8"  };
		//int[] sizes = new int[]{ 174848,194433,466544,14344389 };
		//int[] sizes = new int[]{ 174848,194433,466544,14344389 };
		int[] sizes = new int[]{ 174848,194433,466544,20000000,1370684 };
		int[] sizesPrimes = new int[]{ 174851,194483,466547,14344403,1370687 };
		if( useFiles) {
			processFile( funcionHash, arrayNames[0], charsetNames[0], usePrime? sizesPrimes[0]: sizes[0], withHistogram );
			processFile( funcionHash, arrayNames[1], charsetNames[1], usePrime? sizesPrimes[1]: sizes[1], withHistogram );
			processFile( funcionHash, arrayNames[2], charsetNames[2], usePrime? sizesPrimes[2]: sizes[2], withHistogram );
		}
		
		if( useRockyou ) {
			processFile( funcionHash, arrayNames[3], charsetNames[3], usePrime? sizesPrimes[3]: sizes[3], withHistogram );
		}
		
		if( useClean ) {
			processFile( funcionHash, arrayNames[4], charsetNames[4], usePrime? sizesPrimes[4]: sizes[4], withHistogram );
		}
		
		if( oneBitDistinct ) {
			System.out.print( "=====> CLEANING UP..." );
			System.gc();
			System.out.println( " DONE ==>");
	
			int size = 120000;
			int sizePrime = 120031;
			processBites( 15000,funcionHash, usePrime? sizePrime: size, withHistogram );
		}

		if( randomBites ) {
			System.out.print( "=====> CLEANING UP..." );
			System.gc();
			System.out.println( " DONE ==>");
	
			int sizePrime = 120031;
			int size = 120000;
			
			int sizeBytes = 15000;
			processBitesRandom( sizeBytes,sizeBytes*8,funcionHash, usePrime? sizePrime: size, withHistogram );
		}

	}

	/**
	 * 
	 * @param fileName
	 * @param charset
	 */
	public void processFile( FuncionHash funcionHash, String fileName, String charset, int sizeOfTable, boolean withHistogram ) {

		long timeIni = System.currentTimeMillis();
		System.out.println( "=====> START FILE="+fileName+"-"+sizeOfTable );

		int total = 0;
		
		//int capacityOfTable = 15000000;
		System.out.println( "===> FUNCION="+funcionHash.toString() );
		TablaHash tablaHash = new TablaHash( funcionHash, sizeOfTable );
		
		//System.out.println( "tensada".hashCode() == "friabili".hashCode() );
		
		InputStream istream = TablaHash.class.getResourceAsStream( fileName );  
		BufferedInputStream bis = new BufferedInputStream(istream);
		BufferedReader reader = null;
		try {
			reader = new BufferedReader(new InputStreamReader(bis,charset) );
		} catch (UnsupportedEncodingException e) {
			e.printStackTrace();
		}
		
		String word = null;
		try {
			while( (word=reader.readLine() )!=null ) {
				if( word.length() > 0 ) {
					//System.out.println( "TOT="+total );
					tablaHash.put( word.getBytes(StandardCharsets.UTF_8),null );
					total++;
				}
			}
		} catch (IOException e) {
			e.printStackTrace();
		}

		//System.out.println( tablaHash.toSizes() );
		
		int sizeTable = tablaHash.size();
		int ocupadas = tablaHash.getCasillasOcupadas();
		int vacias = sizeTable - ocupadas;
		double porcentaje = vacias<=0? 100: ocupadas*100/(double)sizeTable;
		double porcentajeVacias = vacias<=0? 0: vacias*100/(double)sizeTable;
		int maxCasillas = tablaHash.getMaxCasillas();
		double promedioCasillas = tablaHash.getPromedioCasillas();
		int colisiones = tablaHash.getColisiones();
		
		tree.put( vacias+"-"+funcionHash.toString(), funcionHash.toString() );

		
		System.out.println( "TOTAL ELEMENTS="+total );
		System.out.println( "TABLE SIZE="+sizeTable );
		System.out.println( "BUSY/EMPTY="+ocupadas+"/"+vacias+"-"+porcentaje+"%"+"-"+porcentajeVacias+"%" );
		System.out.println( "MAX LINK LIST="+maxCasillas );
		System.out.println( "AVERAGE LINK LIST="+promedioCasillas );
		System.out.println( "COLISIONS=["+colisiones+"]" );
 
		long timeNow = System.currentTimeMillis() - timeIni;
		System.out.println( "TIME = "+
				"["+(timeNow/(1000.0))+"] secs,"+
				"["+(timeNow/(1000.0*60.0))+"] mins." );
		
		if( withHistogram ) {
			VentanaPrincipal window = new VentanaPrincipal();
			PanelPrincipal panel = window.getPanelPrincipal();
			panel.setCasillas( tablaHash.getCasillasToArray(), tablaHash.getMaxCasillas());
			window.setVisible( true );			
		}
	}

	/**
	 * 
	 */
	public static void processBites( int size, FuncionHash funcionHash, int capacityOfTable, boolean withHistogram ) {
		
		long timeIni = System.currentTimeMillis();
		System.out.println( "=====> START TWO BIT DISTINCT="+size );
		
		System.out.println( "===> FUNCION="+funcionHash.toString() );
		TablaHash tablaHash = new TablaHash( funcionHash, capacityOfTable );

		/*
		byte[] bitesA =  getRandomBitesFromFile( "/colisiones-java-1.txt", 8000 );
		byte[] bitesB =  getRandomBitesFromFile( "/colisiones-java-2.txt", 8000 );
		tablaHash.add( new String( bitesA) );
		tablaHash.add( new String( bitesB) );
		*/
		
		byte[] bites = getRandomBites( size ); //getRandomBitesFromFile( "/bitesRandom3.txt",8000 ); //  
		System.out.println( "STARTING..." );
		//System.out.println( "<"+Arrays.toString(bites)+">-["+bites.length+"]" );

		byte biteAnt;
		int total = 0;
		for( int index=0; index<bites.length; index++ ) {
			//System.out.println( "BITES="+ Integer.toBinaryString( bites[index] & 0xFF ) );
			for( int offset=7; offset>=0; offset-- ) {
				biteAnt = bites[index];
				
				bites[index] ^= (1 << offset); // Toogle bit.
				//System.out.println( Integer.toBinaryString( bites[index] & 0xFF ) );

				tablaHash.put( bites.clone(),null ); // Add next string, 2 bits different to all entries.
				bites[index] = biteAnt;
				total++;
			}
		}

		int sizeTable = tablaHash.size();
		int ocupadas = tablaHash.getCasillasOcupadas();
		int vacias = sizeTable - ocupadas;
		double porcentaje = vacias<=0? 100: ocupadas*100/(double)sizeTable;
		double porcentajeVacias = vacias<=0? 0: vacias*100/(double)sizeTable;
		int maxCasillas = tablaHash.getMaxCasillas();
		double promedioCasillas = tablaHash.getPromedioCasillas();
		int colisiones = tablaHash.getColisiones();
		
		System.out.println( "TABLE SIZE="+sizeTable );
		System.out.println( "Ocupadas/Vacias="+ocupadas+"/"+vacias+"-"+porcentaje+"%"+"-"+porcentajeVacias+"%" );
		System.out.println( "MAX Chaining="+maxCasillas );
		System.out.println( "Average Casillas="+promedioCasillas );
		System.out.println( "Colisiones=["+colisiones+"]" );
		System.out.println( "TOTAL="+total );

		long timeNow = System.currentTimeMillis() - timeIni;
		System.out.println( "TIME = "+
				"["+(timeNow/(1000.0))+"] secs,"+
				"["+(timeNow/(1000.0*60.0))+"] mins." );
		
		if( withHistogram ) {
			VentanaPrincipal window = new VentanaPrincipal();
			PanelPrincipal panel = window.getPanelPrincipal();
			panel.setCasillas( tablaHash.getCasillasToArray(), tablaHash.getMaxCasillas());
			window.setVisible( true );
		}
	}	

	/**
	 * 
	 * @param funcionHash
	 * @param capacityOfTable
	 * @param withHistogram
	 */
	public static void processBitesRandom( int size, int numCads, FuncionHash funcionHash, int capacityOfTable, boolean withHistogram ) {
		
		long timeIni = System.currentTimeMillis();
		System.out.println( "=====> START RANDOM BYTES="+size );
		
		System.out.println( "===> FUNCTION="+funcionHash.toString() );
		TablaHash tablaHash = new TablaHash( funcionHash, capacityOfTable );

		/*
		byte[] bitesA =  getRandomBitesFromFile( "/colisiones-java-1.txt", 8000 );
		byte[] bitesB =  getRandomBitesFromFile( "/colisiones-java-2.txt", 8000 );
		tablaHash.add( new String( bitesA) );
		tablaHash.add( new String( bitesB) );
		*/
		
		byte[] bites;  
		System.out.println( "STARTING..." );
		//System.out.println( "<"+Arrays.toString(bites)+">-["+bites.length+"]" );

		int total = 0;
		for( int i=0; i< numCads; i++ ) {
			//System.out.println( "BITES="+ Integer.toBinaryString( bites[index] & 0xFF ) );
			
			bites = getRandomBites( size );
			tablaHash.put( bites,null );
			total++;
			
		}

		int sizeTable = tablaHash.size();
		int ocupadas = tablaHash.getCasillasOcupadas();
		int vacias = sizeTable - ocupadas;
		double porcentaje = vacias<=0? 100: ocupadas*100/(double)sizeTable;
		double porcentajeVacias = vacias<=0? 0: vacias*100/(double)sizeTable;
		int maxCasillas = tablaHash.getMaxCasillas();
		double promedioCasillas = tablaHash.getPromedioCasillas();
		int colisiones = tablaHash.getColisiones();
		
		tree.put( vacias+"-"+funcionHash.toString(), funcionHash.toString() );

		
		System.out.println( "TOTAL ELEMENTS="+total );
		System.out.println( "TABLE SIZE="+sizeTable );
		System.out.println( "BUSY/EMPTY="+ocupadas+"/"+vacias+"-"+porcentaje+"%"+"-"+porcentajeVacias+"%" );
		System.out.println( "MAX LINK LIST="+maxCasillas );
		System.out.println( "AVERAGE LINK LIST="+promedioCasillas );
		System.out.println( "COLISIONS=["+colisiones+"]" );

		long timeNow = System.currentTimeMillis() - timeIni;
		System.out.println( "TIME = "+
				"["+(timeNow/(1000.0))+"] secs,"+
				"["+(timeNow/(1000.0*60.0))+"] mins." );
		
		if( withHistogram ) {
			VentanaPrincipal window = new VentanaPrincipal();
			PanelPrincipal panel = window.getPanelPrincipal();
			panel.setCasillas( tablaHash.getCasillasToArray(), tablaHash.getMaxCasillas());
			window.setVisible( true );
		}
	}	

	/**
	 * Get an array of random bytes of longitud <size>.
	 * @param size
	 * @return
	 */
	public static byte[] getRandomBites( int size ) {
		byte[] bites = new byte[size];
		
		Random random = new Random();
		int tope = Byte.MAX_VALUE - Byte.MIN_VALUE;
		int rdm;
		for( int i=0; i<size; i++ ) {
			rdm = random.nextInt( tope+1 );
			if( rdm <= Byte.MAX_VALUE ) {
				bites[i] = (byte)rdm;
			} else {
				bites[i] = (byte)( -(rdm - Byte.MAX_VALUE) );
			}
		}
		
		return bites;
	}
	
	/**
	 * @param args
	 */
	public static void main(String[] args) {
		
		byte[] key = new byte[16];
		new SecureRandom().nextBytes(key);
        
		main2(args,key);
		
		int iterations = 1;
		
		Main main = new Main();
		
		/*
		System.out.print( "=====> CLEANING UP..." );
		System.gc();
		System.out.println( " DONE ==>");

		System.out.print( "=====> CLEANING UP..." );
		System.gc();
		System.out.println( " DONE ==>");

		main.test( new SHA(iterations) );

		System.out.print( "=====> CLEANING UP..." );
		System.gc();
		System.out.println( " DONE ==>");

		main.test( new Azrael64(iterations) );

		System.out.print( "=====> CLEANING UP..." );
		System.gc();
		System.out.println( " DONE ==>");
		*/
		
		main.test( new SipHashImpl(iterations,key) );
		main.test( new Azrael64(iterations) );
		main.test( new MD5(iterations) );
		main.test( new SHA_256(iterations) );
		main.test( new Azrael320(iterations) );
		main.test( new SHA3(iterations) );
		main.test( new Azrael512(iterations) );
		
		for (Map.Entry<String, String> entry : tree.entrySet()) { 
			System.out.println( "[" + entry.getKey() + ", " + entry.getValue() + "]");
		}
		
	}

	public static void gcCall() {
		System.out.print( "=====> CLEANING UP..." );
		System.gc();
		System.out.println( " DONE ==>");
	}
	
	/**
	 * @param args
	 */
	public static void main2(String[] args , byte[] key ) {
		// TODO Auto-generated method stub
		
		String stringValue = "";
		byte[] bites = stringValue.getBytes(StandardCharsets.UTF_8);
		
		//String hashedVal = Base64.getEncoder().encodeToString(DigestUtils.sha1(bites));
		//System.out.println( hashedVal );
		
        byte[] hash000 = new SipHashImpl(1,key).getHashEval( bites );
		byte[] hash00 = new Azrael64(1).getHashEval( bites );
		byte[] hash0 = new DigestUtils(MD2).digest(bites);
		byte[] hash1 = new DigestUtils(MD5).digest(bites);
		byte[] hash2 = new DigestUtils(SHA_1).digest(bites);
		byte[] hash3 = new DigestUtils(SHA_256).digest(bites);
		byte[] hash4 = new Azrael320(1).getHashEval( bites );
		byte[] hash5 = new DigestUtils(SHA_512).digest(bites);
		byte[] hash6 = new SHA3(1).getHashEval( bites );
		byte[] hash7 = new Azrael512(1).getHashEval( bites );

		System.out.println( "Starting.. hashing the empty string" );

		System.out.println( "siphash    (Base64)= "+ Base64.getEncoder().encodeToString(hash000) );
		System.out.println( "siphash-key(Base64)= "+ Base64.getEncoder().encodeToString(key) );
		System.out.println( "azrael64   (Base64)= "+ Base64.getEncoder().encodeToString(hash00) );
		System.out.println( "md2        (Base64)= "+ Base64.getEncoder().encodeToString(hash0) );
		System.out.println( "md5        (Base64)= "+ Base64.getEncoder().encodeToString(hash1) );
		System.out.println( "sha160     (Base64)= "+ Base64.getEncoder().encodeToString(hash2) );
		System.out.println( "sha256     (Base64)= "+ Base64.getEncoder().encodeToString(hash3) );
		System.out.println( "azrael320  (Base64)= "+ Base64.getEncoder().encodeToString(hash4) );
		System.out.println( "sha512     (Base64)= "+ Base64.getEncoder().encodeToString(hash5) );
		System.out.println( "sha3_512   (Base64)= "+ Base64.getEncoder().encodeToString(hash6) );
		System.out.println( "azrael512  (Base64)= "+ Base64.getEncoder().encodeToString(hash7) );
		
		System.out.println( "siphash    (Hex   )= "+ Hex.encodeHexString( hash000 ) );
		System.out.println( "siphash-key(Hex   )= "+ Hex.encodeHexString(key) );
		System.out.println( "azrael64   (Hex   )= "+ Hex.encodeHexString( hash00 ) );
		System.out.println( "md2        (Hex   )= "+ Hex.encodeHexString( hash0 ) );
		System.out.println( "md5        (Hex   )= "+ Hex.encodeHexString( hash1 ) );
		System.out.println( "sha160     (Hex   )= "+ Hex.encodeHexString( hash2 ) );
		System.out.println( "sha256     (Hex   )= "+ Hex.encodeHexString( hash3 ) );
		System.out.println( "azrael320  (Hex   )= "+ Hex.encodeHexString( hash4 ) );
		System.out.println( "sha512     (Hex   )= "+ Hex.encodeHexString( hash5 ) );
		System.out.println( "sha3_512   (Hex   )= "+ Hex.encodeHexString( hash6 ) );
		System.out.println( "azrael512  (Hex   )= "+ Hex.encodeHexString( hash7 ) );

		//String hdigest = new DigestUtils(SHA_1).digestAsHex(new File("pom.xml"));

		System.out.println( "END" );

	}
}
