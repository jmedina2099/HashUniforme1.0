package hash;

import java.math.BigInteger;
import java.util.ArrayList;
import java.util.Iterator;

/**
 * @author jmedina
 *
 */
public class TablaHash {

	private static final int _initialCapacity = 10;

	private Object[] _lista;
	private FuncionHash _funcionHash;
	private int _numberOfElements = 0;
	private int casillasOcupadas = 0;
	private int colisiones = 0;
	
	private boolean doRehash = true;

	private boolean debugColisiones = false;

	/**
	 * 
	 */
	public TablaHash( FuncionHash funcionHash ) {
		this( funcionHash, _initialCapacity );
	}

	/**
	 * 
	 */
	public TablaHash( FuncionHash funcionHash, int capacity ) {
		this._lista = new Object[capacity];
		this._funcionHash = funcionHash;
	}

	/**
	 * 
	 * @return
	 */
	public int size() {
		return this._lista.length;
	}
	
	/**
	 * @return the casillasOcupadas
	 */
	public int getCasillasOcupadas() {
		return casillasOcupadas;
	}

	/**
	 * @return the colisiones
	 */
	public int getColisiones() {
		return colisiones;
	}

	/**
	 * 
	 * @return
	 */
	public int getMaxCasillas() {
		int max = 0;
		
		for( int i=0; i<this._lista.length; i++ ) {
			if( this._lista[i] == null ) {
				continue;
			}

			@SuppressWarnings("unchecked")
			ArrayList<Object[]> casillas = (ArrayList<Object[]>)this._lista[i];
			int size = casillas.size();
			if( max < size ) {
				max = size;
			}
		}
		
		return max;
	}
	
	/**
	 * 
	 * @return
	 */
	public double getPromedioCasillas() {
		int suma = 0;
		
		for( int i=0; i<this._lista.length; i++ ) {
			if( this._lista[i] == null ) {
				continue;
			}

			@SuppressWarnings("unchecked")
			ArrayList<Object[]> casillas = (ArrayList<Object[]>)this._lista[i];
			int size = casillas.size();
			suma += size;
		}
		
		return suma/(double)casillasOcupadas;
	}

	/**
	 * 
	 * @return
	 */
	@SuppressWarnings("unchecked")
	public int[] getCasillasToArray() {
		int[] casillas = new int[this._lista.length];
		
		for( int i=0; i<this._lista.length; i++ ) {
			if( this._lista[i] == null ) {
				casillas[i] = 0;
			} else {
				casillas[i] = ((ArrayList<Object[]>)this._lista[i]).size();
			}
		}
		
		return casillas;
	}

	/**
	 * 
	 */
	public Object get( String key ) {
		if( key == null ) {
			return null;
		}

		BigInteger hashValue = this._funcionHash.getHash( key );
		int hash = hashValue.mod( new BigInteger( ""+this._lista.length) ).intValue();
		if( this._lista[hash] == null ) {
			return null;
		}
		@SuppressWarnings("unchecked")		
		ArrayList<Object[]> casillas = (ArrayList<Object[]>)(this._lista[hash]);

		Object[] celdas;
		Iterator<Object[]> iterator = casillas.iterator();
		while( iterator.hasNext() ) {
			celdas = (Object[])iterator.next();
			if( key.equals(celdas[0]) ) {
				return celdas[1];
			}
		}		
		
		return null;
	}

	/**
	 * 
	 */
	public Object put( String key, Object value ) {
		if( key == null ) {
			return null;
		}
		
		if( this._numberOfElements == this._lista.length ) {
			if( doRehash )
				rehash();
		}

		BigInteger hashValue = this._funcionHash.getHash( key );
		int hash = hashValue.mod( new BigInteger( ""+this._lista.length) ).intValue();
		
		if( this._lista[hash] == null ) {
			this._lista[hash] = new ArrayList<Object[]>();
			this.casillasOcupadas++;
		}
		
		@SuppressWarnings("unchecked")
		ArrayList<Object[]> casillas = (ArrayList<Object[]>)(this._lista[hash]);

		Object valueAnt;
		Object[] celdas;
		Iterator<Object[]> iterator = casillas.iterator();
		while( iterator.hasNext() ) {
			celdas = (Object[])iterator.next();
			if( hashValue.equals( celdas[2] ) ) {
				colisiones++;
				if( debugColisiones  ) {
					System.out.println( colisiones+") HASH COLISION!!! ("+key+","+celdas[0]+")-["+hashValue+"]"+(this._numberOfElements+1)+","+celdas[3] );
				}
			}
			if( key.equals(celdas[0]) ) {
				celdas[0] = key;
				valueAnt = celdas[1];
				celdas[1] = value;
				this._numberOfElements++;
				return valueAnt;
			}
		}
		
		casillas.add( new Object[]{ key,value,hashValue,(this._numberOfElements+1) } );
		this._numberOfElements++;
		
		return null;
	}

	/**
	 * 
	 */
	private void rehash() {
		System.out.println( "==> REHASH()" );
		
		Object[] listaAnt = this._lista;

		this._lista = new Object[this._lista.length*2];
		this._numberOfElements = 0;
		this.casillasOcupadas = 0;

		for( int i=0; i<listaAnt.length; i++ ) {
			if( listaAnt[i] == null ) {
				continue;
			}

			@SuppressWarnings("unchecked")
			ArrayList<Object[]> casillas = (ArrayList<Object[]>)listaAnt[i];

			Object[] celdas;
			Iterator<Object[]> iterator = casillas.iterator();
			while( iterator.hasNext() ) {
				celdas = (Object[])iterator.next();
				put( (String)celdas[0], celdas[1] );
			}
		}
	}

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		HashIterativeBoolean funcionHash = new HashIterativeBoolean();
		TablaHash tablaHash = new TablaHash(funcionHash);
		tablaHash.put("0", "cero");
		tablaHash.put("1", "uno");
		tablaHash.put("2", "dos");
		
		System.out.println( tablaHash.get("0") );
		System.out.println( tablaHash.get("1") );
		System.out.println( tablaHash.get("2") );
	}

}
