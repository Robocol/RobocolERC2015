/*
 * stepper_robocol.h
 *
 *  Created on: 	Feb-Mar  2015
 *  Authors:		Daniel Ochoa
 * 					Germán Giraldo    
 */

#ifndef  STEPPER_ROBOCOL_H
#define STEPPER_ROBOCOL_H

/*--------------------------------------------------------------------------*/
/*
 *                 INCLUSIÓN DE LIBRERIAS
 */
#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include <errno.h>
#include <linux/types.h>
#include "gpio_robocol.h"
#include "spi_robocol.h"
#include "pwmg_robocol.h"
#include "misc_robocol.h"

/*--------------------------------------------------------------------------*/
/*
 *                 DIRECCIONES DE REGISTRO
 */

#define 	ABS_POS		0x01		//Posición Absoluta
#define 	EL_POS		0x02		//Posición Eléctrica: Paso y micropaso actual
#define 	MARK		0x03		//??
#define 	TVAL		0x09		//Corriente del regulador de torque en unidades de TCURR_U
#define 	T_FAST		0x0E		//TOFF_FAST|FALL_STEP 
#define 	TON_MIN		0x0F		//
#define 	AOFF_MIN	0x10		//
#define 	ADC_OUT 	0x12		//Valor del ADC
#define 	OCD_TH		0x13		//Corriente de overcurrent threshold en unidades de OCD_U
#define 	STEP_MODE	0x16		// 1|SYNC_SEL|1|STEP_SEL
#define 	ALARM_EN	0x17		//
#define		CONFIG		0x18		//
#define		STATUS		0xD0		//

#define		ENABLE		0xA8
#define		ENABLE		0xB8

/*Máscaras de EL_POS*/
#define	STEP					0x07	//
#define	MICROSTEP				0x00	//

/*Máscaras de T_FAST*/
#define	TOFF_FAST				0x05	//
#define	FALL_STEP				0x00	//

/*Máscaras de STEP_MODE*/
#define	SYNC_SEL				0x05	//
#define	STEP_SEL				0x00	//

/*Máscaras de ALARM_EN*/
#define	OVCURRENT				0x00	//
#define	THERMAL_SHUTDOWN		0x01	//
#define	THERMAL_WARNING			0x02	//
#define	UNDERVOLTAGE			0x03	//
#define	SWITCH_TURNON			0x06	//
#define	NOT_PERFORMED_COMMAND	0x07	//

/*Máscaras de CONFIG*/
#define	TOFF					0x0A	//
#define	POW_SR					0x08	//
#define	OC_SD					0x07	//
#define	EN_TQREQ				0x05	//
#define	EXT_CLK					0x03	//
#define	OSC_SEL					0x00	//

/*Máscaras de STATUS*/
#define	OCD						0x0C	//
#define	TH_SD					0x0B	//
#define	TH_WRN					0x0A	//
#define	UVLO					0x09	//
#define	WRONG_CMD				0x08	//
#define	DIR						0x04	//
#define	HiZ						0x00	//

 /*Duty Cycle PWM*/
 #define DUTY_CYCLE 			0x32 	//

/*--------------------------------------------------------------------------*/
/*
 *                   TYPES DE LA LIBRERIA
 */	
typedef unsigned char stp_st;
typedef struct stp_device{
	uint8_t pin_cs;
	uint8_t pin_dir;
	uint8_t pin_stndby;
	uint8_t pin_flag;
	spi_device* spi;
	uint8_t pin_pwm;
	uint32_t period;
}stp_device;

// MODOS

// velocidad constante
// Ir a Posición Absoluta
// Ir a posiciíon relativa


/*--------------------------------------------------------------------------*/
/*
 *                 VARIABLES DE LA LIBRERIA
 */	


/*======================================================================*/
/*								GETTERS									*/
/*======================================================================*/

/*
** ===================================================================
**     Método      :  stp_build
*/
/*!
**     @resumen
**          Contruye el dispositivo. Es necesario llamar este método 
**			antes de usar el motor
**     @param
**          dev     	   	- Puntero al dispositivo sobre el que recae la 
**							acción.
**     @return
**                         	- Estado salida del método. 
*/
/* ===================================================================*/
stp_st stp_build(stp_device* dev);

/*
** ===================================================================
**     Método      :  stp_setParam
*/
/*!
**     @resumen
**          Asigna un valor especificado por parámetro a uno de los parámetros
**			de operación del stepper
**     @param
**          dev     	   	- Puntero al dispositivo sobre el que recae la 
**							acción.
**     @param
**          param     	   	- Indicador del parámetro de operación a modificar.
**     @param
**          buff     	   	- Valor a asignar al parámetro especificado.
**     @param
**          len     	   	- Largo en bytes de buff.
**     @return
**                         	- Estado salida del método. 
*/
/* ===================================================================*/
stp_st stp_setParam(stp_device* dev, uint8_t param, uint8_t* buff,uint8_t len);

/*
** ===================================================================
**     Método      :  stp_getParam
*/
/*!
**     @resumen
**          Obtener el valor actual del parámetro especificado
**
**     @param
**          dev     	   	- Puntero al dispositivo sobre el que recae la 
**							acción.
**     @param
**          param     	   	- Indicador del parámetro de operación a modificar.
**     @param
**          buff     	   	- Puntero a un arreglo en el cual se almacenará
**							la información del parámetro.
**     @param
**          len     	   	- Largo en bytes de buff.
**
**     @return
**                         	- Estado salida del método. 
*/
/* ===================================================================*/
stp_st stp_getParam(stp_device* dev, uint8_t param, uint8_t* buff,uint8_t len);

/*
** ===================================================================
**     Método      :  stp_getPosition
*/
/*!
**     @resumen
**          Obtiente la posición actual absoluta del stepper
**
**     @param
**          dev     	   	- Puntero al dispositivo sobre el que recae 
**							la acción.
**     @param
**          pos     	   	- Puntero a la posición de memoria en la cual
**							se almacenará la información del parámetro.
**
**     @return
**                         	- Estado salida del método. 
*/
/* ===================================================================*/
stp_st stp_getPosition(stp_device* dev, int32_t* pos);

/*
** ===================================================================
**     Método      :  stp_getTVal
*/
/*!
**     @resumen
**          Obtiene el valor de corriente asignado al regulador de torque
**
**     @param
**          dev     	   	- Puntero al dispositivo sobre el que recae la 
**							acción.
**     @param
**          tval     	   	- Puntero a un arreglo en el cual se almacenará
**							la información del parámetro.
**
**     @return
**                         	- Estado salida del método. 
*/
/* ===================================================================*/
stp_st stp_getTVAL(stp_device* dev, int8_t* tval);

/*
** ===================================================================
**     Método      :  stp_getTOffFast
*/
/*!
**     @resumen
**          Obtiene el tiempo máximo de apagado rápido actual (Auto 
**			adjusted decay)
**
**     @param
**          dev     	   	- Puntero al dispositivo sobre el que recae 
**							la acción.
**     @param
**          pos     	   	- Puntero a la posición de memoria en la cual
**							se almacenará la información del parámetro.
**
**     @return
**                         	- Estado salida del método. 
*/
/* ===================================================================*/
stp_st stp_getTOffFast(stp_device* dev, int8_t* tof);

/*
** ===================================================================
**     Método      :  stp_getFallStep
*/
/*!
**     @resumen
**          Obtiene el tiempo mínimo de apagado actual (Auto adjusted decay)
**
**     @param
**          dev     	   	- Puntero al dispositivo sobre el que recae 
**							la acción.
**     @param
**          pos     	   	- Puntero a la posición de memoria en la cual
**							se almacenará la información del parámetro.
**
**     @return
**                         	- Estado salida del método. 
*/
/* ===================================================================*/
stp_st stp_getFallStep(stp_device* dev, int8_t* ft);
/*
** ===================================================================
**     Método      :  stp_getTOnMin
*/
/*!
**     @resumen
**          Obtiene el tiempo de encendido mínimo del control de corriente
**
**     @param
**          dev     	   	- Puntero al dispositivo sobre el que recae 
**							la acción.
**     @param
**          tonm     	   	- Puntero a la posición de memoria en la cual
**							se almacenará la información del parámetro.
**
**     @return
**                         	- Estado salida del método. 
*/
/* ===================================================================*/
stp_st stp_getTONMIN(stp_device* dev, int8_t* tonm);
/*
** ===================================================================
**     Método      :  stp_getTOffMin
*/
/*!
**     @resumen
**          Obtiene el tiempo de apagado mínimo del control de corriente
**
**     @param
**          dev     	   	- Puntero al dispositivo sobre el que recae 
**							la acción.
**     @param
**          toffm     	   	- Puntero a la posición de memoria en la cual
**							se almacenará la información del parámetro.
**
**     @return
**                         	- Estado salida del método. 
*/
/* ===================================================================*/
stp_st stp_getTOffMin(stp_device* dev, int8_t* toffm);

/*
** ===================================================================
**     Método      :  stp_getADC
*/
/*!
**     @resumen
**          Obtiene valor registrado por el ADC
**
**     @param
**          dev     	   	- Puntero al dispositivo sobre el que recae 
**							la acción.
**     @param
**          adc     	   	- Puntero a la posición de memoria en la cual
**							se almacenará la información del parámetro.
**
**     @return
**                         	- Estado salida del método. 
*/
/* ===================================================================*/
stp_st stp_getADC(stp_device* dev, int8_t* adc);

/*
** ===================================================================
**     Método      :  stp_getOCDT
*/
/*!
**     @resumen
**          Obtiene valor del OverCurrent Detection threshold
**
**     @param
**          dev     	   	- Puntero al dispositivo sobre el que recae 
**							la acción.
**     @param
**          ocdt     	   	- Puntero a la posición de memoria en la cual
**							se almacenará la información del parámetro.
**
**     @return
**                         	- Estado salida del método. 
*/
/* ===================================================================*/
stp_st stp_getOCDT(stp_device* dev, int8_t* ocdt);

/*
** ===================================================================
**     Método      :  stp_getStepSel
*/
/*!
**     @resumen
**          Obtiene valor del selector de paso.
**
**     @param
**          dev     	   	- Puntero al dispositivo sobre el que recae 
**							la acción.
**     @param
**          step_sel     	   	- Puntero a la posición de memoria en la cual
**							se almacenará la información del parámetro.
**
**     @return
**                         	- Estado salida del método. 
*/
/* ===================================================================*/
stp_st stp_getStepSel(stp_device* dev, int8_t* step_sel);


/*
** ===================================================================
**     Método      :  stp_getAlarmEn
*/
/*!
**     @resumen
**          Obtiene el registro de alarmas activadas
**
**     @param
**          dev     	   	- Puntero al dispositivo sobre el que recae 
**							la acción.
**     @param
**          al_en	   	- Puntero a la posición de memoria en la cual
**							se almacenará la información del parámetro.
**
**     @return
**                         	- Estado salida del método. 
*/
/* ===================================================================*/
stp_st stp_getAlarmEn(stp_device* dev, int8_t* al_en);

/*
** ===================================================================
**     Método      :  stp_getConfig
*/
/*!
**     @resumen
**          Obtiene valor del registro de configuración
**
**     @param
**          dev     	   	- Puntero al dispositivo sobre el que recae 
**							la acción.
**     @param
**          config	   	- Puntero a la posición de memoria en la cual
**							se almacenará la información del parámetro.
**
**     @return
**                         	- Estado salida del método. 
*/
/* ===================================================================*/
stp_st stp_getConfig(stp_device* dev, int32_t* osc_sel);

/*
** ===================================================================
**     Método      :  stp_getStatus
*/
/*!
**     @resumen
**          Obtiene valor del registro de configuración
**
**     @param
**          dev     	   	- Puntero al dispositivo sobre el que recae 
**							la acción.
**     @param
**          config	   	- Puntero a la posición de memoria en la cual
**							se almacenará la información del parámetro.
**
**     @return
**                         	- Estado salida del método. 
*/
/* ===================================================================*/
stp_st stp_getStatus(stp_device* dev, int32_t* stats);
















/*======================================================================*/
/*								SETTERS									*/
/*======================================================================*/




/*
** ===================================================================
**     Método      :  stp_setPosition
*/
/*!
**     @resumen
**          asigna la posición actual absoluta del stepper
**
**     @param
**          dev     	   	- Puntero al dispositivo sobre el que recae 
**							la acción.
**     @param
**          pos     	   	- Puntero a la posición de memoria en la cual
**							se almacenará la información del parámetro.
**
**     @return
**                         	- Estado salida del método. 
*/
/* ===================================================================*/
stp_st stp_setPosition(stp_device* dev, int32_t* pos);


/*
** ===================================================================
**     Método      :  stp_setTVal
*/
/*!
**     @resumen
**          Asigna valor de corriente al regulador de torque
**
**     @param
**          dev     	   	- Puntero al dispositivo sobre el que recae la 
**							acción.
**     @param
**          tval     	   	- Puntero a un arreglo en el cual se almacenará
**							la información del parámetro.
**
**     @return
**                         	- Estado salida del método. 
*/
/* ===================================================================*/
stp_st stp_setTVAL(stp_device* dev, int8_t* tval);

/*
** ===================================================================
**     Método      :  stp_setTOnMin
*/
/*!
**     @resumen
**          Asigna el tiempo de encendido mínimo del control de corriente
**
**     @param
**          dev     	   	- Puntero al dispositivo sobre el que recae 
**							la acción.
**     @param
**          tonm     	   	- Puntero a la posición de memoria en la cual
**							se almacenará la información del parámetro.
**
**     @return
**                         	- Estado salida del método. 
*/
/* ===================================================================*/
stp_st stp_setTONMIN(stp_device* dev, int8_t* tonm);

/*
** ===================================================================
**     Método      :  stp_setTOffMin
*/
/*!
**     @resumen
**          Asigna el tiempo de apagado mínimo del control de corriente
**
**     @param
**          dev     	   	- Puntero al dispositivo sobre el que recae 
**							la acción.
**     @param
**          toffm     	   	- Puntero a la posición de memoria en la cual
**							se almacenará la información del parámetro.
**
**     @return
**                         	- Estado salida del método. 
*/
/* ===================================================================*/
stp_st stp_setTOffMin(stp_device* dev, int8_t* toffm);
/*
** ===================================================================
**     Método      :  stp_setOCDT
*/
/*!
**     @resumen
**          Asigna valor del OverCurrent Detection Threshold
**
**     @param
**          dev     	   	- Puntero al dispositivo sobre el que recae 
**							la acción.
**     @param
**          ocdt     	   	- Puntero a la posición de memoria en la cual
**							se almacenará la información del parámetro.
**
**     @return
**                         	- Estado salida del método. 
*/
/* ===================================================================*/
stp_st stp_setOCDT(stp_device* dev, int8_t* ocdt);
/*
** ===================================================================
**     Método      :  stp_setStepSel
*/
/*!
**     @resumen
**          Asigna valor del selector de paso.
**
**     @param
**          dev     	   	- Puntero al dispositivo sobre el que recae 
**							la acción.
**     @param
**          step_sel     	   	- Puntero a la posición de memoria en la cual
**							se almacenará la información del parámetro.
**
**     @return
**                         	- Estado salida del método. 
*/
/* ===================================================================*/
stp_st stp_setStepSel(stp_device* dev, int8_t* step_sel);

/*
** ===================================================================
**     Método      :  stp_setAlarmEn
*/
/*!
**     @resumen
**          Asigna valor al registro de alarmas activadas
**
**     @param
**          dev     	   	- Puntero al dispositivo sobre el que recae 
**							la acción.
**     @param
**          al_en	   	- Puntero a la posición de memoria en la cual
**							se almacenará la información del parámetro.
**
**     @return
**                         	- Estado salida del método. 
*/
/* ===================================================================*/
stp_st stp_setAlarmEn(stp_device* dev, int8_t* al_en);

/*
** ===================================================================
**     Método      :  stp_setConfig
*/
/*!
**     @resumen
**          Asigna valor del registro de configuración
**
**     @param
**          dev     	   	- Puntero al dispositivo sobre el que recae 
**							la acción.
**     @param
**          config	   	- Puntero a la posición de memoria en la cual
**							se almacenará la información del parámetro.
**
**     @return
**                         	- Estado salida del método. 
*/
/* ===================================================================*/
stp_st stp_setConfig(stp_device* dev, int32_t* config);

/*
** ===================================================================
**     Método      :  stp_enable
*/
/*!
**     @resumen
**          Habilita el dispositivo especificado por parámetro
**
**     @param
**          dev     	   	- Puntero al dispositivo sobre el que recae 
**							la acción.
*/
/* ===================================================================*/
stp_st stp_enable(stp_device* dev);

/*
** ===================================================================
**     Método      :  stp_disable
*/
/*!
**     @resumen
**          Deshabilita el dispositivo especificado por parámetro
**
**     @param
**          dev     	   	- Puntero al dispositivo sobre el que recae 
**							la acción.
*/
/* ===================================================================*/
stp_st stp_disable(stp_device* dev);





#endif