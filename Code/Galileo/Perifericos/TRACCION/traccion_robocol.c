#include "traccion_robocol.h"

tr_st tr_build(uint8_t type,uint8_t side){
	uint8_t master;
	uint8_t right;

	if (tr_device.device_built){
		printf("Ya se ha construido este dispositivo.Solo puede construir este dispositivo una vez.(tr_build -> traccion_robocol.c)\n");
		return TR_ERROR;
	}

	if ((type&(TR_MASTER|TR_SLAVE))>2){
		printf("Error en el type ingresado por prarámetro.El número en cuestión presenta conflicto entre dos tipos excluyentes.\n");
		return TR_ERROR;
	}


	if (side&(TR_LEFT_SIDE|TR_RIGHT_SIDE)>3){
		printf("Error en el side ingresado por prarámetro.El número en cuestión presenta conflicto entre dos tipos excluyentes.\n");
	}


	tr_device.type=type;
	tr_device.side=side;

	ph_dev* f_motor=malloc(sizeof(ph_dev));
	ph_dev* b_motor=malloc(sizeof(ph_dev));

	tr_device.front_ph=f_motor;
	tr_device.back_ph=b_motor;



	(*(tr_device.front_ph)).pin_cs=PINA0;
	(*(tr_device.front_ph)).pin_in_a=0;
	(*(tr_device.front_ph)).pin_in_b=1;
	(*(tr_device.front_ph)).pin_enable=4;
	(*(tr_device.front_ph)).addr=EXP1;

	(*(tr_device.back_ph)).pin_cs=PINA1;
	(*(tr_device.back_ph)).pin_in_a=2;
	(*(tr_device.back_ph)).pin_in_b=3;
	(*(tr_device.back_ph)).pin_enable=4;
	(*(tr_device.back_ph)).addr=EXP1;

	if(ph_build(tr_device.front_ph)){
		printf("Error en la construcción del puenteH que controla el motor frontal.(tr_build->traccion_robocol.c)\n");
		return TR_ERROR;
	}

	if(ph_build(tr_device.back_ph)){
		printf("Error en la construcción del puenteH que controla el motor trasero.(tr_build->traccion_robocol.c)\n");
		return TR_ERROR;
	}
	

	if(ph_setEstado(tr_device.front_ph,MANUAL_EST)){
		printf("Error en asignación de estado a AUTO_EST para front_ph.(tr_build->traccion_robocol.c)\n");
		return TR_ERROR;
	}

	if(ph_setVel(tr_device.front_ph,0)){
		printf("Error en asignación de velocidad a 0 para front_ph.(tr_build->traccion_robocol.c)\n");
		return TR_ERROR;
	}

	if(ph_setEstado(tr_device.back_ph,MANUAL_EST)){
		printf("Error en asignación de estado a AUTO_EST para el back_ph.(tr_build->traccion_robocol.c)\n");
		return TR_ERROR;
	}

	if(ph_setVel(tr_device.back_ph,0)){
		printf("Error en asignación de velocidad a 0 para front_ph.(tr_build->traccion_robocol.c)\n");
		return TR_ERROR;
	}

	if (ph_enable(tr_device.front_ph)){
		printf("Error en la habilitación de front_ph.(tr_build->traccion_robocol.c)\n");
		return TR_ERROR;		
	}
	printf("front_ph HABILITADO\n");

	if(ph_enable(tr_device.back_ph)){
		printf("Error en la habilitación de back_ph.(tr_build->traccion_robocol.c)\n");
		return TR_ERROR;
	}
	printf("back_ph HABILITADO\n");

	tr_device.mv_state=TR_STOPPED;
	tr_device.ctl_state=TR_MANUAL;
	tr_device.vel_pwm=0;
	tr_device.device_built=TR_BUILT;

	return TR_OK;
}

tr_st tr_forward(uint8_t vp){

	if (tr_device.mv_state==TR_BACKWARD){
		if(tr_setVP(0)){
			printf("Error llevando velocidad a cero. (tr_forward -> traccion_robocol.c) \n");
			return TR_ERROR;
		}
	}


	if (ph_setDireccion(tr_device.front_ph,tr_device.side&TR_FORWARD)){
		printf("Error en asignación de dirección para front_ph. (tr_forward -> traccion_robocol.c) \n");
		return TR_ERROR;
	}

	if (ph_setDireccion(tr_device.back_ph,tr_device.side&TR_FORWARD)){
		printf("Error en asignación de dirección para back_ph. (tr_forward -> traccion_robocol.c) \n");
		return TR_ERROR;
	}

	tr_device.mv_state=TR_FORWARD;
	if(tr_setVP(vp)){
		printf("Error llevando vel_pwm a %d. (tr_forward -> traccion_robocol.c) \n",vp);
		return TR_ERROR;
	}

	return TR_OK;

}


tr_st tr_eBrake(void){
	if (ph_vccBrake(tr_device.front_ph)){
		printf("Error en eBrake para front_ph.(tr_eBrake -> traccion_robocol.c)\n");
		return TR_ERROR;
	}

	if (ph_vccBrake(tr_device.back_ph)){
		printf("Error en eBrake para back_ph.(tr_eBrake -> traccion_robocol.c)\n");
		return TR_ERROR;
	}

	return TR_OK;
}

tr_st tr_setVP(uint8_t vp){
	printf("changing speed to %d\n",vp);

	if (tr_device.mv_state==TR_STOPPED){
		printf("Debe primero asignar la dirección de movimiento. Estado de movimiento se encuentra en TR_STOPPED. (tr_setVP->traccion_robocol.c)\n");
		return TR_ERROR;
	}else{
		if(tr_device.ctl_state==TR_AUTO){
			if(ph_setVel(tr_device.front_ph,vp)){
				printf("Error en asignación de velocidad para front_ph.(tr_setVP->traccion_robocol.c)\n");
				return TR_ERROR;
			}

			if(ph_setVel(tr_device.back_ph,vp)){
				printf("Error en asignación de velocidad para back_ph.(tr_setVP->traccion_robocol.c)\n");
				return TR_ERROR;
			}

		}else if(tr_device.ctl_state==TR_MANUAL){
			if(ph_setPWM(tr_device.front_ph,vp)){
				printf("Error en asignación de PWM para front_ph.(tr_setVP->traccion_robocol.c)\n");
				return TR_ERROR;
			}
			printf("pwm front set to %d\n",vp);

			if(ph_setPWM(tr_device.back_ph,vp)){
				printf("Error en asignación de PWM para back_ph.(tr_setVP->traccion_robocol.c)\n");
				return TR_ERROR;
			}

		}else{
			printf("Error en el estado de la estructura tracción. No se identifica estado TR_MANUAL O TR_AUTO. (tr_setVP->traccion_robocol.c)\n");
			return TR_ERROR;
		}

		if (vp==0){
			tr_device.mv_state=TR_STOPPED;
		}

		tr_device.vel_pwm=vp;
	}
	return TR_OK;
}







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
tr_st tr_backward(uint8_t vp){

	if (tr_device.mv_state==TR_FORWARD){
		if(tr_setVP(0)){
			printf("Error llevando velocidad a cero. (tr_backward -> traccion_robocol.c) \n");
			return TR_ERROR;
		}
	}


	/*TODO: Definir método de espera*/

	if (ph_setDireccion(tr_device.front_ph,tr_device.side&TR_BACKWARD))
	{
		printf("Error en asignación de dirección para front_ph. (tr_backward -> traccion_robocol.c) \n");
		return TR_ERROR;
	}

	if (ph_setDireccion(tr_device.back_ph,tr_device.side&TR_BACKWARD))
	{
		printf("Error en asignación de dirección para back_ph. (tr_backward -> traccion_robocol.c) \n");
		return TR_ERROR;
	}

	tr_device.mv_state=TR_BACKWARD;

	if(tr_setVP(vp)){
		printf("Error llevando el Rover a la velocidad final. (tr_backward -> traccion_robocol.c) \n");
		return TR_ERROR;
	}

	return TR_OK;
}

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
tr_st tr_spin(uint8_t dir, uint8_t arg){

	if(dir!=TR_TURN_LEFT && dir!=TR_TURN_RIGHT){
		printf("La dirección ingresada no es válida. Debe utilizarse TR_TURN_LEFT o TR_TURN_RIGHT  (tr_spin -> traccion_robocol.c)\n");
		return TR_ERROR;
	}

	if(dir==TR_TURN_RIGHT){
		if(tr_device.side==TR_LEFT_SIDE){
			if(tr_forward(arg)){
				printf("Error al tratar de mover el lado izquierdo hacia adelante (tr_spin -> traccion_robocol.c)\n");
				return TR_ERROR;
			}	
		}else{
			if(tr_backward(arg)){
				printf("Error al tratar de mover el lado izquierdo hacia atrás (tr_spin -> traccion_robocol.c)\n");
				return TR_ERROR;
			}				
		}
	}else{
		if(tr_device.side==TR_RIGHT_SIDE){
			if(tr_forward(arg)){
				printf("Error al tratar de mover el lado derecho hacia adelante (tr_spin -> traccion_robocol.c)\n");
				return TR_ERROR;
			}	
		}else{
			if(tr_backward(arg)){
				printf("Error al tratar de mover el lado derecho hacia atrás (tr_spin -> traccion_robocol.c)\n");
				return TR_ERROR;
			}				
		}
	}

	return TR_OK;
}

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
tr_st tr_diffTurn(uint8_t dir, uint32_t arg){

	uint8_t prev_vp;

	if(dir!=TR_TURN_LEFT && dir!=TR_TURN_RIGHT){
		printf("La dirección ingresada no es válida. Debe utilizarse TR_TURN_LEFT o TR_TURN_RIGHT  (tr_diffTurn -> traccion_robocol.c)\n");
		return TR_ERROR;
	}

	prev_vp=tr_device.vel_pwm;

	if(dir==TR_TURN_RIGHT){
		if(tr_device.side==TR_RIGHT_SIDE){
			if(tr_device.vel_pwm < arg){
				if(tr_backward(arg - tr_device.vel_pwm)){
					printf("Error al tratar de mover el lado derecho hacia adelante (tr_diffTurn -> traccion_robocol.c)\n");
					return TR_ERROR;
				}
			}else{
				if(tr_forward(tr_device.vel_pwm-arg)){
					printf("Error al tratar de mover el lado dercho hacia atrás (tr_diffTurn -> traccion_robocol.c)\n");
					return TR_ERROR;
				}
			}
		}else{
			if(tr_forward(tr_device.vel_pwm)){
				printf("Error al tratar de mover el lado izquierdo hacia adelante (tr_diffTurn -> traccion_robocol.c)\n");
				return TR_ERROR;
			}
		}
	}else{
		if(tr_device.side==TR_LEFT_SIDE){
			if(tr_device.vel_pwm < arg){
				if(tr_backward(arg - tr_device.vel_pwm)){
					printf("Error al tratar de mover el lado izquierdo hacia adelante (tr_diffTurn -> traccion_robocol.c)\n");
					return TR_ERROR;
				}
			}else{
				if(tr_forward(tr_device.vel_pwm-arg)){
					printf("Error al tratar de mover el lado izquierdo hacia atrás (tr_diffTurn -> traccion_robocol.c)\n");
					return TR_ERROR;
				}
			}
		}else{
			if(tr_forward(tr_device.vel_pwm)){
				printf("Error al tratar de mover el lado derecho hacia adelante (tr_diffTurn -> traccion_robocol.c)\n");
				return TR_ERROR;
			}
		}
	}

	tr_device.vel_pwm=prev_vp;
	
	return TR_OK;
}


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
tr_st tr_diagonalDiffTurn(uint8_t dir, uint32_t arg){

	uint8_t prev_vp;

	if(dir!=TR_TURN_LEFT && dir!=TR_TURN_RIGHT){
		printf("La dirección ingresada no es válida. Debe utilizarse TR_TURN_LEFT o TR_TURN_RIGHT  (tr_diagonalDiffTurn -> traccion_robocol.c)\n");
		return TR_ERROR;
	}

	prev_vp=tr_device.vel_pwm;

	if(dir==TR_TURN_RIGHT){
		if(tr_device.side==TR_RIGHT_SIDE){
			if(tr_device.vel_pwm < arg){
				if(tr_backward(arg - tr_device.vel_pwm)){
					printf("Error al tratar de mover el lado derecho hacia atrás (tr_diagonalDiffTurn -> traccion_robocol.c)\n");
					return TR_ERROR;
				}
			}else{
				if(tr_forward(tr_device.vel_pwm-arg)){
					printf("Error al tratar de mover el lado derecho hacia adelante (tr_diagonalDiffTurn -> traccion_robocol.c)\n");
					return TR_ERROR;
				}
			}

			if(tr_device.ctl_state==TR_MANUAL){
				if(ph_setPWM(tr_device.front_ph,0)){
					printf("Error detener motor frontal (tr_diagonalDiffTurn -> traccion_robocol.c)\n");
					return TR_ERROR;
				}
			}else{
				if(ph_setVel(tr_device.front_ph,0)){
					printf("Error detener motor frontal (tr_diagonalDiffTurn -> traccion_robocol.c)\n");
					return TR_ERROR;
				}
			}

		}else {
			if(tr_forward(tr_device.vel_pwm)){
				printf("Error al tratar de mover el lado izquierdo hacia adelante (tr_diagonalDiffTurn -> traccion_robocol.c)\n");
				return TR_ERROR;
			}

			if(tr_device.ctl_state==TR_MANUAL){
				if(ph_setPWM(tr_device.back_ph,0)){
					printf("Error detener motor trasero (tr_diagonalDiffTurn -> traccion_robocol.c)\n");
					return TR_ERROR;
				}
			}else{
				if(ph_setVel(tr_device.back_ph,0)){
					printf("Error detener motor trasero (tr_diagonalDiffTurn -> traccion_robocol.c)\n");
					return TR_ERROR;
				}
			}

		}
	}else{
		if(tr_device.side==TR_LEFT_SIDE){
			
			if(tr_device.vel_pwm < arg){
				if(tr_backward(arg - tr_device.vel_pwm)){
					printf("Error al tratar de mover el lado izquierdo hacia adelante (tr_diagonalDiffTurn -> traccion_robocol.c)\n");
					return TR_ERROR;
				}
			}else{
				if(tr_forward(tr_device.vel_pwm-arg)){
					printf("Error al tratar de mover el lado izquierdo hacia atrás (tr_diagonalDiffTurn -> traccion_robocol.c)\n");
					return TR_ERROR;
				}
			}

			if(tr_device.ctl_state==TR_MANUAL){
				if(ph_setPWM(tr_device.front_ph,0)){
					printf("Error detener motor frontal (tr_diagonalDiffTurn -> traccion_robocol.c)\n");
					return TR_ERROR;
				}
			}else{
				if(ph_setVel(tr_device.front_ph,0)){
					printf("Error detener motor frontal (tr_diagonalDiffTurn -> traccion_robocol.c)\n");
					return TR_ERROR;
				}
			}

		}else{
			if(tr_forward(tr_device.vel_pwm)){
				printf("Error al tratar de mover el lado derecho hacia adelante (tr_diagonalDiffTurn -> traccion_robocol.c)\n");
				return TR_ERROR;
			}

			if(tr_device.ctl_state==TR_MANUAL){
				if(ph_setPWM(tr_device.back_ph,0)){
					printf("Error detener motor trasero (tr_diagonalDiffTurn -> traccion_robocol.c)\n");
					return TR_ERROR;
				}
			}else{
				if(ph_setVel(tr_device.back_ph,0)){
					printf("Error detener motor trasero (tr_diagonalDiffTurn -> traccion_robocol.c)\n");
					return TR_ERROR;
				}
			}
		}
	}

	tr_device.vel_pwm=prev_vp;

	return TR_OK;
}

tr_st tr_diagnostico(void){
	char* line;
	uint8_t velf,tempf,corrf;
	uint8_t velb,tempb,corrb;

	if(ph_getVelocidad(tr_device.front_ph,&velf)){
		printf("Error leyendo la velocidad del motor frontal\n");
		return TR_ERROR;
	}

	if(ph_getTemperatura(tr_device.front_ph,&tempf)){
		printf("Error leyendo la temperatura del motor frontal\n");
		return TR_ERROR;
	}

	if(ph_getCorriente(tr_device.front_ph,&corrf)){
		printf("Error leyendo la corriente del motor frontal\n");
		return TR_ERROR;
	}



	if(ph_getVelocidad(tr_device.back_ph,&velb)){
		printf("Error leyendo la velocidad del motor trasero\n");
		return TR_ERROR;
	}

	if(ph_getTemperatura(tr_device.back_ph,&tempb)){
		printf("Error leyendo la temperatura del motor trasero\n");
		return TR_ERROR;
	}

	if(ph_getCorriente(tr_device.back_ph,&corrb)){
		printf("Error leyendo la corriente del motor trasero\n");
		return TR_ERROR;
	}

	sprintf(line, "%d,%d,%d/n",corrb,tempb,velb);

}






