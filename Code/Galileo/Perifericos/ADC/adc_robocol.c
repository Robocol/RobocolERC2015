#include "adc_robocol.h"
/*
** ===================================================================
**     Método      :  build_adc
*/
/*!
**     @resumen
**				Realiza la habilitación para funcionamiento del ADC con
**				el número especificado en el apuntador a la estructura de
**  	        adc_dev pasada por parámetro.
**     @param
**         devptr  - Puntero hacia estructura adc_dev que contiene
**					 el file descriptor, numero y buffer de lectura del
**				     ADC.
**
*/
/* ===================================================================*/
adc_st build_adc(adc_dev* devptr){
	char* gexport= malloc(sizeof(char));
	uint8_t adc_num=(*devptr).adc_num;

	if (adc_num<0 || adc_num>6)
	{
		printf("El número de ADC especificado en la estructura no es valido.\n");
		return ADC_ERROR;
	}

	if (adc_num==4 || adc_num==5)
	{
		gexport=u8toa(GPIO29);
		gpio_set_dir(GPIO29, OUT);
		gpio_gal_value(GPIO29, 1);
	}

	uint8_t* gpio_adc;
	gpio_adc=malloc(sizeof(uint8_t));
	if(parse_adc_gpio(adc_num,gpio_adc)!=0){
		printf("El número de ADC especificado para el parser_adc_gpio no es valido\n");
		return ADC_ERROR;
	}
	gexport=u8toa(*gpio_adc);
	gpio_set_dir(*gpio_adc, OUT);
	gpio_gal_value(*gpio_adc, 0);

	// char* ruta=malloc(40);
	// if(parse_adc_route(adc_num,ruta)!=0){
	// 	printf("Error en parse_adc_route para la lectura del ADC%d\n",adc_num);
	// 	return ADC_ERROR;
	// }

	// if(((*devptr).fd=open(ruta,O_RDONLY))<0){
	// 	printf("Error al abrir fd en %s\n",ruta );
	// 	perror("Descripción");
	// 	free(ruta);
	// 	return ADC_ERROR;
	// }

	// free(ruta);
	return ADC_OK;
}
/*
** ===================================================================
**     Método      :  get_adc_value
*/
/*!
**     @resumen
**				   - Obtiene el último valor leído por el ADC
**     @param
**         devptr  - Puntero hacia estructura adc_dev que contiene
**					 el file descriptor, numero y buffer de lectura del
**				     ADC.
*/
/* ===================================================================*/
adc_st get_adc_value(adc_dev* devptr){
	char* ruta=malloc(40);
	uint8_t adc_num=(*devptr).adc_num;
	if(parse_adc_route(adc_num,ruta)!=0){
		printf("Error en parse_adc_route para la lectura del ADC%d\n",adc_num);
		return ADC_ERROR;
	}

	if(((*devptr).fd=open(ruta,O_RDWR))<0){
		printf("Error al abrir fd en %s\n",ruta );
		perror("Descripción");
		free(ruta);
		return ADC_ERROR;
	}
	free(ruta);
	char *buff;
	buff=malloc(4*sizeof(char));

	if(read((*devptr).fd,buff,4)<0){
		printf("Error al leer valor de ADC: %d\n",adc_num );
		perror("Descripción");
		return ADC_ERROR;
	}
	*((*devptr).data)=atoi(buff);
	free(buff);
	close((*devptr).fd);
	return ADC_OK;
}

adc_st get_voltage(adc_dev *devptr,float* voltage){
	if (get_adc_value(devptr)){
		return ADC_ERROR;
	}
	(*voltage)=(*((*devptr).data))*(((*devptr)).v_ref)/4095;
	return ADC_OK;
}

/*
** ===================================================================
**     Método      :  parse_adc_gpio
*/
/*!
**     @resumen
**				   - Realiza una conversión entre el número de ADC pasado
**					 por parámetro y el correspondiente GPIO de la galileo
**     @param
**			adc_num 
**				   - Numero de ADC en la Galileo. Variable con rango permitido
**					 de 0 a 5.
**
**			gpio_adc 
**				   - Puntero que refiere al lugar donde se almacena el
**					 número de GPIO asociado con el número de ADC pasado por 
**					 parámetro.
*/
/* ===================================================================*/
adc_st parse_adc_gpio(uint8_t adc_num ,uint8_t* gpio_adc ){
	if (adc_num==0){
		*gpio_adc=AD0;
		return ADC_OK;
	}else if(adc_num==1){
		*gpio_adc=AD1;
		return ADC_OK;
	}else if(adc_num==2){
		*gpio_adc=AD2;
		return ADC_OK;
	}else if(adc_num==3){
		*gpio_adc=AD3;
		return ADC_OK;
	}else if(adc_num==4){
		*gpio_adc=AD4;
		return ADC_OK;
	}else if(adc_num==5){
		*gpio_adc=AD5;
		return ADC_OK;
	}
	return ADC_ERROR;
}

/*
** ===================================================================
**     Método      :  parse_adc_route
/*!
**     @resumen
**				   - Realiza una conversión entre el número de ADC pasado
**					 por parámetro y un char array que contiene la 
**					 ruta de lectura para la obtención del valor del ADC.
**     @param
**			adc_num 
**				   - Numero de ADC en la Galileo. Variable con rango permitido
**					 de 0 a 5.
**
**			route_adc 
**				   - Puntero que refiere al lugar donde se almacena el
**					 la ruta par lectura  asociada con el número de ADC 
**					 pasado por parámetro.
*/
/* ===================================================================*/
adc_st parse_adc_route(uint8_t adc_num, char* ruta){

	if(sprintf(ruta ,"/sys/bus/iio/devices/iio:device0/in_voltage%d_raw",adc_num)<0){
		return ADC_ERROR;
	}
	return ADC_OK;
}

/*(UTIL SI SE MEJORA LA MANERA DE OBTENER LOS DATOS DEL ADC, i.e. ,
** no cerrando y abriendo el archivo de lectura cada vez que se obtiene
** el dato del dispositivo)
** ===================================================================
**     Método      :  close_adc
/*!
**     @resumen
**				Cierra el file descriptor asociado con el archivo de
**				lectura del valor del ADC.
**     @param
**         devptr  - Puntero hacia estructura adc_dev que contiene
**					 el file descriptor, numero y buffer de lectura del
**				     ADC.
**
/* ===================================================================*/
adc_st close_adc(adc_dev* devptr){
	if(close((*devptr).fd)<0){
		printf("Error closing the ADC%d\n",(*devptr).adc_num );
		return ADC_ERROR;
	}
	return ADC_OK;
}