package ui;

import java.awt.Dimension;

import javax.swing.JFrame;
import javax.swing.JScrollPane;

import hash.Azrael64;
import hash.FuncionHash;
import hash.SHA;
import hash.Java;
import main.Main;

/**
 * @author jmedina
 *
 */
public class VentanaPrincipal extends JFrame {

	/**
	 * 
	 */
	private static final long serialVersionUID = -3572735786473097402L;
	private PanelPrincipal _panelPrincipal;
	private JScrollPane _scrollPane;

	/**
	 * 
	 */
	public VentanaPrincipal() {
		super( "TablaHash v1.0" );
		init();
	}
	
	public PanelPrincipal getPanelPrincipal() {
		return this._panelPrincipal;
	}

	/**
	 *
	 */
	private void init() {
		setDefaultCloseOperation( VentanaPrincipal.DISPOSE_ON_CLOSE );
		
		this._panelPrincipal = new PanelPrincipal();
		this._scrollPane = new JScrollPane( this._panelPrincipal,
				JScrollPane.VERTICAL_SCROLLBAR_ALWAYS,
				JScrollPane.HORIZONTAL_SCROLLBAR_ALWAYS );
		this._scrollPane.setPreferredSize( new Dimension(800,600) );
		
		setContentPane( this._scrollPane );
		pack();
		
		setLocationRelativeTo( null );
	}

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		
		boolean withHistogram = true;
		
		Main main = new Main();
		
		FuncionHash funcionHash = new Java();
		main.processFile( funcionHash, "/esp.txt", "ISO-8859-1", 174848, withHistogram);

		funcionHash = new SHA();
		main.processFile( funcionHash, "/esp.txt", "ISO-8859-1", 174848, withHistogram); //174848

		funcionHash = new Azrael64(false);
		main.processFile( funcionHash, "/esp.txt", "ISO-8859-1", 174848, withHistogram);
		
	}

}
