#ifndef MISC_ROBOCOL_H
#define MISC_ROBOCOL_H
/*
 * --------------------------------------------------------------------------------------
 * misc_robocol.c (0.9)
 *
 *  Created on: Mar 12, 2015
 *
 *      Authors:	ROBOCOL
 * -------------------------------------------------------------------------------------
 */

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdint.h>
#include <string.h>

/* =====================================================================================*/
/*
					DEFINICION DE TIPOS
A continuación se realiza la definición de los tipos necesarios para el 
funcionamiento de la libreria.
 */
/* =====================================================================================*/

typedef uint8_t st_misc;

/* =====================================================================================*/
/*
					DEFINICIÓN DE CONSTANTES
A continuación se realiza la definición de las constantes globales necesarias para el 
funcionamiento de la libreria.
 */
/* =====================================================================================*/
#ifndef FALSE
#define FALSE 0x00
#endif

#ifndef TRUE
#define TRUE 0x01
#endif

#ifndef MISC_OK
#define MISC_OK 0x00
#endif

#ifndef MISC_ERROR
#define MISC_ERROR 0x00
#endif

/*
** ===================================================================
**     Método      :  g_write_file
*/
/*!
**     @resumen
**          Convierte un uint8_t a una cadena de caracteres (string)
**     @param
**          i 	    	   	- Número a convertir
**     @preturn
**          				- Apuntador a la primera posición del array.
*/
/* ===================================================================*/
st_misc g_write_file(char* ruta, char* buff, uint8_t len );

/*
** ===================================================================
**     Método      :  g_read_file
*/
/*!
**     @resumen
**          Convierte un uint8_t a una cadena de caracteres (string)
**     @param
**          i 	    	   	- Número a convertir
**     @preturn
**          				- Apuntador a la primera posición del array.
*/
/* ===================================================================*/
st_misc g_read_file(char* ruta, char* buff, uint8_t len );

/*
** ===================================================================
**     Método      :  u8toa
*/
/*!
**     @resumen
**          Convierte un uint8_t a una cadena de caracteres (string)
**     @param
**          i 	    	   	- Número a convertir
**     @preturn
**          				- Apuntador a la primera posición del array.
*/
/* ===================================================================*/
char* u8toa(uint8_t i);

/*
** ===================================================================
**     Método      :  u32toa
*/
/*!
**     @resumen
**          Convierte un uint32_t a una cadena de caracteres (string)
**     @param
**          i 	    	   	- Número a convertir
**     @preturn
**          				- Apuntador a la primera posición del array.
*/
/* ===================================================================*/
char* u32toa(uint32_t i);


/*
** ===================================================================
**     Método      array_to_i32
*/
/*!
**     @resumen
**          Convierte un a int32_t un arraglo de bytes
**     @param
**          array 	    	   	- Arreglo de bytes a convertir
**     @param
**          len 	    	   	- Tamaño del arreglo a convertir
**     @param
**          little_endian 	    - Indicador de little_endian o big_endian.
**
**     @preturn
**          				- Numero convertido a int32_t.
*/
/* ===================================================================*/
int32_t array_to_i32(uint8_t* array, uint8_t len, uint8_t little_endian);


#endif