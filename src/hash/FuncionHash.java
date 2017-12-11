/**
 * 
 */
package hash;

import java.math.BigInteger;

/**
 * @author jmedina
 *
 */
public interface FuncionHash {

	public BigInteger getHash( byte[] o);
	public String toString();
	
}
