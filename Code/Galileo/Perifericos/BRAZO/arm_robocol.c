#include "arm_robocol.h"

arm_st arm_build(void){


	armdev=malloc(sizeof(arm_dev));

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

	// (*wrist).pin_cs=PIN6;
	// (*wrist).pin_dir=PIN2;
	// (*wrist).pin_pwm=3;
	// (*wrist).exp=EXP2;
	// (*wrist).gear_ratio=14.9;
	// (*wrist).step=2;
	// (*wrist).pin_stndby=PINE0;
	// (*wrist).pin_flag=PINE1;
	
	(*wrist).pin_cs=PIN7;
	(*wrist).pin_dir=PIN5;
	(*wrist).pin_stndby=PINE1;
	(*wrist).pin_flag=0;
	(*wrist).exp=EXP2;
	(*wrist).pin_pwm=3;
	//(*wrist).gear_ratio=14.9;
	(*wrist).gear_ratio=15;
	(*wrist).step=0;
	(*wrist).period=5000;

	// (*sup).pin_cs=PIN7;
	// (*sup).pin_dir=PIN4;
	// (*sup).pin_pwm=3;
	// (*sup).exp=EXP2;
	// (*sup).gear_ratio=14.9;
	// (*sup).step=2;
	// (*sup).pin_stndby=PINE2;
	// (*sup).pin_flag=PINE3;
	// (*sup).period=5000;


	(*sup).pin_cs=PIN7;
	(*sup).pin_dir=PIN4;
	(*sup).pin_pwm=3;
	(*sup).exp=EXP2;
	(*sup).gear_ratio=14.9;
	(*sup).step=0;
	(*sup).pin_stndby=PINE2;
	(*sup).pin_flag=PINE3;
	(*sup).period=5000;

	// (*claw).pin_cs=PIN8;
	// (*claw).pin_dir=PIN5;
	// (*claw).pin_pwm=3;
	// (*claw).exp=EXP2
	// (*claw).gear_ratio=14.9;
	// (*claw).step=2;	
	// (*claw).pin_stndby=PINE4;
	// (*claw).pin_flag=PINE5;
	// (*claw).period=5000;

	(*claw).pin_cs=PIN8;
	(*claw).pin_dir=PIN4;
	(*claw).pin_pwm=6;
	(*claw).exp=EXP2
	(*claw).gear_ratio=14.9;
	(*claw).step=0;	
	(*claw).pin_stndby=PINE3;
	(*claw).pin_flag=PINE5;
	(*claw).period=5000;


	posicionA.ang_motor=0;
	posicionA.ang_b_actuator=0;
	posicionA.ang_u_actuator=-30;
	posicionA.ang_sup=-30;
	posicionA.ang_wrist=0;
	posicionA.ang_claw=0;

	posicionB.ang_motor=0;
	posicionB.ang_b_actuator=-45;
	posicionB.ang_u_actuator=-45;
	posicionB.ang_sup=20;
	posicionB.ang_wrist=0;
	posicionB.ang_claw=0;

	posicionC.ang_motor=0;
	posicionC.ang_b_actuator=-45;
	posicionC.ang_u_actuator=-45;
	posicionC.ang_sup=-20;
	posicionC.ang_wrist=0;
	posicionC.ang_claw=0;

	posicionD.ang_motor=0;
	posicionD.ang_b_actuator=50;
	posicionD.ang_u_actuator=-35;
	posicionD.ang_sup=-15;
	posicionD.ang_wrist=0;
	posicionD.ang_claw=0;

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
		return ARM_ERROR;
	}

	if (stp_build(wrist)){
		printf("Error en la inicialización del stepper que realiza el movimiento de la muñeca\n");
		return ARM_ERROR;
	}

	if (stp_build(claw)){
		printf("Error en la inicialización del stepper que mueve la garra\n");
		return ARM_ERROR;
	}



	(*armdev).b_motor=b_motor;
	(*armdev).b_actuator=b_actuator;
	(*armdev).u_actuator=u_actuator;
	(*armdev).sup=sup;
	(*armdev).wrist=wrist;
	(*armdev).claw=claw;

	return ARM_OK;
}

arm_st arm_ph_step(uint8_t ph_num,  uint8_t dir){

	switch(ph_num){
		case 1:
			if (dir){
				ph_step((*armdev).b_actuator,64,dir);
			}else{
				ph_step((*armdev).b_actuator,32,dir);
			}
			break;
		case 2:
			if (dir){
				ph_step((*armdev).u_actuator,64,dir);
			}else{
				ph_step((*armdev).u_actuator,32,dir);
			}
			break;
		case 3:
			ph_step((*armdev).b_motor,64,dir);
			break;
		default:
			printf("El número de puente h ingresado por parámetro no es válido.\n");
			return ARM_ERROR;
	}
	return ARM_OK;
}


arm_st validate_stepper(uint8_t stp_num){
	if (stp_num!=1 && stp_num!=2 && stp_num!=3 ){
		return ARM_ERROR;
	}
	return ARM_OK;
}

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
**          stepper    	   	- Constante numérica que representa el
**							stepper a mover. Las constantes definidas
**							son SUP, WRIST y CLAW.
*/
/* ===================================================================*/
arm_st arm_hand_step(uint8_t stepper){
	switch(stepper){
		case 1:
			if(stp_moveTime((*armdev).sup,TIME_STEP*1000)){
				return ARM_ERROR;
				break;
			}
		case 2:
			if(stp_moveTime((*armdev).sup,TIME_STEP*1000)){
				return ARM_ERROR;
				break;
			}
		case 3:
			if(stp_moveTime((*armdev).sup,TIME_STEP*1000)){
				return ARM_ERROR;
				break;
			}
		default:
			printf("El número de puente h ingresado por parámetro no es válido.\n");
			return ARM_ERROR;
	}
	return ARM_OK;
}

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
**          stepper    	   	- Constante numérica que representa el
**							stepper a mover. Las constantes definidas
**							son SUP, WRIST y CLAW.
*/
/* ===================================================================*/

arm_st arm_get_ph(uint8_t ph_num,ph_dev** dev){
	switch(ph_num){
		case BACTUATOR:
			*dev=(*armdev).b_actuator;
			return ARM_OK;
		case UACTUATOR:
			*dev=(*armdev).u_actuator;
			return ARM_OK;
		case BMOTOR:
			*dev=(*armdev).b_motor;
			return ARM_OK;
		default:
			printf("El número de dispositivo puente h ingresado por parámetro no es válido.\n");
			return ARM_ERROR;
	}
}


arm_st arm_get_stp(uint8_t stp_num,stp_device** dev){

	printf("stndby DENTRO DEL MÉTODO: %d\n",(*(*armdev).sup).pin_stndby);
	switch(stp_num){
		case SUP:
			*dev=(*armdev).sup;
			return ARM_OK;
		case WRIST:
			*dev=(*armdev).wrist;
			return ARM_OK;
		case CLAW:
			*dev=(*armdev).claw;
			return ARM_OK;
		default:
			printf("El número de dispositivo stepper ingresado por parámetro no es válido.\n");
			return ARM_ERROR;
	}
}

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
arm_st arm_moveBActToAngle(int8_t angle){
	uint8_t curr_pos,goal_pos;

	if(ph_getVelocidad((*armdev).b_actuator, &curr_pos)){
		printf("Error al obtener posición. (ph_robocol.c -> ph_move_to_angle)\n");
		return PH_ERROR;
	}

	sleep(1);

	goal_pos=(int)(angle/0.84236 + (134.62112/0.84236));

	printf("Goal Pos B%d\n",goal_pos );

	if(ph_setVel((*armdev).b_actuator, goal_pos)){
		printf("Error al definir posición objetivo (ph_robocol.c -> ph_move_to_angle)\n");
		perror("Descripción:");
		return PH_ERROR;
	}

	sleep(1);

	if((curr_pos-goal_pos)<0){
		printf("SUBIENDO\n");
		if(ph_setDireccion((*armdev).b_actuator, 1)){
			printf("Error al definir dirección. (ph_robocol.c -> ph_move_to_angle\n)");
			return PH_ERROR;
		}
	}else{
		printf("BAJAR\n");
		if(ph_setDireccion((*armdev).b_actuator,0)){
			printf("Error al definir dirección. (ph_robocol.c -> ph_move_to_angle\n)");
			return PH_ERROR;
		}
	}


	return ARM_OK;
}

/*
** ===================================================================
**     Método      :  arm_moveUActToAngle
*/
/*!
**     @resumen
**          Mueve el actuador superior a un ángulo absoluto ingresado 
**			por parámetro
**
**     @param
**          angle    	   	- Ángulo objetivo.
*/
/* ===================================================================*/
arm_st arm_moveUActToAngle(int8_t angle){
	uint8_t curr_pos,goal_pos;

	if(ph_getVelocidad((*armdev).u_actuator, &curr_pos)){
		printf("Error al obtener posición. (ph_robocol.c -> ph_move_to_angle)\n");
		return PH_ERROR;
	}

	usleep(100000);

	goal_pos=(int)(angle/0.9569325 + (162.298373/0.9569325));

	printf("Goal Pos U%d\n",goal_pos );
	printf("Curr Pos U%d\n",curr_pos );

	if(ph_setVel((*armdev).u_actuator, goal_pos)){
		printf("Error al definir posición objetivo (ph_robocol.c -> ph_move_to_angle)\n");
		perror("Descripción:");
		return ARM_ERROR;
	}

	usleep(100000);

	if((curr_pos-goal_pos)<0){
		printf("SUBIENDO\n");
		if(ph_setDireccion((*armdev).u_actuator, 1)){
			printf("Error al definir dirección. (ph_robocol.c -> ph_move_to_angle\n)");
			return ARM_ERROR;
		}
	}else{
		printf("BAJAR\n");
		if(ph_setDireccion((*armdev).u_actuator,0)){
			printf("Error al definir dirección. (ph_robocol.c -> ph_move_to_angle\n)");
			return ARM_ERROR;
		}
	}


	return ARM_OK;

}

/*
** ===================================================================
**     Método      :  arm_moveSuptToAngle
*/
/*!
**     @resumen
**          Mueve el stepper supinador a un ángulo absoluto ingresado 
**			por parámetro
**
**     @param
**          angle    	   	- Ángulo objetivo.
*/
/* ===================================================================*/
arm_st arm_moveSupToAngle(int8_t angle){
	//return stp_moveToAngle((*armdev).sup,angle);

	int32_t 	curr_ang,degrees;
	int32_t		t;
	stp_device* dev=(*armdev).sup;
	uint8_t 	step=((*dev).step>4)? 4 : (*dev).step;
	uint32_t	goal_pos=(int)(degrees*pow(2,step)*((*dev).gear_ratio)/1.8);


	if(stp_getAngularPosition(dev,&curr_ang)){
		printf("Error getting current position. (stepper_robocol.c>stp_relative_displacement)\n");
		return STP_ERROR;
	}
	
	degrees=angle-curr_ang;

	if(degrees<0){
		if(stp_dir(dev,0)){
			printf("Error getting current position. (stepper_robocol.c>stp_relative_displacement)\n");
			return STP_ERROR;
		}
	}else{
		if(stp_dir(dev,1)){
			printf("Error getting current position. (stepper_robocol.c>stp_relative_displacement)\n");
			return STP_ERROR;
		}
	}

	degrees=abs(degrees);

	goal_pos=(int)(degrees*pow(2,step)*((*dev).gear_ratio)/1.8);
	stp_clk_disable(dev);



	t=goal_pos*(*dev).period;

	stp_output_enable(dev);
	stp_clk_enable(dev);


	

	pid_t pid;
	printf("alarm application starting\n");
	pid = fork();
	switch(pid) {
		case -1:
		/* Failure */
		perror("fork failed");
		exit(1);
		case 0:
		/* child */
		usleep(t);
		kill(getppid(), SIGALRM);
		exit(0);
	}

	/* if we get here we are the parent process */
	return ARM_OK;

}

/*
** ===================================================================
**     Método      :  arm_moveToPos
*/
/*!
**     @resumen
**          Muevo el brazo a la posición especificada
**
*/
/* ===================================================================*/
arm_st arm_moveToPos(arm_pos posicion, uint8_t delay){

	// if(arm_moveMotorToAngle(posicion.ang_b_actuator)){
	// 	printf("Error llevando el BActuator a la posición\n");
	// }
	i2c_chaddr((*(*armdev).b_actuator).addr);

	arm_st st=ARM_OK;
	if(arm_moveBActToAngle(posicion.ang_b_actuator)){
		printf("Error llevando el BActuator a la posición\n");
		st=ARM_ERROR;
	}

	sleep(delay);

	if(arm_moveUActToAngle(posicion.ang_u_actuator)){
		printf("Error llevando el UActuator a la posición\n");
		st=ARM_ERROR;
	}

	i2c_chaddr((*(*armdev).sup).exp);

	if(arm_moveSupToAngle(posicion.ang_sup)){
		printf("Error llevando el Supinador a la posición\n");
		st=ARM_ERROR;
	}

	// if(arm_moveWristToAngle(posicion.ang_sup)){
	// 	printf("Error llevando el Supinador a la posición\n");
	//	st=ARM_ERROR;
	// }

	// if(arm_moveClawToAngle(posicion.ang_sup)){
	// 	printf("Error llevando el Supinador a la posición\n");
	//	st=ARM_ERROR;
	// }

	return st;
}


