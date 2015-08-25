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

#include "gps_robocol.h"

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
gps_st gps_build(char* ruta){
	gps_reading = FALSE;
	if(term_open(&gps_term,"/dev/ttyS0","/dev/ttyS0")){
		printf("Error en term_open.(gps_robocol -> gps_build)\n");
		return 1;
	}
	gps_device.latitude=4.7539505;
	gps_device.longitude=-74.0657646;
}

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
gps_st gps_start(void){
	gps_reading=TRUE;

	if (pthread_create(&a_thread, NULL, gps_continuousUpdate, (void *)NULL)) {
		perror("Thread creation failed. (gps_robocol -> gps_start )");
		return GPS_ERROR;
	}

	return GPS_OK;
}

/*
** ===================================================================
**     Método      :  gps_stop
*/
/*!
**     @resumen
**          Detiene la toma de datos del gps.
*/
/* ===================================================================*/
gps_st gps_stop(void){
	gps_reading=FALSE;
	printf("Waiting for thread to finish... (gps_robocol -> gps_start )\n");
	if (pthread_join(a_thread, &thread_result)) {
		perror("Thread join failed. (gps_robocol -> gps_start )");
		return GPS_ERROR;
	}
}

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
gps_st gps_getData(gps_dev *dev){

	*dev=gps_device;

	return GPS_OK;
}

/*
** ===================================================================================
**     Método      :  gps_parser
*/
/*!
**     @resumen
**			Lee el archivo de reporte y procesa la información en el 
**			contenida para almacenarla en la estructura gps_device.
**			recuerde que la estructura del mensage debe ser la siguiente
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
gps_st gps_parser(char* line){
	char 	*word=malloc(10*sizeof(char));
	char 	**pline	= &line;
	const char	delim[2]=",";


	//Identificador del protocolo
	printf("line: %s\n",line );
	if((word = strsep(pline,delim))==NULL){
		printf("Error: No se encontraron mas tokens en el string. (gps_robocol -> gps_parser)\n");
		return GPS_ERROR;
	}
	printf("word: %s\n",word );


	if(!strcmp(word,"$GNGSA")||!strcmp(word,"$GPGSV")){
		printf("Linea de control del GPS. Ignorada\n");
		return GPS_OK;
	}else if(strcmp(word,"$GPRMC")){
		printf("La linea ingresada por parámetro no corresponte al formato RMC. (gps_robocol -> gps_parser )\n");
		return GPS_ERROR;
	}
	//Hora
	if((word = strsep(pline,delim))==NULL){
		printf("Error: No se encontraron mas tokens en el string. (gps_robocol -> gps_parser)\n");
		return GPS_ERROR;
	}
	printf("%c%c:%c%c:%c%c\n",word[0],word[1],word[2],word[3],word[4],word[5]);
	sprintf(gps_device.time,"%c%c:%c%c:%c%c",word[0],word[1],word[2],word[3],word[4],word[5]);


	//status
	if((word = strsep(pline,delim))==NULL){
		printf("Error: No se encontraron mas tokens en el string. (gps_robocol -> gps_parser)\n");
		return GPS_ERROR;
	}
	
	gps_device.status = word[0];

	//Latitude
	if((word = strsep(pline,delim))==NULL){
		printf("Error: No se encontraron mas tokens en el string. (gps_robocol -> gps_parser)\n");
		return GPS_ERROR;
	}

	gps_device.latitude=gps_parseLatitude(word);
	if((word = strsep(pline,delim))==NULL){
		printf("Error: No se encontraron mas tokens en el string. (gps_robocol -> gps_parser)\n");
		return GPS_ERROR;
	}
	if(*word=='S'){
		gps_device.latitude=(-1)*gps_device.latitude;
	}

	//Longitude
	if((word = strsep(pline,delim))==NULL){
		printf("Error: No se encontraron mas tokens en el string. (gps_robocol -> gps_parser)\n");
		return GPS_ERROR;
	}

	gps_device.longitude=gps_parseLongitude(word);

	if((word = strsep(pline,delim))==NULL){
		printf("Error: No se encontraron mas tokens en el string. (gps_robocol -> gps_parser)\n");
		return GPS_ERROR;
	}
	if(*word=='W'){
		gps_device.longitude=(-1)*gps_device.longitude;
	}

	//Velocidad
	if((word = strsep(pline,delim))==NULL){
		printf("Error: No se encontraron mas tokens en el string. (gps_robocol -> gps_parser)\n");
		return GPS_ERROR;
	}
	gps_device.speed=atof(word)*1.852;
	printf("speed: %f\n", gps_device.speed);

	//Track Angle
	if((word = strsep(pline,delim))==NULL){
		printf("Error: No se encontraron mas tokens en el string. (gps_robocol -> gps_parser)\n");
		return GPS_ERROR;
	}
	gps_device.track_angle=atof(word);
	printf("t_angle: %f\n", gps_device.track_angle);

	//Date
	if((word = strsep(pline,delim))==NULL){
		printf("Error: No se encontraron mas tokens en el string. (gps_robocol -> gps_parser)\n");
		return GPS_ERROR;
	}
	printf("date: %c%c/%c%c/%c%c\n",word[0],word[1],word[2],word[3],word[4],word[5]);
	sprintf(gps_device.date,"%c%c/%c%c/%c%c",word[0],word[1],word[2],word[3],word[4],word[5]);

	//Magnetic Deviation
	if((word = strsep(pline,delim))==NULL){
		printf("Error: No se encontraron mas tokens en el string. (gps_robocol -> gps_parser)\n");
		return GPS_ERROR;
	}
	gps_device.mag_dev=atof(word);
	printf("m_dev: %f\n", gps_device.mag_dev);

	if((word = strsep(pline,delim))==NULL){
		printf("Error: No se encontraron mas tokens en el string. (gps_robocol -> gps_parser)\n");
		return GPS_ERROR;
	}
	if(word[0]=='W'){
		gps_device.mag_dev=(-1)*gps_device.mag_dev;
	}
	printf("m_dev: %f\n", gps_device.mag_dev);

	return GPS_OK;
}

float gps_parseLatitude(char* word){
	char *s_deg=alloca(10*sizeof(char));
	char *s_min;
	float deg=0;
	float min=0;

	printf("latitude %s\n", word);
	strcpy(s_deg,word);

	s_deg[2]='\0';

	s_min=&word[2];

	deg=atof(s_deg);

	min=atof(s_min);

	deg=deg + min/60;
	return deg;
}


float gps_parseLongitude(char* word){
	char *s_deg=alloca(10*sizeof(char));
	char *s_min;
	float deg=0;
	float min=0;

	strcpy(s_deg,word);

	s_deg[3]='\0';

	printf("s_deg%s\n", s_deg);
	
	s_min=&word[3];

	deg=atof(s_deg);

	min=atof(s_min);

	deg=deg+min/60;
	return deg;
}


/*
** ===================================================================
**     Método      :  gps_continuousUpdate
*/
/*!
**     @resumen
**			Lee el canal UART y procesa la información obtenida
**			para almacenarla en la estructura gps_device
*/
/* ===================================================================*/
void *gps_continuousUpdate(void* arg){
	size_t size=100;
	char *line=alloca(size*sizeof(char));

	while(gps_reading){
	    printf("Dentro del loop\n");
	    
	    if(term_readln(&gps_term,&line)){
	    	printf("Error leyendo gps por puerto UART. (gps_robocol -> gps_continuousUpdate)\n");
	    }

		if(gps_parser(line)){
		   	printf("Error al procesar la información obtenida del GPS. (gps_robocol -> gps_continuousUpdate)\n");
		}
		
	    printf("LONGITUD DESPUES DE PARSER: %f\n",gps_device.longitude );
	    printf("\n--------------------------------------------------------\n");
	}

	free(line);
	pthread_exit("writting stopped. (gps_robocol -> gps_writeFile )");
}

/*
** ===================================================================
**     Método      :  gps_report
*/
/*!
**     @resumen
**			Lee el canal UART y procesa la información obtenida
**			para almacenarla en la estructura gps_device
*/
/* ===================================================================*/
gps_st gps_report(char *line){

	if(sprintf(line,"%f,%f,%f", gps_device.latitude, gps_device.longitude, gps_device.speed)<0){
		printf("Error abriendo el archivo de reporte. (gps_robocol -> gps_reportFile)\n");
		return GPS_ERROR;
	}
	
	return GPS_OK;
}

