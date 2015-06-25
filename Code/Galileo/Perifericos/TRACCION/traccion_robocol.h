/*
 * --------------------------------------------------------------------------------------
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

#include "ph_robocol.h"

/*--------------------------------------------------------------------------*/
/*
 *                  DEFINICIÓN DE ESTRUCTURAS
 */
	typedef int tr_st;

	typedef struct {
		uint8_t mv_state;
		uint8_t ctl_state;
		uint8_t type;
		uint8_t vel_pwm;
		ph_dev *front_ph;
		ph_dev *back_ph;
		uint8_t device_built;
	} tr_dev;

#define TR_ERROR	0x01
#define TR_OK 		0x00


const uint8_t TR_BUILT =0x01;
const uint8_t TR_NOT_BUILT=0x00;
const uint8_t CW_DIR=1;
const uint8_t ANTI_CW_DIR=0;
const uint8_t UPPER_BMASK=0b11110000;
const uint8_t LOWER_BMASK=0b00001111;
/*--------------------------------------------------------------------------*/
/*
 *                  DEFINICIÓN DE TYPES
 */
const uint8_t TR_MASTER 	= 0x01;
const uint8_t TR_SLAVE 		= 0x02;
const uint8_t TR_LEFT_SIDE  = 0x04;
const uint8_t TR_RIGHT_SIDE = 0x08;

/*--------------------------------------------------------------------------*/
/*
 *                  DEFINICIÓN DE STATES
 */
const uint8_t TR_STOPPED 	= 0x01;
const uint8_t TR_FORWARD 	= 0x02;
const uint8_t TR_BACKWARD 	= 0x04;
const uint8_t TR_STEER 		= 0x08;
const uint8_t TR_AUTO 		= 0x10;
const uint8_t TR_MANUAL		= 0x20;



tr_dev	tr_device;
tr_device.device_built=TR_NOT_BUILT;

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
**          vp     	   	- Entero representando el PWM o la velocidad
**							a la que se desea que los motores retrocedan
*/
/* ===================================================================*/
tr_st tr_forward(uint8_t vp);

/*
** ===================================================================
**     Método      :  tr_backward
*/
/*!
**     @resumen
**          El dispositivo se dirije hacia adelante a el argumento 
**			ingresado
**     @param
**          vp     	   	- Entero representando el PWM o la velocidad
**							a la que se desea que los motores retrocedan
*/
/* ===================================================================*/
tr_st tr_backward(uint8_t vp);


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
tr_st tr_spin(uint8_t dir);

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
tr_st tr_diffTurn(uint8_t dir);

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
tr_st tr_Turn(uint8_t dir);

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
tr_st tr_diagonalSpin(uint8_t dir);


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
tr_st tr_diagonalSpin(uint8_t dir);

/*
** ===================================================================
**     Método      :  tr_eBrake
*/
/*!
**     @resumen
**			Freno de emergencia que activa el freno a VCC de los puentes
** 			H de la tracción.
*/
/* ===================================================================*/
tr_st tr_eBrake(void);

/*
** ===================================================================
**     Método      :  tr_setVP
*/
/*!
**     @resumen
**			Asignar pwm o velocidad objetivo según el estado de operación
**			de la tracción. Se asigna también a la estructura
**			global de tracción.
*/
/* ===================================================================*/
tr_st tr_setVP(uint8_t vp);




#endif