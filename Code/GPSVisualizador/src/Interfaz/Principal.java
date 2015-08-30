package Interfaz;

import java.awt.BorderLayout;
import java.io.IOException;

import javax.swing.JFrame;

public class Principal extends JFrame
{
	
	public Principal() throws IOException
	{
		
		PanelGPS gps = new PanelGPS();
		this.setVisible(true);
		this.setDefaultCloseOperation(DISPOSE_ON_CLOSE);
		this.setLayout(new BorderLayout());
		this.setTitle("ROBOCOL URC - VISUALIZADOR GPS");
		this.setSize(800, 600);
		this.setResizable(false);
		this.add(gps, BorderLayout.CENTER);
		
	}
	public static void main(String[] args) throws IOException
	{
		Principal p = new Principal();
		System.out.println("FLAG!");
	}
}
