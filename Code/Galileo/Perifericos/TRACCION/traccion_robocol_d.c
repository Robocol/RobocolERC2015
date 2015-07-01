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
	

	if(ph_setEstado(tr_device.front_ph,AUTO_EST)){
		printf("Error en asignación de estado a AUTO_EST para front_ph.(tr_build->traccion_robocol.c)\n");
		return TR_ERROR;
	}

	if(ph_setVel(tr_device.front_ph,0)){
		printf("Error en asignación de velocidad a 0 para front_ph.(tr_build->traccion_robocol.c)\n");
		return TR_ERROR;
	}

	if(ph_setEstado(tr_device.back_ph,AUTO_EST)){
		printf("Error en asignación de estado a AUTO_EST para el back_ph.(tr_build->traccion_robocol.c)\n");
		return TR_ERROR;
	}

	if(ph_setVel(tr_device.back_ph,0)){
		printf("Error en asignación de velocidad a 0 para front_ph.(tr_build->traccion_robocol.c)\n");
		return TR_ERROR;
	}

	if (ph_enable(*tr_device.front_ph)){
		printf("Error en la habilitación de front_ph.(tr_build->traccion_robocol.c)\n");
		return TR_ERROR;		
	}
	printf("front_ph HABILITADO\n");

	if(ph_enable(*tr_device.back_ph)){
		printf("Error en la habilitación de back_ph.(tr_build->traccion_robocol.c)\n");
		return TR_ERROR;
	}
	printf("back_ph HABILITADO\n");

	tr_device.mv_state=TR_STOPPED;
	tr_device.ctl_state=TR_AUTO;
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


	if (ph_setDireccion(tr_device.front_ph,tr_device.side&TR_FORWARD))
	{
		printf("Error en asignación de dirección para front_ph. (tr_forward -> traccion_robocol.c) \n");
		return TR_ERROR;
	}

	if (ph_setDireccion(tr_device.back_ph,tr_device.side&TR_FORWARD))
	{
		printf("Error en asignación de dirección para back_ph. (tr_forward -> traccion_robocol.c) \n");
		return TR_ERROR;
	}

	tr_device.state=TR_FORWARD;
	if(tr_setVP(vp)){
		printf("Error llevando vel_pwm a %d. (tr_forward -> traccion_robocol.c) \n",vp);
		return TR_ERROR;
	}

	return
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
	if (tr_device.state&TR_STOPPED)
	{
		printf("Debe primero asignar la dirección de movimiento. Estado de movimiento se encuentra en TR_STOPPED. (tr_setVP->traccion_robocol.c)\n");
		return TR_ERROR;
	}else{
		if(tr_device.state&(TR_AUTO)){
			if(ph_setVel(tr_device.front_ph,vp)){
				printf("Error en asignación de velocidad para front_ph.(tr_setVP->traccion_robocol.c)\n");
				return TR_ERROR;
			}

			if(ph_setVel(tr_device.back_ph,vp)){
				printf("Error en asignación de velocidad para back_ph.(tr_setVP->traccion_robocol.c)\n");
				return TR_ERROR;
			}

		}else if(tr_device.state&(TR_MANUAL)){
			if(ph_setPWM(tr_device.front_ph,vp)){
				printf("Error en asignación de PWM para front_ph.(tr_setVP->traccion_robocol.c)\n");
				return TR_ERROR;
			}

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

