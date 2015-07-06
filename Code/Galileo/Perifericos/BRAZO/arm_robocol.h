#ifndef ARM_ROBOCOL_H
#define ARM_ROBOCOL_H
#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "ph_robocol.h"
#include "stepper_robocol.h"

#define ARM_OK 			0x00
#define ARM_ERROR 		0x01
#define BACTUATOR 		0x01
#define UACTUATOR 		0x02
#define BMOTOR 			0x03
#define SUP 			0x01
#define WRIST 			0x02
#define CLAW 			0x03

#define TIME_STEP 			0x03FF


typedef uint8_t arm_st;

typedef struct 
{
	ph_dev* b_motor;
	ph_dev* b_actuator;
	ph_dev* u_actuator;
	stp_device* sup;
	stp_device* wrist;
	stp_device* claw;
	
}arm_dev;

typedef struct 
{
	int8_t ang_motor;
	int8_t ang_b_actuator;
	int8_t ang_u_actuator;
	int8_t ang_sup;
	int8_t ang_wrist;
	int8_t ang_claw;
	
}arm_pos;

arm_dev* armdev;

arm_pos posicionA;
arm_pos posicionB;
arm_pos posicionC;
arm_pos posicionD;

/*
** ===================================================================
**     Método      :  arm_build
*/
/*!
**     @resumen
**          Inicializa y configura el brazo según valores ingresados por
**			parámetro.
*/
/* ===================================================================*/
arm_st arm_build(void);

/*
** ===================================================================
**     Método      :  arm_hand_step
*/
/*!
**     @resumen
**          Mueve uno de los motores de pasos durante un corto periodo
**		 	de tiempo.
**
**     @param
**          ph_num    	   	- Constante numérica que representa el
**							puente H a mover. Las constantes definidas
**							son ARM_UACTUATOR, ARM_BACTUATOR y ARM_BMOTOR.
*/
/* ===================================================================*/
arm_st arm_ph_step(uint8_t ph_num,uint8_t dir);



// arm_st arm_ph();

// arm_st arm_st();

/*
** ===================================================================
**     Método      :  arm_hand_step
*/
/*!
**     @resumen
**          Mueve uno de los motores de pasos durante un corto periodo
**		 	de tiempo.
**
**     @param
**          stp_num    	   	- Constante numérica que representa el
**							stepper a mover. Las constantes definidas
**							son ARM_SUP, ARM_WRIST y ARM_CLAW.
*/
/* ===================================================================*/
arm_st arm_hand_step(uint8_t stp_num);


arm_st arm_get_ph(uint8_t ph_num,ph_dev** dev);


arm_st arm_get_stp(uint8_t stp_num,stp_device** dev);

/*
** ===================================================================
**     Método      :  arm_moveBActToAngle
*/
/*!
**     @resumen
**          Mueve el actuador inferior a un ángulo absoluto ingresado 
**			por parámetro
**
**     @param
**          angle    	   	- Ángulo objetivo.
*/
/* ===================================================================*/
arm_st arm_moveBActToAngle( int8_t angle);


#endif