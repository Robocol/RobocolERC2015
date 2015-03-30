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
#include <string.h>
#include <linux/types.h>

/*--------------------------------------------------------------------------*/
/*
 *                 DIRECCIONES DE REGISTRO
 */

#include 	ABS_POS		0x01		//Posición Absoluta
#include 	EL_POS		0x02		//Posición Eléctrica: Paso y micropaso actual
#include 	MARK		0x03		//??
#include 	TVAL		0x09		//Corriente del regulador de torque en unidades de TCURR_U
#include 	T_FAST		0x0E		//TOFF_FAST|FALL_STEP 
#include 	TON_MIN		0x0F		//
#include 	AOFF_MIN	0x10		//
#include 	ADC_OUT 	0x12		//Valor del ADC
#include 	OCD_TH		0x13		//Corriente de overcurrent threshold en unidades de OCD_U
#include 	STEP_MODE	0x16		// 1|SYNC_SEL|1|STEP_SEL
#include 	ALARM_EN	0x17		//
#include	CONFIG		0x18		//
#include	STATUS		0x18		//

/*Máscaras de EL_POS*/
#include	STEP					0x07	//
#include	MICROSTEP				0x00	//

/*Máscaras de T_FAST*/
#include	TOFF_FAST				0x05	//
#include	FALL_STEP				0x00	//

/*Máscaras de STEP_MODE*/
#include	SYNC_SEL				0x05	//
#include	STEP_SEL				0x00	//

/*Máscaras de ALARM_EN*/
#include	OVCURRENT				0x00	//
#include	THERMAL_SHUTDOWN		0x01	//
#include	THERMAL_WARNING			0x02	//
#include	UNDERVOLTAGE			0x03	//
#include	SWITCH_TURNON			0x06	//
#include	NOT_PERFORMED_COMMAND	0x07	//

/*Máscaras de CONFIG*/
#include	TOFF					0x0A	//
#include	POW_SR					0x08	//
#include	OC_SD					0x07	//
#include	EN_TQREQ				0x05	//
#include	EXT_CLK					0x03	//
#include	OSC_SEL					0x00	//

/*Máscaras de STATUS*/
#include	OCD						0x0C	//
#include	TH_SD					0x0B	//
#include	TH_WRN					0x0A	//
#include	UVLO					0x09	//
#include	WRONG_CMD				0x08	//
#include	DIR						0x04	//
#include	HiZ						0x00	//

/*--------------------------------------------------------------------------*/
/*
 *                 VARIABLES DE LA LIBRERIA
 */	

typedef unsigned char stp_st;
typedef struct stp_device{
	uint8_t pin_cs;
	uint8_t pin_flag;
	uint8_t pin_stndby;
	uint8_t pin_stck;
	uint8_t pin_dir;
}stp_device;

struct stp_devices{
	stp_device* dev0;
	stp_device* dev1;
	stp_device* dev2;
}stp_device;

#endif