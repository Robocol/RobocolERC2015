
#include "traccion_robocol.h"
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

	if (ph_setDireccion(tr_device.front_ph,CW_DIR))
	{
		printf("Error en asignación de dirección para front_ph. (tr_backward -> traccion_robocol.c) \n");
		return TR_ERROR;
	}

	if (ph_setDireccion(tr_device.back_ph,CW_DIR))
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

	if(dir!=TR_TURN_LEFT && dir!=TR_TURN_RIGHT){
		printf("La dirección ingresada no es válida. Debe utilizarse TR_TURN_LEFT o TR_TURN_RIGHT  (tr_diffTurn -> traccion_robocol.c)\n");
		return TR_ERROR;
	}

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

	if(dir!=TR_TURN_LEFT && dir!=TR_TURN_RIGHT){
		printf("La dirección ingresada no es válida. Debe utilizarse TR_TURN_LEFT o TR_TURN_RIGHT  (tr_diagonalDiffTurn -> traccion_robocol.c)\n");
		return TR_ERROR;
	}

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

		}else{
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

	return TR_OK;
}