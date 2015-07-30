/*
 * --------------------------------------------------------------------------------------
 * spi_robocol.c (0.5)
 *
 *  Created on: Jul 12, 2015
 *
 *      Authors:	Daniel Ochoa 
 *				    Germán Giraldo
 * -------------------------------------------------------------------------------------
 */


#ifndef GPS_ROBOCOL_H
#define GPS_ROBOCOL_H

 
/*
** =====================================================================================
**     USO DEL DRIVER
*/
/*!
**     @resumen
**          Este driver facilita el manejo de GPS Garmin a utilizarse en el Rover de exploración
**			marciana desarrollado por el equipo ROBOCOL para  participar en la competencia ERC
**			en su edición 2015. El protocolo de mensajes utilizado en el GPS es el RMC (Recommended
**			minimun navigation information) definido por el standar NMEA 1083 el cual tiene la 
**			siguiente estructura:
**
**		$GPRMC,123519,A,4807.038,N,01131.000,E,022.4,084.4,230394,003.1,W*6A
**
**		Where:
**		     RMC          Recommended Minimum sentence C
**		     123519       12:35:19 UTC - Hora a la que se obtiene la informacion 
**		     A            Status A=active or V=Void - Indica si existe conexion satelital
**		     4807.038,N   Latitude 48 deg 07.038' N - Latitud en grados y minutos
**		     01131.000,E  Longitude 11 deg 31.000' E - Longitud 
**		     022.4        22.4kn - Velocidad en nudos
**		     084.4        84.4 deg. Angulo de trayectoria respecto al norte geografico
**		     230394       23rd of March 1994 - Fecha de la medicion
**		     003.1,W      3.1 deg al oeste - Desviación del polo magnetico respecto al geografico
**		     *6A          The checksum data, always begins with *
**
*/
/* =====================================================================================*/


/*--------------------------------------------------------------------------*/
/*
 *                 INCLUSIÓN DE LIBRERIAS
 */
#include <stdio.h>
#include <stdint.h>
#include <pthread.h>
#include "misc_robocol.h"
#include "uart_robocol.h"

/*--------------------------------------------------------------------------*/
/*
 *                  DEFINICIÓN DE TYPES
 */
	typedef int gps_st;

	typedef struct gps_dev{
		char*	ruta;			//Ruta al archivo de reporte
		char	status;			//Status A=active or V=Void - Indica si existe conexion satelital
		float 	latitude;		//Latitud en grados. Negativo si es latitud sur, positivo de lo contrario
		float 	longitude;		//Longitud en grados. Negativo si es latitud sur, positivo de lo contrario
		float	speed;			//Velocidad en kilometros por hora
		float 	track_angle;	//Angulo de trayectoria respecto al norte geografico
		float	mag_dev;	//Desviación del polo magnetico respecto al geografico
		char	date[10];			//Fecha en formato DD/MM/AA
		char	time[10];			//Hora en formato hh:mm:ss UTC
	} gps_dev;

	uint8_t reading;
	struct gps_dev gps_device;

#define GPS_ERROR	0x01
#define GPS_OK 		0x00

	static char UART_PATH[11]= "/dev/ttyS0";
	//static char UART_PATH[14]= "./testGPS.log";
	pthread_t a_thread;
	void *thread_result;

/* =====================================================================================*/
/*
						DEFINICIÓN DE FUNCIONES 

Nota: Esta librería no garantiza que los puertos UART de la tarjeta se encuentren
conectados por los multiplexores de selección. Para configurar adecuadamente estos 
multiplexores, referirse al esquema de conexión de los GPIO para la Intel Galileo Gen1.
 */
/* =====================================================================================*/


/*
** ===================================================================
**     Método      :  gps_build
*/
/*!
**     @resumen
**          
**     @param
**          ruta     	   	- Ruta absoluta del archivo de reporte del GPS 
*/
/* ===================================================================*/
gps_st gps_build(char* ruta);

/*
** ===================================================================
**     Método      :  gps_start
*/
/*!
**     @resumen
** 			Inicia toma de datos del GPS y los almacena en el archivo 
**			especificado por el campo ruta de la estructuda gps_device
*/
/* ===================================================================*/
gps_st gps_start(void);

/*
** ===================================================================
**     Método      :  gps_stop
*/
/*!
**     @resumen
**          Detiene la toma de datos del gps.
*/
/* ===================================================================*/
gps_st gps_stop(void);

/*
** ===================================================================
**     Método      :  gps_getData
*/
/*!
**     @resumen
**          El puntero ingresado por parametro ahora apunta a la posicion
**			de una estructura gps_dev donde se encuentra la informacion
**			mas recientemente recolectada del GPS
*/
/* ===================================================================*/
gps_st gps_getData(gps_dev *dev);

/*
** ===================================================================
**     Método      :  gps_parser
*/
/*!
**     @resumen
**			Lee el archivo de reporte y procesa la información en el 
**			contenida para almacenarla en la estructura gps_device
**          
*/
/* ===================================================================*/
gps_st gps_parser(char* line);

/*
** ===================================================================
**     Método      :  gps_continuousUpdate
*/
/*!
**     @resumen
**			Lee el archivo de reporte y procesa la información en el 
**			contenida para almacenarla en la estructura gps_device
**          
*/
/* ===================================================================*/
void *gps_continuousUpdate(void* arg);


float gps_parseLatitude(char* word);
float gps_parseLongitude(char* word);

#endif