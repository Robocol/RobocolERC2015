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

ph_st ph_enable(ph_dev* dev){
	gpio_exp_set((*dev).pin_enable);
}
ph_st ph_disable(ph_dev* dev){
	gpio_exp_clear((*dev).pin_enable);
}

ph_st ph_setPWM(ph_dev* dev,uint8_t pwm){

	uint8_t rx,tx;

	tx=CAMBIAR_PWM;
	rx=0x00;
	if(spi_rw((*dev).spi,&tx,&rx,1)){
		perror("Error in PWM_CHANGE");
	}

	if(spi_rw((*dev).spi,&pwm,&rx,1)){
		perror("Error while setting PWM");
	}
	return (ph_st)rx;
}

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


ph_st ph_setEstado(ph_dev* dev,uint8_t est){
	uint8_t rx,tx;

	tx=CAMBIAR_ESTADO;
	rx=0x00;
	if(spi_rw((*dev).spi,&tx,&rx,1)){
		perror("Error in CAMBIAR_ESTADO");
	}

	if(spi_rw((*dev).spi,&est,&rx,1)){
		perror("Error while setting ESTADO");
	}
	return (ph_st)rx;
}


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


ph_st getCorriente(ph_dev* dev, uint8_t* corr){
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


ph_st getVelocidad(ph_dev* dev,uint8_t* vel){
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


ph_st getEstado(ph_dev* dev,uint8_t* est){
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

ph_st getTemperatura(ph_dev* dev,uint8_t* temp){
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
