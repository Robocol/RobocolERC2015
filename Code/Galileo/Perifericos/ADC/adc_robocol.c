#include "adc_robocol.h"

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

adc_st parse_adc_route(uint8_t adc_num, char* ruta){

	if(sprintf(ruta ,"/sys/bus/iio/devices/iio:device0/in_voltage%d_raw",adc_num)<0){
		return ADC_ERROR;
	}
	return ADC_OK;
}

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

adc_st close_adc(adc_dev* devptr){
	if(close((*devptr).fd)<0){
		printf("Error closing the ADC%d\n",(*devptr).adc_num );
		return ADC_ERROR;
	}
	return ADC_OK;
}