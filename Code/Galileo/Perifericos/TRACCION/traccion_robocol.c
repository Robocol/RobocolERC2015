#include "traccion_robocol.h"

/*
** ===================================================================
**     Método      :  tr_build
*/
/*!
**     @resumen
**          Se inicializa el dispositivo encargado de controlar la 
**			tracción del Rover. Para esto, se crean los dispositios que
**			controlan los puente H, usando la librería ph_robocol.
**     @param
**          type     	   	- Tipo del dispositivo, indicando se es MASTER
**							o SLAVE, y si está ubicado en RIGHT_SIDE o 
**							LEFT_SIDE. Se puede instanciar este argumento
**							usando OR bit a bit. Por ejemplo:
**
**								tr_build(MASTER|RIGHT);
**							
*/
/* ===================================================================*/
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

/*
** ===================================================================
**     Método      :  tr_setVP
*/
/*!
**     @resumen
**			Asignar pwm o velocidad objetivo según el estado de operación
**			de la tracción. Se asigna también a la estructura
**			global de tracción.
*/
/* ===================================================================*/
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


/*
** ===================================================================
**     Método      :  tr_eBrake
*/
/*!
**     @resumen
**			Freno de emergencia que activa el freno a VCC de los puentes
** 			H de la tracción.
*/
/* ===================================================================*/
tr_st tr_eBrake(void){
	if (ph_vccBrake(tr_device.front_ph)){
		printf("Error en eBrake para front_ph.(tr_eBrake -> traccion_robocol.c)\n");
		return TR_ERROR;
	}

	if (ph_vccBrake(tr_device.back_ph)){
		printf("Error en eBrake para back_ph.(tr_eBrake -> traccion_robocol.c)\n");
		return TR_ERROR;
	}

	if (tr_setVP(0)){
		printf("Error en el set de velocidad/pwm 0. (tr_eBrake -> traccion_robocol.c)\n");
		return TR_ERROR;
	}

	tr_device.mv_state=TR_STOPPED;

	return TR_OK;
}

tr_st tr_enable(void){

	if(ph_enable(tr_device.front_ph)){
		printf("Error en la habilitación del puenteH delantero.(tr_enable -> traccion_robocol.c)\n");
		return TR_ERROR;
	}

	if (ph_enable(tr_device.back_ph)){
		printf("Error en la habilitación del puenteH trasero.(tr_enable -> traccion_robocol.c)\n");
		return TR_ERROR;
	}

	return TR_OK;

}

tr_st tr_disable(void){

	if(ph_disable(tr_device.front_ph)){
		printf("Error en la habilitación del puenteH delantero.(tr_enable -> traccion_robocol.c)\n");
		return TR_ERROR;
	}

	if (ph_disable(tr_device.back_ph)){
		printf("Error en la habilitación del puenteH trasero.(tr_enable -> traccion_robocol.c)\n");
		return TR_ERROR;
	}

	return TR_OK;

}

tr_st tr_setVP(uint8_t vp){
	printf("Changing speed to %d (tr_setVP -> traccion_robocol.c)\n",vp);

/*	if (tr_device.mv_state==TR_STOPPED){
		printf("Debe primero asignar la dirección de movimiento. Estado de movimiento se encuentra en TR_STOPPED. (tr_setVP->traccion_robocol.c)\n");
		return TR_ERROR;
	}else{*/
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
	//}
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
tr_st tr_diffTurn(uint8_t dir, uint8_t vp, uint32_t arg){


	if(dir!=TR_TURN_LEFT && dir!=TR_TURN_RIGHT){
		printf("La dirección ingresada no es válida. Debe utilizarse TR_TURN_LEFT o TR_TURN_RIGHT  (tr_diffTurn -> traccion_robocol.c)\n");
		return TR_ERROR;
	}


	if(dir==TR_TURN_RIGHT){
		if(tr_device.side==TR_RIGHT_SIDE){
			if(vp < arg){
				if(tr_backward(arg -vp)){
					printf("Error al tratar de mover el lado derecho hacia adelante (tr_diffTurn -> traccion_robocol.c)\n");
					return TR_ERROR;
				}
			}else{
				if(tr_forward(vp-arg)){
					printf("Error al tratar de mover el lado dercho hacia atrás (tr_diffTurn -> traccion_robocol.c)\n");
					return TR_ERROR;
				}
			}
		}else{
			if(tr_forward(vp)){
				printf("Error al tratar de mover el lado izquierdo hacia adelante (tr_diffTurn -> traccion_robocol.c)\n");
				return TR_ERROR;
			}
		}
	}else{
		if(tr_device.side==TR_LEFT_SIDE){
			if(vp < arg){
				if(tr_backward(arg - vp)){
					printf("Error al tratar de mover el lado izquierdo hacia adelante (tr_diffTurn -> traccion_robocol.c)\n");
					return TR_ERROR;
				}
			}else{
				if(tr_forward(vp-arg)){
					printf("Error al tratar de mover el lado izquierdo hacia atrás (tr_diffTurn -> traccion_robocol.c)\n");
					return TR_ERROR;
				}
			}
		}else{
			if(tr_forward(vp)){
				printf("Error al tratar de mover el lado derecho hacia adelante (tr_diffTurn -> traccion_robocol.c)\n");
				return TR_ERROR;
			}
		}
	}

	tr_device.vel_pwm=vp;
	
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
tr_st tr_diagonalDiffTurn(uint8_t dir, uint8_t vp, uint32_t arg){

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

/*
** ===================================================================
**     Método      :  tr_diaganostico
*/
/*!
**     @resumen
**			Método encargado de revisar conexión con los microcontroladores
**			responsables del manejo de los motores . En dado caso 
**			que estos dispositivos se encuentren on-line, se efectua
**			petición de variables de corriente, velocidad y temperatura.
**			La información obtenida por dicho proceso es guardada
** 			en el archivo con ruta especificada por parámetro.
*/
/* ===================================================================*/
tr_st tr_diagnostico(char *d_path){
	char *path=d_path;
	FILE* fdl;
	uint8_t velf,tempf,corrf;
	uint8_t velb,tempb,corrb;


	printf("1\n");

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


	printf("1\n");
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


	if((fdl=fopen(path,"w"))<0){
		printf("Error en apertura de archivo para impresion de log\n");
		perror("Causa:");
		exit(1);
	}

	printf("1\n");

	if(tr_device.side==TR_LEFT_SIDE){
		if(fprintf(fdl, "brazo:0,0,0,0,0\n"
					"motorrf:0,0,0\n"
					"motorrb:0,0,0\n"					
					"motorlf:%3d,%3d,%3d\n"
					"motorlb:%3d,%3d,%3d\n"
					"fin\n",corrf,tempf,velf,corrb,tempb,velb)<0){
			printf("Error escribiendo sobre el archivo de diagnóstico del motor izquierdo\n");
		}
	}else if(tr_device.side==TR_RIGHT_SIDE){
		if(fprintf(fdl, "brazo:0,0,0,0,0\n"
					"motorrf:%3d,%3d,%3d\n"
					"motorrb:%3d,%3d,%3d\n"
					"motorlf:0,0,0\n"
					"motorlb:0,0,0\n"	
					"fin\n",corrf,tempf,velf,corrb,tempb,velb)<0){
			printf("Error escribiendo sobre el archivo de diagnóstico del motor derecho\n");
		}
	}

	fclose(fdl);
	return TR_OK;
}

/*
** ===================================================================
**     Método      :  tr_setCtlState
*/
/*!
**     @resumen
**			Método encargado de modificar el estado de operación
**			de la tracción a TR_AUTO o TR_MANUAL según indicado 
**			por parámetro.
*/
/* ===================================================================*/
tr_st tr_setCtlState(uint8_t state){
	uint8_t est=0;

	if(state==TR_AUTO){
		if(ph_setEstado(tr_device.back_ph,AUTO_EST)){
			printf("Error cambiando a estado AUTO el motor trasero\n");
			return TR_ERROR;
		}

		if(ph_setEstado(tr_device.front_ph,AUTO_EST)){
			printf("Error cambiando a estado AUTO el motor delantero\n");
			return TR_ERROR;			
		}

		printf("Estado auto\n");
	}else if(state==TR_MANUAL){
		if(ph_setEstado(tr_device.back_ph,MANUAL_EST)){
			printf("Error cambiando a estado AUTO el motor trasero\n");
			return TR_ERROR;			
		}

		if(ph_setEstado(tr_device.front_ph,MANUAL_EST)){
			printf("Error cambiando a estado AUTO el motor delantero\n");
			return TR_ERROR;			
		}
		printf("Estado manual\n");
	}else{
			printf("Error: El estado de control ingresado (%d) no es válido\n",state);
			return TR_ERROR;
	}

	ph_getEstado( tr_device.front_ph, &est);
	printf("estado:%d\n", est);
	tr_device.ctl_state=state;
}