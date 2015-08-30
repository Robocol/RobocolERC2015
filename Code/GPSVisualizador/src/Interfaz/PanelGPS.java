package Interfaz;
import java.awt.Color;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Observable;
import java.util.Observer;

import javax.swing.BorderFactory;
import javax.swing.JButton;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JTextField;

import org.openstreetmap.gui.jmapviewer.Coordinate;
import org.openstreetmap.gui.jmapviewer.JMapViewer;
import org.openstreetmap.gui.jmapviewer.MapMarkerCircle;
import org.openstreetmap.gui.jmapviewer.MapMarkerDot;

import Mundo.ConexionTCP;
import Mundo.OfflineOsmTileSource;
import Mundo.PosicionActual;


public class PanelGPS extends JPanel implements ActionListener, Observer
{
	// Constantes

	public final static String RUTA_TILES						= "/tiles";
	public final static String IP_DEFECTO 						= "157.253.224.138";

	//Casa de Bacca
	//public final static double LATITUD_DEFECTO					= 4.7539505;
	//public final static double LONGITUD_DEFECTO					= -74.0657646;
	
	//Uniandes
	public final static double LATITUD_DEFECTO					= 4.6014429;
	public final static double LONGITUD_DEFECTO					= -74.0655502;
	
	//Kielce
	//public final static double LATITUD_DEFECTO						= 50.7813513;
	//public final static double LONGITUD_DEFECTO						= 20.4646646;

	public final static int PANEL_GPS_ANCHO						= 800;
	public final static int PANEL_GPS_ALTO						= 600;

	public final static int PANEL_CONEXION_ANCHO				= 200;
	public final static int PANEL_CONEXION_ALTO					= 100;

	public final static int PANEL_MARCADORES_ANCHO				= 200;
	public final static int PANEL_MARCADORES_ALTO				= 300;

	public final static String COMANDO_AGREGAR_MARCADOR 		= "AGREGAR MARCADOR";
	public final static String COMANDO_CENTRAR_MAPA		 		= "CENTRAR MAPA";
	public final static String COMANDO_POSICION_ACTUAL	 		= "POSICION ACTUAL";

	public final static String COMANDO_REINICIAR_MARCADORES		= "REINICIAR MARCADORES";
	public final static String COMANDO_REINICIAR_TRAZA	 		= "REINICIAR TRAZA";
	public final static String COMANDO_ACTIVAR_TRAZA	 		= "ACTIVAR TRAZA";

	public final static String COMANDO_CONECTAR					= "CONECTAR";



	// Atributos

	private JPanel 			panelConexion;
	private JTextField		txtIP;
	private JButton			btnConectar;


	private JPanel 			panelMarcadores;
	private JLabel			lblLatitud;
	private JTextField		txtLatitud;
	private JLabel			lblLongitud;
	private JTextField		txtLongitud;
	private JButton			btnAgregarMarcador;
	private JButton			btnCentrarMapa;
	private JButton			btnPosicionActual;
	private JButton			btnReiniciarMarcadores;
	private JButton			btnReiniciarTraza;
	private JButton			btnActivarTraza;

	private JMapViewer panelMapa;

	private ArrayList<MapMarkerDot> marcadores;
	private ArrayList<MapMarkerCircle> marcadoresTraza;
	private MapMarkerDot marcadorPosActual;

	private PosicionActual posicionActual;

	private boolean dejarTraza;

	private ConexionTCP conexionTCP;


	// Constructor

	public PanelGPS() throws IOException
	{
		this.setSize(PANEL_GPS_ANCHO, PANEL_GPS_ALTO);
		this.setLayout(null);

		////////////////////////////////////////
		// 			Panel Conexion
		///////////////////////////////////////

		panelConexion = new JPanel();
		panelConexion.setSize(PANEL_CONEXION_ANCHO, PANEL_CONEXION_ALTO);
		panelConexion.setLocation(PANEL_GPS_ANCHO - PANEL_CONEXION_ANCHO - 20, 10);
		panelConexion.setBorder(BorderFactory.createTitledBorder("Conexi�n"));
		panelConexion.setLayout(null);

		txtIP = new JTextField();
		txtIP.setText(IP_DEFECTO);
		txtIP.setSize(100, 20);
		txtIP.setLocation(PANEL_CONEXION_ANCHO/2 - txtIP.getWidth()/2, 30);
		panelConexion.add(txtIP);

		btnConectar = new JButton();
		btnConectar.setText("Conectar!");
		btnConectar.setSize(160, 20);
		btnConectar.setLocation(PANEL_CONEXION_ANCHO/2 - btnConectar.getWidth()/2, 60);
		btnConectar.setActionCommand(COMANDO_CONECTAR);
		btnConectar.addActionListener(this);
		panelConexion.add(btnConectar);

		this.add(panelConexion);


		////////////////////////////////////////
		// 			Panel Marcadores
		///////////////////////////////////////



		panelMarcadores = new JPanel();
		panelMarcadores.setSize(PANEL_MARCADORES_ANCHO, PANEL_MARCADORES_ALTO);
		panelMarcadores.setLocation(PANEL_GPS_ANCHO - PANEL_CONEXION_ANCHO - 20, 
				panelConexion.getLocation().y + PANEL_CONEXION_ALTO );
		panelMarcadores.setBorder(BorderFactory.createTitledBorder("Marcadores"));
		panelMarcadores.setLayout(null);

		lblLatitud = new JLabel();
		lblLatitud.setText("Latitud:");
		lblLatitud.setSize(70, 20);
		lblLatitud.setLocation(20, 20);
		panelMarcadores.add(lblLatitud);

		txtLatitud = new JTextField();
		txtLatitud.setText(""+LATITUD_DEFECTO);
		txtLatitud.setSize(75, 20);
		txtLatitud.setLocation(20, 40);
		panelMarcadores.add(txtLatitud);


		lblLongitud = new JLabel();
		lblLongitud.setText("Longitud:");
		lblLongitud.setSize(70, 20);
		lblLongitud.setLocation(panelMarcadores.getWidth() - lblLongitud.getWidth() - 20, 20);
		panelMarcadores.add(lblLongitud);

		txtLongitud = new JTextField();
		txtLongitud.setText(""+LONGITUD_DEFECTO);
		txtLongitud.setSize(75, 20);
		txtLongitud.setLocation(panelMarcadores.getWidth() - lblLongitud.getWidth() - 20, 40);
		panelMarcadores.add(txtLongitud);

		btnAgregarMarcador = new JButton();
		btnAgregarMarcador.setText("Agregar marcador");
		btnAgregarMarcador.setSize(160, 20);
		btnAgregarMarcador.setLocation(panelMarcadores.getWidth()/2 - btnAgregarMarcador.getWidth()/2, 80);
		btnAgregarMarcador.addActionListener(this);
		btnAgregarMarcador.setActionCommand(COMANDO_AGREGAR_MARCADOR);
		panelMarcadores.add(btnAgregarMarcador);

		btnCentrarMapa = new JButton();
		btnCentrarMapa.setText("Centrar mapa");
		btnCentrarMapa.setSize(160, 20);
		btnCentrarMapa.setLocation(panelMarcadores.getWidth()/2 - btnCentrarMapa.getWidth()/2, 
				btnAgregarMarcador.getLocation().y + btnCentrarMapa.getHeight()+10);
		btnCentrarMapa.addActionListener(this);
		btnCentrarMapa.setActionCommand(COMANDO_CENTRAR_MAPA);
		panelMarcadores.add(btnCentrarMapa);

		btnPosicionActual = new JButton();
		btnPosicionActual.setText("Posici�n actual");
		btnPosicionActual.setSize(160, 20);
		btnPosicionActual.setLocation(panelMarcadores.getWidth()/2 - btnPosicionActual.getWidth()/2, 
				btnCentrarMapa.getLocation().y + btnPosicionActual.getHeight()+10);
		btnPosicionActual.addActionListener(this);
		btnPosicionActual.setActionCommand(COMANDO_POSICION_ACTUAL);
		panelMarcadores.add(btnPosicionActual);

		btnReiniciarMarcadores = new JButton();
		btnReiniciarMarcadores.setText("Reiniciar marcadores");
		btnReiniciarMarcadores.setSize(160, 20);
		btnReiniciarMarcadores.setLocation(panelMarcadores.getWidth()/2 - btnReiniciarMarcadores.getWidth()/2, 
				btnPosicionActual.getLocation().y + btnReiniciarMarcadores.getHeight()+10);
		btnReiniciarMarcadores.addActionListener(this);
		btnReiniciarMarcadores.setActionCommand(COMANDO_REINICIAR_MARCADORES);
		panelMarcadores.add(btnReiniciarMarcadores);

		btnReiniciarTraza = new JButton();
		btnReiniciarTraza.setText("Reiniciar traza");
		btnReiniciarTraza.setSize(160, 20);
		btnReiniciarTraza.setLocation(panelMarcadores.getWidth()/2 - btnReiniciarTraza.getWidth()/2, 
				btnReiniciarMarcadores.getLocation().y + btnReiniciarTraza.getHeight()+10);
		btnReiniciarTraza.addActionListener(this);
		btnReiniciarTraza.setActionCommand(COMANDO_REINICIAR_TRAZA);
		panelMarcadores.add(btnReiniciarTraza);

		btnActivarTraza = new JButton();
		btnActivarTraza.setText("Activar traza");
		btnActivarTraza.setSize(160, 20);
		btnActivarTraza.setLocation(panelMarcadores.getWidth()/2 - btnActivarTraza.getWidth()/2, 
				btnReiniciarTraza.getLocation().y + btnActivarTraza.getHeight()+10);
		btnActivarTraza.addActionListener(this);
		btnActivarTraza.setActionCommand(COMANDO_ACTIVAR_TRAZA);
		panelMarcadores.add(btnActivarTraza);


		this.add(panelMarcadores);




		////////////////////////////////////////
		// 			Panel Mapa
		///////////////////////////////////////

		String path = new File(".").getCanonicalPath();
		panelMapa = new JMapViewer();
		panelMapa.setTileSource(new OfflineOsmTileSource("file:///" +path+ RUTA_TILES,18,18));
		System.out.println(path);
		panelMapa.setDisplayPosition(new Coordinate(LATITUD_DEFECTO, LONGITUD_DEFECTO), 18);
		panelMapa.setSize(550, 550);
		panelMapa.setLocation(10, 10);
		this.add(panelMapa);

		marcadores = new ArrayList<MapMarkerDot>();
		marcadoresTraza = new ArrayList<MapMarkerCircle>();

		posicionActual = new PosicionActual(this);
		posicionActual.addObserver(this);

		dejarTraza = false;
		
		conexionTCP= new ConexionTCP(IP_DEFECTO, posicionActual, this);


	}

	public void desconectar()
	{
		btnConectar.setText("Conectar!");
		btnConectar.setEnabled(true);
		txtIP.setEnabled(true);
		conexionTCP.desconectar();
	}

	public void agregarMarcador(double latitud, double longitud)
	{
		MapMarkerDot mark = new MapMarkerDot(latitud, longitud);
		panelMapa.addMapMarker(mark);
		marcadores.add(mark);
	}

	public void agregarMarcadorPosActual(double latitud, double longitud)
	{
		MapMarkerDot mark = new MapMarkerDot(latitud, longitud);
		mark.setColor(Color.RED);
		mark.setBackColor(Color.RED);
		panelMapa.addMapMarker(mark);
		marcadorPosActual = mark;
	}

	public void agregarTraza(double latitud, double longitud)
	{
		MapMarkerCircle mark = new MapMarkerCircle(latitud, longitud,0.0000085);
		mark.setColor(Color.RED);
		mark.setBackColor(Color.RED);
		marcadoresTraza.add(mark);
		panelMapa.addMapMarker(mark);
	}

	public void centrarMapa(double latitud, double longitud)
	{
		Coordinate coordenada = new Coordinate(latitud,longitud);
		panelMapa.setDisplayPosition(coordenada, 18);
	}

	public void reiniciarMarcadores()
	{

		for(int i = 0; i < marcadores.size(); i++)
		{
			panelMapa.removeMapMarker(marcadores.get(i));
		}
		marcadores.removeAll(marcadores);

	}

	public void reiniciarMarcadoresTraza()
	{

		for(int i = 0; i < marcadoresTraza.size(); i++)
		{
			panelMapa.removeMapMarker(marcadoresTraza.get(i));
		}
		marcadoresTraza.removeAll(marcadoresTraza);

	}
	@Override
	public void actionPerformed(ActionEvent e) 
	{
		String comando = e.getActionCommand();

		if(comando.equals(COMANDO_AGREGAR_MARCADOR))
		{
			agregarMarcador(Double.parseDouble(txtLatitud.getText()),Double.parseDouble(txtLongitud
					.getText()));
		}
		else if(comando.equals(COMANDO_CENTRAR_MAPA))
		{
			centrarMapa(Double.parseDouble(txtLatitud.getText()),Double.parseDouble(txtLongitud
					.getText()));
		}
		else if(comando.equals(COMANDO_REINICIAR_MARCADORES))
		{
			reiniciarMarcadores();
		}
		else if(comando.equals(COMANDO_REINICIAR_TRAZA))
		{
			reiniciarMarcadoresTraza();
		}
		else if(comando.equals(COMANDO_POSICION_ACTUAL))
		{
			txtLatitud.setText(""+posicionActual.darLatitudActual());
			txtLongitud.setText(""+posicionActual.darLongitudActual());
		}
		else if(comando.equals(COMANDO_ACTIVAR_TRAZA))
		{
			if(dejarTraza)
			{
				btnActivarTraza.setText("Activar Traza");
				dejarTraza = false;
				
			}
			else
			{
				btnActivarTraza.setText("Desactivar Traza");
				dejarTraza = true;
			}
		}
		else if(comando.equals(COMANDO_CONECTAR))
		{
			if(!conexionTCP.darConectado())
			{
				conectar();
			}
			else
			{
				desconectar();
				
			}

		}
		this.repaint();

	}

	public void conectar() 
	{
		try 
		{
			conexionTCP = new ConexionTCP(txtIP.getText(),posicionActual, this);
			
			btnConectar.setText("Conectando...");
			btnConectar.setEnabled(false);
			txtIP.setEnabled(false);
			
			conexionTCP.start();
			
		} 
		catch (Exception e) 
		{

			e.printStackTrace();
		} 


	}
	public void cambiarInterfazConectado()
	{
		btnConectar.setText("Desconectar");
		btnConectar.setEnabled(true);
	}

	@Override
	public void update(Observable arg0, Object arg1) 
	{
		if(marcadorPosActual == null ||( posicionActual.darLatitudActual() != marcadorPosActual.getCoordinate().getLat() &&
				posicionActual.darLongitudActual() != marcadorPosActual.getCoordinate().getLon()))
		{
			panelMapa.removeMapMarker(marcadorPosActual);
			if(dejarTraza)
			{
				agregarTraza(marcadorPosActual.getCoordinate().getLat(), marcadorPosActual.getCoordinate().getLon());
			}
			System.out.println(posicionActual.darLatitudActual() + "     "+posicionActual.darLongitudActual());
			agregarMarcadorPosActual(posicionActual.darLatitudActual(), posicionActual.darLongitudActual());	
		}

	}

}
