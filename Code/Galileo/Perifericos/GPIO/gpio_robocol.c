/*
 * gpio_robocol.h (2.0)
 *
 *  Created on: Mar 13, 2015
 *      Authors:	Daniel Ochoa
 *				    Germán Giraldo
 */
#include "gpio_robocol.h"

/*
** ===================================================================
**     Método      :  gpio_export
*/
/*!
**     @resumen
**          Retira el gpio especificado al Sysfs
**     @param
**          str_num    	   	- String del número del gpio a retirar
**     @param
**          len	    	   	- Largo en caracteres del str_num
**     @return
**          				- Estado de finalización
*/
/* ===================================================================*/
static gpio_st gpio_export(char* str_num, uint8_t len ){
	//printf("Realizando export de gpio en ruta /sys/class/gpio/export para gpio %d(gpio_robocol.c>gpio_export)\n",str_num);
	if(g_write_file("/sys/class/gpio/export",str_num,len)){
		printf("Error en la escritura en archivo: sys/class/gpio/export .(gpio_robocol.c>gpio_export)\n");
		perror("Descripción: ");
		return GPIO_ERROR;
	}
	return GPIO_OK;

}

/*
** ===================================================================
**     Método      :  gpio_unexport
*/
/*!
**     @resumen
**          Desactiva el gpio especificado en el Sysfs
**     @param
**          str_num    	   	- String del número del gpio a desactivar
**     @param
**          len	    	   	- Largo en caracteres del str_num
**     @return
**          				- Estado de finalización
*/
/* ===================================================================*/
static gpio_st gpio_unexport(char* str_num, uint8_t len ){
	//printf("Realizando export de gpio en ruta /sys/class/gpio/unexport para gpio %d(gpio_robocol.c>gpio_unexport)\n",str_num);
	if(g_write_file("/sys/class/gpio/unexport",str_num,len)){
		printf("Error en la escritura en archivo: sys/class/gpio/unexport .(gpio_robocol.c>gpio_unexport)\n");
		perror("Descripción: ");
		return GPIO_ERROR;
	}
	return GPIO_OK;
}

/*
** ===================================================================
**     Método      :  gpio_set_dir
*/
/*!
**     @resumen
**          Define el GPIO especificado como una entrada o salida
**			según se especifica por parámetro.
**     @param
**          num     	   	- Número del GPIO
**     @param
**          esEntrada		- Booleano de valor 1 en caso de que el 
**							GPIO se desee definir como entrada.
*/
/* ===================================================================*/
gpio_st gpio_set_dir(uint8_t num,int esEntrada){

	char* ruta=malloc(40);
	uint8_t len;

	//Se convierte el entero a string
	char* num_str=u8toa( num );
	

	//Se determina el tamaño y la validez del número dado
	if(num < 10){
		len=1;
	}else if (num<56){
		len=2;
	}else{
		printf("El número de GPIO especificado, no es válido");
	}
	//printf("Llamada a gpio_export.(gpio_robocol.c>gpio_set_dir)\n");
	//Se incluye el GPIO en sysfs
	if(gpio_export(num_str,len)){
		printf("Error en el export de gpio %d. (gpio_robocol.c>gpio_set_dir)\n",num );
		return GPIO_ERROR;
	}

	//Se define la ruta del archivo de dirección
	sprintf(ruta, "/sys/class/gpio/gpio%d/direction", num);

	if(!esEntrada){
		char buf[]="out";
		//printf("Llamada a gpio_writefile.(gpio_robocol.c>gpio_set_dir)\n");
		g_write_file(ruta,buf,3);
	}else{
		char buf[]="in";
		//printf("Llamada a g_write_file.(gpio_robocol.c>gpio_set_dir)\n");
		g_write_file(ruta,buf,2);		
	}
	//printf("Llamada a gpio_unexport.(gpio_robocol.c>gpio_set_dir)\n");
	//Se extrae el GPIO de sysfs
	if(gpio_unexport(num_str,len)){
		printf("Error en el unexport de gpio %d. (gpio_robocol.c>gpio_set_dir)\n",num );
	}

	return GPIO_OK;
}

/*
** ===================================================================
**     Método      :  gpio_gal_value
*/
/*!
**     @resumen
**          Define el valor del GPIO especificado como 1 o 0
**			según se especifica por parámetro. El GPIO debe
**			estar definido como salida
**
**     @param
**          num     	   	- Número del GPIO
**     @param
**          esEntrada		- Booleano de valor 1 o 0 según se 
**							requiera instanciar el GPIO.
*/
/* ===================================================================*/
gpio_st gpio_gal_value(uint8_t num, uint8_t valor){
	char* num_str;
	uint8_t len;
	char* ruta=malloc(40);
	//printf("Num gpio%d\n", num);
	//printf("Verificación para el manejo de muxes(gpio_robocol.c>gpio_gal_value)\n" );

/*
	if (gpio_muxlock(num)!=0){
		printf("El gpio %d no está habilitado por los multiplexores correspondientes.\n"
			"Realice la configuración para la habilitacion de multiplexores desde la inicializacion del sistema\n",num);
		return GPIO_ERROR;
	}
*/

	//printf("Fin de la Verificación de muxes(gpio_robocol.c>gpio_gal_value)\n" );
	printf("Num gpio%d\n", num);
	num_str=u8toa(num);


	//Se determina el tamaño y la validez del número dado
	if(num<=9){
		len=1;
	}else if (num<=55){
		len=2;
	}else{
		printf("El número de GPIO especificado, no es válido");
	}
	
	//Se incluye el GPIO en sysfs
	if(gpio_export(num_str,len)){
		printf("Error en gpio_export(gpio_robocol.c>gpio_gal_value)\n");
		return GPIO_ERROR;
	}
	
	//Se define la ruta del archivo de value
	sprintf(ruta, "/sys/class/gpio/gpio%d/value", num);


	//Dependiendo de el valor especificado, se escribe 
	//en value
	if(!valor){
		char buf[]="0";
		g_write_file(ruta,buf,1);
	}else{
		char buf[]="1";
		g_write_file(ruta,buf,1);
	}
	
	//Se extrae el GPIO de sysfs
	if(gpio_unexport(num_str,len)){
		printf("Error en gpio_unexport.(gpio_robocol.c>gpio_gal_value)\n");
	}

	return GPIO_OK;
}


/*
** ===================================================================
**     Método      :  gpio_gal_get
*/
/*!
**     @resumen
**          Define el valor del GPIO especificado como 1 o 0
**			según se especifica por parámetro. El GPIO debe
**			estar definido como salida
**
**     @param
**          num     	   	- Número del GPIO
**     @param
**          esEntrada		- Booleano de valor 1 o 0 según se 
**							requiera instanciar el GPIO.
*/
/* ===================================================================*/
gpio_st gpio_gal_get(uint8_t num, uint8_t* valor){
	//Se convierte el entero a string
	//printf("Num Gpio %d\n",num );
	char* num_str=u8toa(num);
	char* buf;
	uint8_t len;
	//Se define la ruta del archivo de value
	char* ruta=malloc(40);

	//printf("Obtención de valor de gpio.(gpio_robocol.c>gpio_gal_get)\n");

	//Se determina el tamaño y la validez del número dado
	if(num<=9){
		len=1;
	}else if (num<=55){
		len=2;
	}else{
		printf("El número de GPIO especificado, no es válido");
	}
	
	//printf("Llamada a gpio_export.(gpio_robocol.c>gpio_gal_get)\n");
	//Se incluye el GPIO en sysfs
	if(gpio_export(num_str,len)){
		printf("Error en el export para obtención de valor de gpio en Galileo(gpio_robocol.c>gpio_gal_get)\n");
		return GPIO_ERROR;
	}

	sprintf(ruta,"/sys/class/gpio/gpio%d/value", num);
	//Dependiendo de el valor especificado, se escribe 
	//en value
	//printf("Llamada a g_read_file.(gpio_robocol.c> gpio_gal_get)\n");
	if(g_read_file(ruta,buf,1)){
		printf("Error en la lectura de archivo en ruta %s\n",ruta );
		return GPIO_ERROR;
	}
	
	*valor = atoi(buf);
	
	//Se extrae el GPIO de sysfs
	if(gpio_unexport(num_str,len)!=0){
		printf("Error en el unexport de gpio %d(gpio_robocol.c>gpio_gal_get)\n",num );
		return GPIO_ERROR;
	}
	return GPIO_OK;
}


/*
** ===================================================================
**     Método      :  gpio_muxlock
*/
/*!
**     @resumen
**          Realiza el manejo de permisos para modificaion de gpios
**			tan solo si de antemano se ha habilitado su salida con el 
**			correspondiente multiplexor
**
**     @param
**          num     	   	- Número del GPIO
*/
/* ===================================================================*/
gpio_st gpio_muxlock(uint8_t num){
	uint8_t mux_val;
	const char* msg="Para el sistema Robocol ERC015 no se permite la modificacion de multiplexores desde esta libreria\n";
	printf("Verificación de muxes(gpio_robocol.c>gpio_muxlock)\n");
	switch(num){
		case 40:
			printf(msg);
			return GPIO_ERROR;
		case 50:
			gpio_gal_get(40,&mux_val);
			if(mux_val!=HIGH){
				return GPIO_ERROR;
			}
			return GPIO_OK;
		case 41:
			printf(msg);
			return GPIO_ERROR;
		case 51:
			gpio_gal_get(41,&mux_val);
			if(mux_val!=HIGH){
				return GPIO_ERROR;
			}
			return GPIO_OK;
		case 31:
			printf(msg);
			return GPIO_ERROR;
		case 0:
			printf(msg);
			return GPIO_ERROR;
		case 14:
			gpio_gal_get(31,&mux_val);
			if(mux_val!=LOW){
				return GPIO_ERROR;
			}
			return GPIO_OK;
		case 32:
			gpio_gal_get(31,&mux_val);
			if(mux_val!=HIGH){
				return GPIO_ERROR;
			}
			return GPIO_OK;
		case 30:
			printf(msg);
			return GPIO_ERROR;
		case 15:
			gpio_gal_get(30,&mux_val);
			if(mux_val!=LOW){
				return GPIO_ERROR;
			}
			return GPIO_OK;		
		case 18:
			gpio_gal_get(30,&mux_val);
			if(mux_val!=HIGH){
				return GPIO_ERROR;
			}
			return GPIO_OK;
		case 28:
			return GPIO_OK;
		case 17:
			return GPIO_OK;
		case 24:
			return GPIO_OK;
		case 27:
			return GPIO_OK;
		case 26:
			return GPIO_OK;
		case 19:
			return GPIO_OK;
		case 42:
			printf(msg);
			return GPIO_ERROR;
		case 10:	
			gpio_gal_get(42,&mux_val);
			if(mux_val!=LOW){
				return GPIO_ERROR;
			}
			return GPIO_OK;
		case 43:	
			printf(msg);
			return GPIO_ERROR;
		case 25:	
			gpio_gal_get(43,&mux_val);
			if(mux_val!=LOW){
				return GPIO_ERROR;
			}
			return GPIO_OK;	
		case 54:
			printf(msg);
			return GPIO_ERROR;
		case 38:	
			gpio_gal_get(54,&mux_val);
			if(mux_val!=LOW){
				return GPIO_ERROR;
			}
			return GPIO_OK;	
		case 55:
			printf(msg);
			return GPIO_ERROR;
		case 39:
			gpio_gal_get(55,&mux_val);
			if(mux_val!=LOW){
				return GPIO_ERROR;
			}
			return GPIO_OK;				
		case 37:
			printf(msg);
			return GPIO_ERROR;		
		case 44:
			gpio_gal_get(37,&mux_val);
			if(mux_val!=HIGH){
				return GPIO_ERROR;
			}
			return GPIO_OK;
		case 36:
			printf(msg);
			return GPIO_ERROR;	
		case 45:
			gpio_gal_get(36,&mux_val);
			if(mux_val!=HIGH){
				return GPIO_ERROR;
			}
			return GPIO_OK;	
		case 23:
			printf(msg);
			return GPIO_ERROR;	
		case 46:
			gpio_gal_get(23,&mux_val);
			if(mux_val!=HIGH){
				return GPIO_ERROR;
			}
			return GPIO_OK;
		case 22:
			printf(msg);
			return GPIO_ERROR;	
		case 47:
			gpio_gal_get(22,&mux_val);
			if(mux_val!=HIGH){
				return GPIO_ERROR;
			}
			return GPIO_OK;
		case 29:
			printf(msg);
			return GPIO_ERROR;	
		case 21:
			printf(msg);
			return GPIO_ERROR;	
		case 20:
			printf(msg);
			return GPIO_ERROR;	
		case 48:
			gpio_gal_get(29,&mux_val);
			if(mux_val!=HIGH){
				return GPIO_ERROR;
			}
			gpio_gal_get(21,&mux_val);
			if(mux_val!=HIGH){
				return GPIO_ERROR;
			}
			return GPIO_OK;
		case 49:
			gpio_gal_get(29,&mux_val);
			if(mux_val!=HIGH){
				return GPIO_ERROR;
			}
			gpio_gal_get(21,&mux_val);
			if(mux_val!=HIGH){
				return GPIO_ERROR;
			}
			return GPIO_OK;
		default:	
			printf("El gpio pasado por parámetro no es válido para la Intel Galileo Gen1\n");
			return GPIO_ERROR;
	}
}


/*
** ===================================================================
**     Método      :  gpio_gal_set
*/
/*!
**     @resumen
**          Define el valor del GPIO especificado como 1. El GPIO debe
**			estar definido como salida
**
**     @param
**          num     	   	- Número del GPIO
*/
/* ===================================================================*/
gpio_st gpio_gal_set(uint8_t num){
	gpio_gal_value(num,HIGH);
}

/*
** ===================================================================
**     Método      :  gpio_gal_clear
*/
/*!
**     @resumen
**          Define el valor del GPIO especificado como 0. El GPIO debe
**			estar definido como salida
**
**     @param
**          num     	   	- Número del GPIO
*/
/* ===================================================================*/
gpio_st gpio_gal_clear(uint8_t num){
	gpio_gal_value(num,LOW);
}

/*
** ===================================================================
**     Método      :  build_expander
*/
/*!
**     @resumen
**          Inicia la comunicación con expansor de pines
**
**     @param
**          addr    	   	- Direccíon del expansor
*/
/* ===================================================================*/
gpio_st build_expander(uint8_t addr){
	unsigned char i2c_result;
	if(i2c_open("/dev/i2c-0",addr)<0){
		perror("Descripcion:");
		return GPIO_ERROR;
	}
	return i2c_result;
}

/*
** ===================================================================
**     Método      :  close_expander
*/
/*!
**     @resumen
**          Termina la comunicación con expansor de pines
**
**     @param
**          addr    	   	- Direccíon del expansor
*/
/* ===================================================================*/
gpio_st close_expander(void){
	i2c_close();
}

gpio_st gpio_exp_clear(uint8_t port){
	if(port<8){
		char val;
		i2c_read(&val,1);
		val&=~(1<<port);
		i2c_write_rw(&val);
		return GPIO_OK;
	}else{
		perror("The port passed by parameter cannot by greater than 7");
		return GPIO_ERROR;
	}	
}

gpio_st gpio_exp_set(uint8_t port){
	if(port<8){
		char val;
		i2c_read(&val,1);
		val|=(1<<port);
		i2c_write_rw(&val);
		return GPIO_OK;
	}else{
		perror("The port passed by parameter cannot by greater than 7");
		return GPIO_ERROR;
	}
}

gpio_st gpio_exp_get(uint8_t port, uint8_t* buff){
	if(port<8){
		char val;
		i2c_read(&val,1);
		val=val>>port;
		val&=0b00000001;
		*buff= (uint8_t)val;
		return GPIO_OK;
	}else{
		perror("The port passed by parameter cannot by greater than 7");
		return GPIO_ERROR;
	}
}

gpio_st reg_clear(void){
	char* value;
	value=malloc(sizeof(char));
	*value=0b00000000;
	i2c_write_rw(value);
}

gpio_st reg_set(void){
	char* value;
	value=malloc(sizeof(char));
	*value=~(0b00000000);
	i2c_write_rw(value);
}

gpio_st reg_set_value(char* value){
	i2c_write_rw(value);
}

gpio_st reg_get(char* buff){
	i2c_read(buff,1);
}
