package Mundo;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.InetSocketAddress;
import java.net.ServerSocket;
import java.net.Socket;
import java.net.SocketException;

import Interfaz.PanelGPS;




public class ConexionTCP extends Thread
{
	private final static int LOCAL_HOST_PORT = 50002;
	public final static int CONNECTION_TIME_OUT = 3000;

	private Socket socket;

	private PrintWriter out;
	private BufferedReader in;

	private boolean conectado;

	private String serverIP;

	private PosicionActual posicionActual;

	private PanelGPS panelGPS;

	private boolean flagKillThread;


	public ConexionTCP(String pServerIP, PosicionActual pPosicionActual, PanelGPS pPanelGPS) throws IOException
	{
		socket = null;
		out = null;
		in = null;
		conectado = false;
		flagKillThread = false;
		serverIP = pServerIP;
		posicionActual = pPosicionActual;
		panelGPS = pPanelGPS;
	}

	public void desconectar()
	{
		flagKillThread = true;
		if(socket == null || !socket.isConnected())
		{
			try 
			{
				//new Socket(serverSocket.getInetAddress(),serverSocket.getLocalPort()).close();
			} 
			catch (Exception e) 
			{
				e.printStackTrace();
			}
		}
		conectado = false;

	}

	public boolean iniciarConexion()
	{
		try
		{
			socket = new Socket();
			socket.connect(new InetSocketAddress(serverIP,LOCAL_HOST_PORT),CONNECTION_TIME_OUT); 
			out = new PrintWriter(socket.getOutputStream(), true); 
			in = new BufferedReader(new InputStreamReader( socket.getInputStream())); 
			conectado = true;
			panelGPS.cambiarInterfazConectado();
			return true;
		} 
		catch (IOException e) 
		{
			conectado = false;
			e.printStackTrace();
			return conectado;
		} 
	}

	public void calcularCoordenadas(String trama)
	{

		String camposTrama[] = trama.split(",");
		if(camposTrama[2].equals("A"))
		{
			double campoLatitud = Double.parseDouble(camposTrama[3]);
			if(camposTrama[4].equals("S")) campoLatitud = campoLatitud*-1;

			double campoLongitud = Double.parseDouble(camposTrama[5]);
			if(camposTrama[6].equals("W")) campoLongitud = campoLongitud*-1;

			double longitudActual = (int)(campoLongitud/100) + (campoLongitud/100.0 - ((int)(campoLongitud/100)) )/60.0*100;
			double latitudActual = (int)(campoLatitud/100) + (campoLatitud/100.0 - ((int)(campoLatitud/100)) )/60.0*100;

			posicionActual.actualizarCoordenadas(latitudActual, longitudActual);

		}
	}

	public boolean darConectado()
	{
		return conectado;
	}




	public void run()
	{
		try
		{


			while(!flagKillThread && iniciarConexion())
			{

				conectado = true;

				out.println("gps");
				String line = in.readLine();
				System.out.println(line);
				System.out.println("RUN!");

				Thread.sleep(1000);

				if(line == null)
				{
					panelGPS.desconectar();
				}
				else
				{
					String aux2[] = line.split(",");
					posicionActual.actualizarCoordenadas(
							Double.parseDouble(aux2[0]), Double.parseDouble(aux2[1]));
					
				}

				socket.close();
				Thread.sleep(1000);
			}
			panelGPS.desconectar();
			conectado = false;

		}
		catch(Exception e)
		{	
			e.printStackTrace();

		}
	}
}





