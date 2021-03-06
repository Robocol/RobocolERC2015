/*
 * --------------------------------------------------------------------------------------
 * ph_robocol.c (0.9)
 *
 *  Created on: Mar 12, 2015
 *
 *      Authors:	Daniel Ochoa
 *					Germán Giraldo
 * -------------------------------------------------------------------------------------
 */
#ifndef	PH_ROBOCOL_H
#define	PH_ROBOCOL_H

/*
** =====================================================================================
**     USO DEL DRIVER
*/
/*!
**     @resumen
**          Este driver facilita el manejo del puente H VHN5019A a través de el 
**			microcontrolador ATTINY84A. Las funciones aquí descritas son compatibles con
**			la versión 1.0 del software controlador de puente H para ATTINY y para su 
**			operación desde las tarjetas Intel Galileo.
**
**			La librería basa su funcionamiento en la estructura de datos ph_dev. Esta 
**			estructura permite definir las características de un puente H particular 
**			conectado al sistema. De esta manera, un mismo programa puede controlar múltiples
**			puentes H mediante la creación de múltiples estructuras ph_dev asociadas.
**
**			También es necesario estar familiarizado con el driver spi_robocol.h, ya que
**			algunos elementos de este deben ser manejados por el usuario.
**
*/
/* =====================================================================================*/

#include <time.h>
#include "spi_robocol.h"
#include "gpio_robocol.h"

/*--------------------------------------------------------------------------*/
/*
 *                  	HEADERS DE FUNCIONES
 */
#define MEDIR_CORRIENTE 0x01
#define MEDIR_VELOCIDAD 0x02
#define CAMBIAR_KP_V 	0x03
#define CAMBIAR_KI_V 	0x04
#define CAMBIAR_KP_C 	0x05
#define CAMBIAR_KI_C 	0x06
#define CAMBIAR_ESTADO 	0x07
#define SP_CORRIENTE 	0x08
#define SP_VELOCIDAD 	0x09
#define CAMBIAR_PWM 	0x0A
#define DAR_ESTADO		0x0B
#define DAR_TEMPERATURA	0x0C

/*--------------------------------------------------------------------------*/
/*
 *                  	ESTADO DE COMUNICACIÓN
 */
#define ACK_galileo 	0xAA
#define ACK_tiny 		0xBB
#define ERROR_tiny 		0x11

/*--------------------------------------------------------------------------*/
/*
 *                  	ESTADOS DE OPERACIÓN
 */
#define AUTO_EST 		0x10
#define MANUAL_EST 		0x20
#define FAIL_EST 		0x30
/*--------------------------------------------------------------------------*/
/*
 *                  CONSTANTES DE DEBUG
 */

#define PH_OK 			0x00
#define PH_ERROR 		0xFF

/*--------------------------------------------------------------------------*/
/*
 *                  DEFINICIÓN DE TYPES
 */
typedef int ph_st;

typedef struct{
	spi_device* spi;		//
	uint8_t	pin_in_a;
	uint8_t	pin_in_b;
	uint8_t	pin_enable;
}ph_dev;

/*--------------------------------------------------------------------------*/
/*
 *                  	FUNCIONES DE LIBRERÍA
 */

/*
** ===================================================================
**     Método      :  ph_enable
*/
/*!
**     @resumen
**          Modifica el ciclo útil del PWM que controla la velocidad del
**			motor.
**     @param
**          dev     	   	- Puntero al dispositivo sobre el que recae la 
**							acción.
**     @return
**                         	- Estado salida del método. 
*/
/* ===================================================================*/
ph_st ph_enable(ph_dev* dev);
ph_st ph_disable(ph_dev* dev);

/*
** ===================================================================
**     Método      :  ph_setPWM
*/
/*!
**     @resumen
**          Modifica el ciclo útil del PWM que controla la velocidad del
**			motor.
**     @param
**          dev     	   	- Puntero al dispositivo sobre el que recae la 
**							acción.
**     @param
**          pwm     	   	- Valor del ciclo útil del PWM:
**								0 	-> 0% ciclo útil
**								255 -> 100% ciclo útil
**     @return
**                         	- Estado salida del método. 
*/
/* ===================================================================*/
ph_st ph_setPWM(ph_dev* dev,uint8_t pwm);

/*
** ===================================================================
**     Método      :  ph_setDireccion
*/
/*!
**     @resumen
**          Modifica la dirección de giro del motor
**     @param
**          dev     	   	- Puntero al dispositivo sobre el que recae la 
**							acción.
**     @param
**          dir     	   	- Indicador de la dirección de giro:
**								0 ->	
**								1 ->	
**     @return
**                         	- Estado salida del método. 
*/
/* ===================================================================*/
ph_st ph_setDireccion(ph_dev* dev, uint8_t dir);

/*
** ===================================================================
**     Método      :  ph_setEstado
*/
/*!
**     @resumen
**          Modifica el estado de operación del puente H
**     @param
**          dev     	   	- Puntero al dispositivo sobre el que recae la 
**							acción.
**     @param
**          est     	   	- Identificador del estado al que se pasará:
**								16 -> AUTO_EST
**								32 -> MANUAL_EST
**								48 -> FAIL_EST
**     @return
**                         	- Estado salida del método. 
*/
/* ===================================================================*/
ph_st ph_setEstado(ph_dev* dev,uint8_t est);


/*
** ===================================================================
**     Método      :  ph_setKPV
*/
/*!
**     @resumen
**          Modifica la ganancia proporcional del controlador PI de 
**			velocidad
**     @param
**          dev     	   	- Puntero al dispositivo sobre el que recae la 
**							acción.
**     @param
**          est     	   	- Identificador del estado al que se pasará.
**								Valor entre 0 y 255.
**     @return
**                         	- Estado salida del método. 
*/
/* ===================================================================*/
ph_st ph_setKPV(ph_dev* dev,uint8_t kpv);


ph_st ph_setKIV(ph_dev* dev,uint8_t kiv);


ph_st ph_setKPC(ph_dev* dev,uint8_t kpc);


ph_st ph_setKIC(ph_dev* dev,uint8_t kic);


ph_st ph_setVel(ph_dev* dev,uint8_t vel);


ph_st ph_getVelocidad(ph_dev* dev,uint8_t* vel);


ph_st ph_getCorriente(ph_dev* dev,uint8_t* corr);


ph_st ph_getEstado(ph_dev* dev,uint8_t* est);


ph_st ph_getTemperatura(ph_dev* dev,uint8_t* temp);


#endif