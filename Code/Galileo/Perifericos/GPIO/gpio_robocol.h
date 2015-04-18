/*
 * gpio_robocol.h (2.0)
 *
 *  Created on: Mar 13, 2015
 *      Authors:	Daniel Ochoa
 *				    Germán Giraldo
 */
#ifndef GPIO_ROBOCOL_H
#define GPIO_ROBOCOL_H


/*--------------------------------------------------------------------------*/
/*
 *                 INCLUSIÓN DE LIBRERIAS
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>
#include "i2c_robocol.h"

/* =====================================================================================*/
/*
					DEFINICIÓN DE PINES

A continuación se definen las asociaciones entre los pines y los GPIO de la tarjeta Intel
Galileo.

Nota: Esta librería no garantiza que el gpio que está asociado al pin se encuentre 
conectado por los multiplexores de selección. Para configurar adecuadamente estos 
multiplexores, referirse al esquema de conexión de los GPIO par ala Intel Galileo Gen1.
 */
/* =====================================================================================*/


#define PIN0		0x32	//Pin 0 corresponde a gpio50
#define PIN1		0x33	//Pin 1 corresponde a gpio51
#define PIN2		0x20	//Pin 2 corresponde a gpio32
#define PIN2FAST	0x0D	//Pin 2 fast corresponde a gpio14
#define PIN3		0x12	//Pin 3 corresponde a gpio18
#define PIN3FAST	0x0E	//Pin 3 fast corresponde a gpio15
#define PIN4		0x1C	//Pin 4 corresponde a gpio28
#define PIN5		0x11	//Pin 5 corresponde a gpio17
#define PIN6		0x18	//Pin 6 corresponde a gpio24
#define PIN7		0x1B	//Pin 7 corresponde a gpio27
#define PIN8		0x1A	//Pin 8 corresponde a gpio26
#define PIN9		0x13	//Pin 9 corresponde a gpio19
#define PIN10		0x00	//Pin 10 corresponde a gpio42 TODO: Hacer Algo
#define PINA0		0x2C	//Pin A0 corresponde a gpio44
#define PINA1		0x2D	//Pin A1 corresponde a gpio45
#define PINA2		0x2E	//Pin A2 corresponde a gpio46
#define PINA3		0x2F	//Pin A3 corresponde a gpio47
#define PINA4		0x30	//Pin A4 corresponde a gpio48
#define PINA5		0x31	//Pin A5 corresponde a gpio49

/*--------------------------------------------------------------------------*/
/*
 *                 TYPES DE LA LIBRERIA
 */

#define GPIO_ERROR	0x01
#define GPIO_OK 	0x00

#define OUT		0
#define IN 		1

#define HIGH	1
#define LOW 	0

typedef uint8_t gpio_st; /* Estado de salida de la función*/

/*--------------------------------------------------------------------------*/
/*
 *                 FUNCIONES DE LA LIBRERIA
 */

/*
** ===================================================================
**     Método      :  gpio_export
*/
/*!
**     @resumen
**          Exporta el gpio especificado al Sysfs
**     @param
**          str_num    	   	- String del número del gpio a exportar
**     @return
**          				- Estado de finalización
*/
/* ===================================================================*/
static gpio_st gpio_export(char* str_num, uint8_t len );

/*
** ===================================================================
**     Método      :  gpio_unexport
*/
/*!
**     @resumen
**          Retira el gpio especificado al Sysfs
**     @param
**          str_num    	   	- String del número del gpio a retira
**     @return
**          				- Estado de finalización
*/
/* ===================================================================*/
static gpio_st gpio_unexport(char* buff, uint8_t len );

/*
** ===================================================================
**     Método      :  gpio_set_dir
*/
/*!
**     @resumen
**          Define el GPIO especificado como una entreada o salida
**			según se especifica por parámetro.
**     @param
**          num     	   	- Número del GPIO
**     @param
**          esEntrada		- Booleano de valor 1 en caso de que el 
**							GPIO se desee definir como entrada.
*/
/* ===================================================================*/
gpio_st gpio_set_dir(uint8_t num,int esEntrada);

/*
** ===================================================================
**     Método      :  gpio_set_value
*/
/*!
**     @resumen
**          Define el valor del GPIO especificado como 1 o 0
**			según se especifica por parámetro. El GPIO debe
**			estar definido como salida
**
**     @param
**          num     	   	- Número del GPIO
**     @param
**          esEntrada		- Booleano de valor 1 o 0 según se 
**							requiera isntanciar el GPIO.
*/
/* ===================================================================*/
gpio_st gpio_gal_value(uint8_t num,uint8_t valor);

gpio_st gpio_muxlock(uint8_t num);

gpio_st gpio_gal_set(uint8_t num);

gpio_st gpio_gal_clear(uint8_t num);

gpio_st build_expander(uint8_t addr);

gpio_st close_expander(void);

gpio_st gpio_exp_clear(uint8_t port);

gpio_st gpio_exp_set(uint8_t port);

gpio_st reg_clear(void);

gpio_st reg_set(void);

gpio_st reg_set_value(char* value);

gpio_st gpio_exp_get(uint8_t port, uint8_t* buff);

gpio_st reg_get(char* buff);


#endif