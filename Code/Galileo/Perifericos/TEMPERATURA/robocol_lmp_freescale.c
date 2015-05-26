#include "robocol_lmp.h"

/* Creacion de variables globales y estructura de datos del LMP*/
lmp_ocupado = FALSE;

st_lmp build_lmp(char* ruta, uint32_t speed, uint8_t cs){
	if(spi_start(*ruta,speed)!=0){
		printf("Error en la configuracion inicial del dispositivo spi\n");
		return LMP_ERROR;
	}
	if(spi_create_device( &spi_dev,0,cs )!=0){
		printf("Error en la inicialización del dispositivo spi\n");
		return LMP_ERROR;
	}
	return LMP_OK;
}

/*
** ===================================================================
**     Método      :  escribir_reg_lmp
*/
/*!
**     @resumen
**         Escribe cierta cantidad de datos en un registro seleccio-
**         nado. Puede escribir más de un dato de forma secuencial lo
**         cual permite escribir registros aledaños.
**     @param
**         address     	   - Dirección del registro
**     @param
**         data     	   - Puntero a los datos que se van a escribir
**     @param
**         num	     	   - Número de bytes a escribir.
**     @return
**                         - Estado de la comuniación antes de enviar. 
*/
/* ===================================================================*/
st_lmp escribir_reg_lmp(uint8_t address,char* data,uint8_t num){
	uint8_t caracter[3+num];
	uint8_t datos[3+num];
				
	if(lmp_ocupado){
			return LMP_ESPERA;
	}
	else{
		char tr1[16]=(WRITE_ADDR<<8)|(FILLER_T1<<4)|(address>>4);
		char rx1[16];
		spi_rw(&spi_dev,&tr1,&rx1,16);
		lmp_ocupado=TRUE;
		
		char tr2[8+num]=(WRITE<<(num+7))|(SZS<<(num+5))|((address&LOWADDR_SL)<<(num-4))|*data;
		char rx2[8+num];
		spi_rw(&spi_dev,&tr2,&rx2,8+num);
	}
	return LMP_OK;
}

/*
** ===================================================================
**     Método      :  leer_reg_lmp
*/
/*!
**     @resumen
**         Lee cierta cantidad de datos  un registro seleccionado. 
**         Puede leer más de un dato lo cual permite leer registros 
**         aledaños. 
**     @param
**         address     	   - Dirección del registro
**     @param
**         data     	   - Puntero a la variable donde se van a 
**         					 escribir los datos.
**     @param
**         num	     	   - Número de bytes a leer
**     @return
**                         - Estado de la comuniación antes de enviar. 
*/
/* ===================================================================*/
st_lmp leer_reg_lmp(uint8_t address,char* data,uint8_t num){
	uint8_t caracter[3+num];
	int i=3;
			
	if(lmp_ocupado){
			return LMP_ESPERA;
	}
	else{
		char tr1[16]=(WRITE_ADDR<<8)|(FILLER_T1<<4)|(address>>4);
		char rx1[16];
		spi_rw(&spi_dev,&tr1,&rx1,16);
		lmp_ocupado=TRUE;
		
		char tr2[8+num]=(READ<<(num+7))|(SZS<<(num+5))|((address&LOWADDR_SL)<<(num-4))|*data;
		spi_rw(&spi_dev,&tr2,data,8+num);
	}
	return LMP_OK;	
}

/*
** ===================================================================
**     Método      :  leer_reg_lmp
*/
/*!
**     @resumen
**         Retorna el estado actual de la comunicación con el lmp.
**     @return
**                         - Estado actual de la comuniación. 
*/
/* ===================================================================*/
st_lmp estado_lmp(void){
	if(lmp_ocupado){
		return LMP_ESPERA;
	}else{
		return LMP_OK;
	}
}

/*
** ===================================================================
**     Método      :  ch_config_lmp
*/
/*!
**     @resumen
**         Configura un canal del LMP90100 según lo requerido por 
**         el usuario. 
**     @param
**         setup     	   - Estructura que contiene los parámetros de
**         					 configuración del canal.
**     @return
**                         - Estado de la comuniación antes de enviar. 
*/
/* ===================================================================*/
st_lmp ch_config_lmp(LMP_ch_config_struct opciones){
	uint8_t control_reg=opciones.channel+CHx_INPUTCN;
	uint8_t config_reg=opciones.channel+CHx_CONFIG;
	char control=((opciones.burnout)<<7) + ((opciones.vref)<<6) + ((opciones.vinp)<<3) + (opciones.vinn);
	char configuracion=((opciones.odr_sel)<<4) + ((opciones.ganancia)<<1) + (opciones.buffer_enable);
	if(lmp_ocupado){
		return LMP_ESPERA;
	}else{
		escribir_reg_lmp(control_reg,&control,1);
		while(estado_lmp()!=LMP_OK);
		escribir_reg_lmp(config_reg,&configuracion,1);		
		return LMP_OK;
	}
}

/*
** ===================================================================
**     Método      :  io_config_lmp
*/
/*!
**     @resumen
**         Configura los pines de proposito general como salida o como
**         entrada. Si el bit correspondiente es 1, ese bit sera salida
**         de lo contrario ese bit sera entrada. 
**     @param
**         salidas     	   - Se ponen en 1 los bits que funcionaran como 
**         					 salida.
**     @return
**                         - Estado de la comuniación antes de enviar. 
*/
/* ===================================================================*/
st_lmp io_config_lmp(char salidas){
	if(lmp_ocupado){
		return LMP_ESPERA;
	}else{
		escribir_reg_lmp(GPIO_DIRCN,&salidas,1);				
		return LMP_OK;
	}
}

/*
** ===================================================================
**     Método      :  escribir_io_lmp
*/
/*!
**     @resumen
**         Escribe el valor deseado en los puertos digitales. Los bits
**         deben estar habilitados como salidas en el registro GPIO_DIRCN
**         sino la asignación del valor no tendrá efecto.
**     @param
**         salidas     	   - Se coloca el valor del char en los pines
**         					 del puerto paralelo asignados como salida.
**     @return
**                         - Estado de la comuniación antes de enviar. 
*/
/* ===================================================================*/
st_lmp escribir_io_lmp(char salidas){
	if(lmp_ocupado){
		return LMP_ESPERA;
	}else{
		escribir_reg_lmp(GPIO_DAT,&salidas,1);				
		return LMP_OK;
	}
}

/*
** ===================================================================
**     Método      :  leer_io_lmp
*/
/*!
**     @resumen
**         Lee el valor del bit indicado y lo devuelve en la dirección
**         indicada.
**     @param
**         bit	     	   - Indica cual es el bit a leer (D0-D6).
**     @param
**         valor     	   - Puntero que guarda el valor del bit en un
**         					 un uint8_t (TRUE=1, FALSE=0).
**     @return
**                         - Estado de la comuniación antes de enviar. 
*/
/* ===================================================================*/
st_lmp leer_io_lmp(char bit, uint8_t* valor){
	char registro=NULL;
	if(lmp_ocupado){
		return LMP_ESPERA;
	}else{
		leer_reg_lmp(GPIO_DAT,&registro,1);
		while(estado_lmp()!=LMP_OK);
		if((bit&registro)==bit){
			*valor=TRUE;
		}else{
			*valor=FALSE;			
		}
		return LMP_OK;
	}
}

/*
** ===================================================================
**     Método      :  config_bkcal_lmp
*/
/*!
**     @resumen
**         Configura las opciones de calibración en segundo plano.
**     @param
**         mode	    	   - Modo de operación de la calibración en
**         					 segundo plano.
**     @return
**                         - Estado de la comuniación antes de enviar. 
*/
/* ===================================================================*/
st_lmp config_bkcal_lmp(char mode){
	if(lmp_ocupado){
		return LMP_ESPERA;
	}else{
		escribir_reg_lmp(BGCALCN,&mode,1);
		return LMP_OK;
	}
}

/*
** ===================================================================
**     Método      :  ajustar_cero_lmp
*/
/*!
**     @resumen
**         Ajusta el cero llenando los registros de offset. El procedi-
**         miento de ajuste del cero es el siguiente:
**         	
**         	1. Aplicar la condición de cero al canal que se va a ajustar.
**         	Ésta condición se aplica con un patrón de medición.
**         	2. Llamar este método para empezar el ajuste.
**         	3. El sistema entrará en modo de determinación del offset y
**         	llenará los registros CHx_SCAL_OFFSET del canal correspon-
**         	diente.
**         	4. El procedimiento debería repetirse varias veces anotando
**         	los valores obtenidos en cada iteración. Al final, se podrá
**         	determinar la precisión del sensor y el valor promedio del
**         	offset.
**     @param
**         canal    	   - Canal que se va a ajustar (CH0-CH3).
**     @param
**         offset    	   - Offset obtenido después de la calibración.
**     @return
**                         - Estado de la comuniación antes de enviar. 
*/
/* ===================================================================*/
st_lmp ajustar_cero_lmp(uint8_t canal, unsigned int* offset){
	char mode=0x01;
	uint8_t lectura[6];
	LDD_TData *receive=&lectura;
	uint8_t reg=CHx_SCAL_OFFSET+(canal*4);
	if(lmp_ocupado){
		return LMP_ESPERA;
	}else{
		escribir_reg_lmp(SCALCN,&mode,1);
		while(estado_lmp()!=LMP_OK);
		leer_reg_lmp(reg,receive,3);
		while(estado_lmp()!=LMP_OK);
		*offset=(lectura[3]<<16)+(lectura[4]<<8)+lectura[5];		
		return LMP_OK;
	}
}

/*
** ===================================================================
**     Método      :  ajustar_ganancia_lmp
*/
/*!
**     @resumen
**         Ajusta la ganancia llenando los registros de ganancia. El procedi-
**         miento de ajuste de ganancia es el siguiente:
**         	
**         	1. Ajustar el cero. 
**         	2.Aplicar la condición de referencia al canal que se va a ajustar.
**         	Ésta condición se aplica con un patrón de medición.
**         	2. Llamar este método para empezar el ajuste. Se debe indicar la
**         	ganancia esperada. 
**         	3. El sistema entrará en modo de determinación de ganancia y
**         	llenará los registros CHx_SCAL_GAIN del canal correspondiente.
**         	4. El procedimiento debería repetirse varias veces anotando
**         	los valores obtenidos en cada iteración. Al final, se podrá
**         	determinar la precisión del sensor y el valor promedio de la
**         	ganancia.
**     @param
**         canal    	   - Canal que se va a ajustar (CH0-CH3).
**     @param
**         ganancia_deseada	   - Ganancia deseada antes de la calibración.
**     @param
**         ganancia    	   - Ganancia obtenida después de la calibración.
**     @return
**                         - Estado de la comuniación antes de enviar. 
*/
/* ===================================================================*/
st_lmp ajustar_ganacia_lmp(uint8_t canal, unsigned int ganancia_deseada,unsigned int* ganancia){
	char mode=0x02;
	uint8_t lectura[6];
	LDD_TData *receive=&lectura;
	uint8_t reg=CHx_SCAL_GAIN+(canal*4);
	char g1=(0xFF0000&ganancia_deseada)>>16;
	char g2=(0xFF00&ganancia_deseada)>>8;
	char g3=(0xFF&ganancia_deseada);
	if(lmp_ocupado){
		return LMP_ESPERA;
	}else{
		escribir_reg_lmp(reg,&g1,1);
		while(estado_lmp()!=LMP_OK);
		escribir_reg_lmp(reg+1,&g2,1);
		while(estado_lmp()!=LMP_OK);
		escribir_reg_lmp(reg+2,&g3,1);
		while(estado_lmp()!=LMP_OK);
		escribir_reg_lmp(SCALCN,&mode,1);
		while(estado_lmp()!=LMP_OK);
		leer_reg_lmp(reg,receive,3);
		while(estado_lmp()!=LMP_OK);
		*ganancia=(lectura[3]<<16)+(lectura[4]<<8)+lectura[5];		
		return LMP_OK;
	}
}

/*
** ===================================================================
**     Método      :  post_cal_lmp
*/
/*!
**     @resumen
**         Configura las opciones de scaling y corrimiento.
**     @param
**         canal    	   - Canal que se ajusta.
**     @param
**         scal	    	   - Valor de multiplicación (max 63).
**     @param
**         selector    	   - Valor del corrimiento (max 7).
**     @return
**                         - Estado de la comuniación antes de enviar. 
*/
/* ===================================================================*/
st_lmp post_cal_lmp(uint8_t canal, char scal, char selector){
	char reg = CHx_SCAL_SCALING + (canal*4);
	char reg1 = CHx_SCAL_BITS_SELECTOR + (canal*4);
	if(lmp_ocupado){
		return LMP_ESPERA;
	}else{
		escribir_reg_lmp(reg,&scal,1);
		while(estado_lmp()!=LMP_OK);
		escribir_reg_lmp(reg1,&selector,1);
		while(estado_lmp()!=LMP_OK);
		return LMP_OK;
	}
}

/*
** ===================================================================
**     Método      :  config_scan_lmp
*/
/*!
**     @resumen
**         Configura el modo de escaneo además del orden de escaneo si
**         se seleccionaron varios canales.
**     @param
**         mode	    	   - Modo de escaneo (MODE0-MODE3).
**     @param
**         primer_canal	   - Primer canal que se escanea.
**     @param
**         ultimo_canal	   - Último canal que se escanea.
**     @return
**                         - Estado de la comuniación antes de enviar. 
*/
/* ===================================================================*/
st_lmp config_scan_lmp(char mode, char primer_canal, char ultimo_canal){
	char recibido[4];
	LDD_TData *receive=&recibido;
	char reg = (mode<<6)+(ultimo_canal<<3)+(primer_canal);
	if(lmp_ocupado){
		return LMP_ESPERA;
	}else{
		leer_reg_lmp(CH_STS,receive,1);
		while(estado_lmp()!=LMP_OK);
		if((recibido[3]&0x02)==(0x02)){
			return LMP_ERROR;
		}else{
			escribir_reg_lmp(CH_SCAN,&reg,1);
		}		
		return LMP_OK;
	}
}

/*
** ===================================================================
**     Método      :  config_corr_lmp
*/
/*!
**     @resumen
**         Configura el valor de las fuentes de corriente IB1 e IB2. Los
**         valores van desde 0uA hasta 1000uA.
**     @param
**         corriente   	   - Valor de corriente en multiplos de 100uA.
**     @return
**                         - Estado de la comuniación antes de enviar. 
*/
/* ===================================================================*/
st_lmp config_corr_lmp(char corriente){
	if(lmp_ocupado){
		return LMP_ESPERA;
	}else{
		escribir_reg_lmp(ADC_AUXCN,&corriente,1);
		return LMP_OK;
	}
}

/*
** ===================================================================
**     Método      :  config_umbral_lmp
*/
/*!
**     @resumen
**         Configura el umbral del lmp.
**     @param
**         umbral    	   - Valor del umbral.
**     @return
**                         - Estado de la comuniación antes de enviar. 
*/
/* ===================================================================*/
st_lmp config_umbral_lmp(uint8_t umbral){
	char regh = (umbral&0xFF00)>>8;
	char regl = (umbral&0x00FF);
	if(lmp_ocupado){
		return LMP_ESPERA;
	}else{
		escribir_reg_lmp(SENDIAG_THLDH,&regh,1);
		while(estado_lmp()!=LMP_OK);
		escribir_reg_lmp(SENDIAG_THLDL,&regl,1);
		return LMP_OK;
	}
}

/*
** ===================================================================
**     Método      :  activar_diagnostico_lmp
*/
/*!
**     @resumen
**         Este método activa el modo de escaneo 3 el cual se utiliza
**         para actualizar las banderas de diagnóstico.
**     @return
**                         - Estado de la comuniación antes de enviar. 
*/
/* ===================================================================*/
st_lmp activar_diagnostico_lmp(void){
	char recibido[4];
	LDD_TData *receive=&recibido;
	char ch1;
	char ch2;
	if(lmp_ocupado){
		return LMP_ESPERA;
	}else{
		leer_reg_lmp(CH_SCAN,receive,1);
		while(estado_lmp()!=LMP_OK);		
		ch1=recibido[3]&0x07;
		ch2=(recibido[3]&0x38)>>3;
		config_scan_lmp(MODE3, ch1, ch2);		
		return LMP_OK;
	}
}

/*
** ===================================================================
**     Método      :  diagnosticar_lmp
*/
/*!
**     @resumen
**         Realiza un diagnóstico del sensor. Los canales diagnosticados
**         deben tener la corriente de burnout habilitada.
**     @param
**         bandera    	   - Indica cual bandera se activo o devuelve
**         					 FLAGS_OK.
**     @param
**         canal    	   - Canal que está siendo muestreado.
**     @return
**                         - Estado de la comuniación antes de enviar. 
*/
/* ===================================================================*/
st_lmp diagnosticar_lmp(char* bandera,char* canal){
	char recibido[4];	
	LDD_TData *receive=&recibido;
	if(lmp_ocupado){
		return LMP_ESPERA;
	}else{
		leer_reg_lmp(SENDIAG_FLAGS,receive,1);
		while(estado_lmp()!=LMP_OK);		
		if((recibido[3]&0x80)==0x80){//Bandera de corto circuito
			*bandera=SHORT_THLD;
		}else if((recibido[3]&0x40)==0x40){//Bandera de Voltaje cercano a los Voltajes de referencia
			*bandera=RAILS;
		}else if((recibido[3]&0x20)==0x20){//Bandera de power on reset
			*bandera=POR_AFT_LST_RD;
		}else if((recibido[3]&0x18)!=0x00){//Banderas de overflow
			*bandera=OFLO;
		}else{
			*bandera=FLAGS_OK;
		}
		*canal=recibido[3]&0x07;
		return LMP_OK;
	}
}

/*
** ===================================================================
**     Método      :  power_lmp
*/
/*!
**     @resumen
**         Cambia el estado de la alimentación del dispositivo.
**     @param
**         mode	    	   - Modo de operación deseado (LMP_STBY,LMP_OFF,
**         					 LMP_ON).
**     @return
**                         - Estado de la comuniación antes de enviar. 
*/
/* ===================================================================*/
st_lmp power_lmp(char mode){
	if(lmp_ocupado){
		return LMP_ESPERA;
	}else{
		escribir_reg_lmp(POWERCN,&mode,1);
		return LMP_OK;
	}
}

/*
** ===================================================================
**     Método      :  config_DRYB_lmp
*/
/*!
**     @resumen
**         Configura el pin DRYB para que salga por D6.
**     @return
**                         - Estado de la comuniación antes de enviar. 
*/
/* ===================================================================*/
st_lmp config_DRYB_lmp(void){
	char msg=0x83;
	if(lmp_ocupado){
		return LMP_ESPERA;
	}else{
		escribir_reg_lmp(SPI_DRDYBCN,&msg,1);
		return LMP_OK;
	}
}

/*
** ===================================================================
**     Método      :  resetar_lmp
*/
/*!
**     @resumen
**         Resetea el dispositivo y si es necesario resetea los registros 
**         de calibración.
**     @param
**         todos    	   - Resetea los registros de calibración.
**     @return
**                         - Estado de la comuniación antes de enviar. 
*/
/* ===================================================================*/
st_lmp resetear_lmp(uint8_t todos){
	char msg=0xC3;
	char reset=0x40;
	if(lmp_ocupado){
		return LMP_ESPERA;
	}else{
		if(todos){
			escribir_reg_lmp(ADC_AUXCN,&reset,1);		
			while(estado_lmp()!=LMP_OK);		
		}
		escribir_reg_lmp(RESETCN,&msg,1);
		return LMP_OK;
	}
}

/*
** ===================================================================
**     Método      :  leer_ADC_lmp
*/
/*!
**     @resumen
**         Lee la conversión del ADC.
**     @param
**         lectura    	   - Puntero que almacena la lectura.
**     @return
**                         - Estado de la comuniación antes de enviar. 
*/
/* ===================================================================*/
st_lmp leer_ADC_lmp(int* lectura){
	char recibido[6];	
	LDD_TData *receive=&recibido;
	if(lmp_ocupado){
		return LMP_ESPERA;
	}else{
		leer_reg_lmp(ADC_DONE,receive,1);		
		while(estado_lmp()!=LMP_OK);
		if(recibido[3]==0xFF){
			return LMP_ERROR;
		}
		leer_reg_lmp(ADC_DOUT,receive,3);
		while(estado_lmp()!=LMP_OK);
		*lectura=(recibido[3]<<16)+(recibido[4]<<8)+recibido[5];
		return LMP_OK;
	}
}
