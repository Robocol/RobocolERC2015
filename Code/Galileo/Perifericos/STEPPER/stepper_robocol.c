#include "stepper_robocol.h"


/*======================================================================*/
/*								GETTERS									*/
/*======================================================================*/

/*
** ===================================================================
**     Método      :  stp_build
*/
/*!
**     @resumen
**          Contruye el dispositivo. Es necesario llamar este método 
**			antes de usar el motor
**     @param
**          dev     	   	- Puntero al dispositivo sobre el que recae la 
**							acción.
**     @return
**                         	- Estado salida del método. 
*/
/* ===================================================================*/
stp_st stp_build(stp_device* dev){
	uint8_t addr=(*dev).exp;
	spi_device* new_spi=malloc(sizeof(spi_device));

	printf("Expander: %d\n",(*dev).exp );
	printf("Address: %x\n",addr);

	printf("Pin_cs en el stp_dev durante stp_build:\t\t%d\n", (*dev).pin_cs);
	if (spi_create_device(new_spi,3,(*dev).pin_cs)!=0)
	{
		printf("Error en la creación del dispositivo spi para el manejo de stepper.(stepper_robocol.c)\n");
		return STP_ERROR;
	}
	printf("Pin en new_spi despues de spi_create_device:\t\t%d\n", (*new_spi).pin);

	(*dev).spi=new_spi;
	printf("Pin en spi de stp_dev:\t\t%d\n", (*(*dev).spi).pin);

	if(pwm_build((*dev).pin_pwm, 5000 , DUTY_CYCLE)!=0){
		printf("Error en la creación del dispositivo pwm para el manejo de stepper.(stepper_robocol.c)\n");
		return STP_ERROR;
	}


	if(gpio_set_dir((*dev).pin_cs,OUT)){
		printf("Error asignando dirección de salida a pin %s\n", (*dev).pin_cs);
		return STP_ERROR;
	}

	if(gpio_set_dir((*dev).pin_dir,OUT)){
		printf("Error asignando dirección de salida a pin %s\n", (*dev).pin_dir);
		return STP_ERROR;
	}

	if(gpio_set_dir((*dev).pin_flag,IN)){
		printf("Error asignando dirección de salida a pin %s\n", (*dev).pin_cs);
		return STP_ERROR;
	}

	printf("Despues de pwm_build\n");
	spi_start("/dev/spidev1.0",100000);
	build_expander(addr);
	return STP_OK;
}

/*
** ===================================================================
**     Método      :  stp_setParam
*/
/*!
**     @resumen
**          Asigna un valor especificado por parámetro a uno de los parámetros
**			de operación del stepper
**     @param
**          dev     	   	- Puntero al dispositivo sobre el que recae la 
**							acción.
**     @param
**          param     	   	- Indicador del parámetro de operación a modificar.
**     @param
**          buff     	   	- Valor a asignar al parámetro especificado.
**     @param
**          len     	   	- Largo en bytes de buff.
**     @return
**                         	- Estado salida del método. 
*/
/* ===================================================================*/
stp_st stp_setParam(stp_device* dev, uint8_t param, uint8_t* buff,uint8_t len){
	
	uint8_t tx=param;				
	uint8_t rx=0;

	printf("TX:\t%X\n", tx);

	if(spi_rw((*dev).spi, &tx, &rx,1)){
		printf("Error en SPI\n");
		return STP_ERROR;
	}
	int i;
	for(i=0;i<len;i++){
		tx=*(buff+(len-i-1));
		printf("TX:\t%X\n",tx );
		if(spi_rw((*dev).spi, &tx, &rx,1)){
			printf("Error en SPI\n");
			return STP_ERROR;
		}
	}

	return STP_OK;

}

/*
** ===================================================================
**     Método      :  stp_getParam
*/
/*!
**     @resumen
**          Obtener el valor actual del parámetro especificado
**
**     @param
**          dev     	   	- Puntero al dispositivo sobre el que recae la 
**							acción.
**     @param
**          param     	   	- Indicador del parámetro de operación a modificar.
**     @param
**          buff     	   	- Puntero a un arreglo en el cual se almacenará
**							la información del parámetro.
**     @param
**          len     	   	- Largo en bytes de buff.
**
**     @return
**                         	- Estado salida del método. 
*/
/* ===================================================================*/
stp_st stp_getParam(stp_device* dev, uint8_t param, uint8_t* buff,uint8_t len){

	uint8_t tx=param;
	uint8_t rx=0;
	if(param!=STATUS){
		tx|=(0x01<<5);
	}

	if(spi_rw((*dev).spi, &tx, &rx,1)){
		printf("Error en SPI\n");
		return STP_ERROR;
	}
	int i;
	printf("PARAM:\t%d\n",param);
	printf("TX:\t%d\n",tx );
	tx=0x00;
	for(i=0;i<len;i++){
		if(spi_rw((*dev).spi, &tx, &rx,1)){
			printf("Error en SPI\n");
			return STP_ERROR;
		}else{
			*(buff+len-i-1)=rx;		//Little-Endian
		}
	}
	return STP_OK;

}

/*
** ===================================================================
**     Método      :  stp_getPosition
*/
/*!
**     @resumen
**          Obtiente la posición actual absoluta del stepper
**
**     @param
**          dev     	   	- Puntero al dispositivo sobre el que recae 
**							la acción.
**     @param
**          pos     	   	- Puntero a la posición de memoria en la cual
**							se almacenará la información del parámetro.
**
**     @return
**                         	- Estado salida del método. 
*/
/* ===================================================================*/
stp_st stp_getPosition(stp_device* dev, int32_t* pos){

	uint8_t buff[4]={0,0,0,0};

	if(stp_getParam(dev,ABS_POS,buff,3)){
		printf("Error obteniendo posición\n");
		return STP_ERROR;
	}

	if(buff[2]&0x20){
		buff[2]|=0xC0;
		buff[3]|=0xFF;
	}
	*pos = array_to_i32(buff,4, TRUE);

	return STP_OK;
}


/*
** ===================================================================
**     Método      :  stp_getTVal
*/
/*!
**     @resumen
**          Obtiene el valor de corriente asignado al regulador de torque
**
**     @param
**          dev     	   	- Puntero al dispositivo sobre el que recae la 
**							acción.
**     @param
**          tval     	   	- Puntero a un arreglo en el cual se almacenará
**							la información del parámetro.
**
**     @return
**                         	- Estado salida del método. 
*/
/* ===================================================================*/
stp_st stp_getTVAL(stp_device* dev, uint8_t* tval){


	if(stp_getParam(dev,TVAL,tval,1)){
		printf("Error obteniendo TVAL\n");
		return STP_ERROR;
	}

	return STP_OK;
}

/*
** ===================================================================
**     Método      :  stp_getTOffFast
*/
/*!
**     @resumen
**          Obtiene el tiempo máximo de apagado rápido actual (Auto 
**			adjusted decay)
**
**     @param
**          dev     	   	- Puntero al dispositivo sobre el que recae 
**							la acción.
**     @param
**          pos     	   	- Puntero a la posición de memoria en la cual
**							se almacenará la información del parámetro.
**
**     @return
**                         	- Estado salida del método. 
*/
/* ===================================================================*/
stp_st stp_getTOffFast(stp_device* dev, int8_t* tof){

	if(stp_getParam(dev,T_FAST,tof,1)){
		printf("Error obteniendo T_FAST\n");
		return STP_ERROR;
	}

	*tof &= (0x0F);

	return STP_OK;
}

/*
** ===================================================================
**     Método      :  stp_getFallStep
*/
/*!
**     @resumen
**          Obtiene el tiempo mínimo de apagado actual (Auto adjusted decay)
**
**     @param
**          dev     	   	- Puntero al dispositivo sobre el que recae 
**							la acción.
**     @param
**          pos     	   	- Puntero a la posición de memoria en la cual
**							se almacenará la información del parámetro.
**
**     @return
**                         	- Estado salida del método. 
*/
/* ===================================================================*/
stp_st stp_getFallStep(stp_device* dev, int8_t* ft){

	if(stp_getParam(dev,T_FAST,ft,1)){
		printf("Error obteniendo T_FAST\n");
		return STP_ERROR;
	}

	*ft >>= FALL_STEP;

	return STP_OK;
}
/*
** ===================================================================
**     Método      :  stp_getTOnMin
*/
/*!
**     @resumen
**          Obtiene el tiempo de encendido mínimo del control de corriente
**
**     @param
**          dev     	   	- Puntero al dispositivo sobre el que recae 
**							la acción.
**     @param
**          tonm     	   	- Puntero a la posición de memoria en la cual
**							se almacenará la información del parámetro.
**
**     @return
**                         	- Estado salida del método. 
*/
/* ===================================================================*/
stp_st stp_getTONMIN(stp_device* dev, int8_t* tonm){

	if(stp_getParam(dev,TON_MIN,tonm,1)){
		printf("Error obteniendo TON_MIN\n");
		return STP_ERROR;
	}

	return STP_OK;
}

/*
** ===================================================================
**     Método      :  stp_getTOffMin
*/
/*!
**     @resumen
**          Obtiene el tiempo de apagado mínimo del control de corriente
**
**     @param
**          dev     	   	- Puntero al dispositivo sobre el que recae 
**							la acción.
**     @param
**          toffm     	   	- Puntero a la posición de memoria en la cual
**							se almacenará la información del parámetro.
**
**     @return
**                         	- Estado salida del método. 
*/
/* ===================================================================*/
stp_st stp_getTOffMin(stp_device* dev, int8_t* toffm){

	if(stp_getParam(dev,TOFF_MIN,toffm,1)){
		printf("Error obteniendo TOFF_MIN\n");
		return STP_ERROR;
	}

	return STP_OK;
}

/*
** ===================================================================
**     Método      :  stp_getADC
*/
/*!
**     @resumen
**          Obtiene valor registrado por el ADC
**
**     @param
**          dev     	   	- Puntero al dispositivo sobre el que recae 
**							la acción.
**     @param
**          adc     	   	- Puntero a la posición de memoria en la cual
**							se almacenará la información del parámetro.
**
**     @return
**                         	- Estado salida del método. 
*/
/* ===================================================================*/
stp_st stp_getADC(stp_device* dev, int8_t* adc){

	if(stp_getParam(dev,ADC_OUT,adc,1)){
		printf("Error obteniendo ADC\n");
		return STP_ERROR;
	}

	return STP_OK;
}

/*
** ===================================================================
**     Método      :  stp_getOCDT
*/
/*!
**     @resumen
**          Obtiene valor del OverCurrent Detection threshold
**
**     @param
**          dev     	   	- Puntero al dispositivo sobre el que recae 
**							la acción.
**     @param
**          ocdt     	   	- Puntero a la posición de memoria en la cual
**							se almacenará la información del parámetro.
**
**     @return
**                         	- Estado salida del método. 
*/
/* ===================================================================*/
stp_st stp_getOCDT(stp_device* dev, int8_t* ocdt){

	if(stp_getParam(dev,OCD_TH,ocdt,1)){
		printf("Error obteniendo OCDT\n");
		return STP_ERROR;
	}

	return STP_OK;
}

/*
** ===================================================================
**     Método      :  stp_getStepSel
*/
/*!
**     @resumen
**          Obtiene valor del selector de paso.
**
**     @param
**          dev     	   	- Puntero al dispositivo sobre el que recae 
**							la acción.
**     @param
**          step_sel     	   	- Puntero a la posición de memoria en la cual
**							se almacenará la información del parámetro.
**
**     @return
**                         	- Estado salida del método. 
*/
/* ===================================================================*/
stp_st stp_getStepSel(stp_device* dev, int8_t* step_sel){

	if(stp_getParam(dev,STEP_MODE,step_sel,1)){
		printf("Error obteniendo STEP_MODE\n");
		return STP_ERROR;
	}

	*step_sel &= 0x07;

	return STP_OK;
}


/*
** ===================================================================
**     Método      :  stp_getAlarmEn
*/
/*!
**     @resumen
**          Obtiene el registro de alarmas activadas
**
**     @param
**          dev     	   	- Puntero al dispositivo sobre el que recae 
**							la acción.
**     @param
**          al_en	   	- Puntero a la posición de memoria en la cual
**							se almacenará la información del parámetro.
**
**     @return
**                         	- Estado salida del método. 
*/
/* ===================================================================*/
stp_st stp_getAlarmEn(stp_device* dev, int8_t* al_en){

	if(stp_getParam(dev,ALARM_EN,al_en,1)){
		printf("Error obteniendo ALARM_EN\n");
		return STP_ERROR;
	}

	return STP_OK;
}

/*
** ===================================================================
**     Método      :  stp_getConfig
*/
/*!
**     @resumen
**          Obtiene valor del registro de configuración
**
**     @param
**          dev     	   	- Puntero al dispositivo sobre el que recae 
**							la acción.
**     @param
**          config	   	- Puntero a la posición de memoria en la cual
**							se almacenará la información del parámetro.
**
**     @return
**                         	- Estado salida del método. 
*/
/* ===================================================================*/
stp_st stp_getConfig(stp_device* dev, int32_t* osc_sel){

	uint8_t buff[2];
	if(stp_getParam(dev,CONFIG,buff,2)){
		printf("Error obteniendo CONFIG\n");
		return STP_ERROR;
	}

	*osc_sel=array_to_i32(buff,2,TRUE);

	return STP_OK;
}

/*
** ===================================================================
**     Método      :  stp_getStatus
*/
/*!
**     @resumen
**          Obtiene valor del registro de configuración
**
**     @param
**          dev     	   	- Puntero al dispositivo sobre el que recae 
**							la acción.
**     @param
**          config	   	- Puntero a la posición de memoria en la cual
**							se almacenará la información del parámetro.
**
**     @return
**                         	- Estado salida del método. 
*/
/* ===================================================================*/
stp_st stp_getStatus(stp_device* dev, int32_t* stats){

	uint8_t buff[2];
	if(stp_getParam(dev,STATUS,buff,2)){
		printf("Error obteniendo STATUS\n");
		return STP_ERROR;
	}

	*stats=array_to_i32(buff,2,TRUE);

	return STP_OK;
}






















/*======================================================================*/
/*								SETTERS									*/
/*======================================================================*/




/*
** ===================================================================
**     Método      :  stp_setPosition
*/
/*!
**     @resumen
**          asigna la posición actual absoluta del stepper
**
**     @param
**          dev     	   	- Puntero al dispositivo sobre el que recae 
**							la acción.
**     @param
**          pos     	   	- Puntero a la posición de memoria en la cual
**							se almacenará la información del parámetro.
**
**     @return
**                         	- Estado salida del método. 
*/
/* ===================================================================*/
stp_st stp_setPosition(stp_device* dev, int32_t pos){

	uint8_t* ptr=(uint8_t*)&pos;
	if(stp_setParam(dev,ABS_POS,ptr,3)){
		printf("Error asignando posición\n");
		return STP_ERROR;
	}

	return STP_OK;

}


/*
** ===================================================================
**     Método      :  stp_setTVal
*/
/*!
**     @resumen
**          Asigna valor de corriente al regulador de torque
**
**     @param
**          dev     	   	- Puntero al dispositivo sobre el que recae la 
**							acción.
**     @param
**          tval     	   	- Puntero a un arreglo en el cual se almacenará
**							la información del parámetro.
**
**     @return
**                         	- Estado salida del método. 
*/
/* ===================================================================*/
stp_st stp_setTVal(stp_device* dev, int8_t tval){

	if(stp_setParam(dev,TVAL,&tval,1)){
		printf("Error asignando TVAL\n");
		return STP_ERROR;
	}

	return STP_OK;
}

/*
** ===================================================================
**     Método      :  stp_setTOnMin
*/
/*!
**     @resumen
**          Asigna el tiempo de encendido mínimo del control de corriente
**
**     @param
**          dev     	   	- Puntero al dispositivo sobre el que recae 
**							la acción.
**     @param
**          tonm     	   	- Puntero a la posición de memoria en la cual
**							se almacenará la información del parámetro.
**
**     @return
**                         	- Estado salida del método. 
*/
/* ===================================================================*/
stp_st stp_setTOnMin(stp_device* dev, int8_t tonm){

	if(stp_setParam(dev,TON_MIN,&tonm,1)){
		printf("Error asignando TON_MIN\n");
		return STP_ERROR;
	}

	return STP_OK;
}

/*
** ===================================================================
**     Método      :  stp_setTOffMin
*/
/*!
**     @resumen
**          Asigna el tiempo de apagado mínimo del control de corriente
**
**     @param
**          dev     	   	- Puntero al dispositivo sobre el que recae 
**							la acción.
**     @param
**          toffm     	   	- Puntero a la posición de memoria en la cual
**							se almacenará la información del parámetro.
**
**     @return
**                         	- Estado salida del método. 
*/
/* ===================================================================*/
stp_st stp_setTOffMin(stp_device* dev, int8_t toffm){

	if(stp_setParam(dev,TOFF_MIN,&toffm,1)){
		printf("Error asignando TEOFF_MIN\n");
		return STP_ERROR;
	}

	return STP_OK;
}

/*
** ===================================================================
**     Método      :  stp_setOCDT
*/
/*!
**     @resumen
**          Asigna valor del OverCurrent Detection Threshold
**
**     @param
**          dev     	   	- Puntero al dispositivo sobre el que recae 
**							la acción.
**     @param
**          ocdt     	   	- Puntero a la posición de memoria en la cual
**							se almacenará la información del parámetro.
**
**     @return
**                         	- Estado salida del método. 
*/
/* ===================================================================*/
stp_st stp_setOCDT(stp_device* dev, int8_t ocdt){

	if(stp_setParam(dev,OCD_TH,&ocdt,1)){
		printf("Error asignando OCD_TH\n");
		return STP_ERROR;
	}

	return STP_OK;
}

/*
** ===================================================================
**     Método      :  stp_setStepSel
*/
/*!
**     @resumen
**          Asigna valor del selector de paso.
**
**     @param
**          dev     	   	- Puntero al dispositivo sobre el que recae 
**							la acción.
**     @param
**          step_sel     	   	- Puntero a la posición de memoria en la cual
**							se almacenará la información del parámetro.
**
**     @return
**                         	- Estado salida del método. 
*/
/* ===================================================================*/
stp_st stp_setStepSel(stp_device* dev, int8_t step_sel){

	(*dev).step=step_sel;

	step_sel &= 0x07;
	step_sel |= 0x88;

	if(stp_setParam(dev,STEP_MODE,&step_sel,1)){
		printf("Error asignando STEP_SEL\n");
		return STP_ERROR;
	}

	return STP_OK;
}
/*
** ===================================================================
**     Método      :  stp_setStepSel
*/
/*!
**     @resumen
**          Asigna el valor del selector de paso según aquél almacenado en el dispositivo.
**
**     @param
**          dev     	   	- Puntero al dispositivo sobre el que recae 
**							la acción.
**     @return
**                         	- Estado salida del método. 
*/
/* ===================================================================*/
stp_st stp_setDevStep(stp_device* dev){

	uint8_t step_sel=(*dev).step;
	
	step_sel &= 0x07;
	step_sel |= 0x88;

	if(stp_setParam(dev,STEP_MODE,&(*dev).step,1)){
		printf("Error asignando STEP_SEL\n");
		return STP_ERROR;
	}

	return STP_OK;
}


/*
** ===================================================================
**     Método      :  stp_setAlarmEn
*/
/*!
**     @resumen
**          Asigna valor al registro de alarmas activadas
**
**     @param
**          dev     	   	- Puntero al dispositivo sobre el que recae 
**							la acción.
**     @param
**          al_en	   	- Puntero a la posición de memoria en la cual
**							se almacenará la información del parámetro.
**
**     @return
**                         	- Estado salida del método. 
*/
/* ===================================================================*/
stp_st stp_setAlarmEn(stp_device* dev, int8_t al_en){

	if(stp_setParam(dev,ALARM_EN,&al_en,1)){
		printf("Error asignando ALARM_EN\n");
		return STP_ERROR;
	}

	return STP_OK;
}

/*
** ===================================================================
**     Método      :  stp_setConfig
*/
/*!
**     @resumen
**          Asigna valor del registro de configuración
**
**     @param
**          dev     	   	- Puntero al dispositivo sobre el que recae 
**							la acción.
**     @param
**          config	   	- Puntero a la posición de memoria en la cual
**							se almacenará la información del parámetro.
**
**     @return
**                         	- Estado salida del método. 
*/
/* ===================================================================*/
stp_st stp_setConfig(stp_device* dev, int32_t config){

	uint8_t* ptr=(uint8_t*)&config;

	if(stp_setParam(dev,CONFIG,ptr,2)){
		printf("Error asignando CONFIG\n");
		return STP_ERROR;
	}

	return STP_OK;
}


/*
** ===================================================================
**     Método      :  stp_clk_enable
*/
/*!
**     @resumen
**          Habilita el clk del dispositivos especificado por parámetro
**
**     @param
**          dev     	   	- Puntero al dispositivo sobre el que recae 
**							la acción.
*/
/* ===================================================================*/
stp_st stp_clk_enable(stp_device* dev){
	
	if(pwm_enable((*dev).pin_pwm)){
		printf("Error durante clk Enable de stepper . (stepper_robocol.c>stp_clk_enable)\n");
		return STP_ERROR;
	}

	printf("Realización de clk Enable de stepper exitosa. (stepper_robocol.c>stp_clk_enable)\n");
	
	return STP_OK;
}

/*
** ===================================================================
**     Método      :  stp_driver_enable
*/
/*!
**     @resumen
**          Habilita el dispositivo driver del especificado por parámetro.
**
**     @param
**          dev     	   	- Puntero al dispositivo sobre el que recae 
**							la acción.
*/
/* ===================================================================*/
stp_st stp_driver_enable(stp_device* dev){
	
	if (gpio_exp_set((*dev).pin_stndby)){
		printf("Error en el set del pin_stndby en stp_driver_enable de libreria stepper. (stepper_robocol.c>stp_driver_enable)\n");
		return STP_ERROR;
	}
	printf("Realización de driver Enable de stepper exitosa. (stepper_robocol.c>stp_driver_enable)\n");

	return STP_OK;
}

/*
** ===================================================================
**     Método      :  stp_output_enable
*/
/*!
**     @resumen
**          Habilita las salidas del dispositivo especificado por parámetro.
**
**     @param
**          dev     	   	- Puntero al dispositivo sobre el que recae 
**							la acción.
*/
/* ===================================================================*/
stp_st stp_output_enable(stp_device* dev){

	uint8_t* u8;
	
	if(stp_setDevStep(dev)){
		printf("Error seleccionando step durante output Enable de stepper. (stepper_robocol.c>stp_output_enable)\n");
		return STP_ERROR;
	}	

	if(stp_setParam(dev,ENABLE_STEPPER,u8,0)){
		printf("Error durante output Enable de stepper. (stepper_robocol.c>stp_output_enable)\n");
		return STP_ERROR;
	}

	printf("Realización de output Enable de stepper exitosa. (stepper_robocol.c>stp_output_enable)\n");


	return STP_OK;
}

/*
** ===================================================================
**     Método      :  stp_master_enable
*/
/*!
**     @resumen
**          Habilita el dispositivo especificado por parámetro, tanto
**			su clk, como las salidas del driver y el driver en si
**
**     @param
**          dev     	   	- Puntero al dispositivo sobre el que recae 
**							la acción.
*/
/* ===================================================================*/
stp_st stp_master_enable(stp_device* dev){
	
	if(stp_driver_enable(dev)){
		return STP_ERROR;
	}

	if(stp_clk_enable(dev)){
		return STP_ERROR;
	}

	if(stp_output_enable(dev)){
		return STP_ERROR;
	}


}		

/*
** ===================================================================
**     Método      :  stp_clk_disable
*/
/*!
**     @resumen
**          Deshabilita el clk del dispositivos especificado por parámetro
**
**     @param
**          dev     	   	- Puntero al dispositivo sobre el que recae 
**							la acción.
*/
/* ===================================================================*/
stp_st stp_clk_disable(stp_device* dev){
	
	if(pwm_disable((*dev).pin_pwm)){
		printf("Error durante clk disable de stepper . (stepper_robocol.c>stp_clk_disable)\n");
		return STP_ERROR;
	}

	printf("Realización de clk disable de stepper exitosa. (stepper_robocol.c>stp_clk_disable)\n");

	return STP_OK;
}

/*
** ===================================================================
**     Método      :  stp_driver_disable
*/
/*!
**     @resumen
**          Deshabilita el driver del dispositivo especificado por parámetro.
**
**     @param
**          dev     	   	- Puntero al dispositivo sobre el que recae 
**							la acción.
*/
/* ===================================================================*/

stp_st stp_driver_disable(stp_device* dev){
	
	if (gpio_exp_clear((*dev).pin_stndby)){
		printf("Error en el set del pin_stndby en stp_driver_disable de libreria stepper. (stepper_robocol.c>stp_driver_disable)\n");
		return STP_ERROR;
	}
	printf("Realización de driver disable de stepper exitosa. (stepper_robocol.c>stp_driver_disable)\n");

	return STP_OK;
}

/*
** ===================================================================
**     Método      :  stp_output_disable
*/
/*!
**     @resumen
**          Deshabilita las salidas del dispositivo especificado por parámetro.
**
**     @param
**          dev     	   	- Puntero al dispositivo sobre el que recae 
**							la acción.
*/
/* ===================================================================*/
stp_st stp_output_disable(stp_device* dev){

	uint8_t* u8;

	if(stp_setParam(dev,DISABLE_STEPPER,u8,0)){
		printf("Error durante output disable de stepper. (stepper_robocol.c>stp_output_disable)\n");
		return STP_ERROR;
	}

	printf("Realización de output disable de stepper exitosa. (stepper_robocol.c>stp_output_disable)\n");


	return STP_OK;
}

/*
** ===================================================================
**     Método      :  stp_master_disable
*/
/*!
**     @resumen
**          Deshabilita el dispositivo especificado por parámetro, tanto
**			su clk, como las salidas del driver y el driver en si
**
**     @param
**          dev     	   	- Puntero al dispositivo sobre el que recae 
**							la acción.
*/
/* ===================================================================*/
stp_st stp_master_disable(stp_device* dev){
	
	if(stp_output_disable(dev)){
		return STP_ERROR;
	}
	if(stp_clk_disable(dev)){
		return STP_ERROR;
	}
	if(stp_driver_disable(dev)){
		return STP_ERROR;
	}

}

/*
** ===================================================================
**     Método      :  stp_period
*/
/*!
**     @resumen
**          Cambia el periodo del pwm para el manejo del stepper.
**
**     @param
**          dev     	   	- Puntero al dispositivo sobre el que recae 
**							la acción.
*/
/* ===================================================================*/
stp_st stp_period(stp_device* dev, uint32_t period){

	if(pwm_set_period((*dev).pin_pwm,period)){
		printf("Error Deshabilitando el pwm desde galileo para manejo de stepper\n");
		return STP_ERROR;
	}
	(*dev).period=period;
	return STP_OK;

}


/*
** ===================================================================
**     Método      :  stp_dir
*/
/*!
**     @resumen
**          Cambia la dirección de giro del stepper según especificado.
**
**     @param
**          dev     	   	- Puntero al dispositivo sobre el que recae 
**							la acción.
**     @param
**          dir     	   	- Dirección a asignar. Los posibles valores 
**							son: CLOCKWISE o COUNTERCLOCKWISE
*/
/* ===================================================================*/
stp_st stp_dir(stp_device* dev, uint32_t dir){
	if(dir==CLOCKWISE){
		if(gpio_gal_clear((*dev).pin_dir)){
			printf("Error cambiando dirección CLOCK_WISE\n");
			return STP_ERROR;
		}
		return STP_OK;
	}else if(dir==COUNTERCLOCKWISE){
			if(gpio_gal_set((*dev).pin_dir)){
			printf("Error cambiando direccion a COUNTER_CLOCK_WISE\n");
			return STP_ERROR;
		}
		return STP_OK;	
	}
}

/*
** ===================================================================
**     Método      :  stp_move_degrees
*/
/*!
**     @resumen
**          Gira el stepper un angulo determinado respecto a su 
**			posicipon actual, en  la dirección indicada. Este giro 
**			tiene en cuenta la relación especificada en el campo 
**			gear ratio del dispositivo.
**
**     @param
**          dev     	   	- Puntero al dispositivo sobre el que recae 
**							la acción.
**     @param
**          degrees     	- Grados hexadecimales de giro
**     @param
**          dir     	   	- Dirección a asignar. Los posibles valores 
**							son: CLOCKWISE o COUNTERCLOCKWISE
*/
/* ===================================================================*/


stp_st stp_move_degrees(stp_device* dev, uint32_t degrees, int32_t dir){
	int32_t 	init_pos,curr_pos;
	int32_t		t;
	uint8_t 	step=((*dev).step>4)? 4 : (*dev).step; 
	uint32_t	goal_pos=(int)(degrees*pow(2,step)*((*dev).gear_ratio)/1.8);

	printf("Goal Pos: %d\n",goal_pos);
	stp_clk_disable(dev);
	stp_dir(dev,dir);

	if(stp_getPosition(dev,&init_pos)){
		printf("Error getting current position. (stepper_robocol.c>stp_relative_displacement)\n");
	}

	t=goal_pos*(*dev).period;

	stp_output_enable(dev);
	stp_clk_enable(dev);

	usleep(t);

	stp_clk_disable(dev);
	stp_output_disable(dev);
}

/*
** ===================================================================
**     Método      :  stp_return_to_cero
*/
/*!
**     @resumen
**          Gira el stepper un angulo determinado respecto a su 
**			posicipon actual, en  la dirección indicada. Este giro 
**			tiene en cuenta la relación especificada en el campo 
**			gear ratio del dispositivo.
**
**     @param
**          dev     	   	- Puntero al dispositivo sobre el que recae 
**							la acción.
**     @param
**          degrees     	- Grados hexadecimales de giro
**     @param
**          dir     	   	- Dirección a asignar. Los posibles valores 
**							son: CLOCKWISE o COUNTERCLOCKWISE
*/
/* ===================================================================*/
stp_st stp_return_to_cero(stp_device* dev){
	int32_t 	pos,curr_degrees;
	int32_t		rel_pos=0;
	uint8_t 	step=((*dev).step>4)? 4 : (*dev).step; 

	if(stp_getPosition(dev,&pos)){
		printf("Error getting current position. (stepper_robocol.c>stp_relative_displacement)\n");
	}

	while(pos<(-5)||pos>5){
		curr_degrees=(int)((pos*(1.8))/(pow(2,step)));
		printf("curr_degrees: %d\n",curr_degrees);

		if(curr_degrees>0){
			if(stp_move_degrees(dev,abs(curr_degrees),CLOCKWISE)){
				printf("Error returning to cero. (stepper_robocol.c>stp_return_to_cero)\n");
			}
		}else{
			if(stp_move_degrees(dev,abs(curr_degrees),COUNTERCLOCKWISE)){
				printf("Error returning to cero. (stepper_robocol.c>stp_return_to_cero)\n");
			}
		}
		if(stp_getPosition(dev,&pos)){
			printf("Error getting current position. (stepper_robocol.c>stp_relative_displacement)\n");
		}
	}
}

/*
** ===================================================================
**     Método      :  stp_move_time
*/
/*!
**     @resumen
**          Gira el stepper un angulo determinado respecto a su 
**			posicipon actual, en  la dirección indicada. Este giro 
**			tiene en cuenta la relación especificada en el campo 
**			gear ratio del dispositivo.
**
**     @param
**          dev     	   	- Puntero al dispositivo sobre el que recae 
**							la acción.
**     @param
**          degrees     	- Grados hexadecimales de giro
**     @param
**          dir     	   	- Dirección a asignar. Los posibles valores 
**							son: CLOCKWISE o COUNTERCLOCKWISE
*/
/* ===================================================================*/
stp_st stp_move_time(stp_device* dev, int32_t t){
	stp_output_enable(dev);
	stp_clk_enable(dev);

	usleep(t);

	stp_clk_disable(dev);
	stp_output_disable(dev);
}
