#ifndef ARM_ROBOCOL
#define ARM_ROBOCOL
#include "ph_robocol.h"
#include "stepper_robocol.h"

#define ARM_OK 			0x00
#define ARM_ERROR 		0x01

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

struct arm_dev* dev=malloc(sizeof(arm_dev));

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


#endif