/*
 * --------------------------------------------------------------------------------------
 * ph_robocol.c (0.9)
 *
 *  Created on: Mar 12, 2015
 *
 *      Authors:	Daniel Ochoa
 *					Germán Giraldo
 * -------------------------------------------------------------------------------------
 */
#include "ph_robocol.h"

/*
** ===================================================================
**     Método      :  ph_build
*/
/*!
**     @resumen
**          Construlle el dispositivo a partir de la información en la structura
**			suministrada por el usuario.
**     @param
**          dev     	   	- Puntero al dispositivo sobre el que recae la 
**							acción.
**     @return
**                         	- Estado salida del método. 
*/
/* ===================================================================*/
ph_st ph_build(ph_dev* dev){
	
	spi_device* new_spi=malloc(sizeof(spi_device));
	uint8_t addr=0b0111000;

	printf("Iniciando creación de dispositivo spi para manejo de puenteH\n");
	if(spi_create_device(new_spi,0,(*dev).pin_cs)){
		printf("Error en la creación de dispositivo spi para manejo de puente H.(ph_robocol.c)\n");
		return PH_ERROR;
	}
	(*dev).spi=new_spi;
	(*dev).pwm=0;
	if(spi_start("/dev/spidev1.0",100000)){
		printf("Error en el arranque comunicación spi para manejo de puente H.(ph_robocol.c)\n");
		return PH_ERROR;
	}

	if (build_expander(addr)){
		printf("Error en la creación del expansor para control de puente H\n" );
	}


	return PH_OK;
}

/*
** ===================================================================
**     Método      :  ph_enable
*/
/*!
**     @resumen
**          Habilita el puente H ingresado por parámetro.
**     @param
**          dev     	   	- Puntero al dispositivo sobre el que recae la 
**							acción.
**     @return
**                         	- Estado salida del método. 
*/
/* ===================================================================*/
ph_st ph_enable(ph_dev* dev){
	return (ph_st)gpio_exp_set((*dev).pin_enable);
}

/*
** ===================================================================
**     Método      :  ph_disable
*/
/*!
**     @resumen
**          Deshabilita el puente H ingresado por parámetro.
**     @param
**          dev     	   	- Puntero al dispositivo sobre el que recae la 
**							acción.
**     @return
**                         	- Estado salida del método. 
*/
/* ===================================================================*/
ph_st ph_disable(ph_dev* dev){
	return (ph_st)gpio_exp_clear((*dev).pin_enable);
}

/*
** ===================================================================
**     Método      :  ph_setPWM
*/
/*!
**     @resumen
**          Modifica el ciclo útil del PWM que controla la velocidad del
**			motor.
**     @param
**          dev     	   	- Puntero al dispositivo sobre el que recae la 
**							acción.
**     @param
**          pwm     	   	- Valor del ciclo útil del PWM:
**								0 	-> 0% ciclo útil
**								255 -> 100% ciclo útil
**     @return
**                         	- Estado salida del método. 
*/
/* ===================================================================*/
ph_st ph_setPWM(ph_dev* dev,uint8_t pwm){

	uint8_t rx,tx;

	tx=CAMBIAR_PWM;
	rx=0x00;

	if(spi_rw((*dev).spi,&tx,&rx,1)){
		perror("Error in PWM_CHANGE");
		return PH_ERROR;
	}

	if(spi_rw((*dev).spi,&pwm,&rx,1)){
		perror("Error while setting PWM");
		return PH_ERROR;
	}
	return PH_OK;
}

/*
** ===================================================================
**     Método      :  ph_setDireccion
*/
/*!
**     @resumen
**          Modifica la dirección de giro del motor
**     @param
**          dev     	   	- Puntero al dispositivo sobre el que recae la 
**							acción.
**     @param
**          dir     	   	- Indicador de la dirección de giro:
**								0 ->	
**								1 ->	
**     @return
**                         	- Estado salida del método. 
*/
/* ===================================================================*/
ph_st ph_setDireccion(ph_dev* dev, uint8_t dir){

	//TODO: Arreglar librerias de expansor de pines y GPIO para evitar conflicto
	if(dir){
		gpio_exp_set((*dev).pin_in_a);
		gpio_exp_clear((*dev).pin_in_b);
	}else{
		gpio_exp_set((*dev).pin_in_b);
		gpio_exp_clear((*dev).pin_in_a);
	}
	return PH_OK;
}

/*
** ===================================================================
**     Método      :  ph_setEstado
*/
/*!
**     @resumen
**          Modifica el estado de operación del puente H
**     @param
**          dev     	   	- Puntero al dispositivo sobre el que recae la 
**							acción.
**     @param
**          est     	   	- Identificador del estado al que se pasará:
**								16 -> AUTO_EST
**								32 -> MANUAL_EST
**								48 -> FAIL_EST
**     @return
**                         	- Estado salida del método. 
*/
/* ===================================================================*/
ph_st ph_setEstado(ph_dev* dev,uint8_t est){
	uint8_t rx,tx;

	tx=CAMBIAR_ESTADO;
	rx=0x00;
	if(spi_rw((*dev).spi,&tx,&rx,1)){
		perror("Error in CAMBIAR_ESTADO");
		return PH_ERROR;
	}

	if(spi_rw((*dev).spi,&est,&rx,1)){
		perror("Error while setting ESTADO");
		return PH_ERROR;
	}
	return PH_OK;
}

/*
** ===================================================================
**     Método      :  ph_setKPV
*/
/*!
**     @resumen
**          Modifica la ganancia proporcional del controlador PI de 
**			velocidad
**     @param
**          dev     	   	- Puntero al dispositivo sobre el que recae la 
**							acción.
**     @param
**          kpv     	   	- Ganancia a utilizar.
**								Valor entre 0 y 255.
**     @return
**                         	- Estado salida del método. 
*/
/* ===================================================================*/
ph_st ph_setKPV(ph_dev* dev,uint8_t kpv){
	uint8_t rx,tx;

	tx=CAMBIAR_KP_V;
	rx=0x00;
	if(spi_rw((*dev).spi,&tx,&rx,1)){
		perror("Error in CAMBIAR_KP_V");
	}

	if(spi_rw((*dev).spi,&kpv,&rx,1)){
		perror("Error while setting PWM");
	}
	return (ph_st)rx;
}


/*
** ===================================================================
**     Método      :  ph_setKIV
*/
/*!
**     @resumen
**          Modifica la ganancia integral del controlador PI de 
**			velocidad
**     @param
**          dev     	   	- Puntero al dispositivo sobre el que recae la 
**							acción.
**     @param
**          kiv     	   	- Ganancia a utilizar.
**								Valor entre 0 y 255.
**     @return
**                         	- Estado salida del método. 
*/
/* ===================================================================*/
ph_st ph_setKIV(ph_dev* dev,uint8_t kiv){
	uint8_t rx,tx;

	tx=CAMBIAR_KI_V;
	rx=0x00;
	if(spi_rw((*dev).spi,&tx,&rx,1)){
		perror("Error in CAMBIAR_KI_V");
	}

	if(spi_rw((*dev).spi,&kiv,&rx,1)){
		perror("Error while setting KI_V");
	}
	return (ph_st)rx;
}

/*
** ===================================================================
**     Método      :  ph_setKPC
*/
/*!
**     @resumen
**          Modifica la ganancia proporcional del controlador PI de 
**			corriente
**     @param
**          dev     	   	- Puntero al dispositivo sobre el que recae la 
**							acción.
**     @param
**          kpc     	   	- Ganancia a utilizar.
**								Valor entre 0 y 255.
**     @return
**                         	- Estado salida del método. 
*/
/* ===================================================================*/
ph_st ph_setKPC(ph_dev* dev,uint8_t kpc){
	uint8_t rx,tx;

	tx=CAMBIAR_KP_C;
	rx=0x00;
	if(spi_rw((*dev).spi,&tx,&rx,1)){
		perror("Error in CAMBIAR_KP_C");
	}

	if(spi_rw((*dev).spi,&kpc,&rx,1)){
		perror("Error while setting KP_C");
	}
	return (ph_st)rx;
}

/*
** ===================================================================
**     Método      :  ph_setKIC
*/
/*!
**     @resumen
**          Modifica la ganancia integral del controlador PI de 
**			corriente.
**     @param
**          dev     	   	- Puntero al dispositivo sobre el que recae la 
**							acción.
**     @param
**          kic     	   	- Ganancia a utilizar.
**								Valor entre 0 y 255.
**     @return
**                         	- Estado salida del método. 
*/
/* ===================================================================*/
ph_st ph_setKIC(ph_dev* dev,uint8_t kic){
	uint8_t rx,tx;

	tx=CAMBIAR_KI_C;
	rx=0x00;
	if(spi_rw((*dev).spi,&tx,&rx,1)){
		perror("Error in CAMBIAR_KI_C");
	}

	if(spi_rw((*dev).spi,&kic,&rx,1)){
		perror("Error while setting KI_C");
	}
	return (ph_st)rx;
}

/*
** ===================================================================
**     Método      :  ph_setVel
*/
/*!
**     @resumen
**          Modifica velocidad objetivo del controlador
**     @param
**          dev     	   	- Puntero al dispositivo sobre el que recae la 
**							acción.
**     @param
**          vel     	   	- Nueva velocidad.
**								Valor entre 0 y 255.
**     @return
**                         	- Estado salida del método. 
*/
/* ===================================================================*/
ph_st ph_setVel(ph_dev* dev,uint8_t vel){
	uint8_t rx,tx;

	tx=SP_VELOCIDAD;
	rx=0x00;
	if(spi_rw((*dev).spi,&tx,&rx,1)){
		perror("Error in SP_VELOCIDAD");
	}

	if(spi_rw((*dev).spi,&vel,&rx,1)){
		perror("Error while setting VELOCIDAD");
	}
	return (ph_st)rx;
}

/*
** ===================================================================
**     Método      :  ph_set
*/
/*!
**     @resumen
**          Modifica velocidad objetivo del controlador
**     @param
**          dev     	   	- Puntero al dispositivo sobre el que recae la 
**							acción.
**     @param
**          vel     	   	- Nueva velocidad.
**								Valor entre 0 y 255.
**     @return
**                         	- Estado salida del método. 
*/
/* ===================================================================*/
ph_st ph_setCorriente(ph_dev* dev,uint8_t vel){
	uint8_t rx,tx;

	tx=SP_CORRIENTE;
	rx=0x00;
	if(spi_rw((*dev).spi,&tx,&rx,1)){
		perror("Error in SP_VELOCIDAD");
	}

	if(spi_rw((*dev).spi,&vel,&rx,1)){
		perror("Error while setting VELOCIDAD");
	}
	return (ph_st)rx;
}

/*
** ===================================================================
**     Método      :  ph_getCorriente
*/
/*!
**     @resumen
**          Obtiene el valor de corriente medido por el puente H
**     @param
**          dev     	   	- Puntero al dispositivo sobre el que recae la 
**							acción.
**     @param
**          corr     	   	- Corriente medida.
**								Valor entre 0 y 255.
**     @return
**                         	- Estado salida del método. 
*/
/* ===================================================================*/
ph_st ph_getCorriente(ph_dev* dev, uint8_t* corr){
	uint8_t rx,tx;

	tx=MEDIR_CORRIENTE;
	rx=0x00;
	if(spi_rw((*dev).spi,&tx,&rx,1)){
		perror("Error in MEDIR_CORRIENTE");
	}

	if(spi_rw((*dev).spi,&tx,corr,1)){
		perror("Error while getting CORRIENTE");
	}
	return (ph_st)rx;
}

/*
** ===================================================================
**     Método      :  ph_getVelocidad
*/
/*!
**     @resumen
**          Obtiene el valor de velocidad medido por el puente H
**     @param
**          dev     	   	- Puntero al dispositivo sobre el que recae la 
**							acción.
**     @param
**          vel     	   	- Puntero a la posición de memoria donde se 
**							guarda la velocidad.
**								Valor entre 0 y 255.
**     @return
**                         	- Estado salida del método. 
*/
/* ===================================================================*/
ph_st ph_getVelocidad(ph_dev* dev,uint8_t* vel){
	uint8_t rx,tx;

	tx=MEDIR_VELOCIDAD;
	rx=0x00;
	if(spi_rw((*dev).spi,&tx,&rx,1)){
		perror("Error in MEDIR_VELOCIDAD");
	}

	if(spi_rw((*dev).spi,&tx,vel,1)){
		perror("Error while getting VELOCIDAD");
	}
	return (ph_st)rx;
}

/*
** ===================================================================
**     Método      :  ph_getEstado
*/
/*!
**     @resumen
**          Obtiene el valor de corriente medido por el puente H
**     @param
**          dev     	   	- Puntero al dispositivo sobre el que recae la 
**							acción.
**     @param
**          est     	   	- Puntero a la posición de memoria donde 
**							almacenar el estado.
**								Valor entre 0 y 255.
**     @return
**                         	- Estado salida del método. 
*/
/* ===================================================================*/
ph_st ph_getEstado(ph_dev* dev,uint8_t* est){
	uint8_t rx,tx;

	tx=DAR_ESTADO;
	rx=0x00;
	if(spi_rw((*dev).spi,&tx,&rx,1)){
		perror("Error in DAR_ESTADO");
	}

	if(spi_read((*dev).spi,est,1)){
		perror("Error while getting ESTADO");
	}
	return (ph_st)rx;
}

/*
** ===================================================================
**     Método      :  ph_getTemperatura
*/
/*!
**     @resumen
**          Obtiene el valor de temperatura medido por el puente H
**     @param
**          dev     	   	- Puntero al dispositivo sobre el que recae la 
**							acción.
**     @param
**          est     	   	- Puntero a la posición de memoria donde guardar
**							la temperatura.
**								Valor entre 0 y 255.
**     @return
**                         	- Estado salida del método. 
*/
/* ===================================================================*/
ph_st ph_getTemperatura(ph_dev* dev,uint8_t* temp){
	uint8_t rx,tx;

	tx=DAR_TEMPERATURA;
	rx=0x00;
	if(spi_rw((*dev).spi,&tx,&rx,1)){
		perror("Error in DAR_TEMPERATURA");
	}

	if(spi_read((*dev).spi,temp,1)){
		perror("Error while getting TEMPERATURA");
	}
	return (ph_st)rx;
}

// ph_st ph_setPWMSmooth(ph_dev* devptr, uint8_t pwm, uint8_t stepsize){
// 	uint8_t curr_pwm;
// 	int8_t steps;
// 	int8_t step;
// 	uint8_t i;
// 	printf("PWM in%d\n",pwm);
// 	printf("stepsize%d\n",stepsize);
// 	curr_pwm=(*devptr).pwm;
// 	printf("Current pwm%d\n",curr_pwm);
// 	steps=(curr_pwm-pwm)/stepsize;
// 	printf("Steps%d\n",steps );

// 	if(steps<0){
// 		steps=(-1*steps);
// 		step=(stepsize);
// 	}else{
// 		step=(-1*stepsize);
// 	}
// 	printf("Steps%d\n",steps );
// 	printf("Step%d\n",step);
// 	for (i = 0; i < steps; ++i)
// 	{
// 		curr_pwm+=step;
// 		printf("Current pwm%d\n",curr_pwm );
// 		if(ph_setPWM(devptr,curr_pwm)){
// 			printf("Error en set de pwm puenteH.(prueba_ph.c)");
// 			perror("Descripción:");
// 			return PH_ERROR;
// 		}
// 	}

// 	if (curr_pwm!=pwm){
// 		if(ph_setPWM(devptr,pwm)){
// 			printf("Error en set de pwm puenteH.(prueba_ph.c)")	;
// 			perror("Descripción:");										
// 			return PH_ERROR;
// 		}
// 		printf("\tPWM cambiado a :%d\n", pwm);					
// 	}

// 	(*devptr).pwm=pwm;
// 	return PH_OK;
// }
