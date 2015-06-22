/*
 * --------------------------------------------------------------------------------------
 * spi_robocol.c (0.5)
 *
 *  Created on: Jun, 2015
 *
 *      Authors:	Daniel Ochoa 
 *				    Germán Giraldo
 * -------------------------------------------------------------------------------------
 */


#ifndef TRACCION_ROBOCOL_H
#define TRACCION_ROBOCOL_H

 
/*
** =====================================================================================
**     USO DEL DRIVER
*/
/*!
**     @resumen
**
*/
/* =====================================================================================*/


/*--------------------------------------------------------------------------*/
/*
 *                 INCLUSIÓN DE LIBRERIAS
 */
#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include "ph_robocol.h"

/*--------------------------------------------------------------------------*/
/*
 *                  DEFINICIÓN DE ESTRUCTURAS
 */
	typedef int tr_st;

	int tr_fd;

	typedef struct {
		uint8_t state;
		uint8_t type;
		uint8_t pwm;
		uint8_t vel;
		ph_dev *front_ph;
		ph_dev *back_ph;
		uint8_t device_built;
	} tr_dev;

tr_dev	tr_device;


#define TR_ERROR	0x01
#define TR_OK 		0x00


/*--------------------------------------------------------------------------*/
/*
 *                  DEFINICIÓN DE TYPES
 */
const uint8_t MASTER = 0x01;
const uint8_t SLAVE = 0x02;
const uint8_t LEFT_SIDE = 0x04;
const uint8_t RIGHT_SIDE= 0x08;

/*--------------------------------------------------------------------------*/
/*
 *                  DEFINICIÓN DE STATES
 */
const uint8_t STOPPED 	= 0x00;
const uint8_t FORWARD 	= 0x01;
const uint8_t BACKWARD 	= 0x02;
const uint8_t STEER 	= 0x04;


/* =====================================================================================*/
/*
						DEFINICIÓN DE FUNCIONES 

Nota: Esta librería no garantiza que el gpio que está asociado al pin se encuentre 
conectado por los multiplexores de selección. Para configurar adecuadamente estos 
multiplexores, referirse al esquema de conexión de los GPIO para la Intel Galileo Gen1.
 */
/* =====================================================================================*/


/*
** ===================================================================
**     Método      :  tr_build
*/
/*!
**     @resumen
**          Se inicializa el dispositivo encargado de controlar la 
**			tracción del Rover. Para esto, se crean los dispositios que
**			controlan los puente H, usando la librería ph_robocol.
**     @param
**          type     	   	- Tipo del dispositivo, indicando se es MASTER
**							o SLAVE, y si está ubicado en RIGHT_SIDE o 
**							LEFT_SIDE. Se puede instanciar este argumento
**							usando OR bit a bit. Por ejemplo:
**
**								tr_build(MASTER|RIGHT);
**							
*/
/* ===================================================================*/
tr_st tr_build(uint8_t type);

/*
** ===================================================================
**     Método      :  tr_forward
*/
/*!
**     @resumen
**          El dispositivo se dirije hacia adelante a el argumento 
**			ingresado
**     @param
**          arg     	   	- Entero representando el PWM o la velocidad
**							a la que se desea que los motores retrocedan
*/
/* ===================================================================*/
spi_st tr_forward(int arg);

/*
** ===================================================================
**     Método      :  tr_backward
*/
/*!
**     @resumen
**          El dispositivo se dirije hacia adelante a el argumento 
**			ingresado
**     @param
**          arg     	   	- Entero representando el PWM o la velocidad
**							a la que se desea que los motores retrocedan
*/
/* ===================================================================*/
spi_st tr_backward(uint8_t arg);


/*
** ===================================================================
**     Método      :  tr_spin
*/
/*!
**     @resumen
**          El dispositivo se dirije hacia adelante a el argumento 
**			ingresado
**     @param
**          dir     	   	- Dirección de giro
*/
/* ===================================================================*/
spi_st tr_spin(int dir);

/*
** ===================================================================
**     Método      :  tr_diffTurn
*/
/*!
**     @resumen
**          El dispositivo se dirije hacia adelante a el argumento 
**			ingresado
**     @param
**          dir     	   	- Dirección de giro
*/
/* ===================================================================*/
spi_st tr_diffTurn(int dir);

/*
** ===================================================================
**     Método      :  tr_spin
*/
/*!
**     @resumen
**          El dispositivo se dirije hacia adelante a el argumento 
**			ingresado
**     @param
**          dir     	   	- Dirección de giro
*/
/* ===================================================================*/
spi_st tr_Turn(int dir);

/*
** ===================================================================
**     Método      :  tr_spin
*/
/*!
**     @resumen
**          El dispositivo se dirije hacia adelante a el argumento 
**			ingresado
**     @param
**          dir     	   	- Dirección de giro
*/
/* ===================================================================*/
spi_st tr_diagonalSpin(int dir);


/*
** ===================================================================
**     Método      :  tr_getTilt
*/
/*!
**     @resumen
**          El dispositivo se dirije hacia adelante a el argumento 
**			ingresado
**     @param
**          dir     	   	- Dirección de giro
*/
/* ===================================================================*/
spi_st tr_diagonalSpin(int dir);

#endif