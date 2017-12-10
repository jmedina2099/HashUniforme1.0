/**
 * 
 */
package misc;

/**
 * @author jmedina
 *
 */
public class Statistics {

	public static double getAverage( String cad ) {
		int promedio = 0;
		
		cad = cad.replace( "-", "");
		
		int digit;
		for( int i=0; i<cad.length(); i++ ) {
			digit = Integer.parseInt( ""+cad.charAt(i) );
			promedio += digit;
		}
		

		return promedio/(double)cad.length();
	}
}
