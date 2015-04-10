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

	spi_device new_spi;
	spi_create_device(&new_spi,0,(*dev).pin_cs);
	(*dev).spi=&new_spi;

	spi_start("/dev/spidev1.0",100000);

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
	
	uint8_t tx=param||(0x01<<6);				//Se agrega el bit que indica escritura
	uint8_t rx=0;

	if(spi_rw((*dev).spi, &tx, &rx,1)){
		printf("Error en SPI\n");
		return STP_ERROR;
	}

	int i;
	for(i=0;i<len;i++){

		tx=*(buff+(len-i-1));
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

	if(spi_rw((*dev).spi, &tx, &rx,1)){
		printf("Error en SPI\n");
		return STP_ERROR;
	}
	int i;
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

	uint8_t buff[2];

	if(stp_getParam(dev,ABS_POS,buff,3)){
		printf("Error obteniendo posición\n");
		return STP_ERROR;
	}

	*pos = array_to_i32(buff,3);

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
stp_st stp_getTVAL(stp_device* dev, int8_t* tval){


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

	*osc_sel=array_to_i32(buff,3);

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

	*stats=array_to_i32(buff,3);

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
stp_st stp_setPosition(stp_device* dev, int32_t* pos){

	uint8_t* ptr=(uint8_t*)&pos;
	if(stp_setParam(dev,ABS_POS,&ptr[1],3)){
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
stp_st stp_setTVAL(stp_device* dev, int8_t* tval){

	if(stp_setParam(dev,TVAL,tval,1)){
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
stp_st stp_setTONMIN(stp_device* dev, int8_t* tonm){

	if(stp_setParam(dev,TON_MIN,tonm,1)){
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
stp_st stp_setTOffMin(stp_device* dev, int8_t* toffm){

	if(stp_setParam(dev,TOFF_MIN,toffm,1)){
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
stp_st stp_setOCDT(stp_device* dev, int8_t* ocdt){

	if(stp_setParam(dev,OCD_TH,ocdt,1)){
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
stp_st stp_setStepSel(stp_device* dev, int8_t* step_sel){

	*step_sel &= 0x07;
	*step_sel |= 0x88;

	if(stp_setParam(dev,STEP_MODE,step_sel,1)){
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
stp_st stp_setAlarmEn(stp_device* dev, int8_t* al_en){

	if(stp_setParam(dev,ALARM_EN,al_en,1)){
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
stp_st stp_setConfig(stp_device* dev, int32_t* config){

	uint8_t* ptr=(uint8_t*)&config;

	if(stp_setParam(dev,CONFIG,&ptr[2],2)){
		printf("Error asignando CONFIG\n");
		return STP_ERROR;
	}

	return STP_OK;
}

/*
** ===================================================================
**     Método      :  stp_enable
*/
/*!
**     @resumen
**          Habilita el dispositivo especificado por parámetro
**
**     @param
**          dev     	   	- Puntero al dispositivo sobre el que recae 
**							la acción.
*/
/* ===================================================================*/
stp_st stp_enable(stp_device* dev){


	if(stp_setParam(dev,ENABLE,NULL,0)){
		printf("Error asignando posición\n");
		return STP_ERROR;
	}

	return STP_OK;
}

/*
** ===================================================================
**     Método      :  stp_disable
*/
/*!
**     @resumen
**          Deshabilita el dispositivo especificado por parámetro
**
**     @param
**          dev     	   	- Puntero al dispositivo sobre el que recae 
**							la acción.
*/
/* ===================================================================*/
stp_st stp_disable(stp_device* dev){


	if(stp_setParam(dev,DISABLE,NULL,0)){
		printf("Error asignando posición\n");
		return STP_ERROR;
	}

	return STP_OK;
}