#include "arm_robocol.h"

arm_st arm_build(void){
	ph_dev* b_motor=malloc(sizeof(ph_dev));
	ph_dev* b_actuator=malloc(sizeof(ph_dev));
	ph_dev* u_actuator=malloc(sizeof(ph_dev));
	stp_device* sup=malloc(sizeof(stp_device));
	stp_device* wrist=malloc(sizeof(stp_device));
	stp_device* claw=malloc(sizeof(stp_device));

	(*b_motor).pin_cs=PINA2;
	(*b_motor).pin_in_a=PINE5;
	(*b_motor).pin_in_b=PINE6;
	(*b_motor).pin_enable=PINE7;
	(*b_motor).addr=EXP1;

	(*b_actuator).pin_cs=PINA0;
	(*b_actuator).pin_in_a=PINE0;
	(*b_actuator).pin_in_b=PINE1;
	(*b_actuator).pin_enable=PINE4;
	(*b_actuator).addr=EXP1;

	(*u_actuator).pin_cs=PINA1;
	(*u_actuator).pin_in_a=PINE2;
	(*u_actuator).pin_in_b=PINE3;
	(*u_actuator).pin_enable=PINE4;
	(*u_actuator).addr=EXP1;

	(*sup).pin_cs=PIN6;
	(*sup).pin_dir=PIN2;
	(*sup).pin_pwm=PIN3;
	(*sup).exp=EXP2;
	(*sup).pin_stndby=PINE0;
	(*sup).pin_flag=PINE1;
	
	(*wrist).pin_cs=PIN7;
	(*wrist).pin_dir=PIN4;
	(*wrist).pin_pwm=PIN3;
	(*wrist).exp=EXP2;
	(*wrist).pin_stndby=PINE2;
	(*wrist).pin_flag=PINE3;

	(*claw).pin_cs=PIN8;
	(*claw).pin_dir=PIN5;
	(*claw).pin_pwm=PIN3;
	(*claw).exp=EXP2;
	(*claw).pin_stndby=PINE4;
	(*claw).pin_flag=PINE5;

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

	(*dev).b_motor=b_motor;
	(*dev).b_actuator=b_actuator;
	(*dev).u_actuator=u_actuator;
	(*dev).sup=sup;
	(*dev).wrist=wrist;
	(*dev).claw=claw;

	return ARM_OK;
}

arm_st arm_ph_step(uint8_t ph_num,  uint8_t dir){

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
