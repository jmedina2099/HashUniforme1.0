/**
 * 
 */
package main;

import java.io.BufferedInputStream;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.UnsupportedEncodingException;
import java.nio.charset.StandardCharsets;
import java.util.Random;

import hash.Azrael320;
import hash.Azrael64;
import hash.FuncionHash;
import hash.Java;
import hash.SHA;
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
	private boolean oneBitDistinct = false;
	private boolean randomBites = true;
	private boolean withHistogram = false;
	


	private void test( FuncionHash funcionHash ) {
		String[] arrayNames = new String[]{ "/esp.txt", "/en.txt", "/words.txt", "/rockyou.txt" };
		String[] charsetNames = new String[]{ "ISO-8859-1", "UTF-8", "UTF-8", "UTF-8" };
		//int[] sizes = new int[]{ 174848,194433,466544,14344389 };
		int[] sizes = new int[]{ 200000,200000,500000,14500000 };
		int[] sizesPrimes = new int[]{ 174851,194483,466547,14344403 };
		if( useFiles) {
			processFile( funcionHash, arrayNames[0], charsetNames[0], usePrime? sizesPrimes[0]: sizes[0], withHistogram );
			processFile( funcionHash, arrayNames[1], charsetNames[1], usePrime? sizesPrimes[1]: sizes[1], withHistogram );
			processFile( funcionHash, arrayNames[2], charsetNames[2], usePrime? sizesPrimes[2]: sizes[2], withHistogram );
		}
		
		if( useRockyou ) {
			processFile( funcionHash, arrayNames[3], charsetNames[3], usePrime? sizesPrimes[3]: sizes[3], withHistogram );
		}
		
		if( oneBitDistinct ) {
			System.out.print( "=====> CLEANING UP..." );
			System.gc();
			System.out.println( " DONE ==>");
	
			int size = 8000;
			int sizePrime = 120031;
			processBites( 1000,funcionHash, usePrime? sizePrime: size, withHistogram );
		}

		if( randomBites ) {
			System.out.print( "=====> CLEANING UP..." );
			System.gc();
			System.out.println( " DONE ==>");
	
			int size = 8000;
			int sizePrime = 120031;
			processBitesRandom( 1000,funcionHash, usePrime? sizePrime: size, withHistogram );
		}

	}

	/**
	 * 
	 * @param fileName
	 * @param charset
	 */
	public void processFile( FuncionHash funcionHash, String fileName, String charset, int sizeOfTable, boolean withHistogram ) {

		long timeIni = System.currentTimeMillis();
		System.out.println( "=====> START FILE="+fileName );

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
					tablaHash.put( word.getBytes(charset),null );
					total++;
				}
			}
		} catch (IOException e) {
			e.printStackTrace();
		}

		//System.out.println( tablaHash.toSizes() );
		
		int sizeTable = tablaHash.size();
		int ocupadas = tablaHash.getCasillasOcupadas();
		double porcentaje = ocupadas*100/(double)total;
		int maxCasillas = tablaHash.getMaxCasillas();
		double promedioCasillas = tablaHash.getPromedioCasillas();
		int colisiones = tablaHash.getColisiones();
		
		System.out.println( "TABLE SIZE="+sizeTable );
		System.out.println( "CasillasOcupadas="+ocupadas+"-"+porcentaje+"%" );
		System.out.println( "MAX Casillas="+maxCasillas );
		System.out.println( "Promedio Casillas="+promedioCasillas );
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
	 */
	public static void processBites( int size, FuncionHash funcionHash, int capacityOfTable, boolean withHistogram ) {
		
		long timeIni = System.currentTimeMillis();
		System.out.println( "=====> START ONE BIT DISTINCT="+size );
		
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
		double porcentaje = ocupadas*100/(double)total;
		int maxCasillas = tablaHash.getMaxCasillas();
		double promedioCasillas = tablaHash.getPromedioCasillas();
		int colisiones = tablaHash.getColisiones();
		
		System.out.println( "TABLE SIZE="+sizeTable );
		System.out.println( "CasillasOcupadas="+ocupadas+"-"+porcentaje+"%" );
		System.out.println( "MAX Casillas="+maxCasillas );
		System.out.println( "Promedio Casillas="+promedioCasillas );
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
	public static void processBitesRandom( int size, FuncionHash funcionHash, int capacityOfTable, boolean withHistogram ) {
		
		long timeIni = System.currentTimeMillis();
		System.out.println( "=====> START RANDOM BYTES="+size );
		
		System.out.println( "===> FUNCION="+funcionHash.toString() );
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
		for( int i=0; i< size*8; i++ ) {
			//System.out.println( "BITES="+ Integer.toBinaryString( bites[index] & 0xFF ) );
			
			bites = getRandomBites( size );
			tablaHash.put( bites,null );
			total++;
			
		}

		int sizeTable = tablaHash.size();
		int ocupadas = tablaHash.getCasillasOcupadas();
		double porcentaje = ocupadas*100/(double)total;
		int maxCasillas = tablaHash.getMaxCasillas();
		double promedioCasillas = tablaHash.getPromedioCasillas();
		int colisiones = tablaHash.getColisiones();
		
		System.out.println( "TABLE SIZE="+sizeTable );
		System.out.println( "CasillasOcupadas="+ocupadas+"-"+porcentaje+"%" );
		System.out.println( "MAX Casillas="+maxCasillas );
		System.out.println( "Promedio Casillas="+promedioCasillas );
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
	 * Get an array of random bytes of longitud <size>.
	 * @param size
	 * @return
	 */
	public static byte[] getRandomBites( int size ) {
		byte[] bites = new byte[size];
		
		Random random = new Random();
		for( int i=0; i<size; i++ ) {
			bites[i] = (byte)random.nextInt( Byte.MAX_VALUE );
		}
		
		return bites;
	}
	
	/**
	 * @param args
	 */
	public static void main(String[] args) {
		
		int iterations = 1000;
		
		Main main = new Main();

		//main.test( new Java(iterations) );
		
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

		main.test( new Azrael320(iterations) );
	}

}
