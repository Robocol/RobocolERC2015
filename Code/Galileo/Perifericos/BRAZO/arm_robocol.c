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

	return ARM_OK;
}

arm_st arm_ph_step(arm_dev* dev, uint8_t ph_num,  uint8_t dir){
	if(validate_ph(ph_num)){
		printf("El número de puente h ingresado por parámetro no es válido.\n");
		return ARM_ERROR;
	}

	switch(ph_num){
		case 1:
			if (dir){
				ph_step((*dev).b_actuator,64,dir);
			}else{
				ph_step((*dev).b_actuator,32,dir);
			}
			break;
		case 2:
			if (dir){
				ph_step((*dev).u_actuator,64,dir);
			}else{
				ph_step((*dev).u_actuator,32,dir);
			}
			break;
		case 3:
			ph_step((*dev).b_motor,64,dir);
			break;
		default:
			printf("El número de puente h ingresado por parámetro no es válido.\n");
			return ARM_ERROR;
	}
	return ARM_OK;
}

arm_st validate_ph(uint8_t ph_num){
	if (ph_num!=1 && ph_num!=2 && ph_num!=3 ){
		return ARM_ERROR;
	}
	return ARM_OK;
}