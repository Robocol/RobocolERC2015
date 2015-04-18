#ifndef MISC_ROBOCOL_H
#define MISC_ROBOCOL_H
/*
 * --------------------------------------------------------------------------------------
 * ph_robocol.c (0.9)
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
void g_write_file(char* ruta, char* buff, uint8_t len );

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
void g_read_file(char* ruta, char* buff, uint8_t len );

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


#endif