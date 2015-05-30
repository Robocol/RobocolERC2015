#include "arm_robocol.h"

arm_st arm_build(arm_dev* dev){
	ph_dev* b_motor=malloc(sizeof(ph_dev));
	(*b_motor).pin_cs=PINA2;
	ph_dev* b_actuator=malloc(sizeof(ph_dev));;
	
	ph_dev* u_actuator=malloc(sizeof(ph_dev));;
	stp_device* sup=malloc(sizeof(stp_device));
	stp_device* wrist=malloc(sizeof(stp_device));
	stp_device* claw=malloc(sizeof(stp_device));

	if (ph_build(b_motor)){
		printf("Error en la inicialización del puente h que maneja el motor inferior \n");
		return ARM_ERROR;
	}

	if (ph_build(b_actuator)){
		printf("Error en la inicialización del puente h que maneja el actuador inferior \n");
		return ARM_ERROR;
	}

	if (ph_build(u_actuator)){
		printf("Error en la inicialización del puente h que maneja el actuador superior \n");
		return ARM_ERROR;
	}

	if (stp_build(sup)){
		printf("Error en la inicialización del stepper que realiza la supinación\n");
		return ARM_ERROR
	}

	if (stp_build(wrist)){
		printf("Error en la inicialización del stepper que realiza el movimiento de la muñeca\n");
		return ARM_ERROR
	}

	if (stp_build(claw)){
		printf("Error en la inicialización del stepper que mueve la garra\n");
		return ARM_ERROR
	}



}