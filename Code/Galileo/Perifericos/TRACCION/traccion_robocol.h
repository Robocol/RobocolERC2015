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

/*
** ==================================================================================
** Estructura tr_dev:
**
**	Estructura de Tracción implementada para el manejo conjunto de dos motores
**	en proyecto ERC Robocol 2015.
**
**	Los elementos estipulados para la estructura se describen a continuación:
**
**	mv_state: Estado de movimiento de la tracción . Los estados manejados
**			  para esta variable son:
**				
** 				-TR_FORWARD: Moviendose hacia adelante
** 				-TR_BACKWARD: Moviendose hacia atras
** 				-TR_STEER: Girando
** 				-TR_STOPPED: Estado estacionario
**
** ctl_state: Para este caso se tienen dos estados esperados por defecto,
**			  heredados de las dos formas de controlar los motores 
**			  a través del driver descrito en ph_robocol.c:
**				-TR_AUTO: Estado en el cual la  velocidad y corriente de
**						  los motores estan siendo controladas por
**						  el controlador en cascada de los microcontroladores.
**			
**				-TR_MANUAL: Estado en el cual es solo modificable el 
**							PWM para el manejo de los motores. No se 
**							tiene habilitado el control de corriente
**							y velocidad de los microcontroladores.
**
** side: En el proyecto ERC Robocol 2015, se implementó
**		 una tracción con cuatro ruedas, un par a cada lado del armazón.
**		 Para el manejo de los motores de dichas ruedas se posicionaron
**		 dos tarjetas Intel Galileo , una de ellas para el par de ruedas
**		 de la izquierda del armazón y la otra para el par de ruedas de 
**		 la derecha. De este modo, se incluye una variable side en la 
**		 estructura que identifica en cual de los dos lados de
**		 la tracción se está declarando la misma.
**		 Los valores estipulados para esta variable se listan a continuación:
**			
**				-TR_LEFT_SIDE
**				-TR_RIGHT_SIDE
**
** type: Esta variable indica si la estructura de tracción para el manejo del
**		 par de motores tan solo ejecuta comandos enviados por la central 
**		 de control, TR_SLAVE, o en adición a ello los retransmite a la otra
**		 tarjeta Intel Galileo encargada del control de la tracción,TR_MASTER. 
**		 Así pues, los valores esperados para esta variable son_
**
**				-TR_MASTER
**				-TR_SLAVE
**
**			
** vel_pwm: Variable que indica el pwm(en modo TR_MANUAL) o velocidad (en modo TR_AUTO)
**			en el/la cual estan opernado los motores que controla esta librería.
**
** front_ph: Puntero a estructura de puente H (descrita en ph_robocol.c)
**			 que controla el motor delantero del lado especificado por side.
** back_ph:  Puntero a estructura de puente H (descrita en ph_robocol.c)
**			 que controla el motor trasero del lado especificado por side.
**
** device_built: Variable que indica si la estructura ya ha sido inicializada o no.
**				Los valores esperados para esta variables son:
**				-TR_BUILT
**				-TR_NOT_BUILT
*/
/*!
*/
/* ==================================================================================*/

typedef struct tr_dev{
	uint8_t mv_state;
	uint8_t ctl_state;
	uint8_t side;
	uint8_t type;
	uint8_t vel_pwm;
	ph_dev *front_ph;
	ph_dev *back_ph;
	uint8_t device_built;
} tr_dev;

#define TR_ERROR	0x01
#define TR_OK 		0x00


#define TR_BUILT 		0x01
#define TR_NOT_BUILT	0x00

/*--------------------------------------------------------------------------*/
/*
 *                  DEFINICIÓN DE TYPES
 */
#define TR_MASTER 		0x01
#define TR_SLAVE 		0x02

#define TR_LEFT_SIDE  	0x01
#define TR_RIGHT_SIDE 	0x02



/*--------------------------------------------------------------------------*/
/*
 *                  DEFINICIÓN DE STATES
 */

#define TR_FORWARD 		0x01
#define TR_BACKWARD 	0x02
#define TR_STEER 		0x04
#define TR_STOPPED 		0x08

#define TR_AUTO 		0x01
#define TR_MANUAL		0x02

#define TR_TURN_LEFT  	0x01
#define TR_TURN_RIGHT 	0x02

struct tr_dev tr_device;
//tr_device.device_built=TR_NOT_BUILT;

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
tr_st tr_build(uint8_t type,uint8_t side);

/*
** ===================================================================
**     Método      :  tr_forward
*/
/*!
**     @resumen
**          El dispositivo se dirije hacia adelante según el argumento 
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
**          El dispositivo se dirije hacia atrás según el argumento 
**			ingresadosado
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
**          El dispositivo gira sobre su propio eje en la dirección y
**			velocidad especificada por parámetro.
**     @param
**          dir     	   	- Dirección de giro
*/
/* ===================================================================*/
tr_st tr_spin(uint8_t dir, uint8_t arg);

/*
** ===================================================================
**     Método      :  tr_diffTurn
*/
/*!
**     @resumen
**          El dispositivo gira diferencialmente. Esto quiere decir que 
**			uno de los lados del vehículo continúa a su velocidad actual
**			mientras que el otro la disminuye según un valor ingresado por
**			parámetro. Si la velocidad resultante es negativa, el otro 
**			lado cambiará su dirección.
**     @param
**          dir     	   	- Dirección de giro
*/
/* ===================================================================*/
tr_st tr_diffTurn(uint8_t dir, uint8_t vp, uint32_t arg);

/*
** ===================================================================
**     Método      :  tr_diagonalDiffTurn
*/
/*!
**     @resumen
**          El dispositivo realiza un giro diferencial utilizando únicamente 
**			los motores de una de las diagonales del robot 
**     @param
**          dir     	   	- Dirección de giro
*/
/* ===================================================================*/
tr_st tr_diagonalDiffTurn(uint8_t dir, uint8_t vp, uint32_t arg);

/*
** ===================================================================
**     Método      :  tr_getTilt
*/
/*!
**     @resumen
**          Obtiene la inclinación relativa de rover leyendo el valor 
**			del ADC del balancín
**     @param
**          tilt     	   	- Puntero a la posición de memoria donde 
*/
/* ===================================================================*/
tr_st tr_getTilt(uint8_t dir);

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


/*
** ===================================================================
**     Método      :  tr_diaganostico
*/
/*!
**     @resumen
**			Método encargado de revisar conexión con los microcontroladores
**			responsables del manejo de los motores . En dado caso 
**			que estos dispositivos se encuentren on-line, se efectua
**			petición de variables de corriente, velocidad y temperatura.
**			La información obtenida por dicho proceso es guardada
** 			en el archivo con ruta especificada por parámetro.
*/
/* ===================================================================*/
tr_st tr_diagnostico(char *d_path);


/*
** ===================================================================
**     Método      :  tr_setCtlState
*/
/*!
**     @resumen
**			Método encargado de modificar el estado de operación
**			de la tracción a TR_AUTO o TR_MANUAL según indicado 
**			por parámetro.
*/
/* ===================================================================*/
tr_st tr_setCtlState(uint8_t state);

#endif