
tr_st tr_build(uint8_t type){
	uint8_t master;
	uint8_t right;
	if (tr_device.device_built){
		printf("Ya se ha construido este dispositivo.Solo puede construir este dispositivo una vez.(tr_build -> traccion_robocol.c)\n");
		return TR_ERROR;
	}

	if ((type>>2)==3 || type&(0b00000011)==3){
		printf("Error en el type ingresado por prarámetro.El número en cuestión presenta conflicto entre dos tipos excluyentes.\n");
		return TR_ERROR;
	}

	tr_device.type=type;

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
	tr_device.state=AUTO|STOPPED;
	tr_device.vel_pwm=0;
	tr_device.device_built=1;

	return TR_OK;
}

tr_st tr_eBrake(void){
	if (ph_totalBrake(tr_device.front_ph)){
		printf("Error en eBrake para front_ph.(tr_eBrake -> traccion_robocol.c)\n");
		return TR_ERROR;
	}

	if (ph_totalBrake(tr_device.back_ph)){
		printf("Error en eBrake para back_ph.(tr_eBrake -> traccion_robocol.c)\n");
		return TR_ERROR;
	}

	return TR_OK;
}

tr_st tr_setVP(uint8_t vp){
	tr_device.vel_pwm=vp;
	return TR_OK;
}
