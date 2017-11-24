package ui;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.geom.Line2D;
import java.awt.geom.Rectangle2D;

import javax.swing.JPanel;

/**
 * @author jmedina
 *
 */
public class PanelPrincipal extends JPanel {

	/**
	 * 
	 */
	private static final long serialVersionUID = -4194947930856449026L;
	private int[] _casillas;
	private int _maxCasilla;

	/**
	 * 
	 */
	public PanelPrincipal() {
		super(true);
	}
	
	public void setCasillas( int[] casillas, int maxCasilla ) {
		this._casillas = casillas;
		this._maxCasilla = maxCasilla;
		
		boolean notEscaled = true;
		if( notEscaled ) {
			setPreferredSize( new Dimension( casillas.length + 80, 600));
		} else {
			setPreferredSize( new Dimension( 800, 600));
		}
	}
	
	@Override
	protected void paintComponent(Graphics g) {
		super.paintComponent(g);
		
		//System.out.println( "==> paintComponent()" );
		
		Graphics2D g2 = (Graphics2D)g;
		
		Dimension dim = getPreferredSize();
		double width = dim.getWidth();
		double height = dim.getHeight();

		g2.setColor( Color.BLACK );
		g2.fill( new Rectangle2D.Double(0,0,width,height) );
		
		pintaAxis( g2, width, height );
		
		if( this._casillas != null ) {
			pintaCasillas( g2, this._casillas, this._maxCasilla );
			pintaLeyendas( g2, this._maxCasilla, this._casillas.length );
		}
	}
	
	private void pintaAxis( Graphics2D g2, double width, double height ) {
		
		//System.out.println( "==> pintaAxis()" );
		
		double borde = 40;
		
		g2.setColor( Color.RED );
		g2.draw( new Line2D.Double( borde,height-borde,width-borde,height-borde) );
		
		g2.setColor( Color.RED );
		g2.draw( new Line2D.Double( borde,borde,borde,height-borde) );
	}
	
	private void pintaLeyendas(Graphics2D g2, int maxY, int maxX) {
		
		Dimension dim = getPreferredSize();
		double width = dim.getWidth();
		double height = dim.getHeight();
		double borde = 40;
		
		g2.setColor( Color.WHITE );
		double x1,y1;
		double yStep;
		double steps = 10;
		String leyenda = "";

		for( int i=0; i<=steps; i++ ) {
			yStep = i*(height-2*borde)/steps;
			x1 = 20;
			y1 = height-borde-yStep;
			leyenda = ""+i*maxY/steps;
			g2.drawString( leyenda, (float)x1, (float)y1+15 );
			g2.draw( new Line2D.Double(x1, y1, x1+borde, y1));
		}
		
		double xStep;
		steps = 5;

		for( int i=1; i<=steps; i++ ) {
			xStep = i*(width-2*borde)/steps;
			x1 = xStep+borde;
			y1 = height-20;
			leyenda = ""+(float)(i*maxX/steps);
			g2.drawString( leyenda, (float)x1-25, (float)y1 );
			g2.draw( new Line2D.Double(x1, y1-15, x1, y1-borde));
		}
		
	}

	public void pintaCasillas( Graphics2D g2, int[] casillas, int yMax ) {
		
		//System.out.println( "==> pintaCasillas() -- yMax = "+yMax );
		
		double borde = 40;
		
		Dimension dim = getPreferredSize();
		double width = dim.getWidth();
		double height = dim.getHeight();
		double yI,y1,y2,x1;
		
		
		
		for( int i=0; i<casillas.length; i++ ) {
			x1 = i*(width-2*borde)/(double)casillas.length + borde;
			yI = casillas[i]*(height-2*borde)/(double)yMax; 
			y1 = height-borde-yI;
			y2 = height-borde;
			
			if( casillas[i] != 0 ) {
				g2.setColor( Color.RED );
				g2.draw( new Line2D.Double( x1,y1,x1,y2) );
			} else {
				g2.setColor( Color.YELLOW );
				g2.draw( new Line2D.Double( x1,y1-5,x1,y2+5) );
			}
			
			//System.out.println( "==> casilla("+x1+","+y1+")-("+x1+","+y2+")" );
		}
	}
}
