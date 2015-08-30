package Mundo;

import java.util.Observable;

import Interfaz.PanelGPS;

public class PosicionActual extends Observable
{

	private double latitudActual;
	private double longitudActual;
	
	public PosicionActual(PanelGPS panelGPS)
	{
		latitudActual = 0;
		longitudActual = 0;
		addObserver(panelGPS);
	}
	
	public double darLatitudActual()
	{
		return latitudActual;
	}
	
	public void actualizarCoordenadas(double pLatitudActual, double pLongitudActual)
	{
		latitudActual =pLatitudActual;
		longitudActual =pLongitudActual;
		setChanged();
		notifyObservers();
	}

	
	public double darLongitudActual()
	{
		return longitudActual;
	}
	

}
