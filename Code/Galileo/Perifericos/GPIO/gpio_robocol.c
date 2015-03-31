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
**     Método      :  u8toa
*/
/*!
**     @resumen
**          Convierte un uint8_t a una cadena de caracteres (string)
**     @param
**          i 	    	   	- Número a convertir
**     @preturn
**          				- Apuntador a la primera posición del array.
*/
/* ===================================================================*/
char* u8toa(uint8_t i) {
	char* buff=malloc(4);	
	sprintf(buff, "%d", i);
	return buff;
}

/*
** ===================================================================
**     Método      :  u32toa
*/
/*!
**     @resumen
**          Convierte un uint32_t a una cadena de caracteres (string)
**     @param
**          i 	    	   	- Número a convertir
**     @preturn
**          				- Apuntador a la primera posición del array.
*/
/* ===================================================================*/
char* u32toa(uint32_t i) {
	char* buff=malloc(11);	
	sprintf(buff, "%d", i);
	return buff;
}

/*
** ===================================================================
**     Método      :  g_write_file
*/
/*!
**     @resumen
**          Convierte un uint8_t a una cadena de caracteres (string)
**     @param
**          i 	    	   	- Número a convertir
**     @preturn
**          				- Apuntador a la primera posición del array.
*/
/* ===================================================================*/
void g_write_file(char* ruta, char* buff, uint8_t len ){
	//Se abre el archivo para incluir el GPIO en sysfs
	int fd;
	fd=open(ruta,O_WRONLY);

	if(fd<0){
		//En caso de error, se imprime la razón
		printf("Error al abrir en %s",ruta );
		perror("Descripción");
	}

	//Se escribe en el archivo el número del GPIO a modificar
	if(write(fd,buff,len)!=len)
	{
		//En caso de error, se notifica y se cierra el archivo			
		printf("Error al escribir en %s",ruta );
		perror("Descripción");
	}
	// Se cierra el archivo
	close(fd);
}

/*
** ===================================================================
**     Método      :  g_read_file
*/
/*!
**     @resumen
**          Convierte un uint8_t a una cadena de caracteres (string)
**     @param
**          i 	    	   	- Número a convertir
**     @preturn
**          				- Apuntador a la primera posición del array.
*/
/* ===================================================================*/
void g_read_file(char* ruta, char* buff, uint8_t len ){
	//Se abre el archivo para incluir el GPIO en sysfs
	int fd;
	fd=open(ruta,O_WRONLY);

	if(fd<0){
		//En caso de error, se imprime la razón
		printf("Error al abrir en %s",ruta );
		perror("Descripción");
	}

	//Se escribe en el archivo el número del GPIO a modificar
	if(read(fd,buff,len)!=len)
	{
		//En caso de error, se notifica y se cierra el archivo			
		printf("Error al escribir en %s",ruta );
		perror("Descripción");
	}
	// Se cierra el archivo
	close(fd);
}

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
static void gpio_export(char* str_num, uint8_t len ){

	g_write_file("/sys/class/gpio/export",str_num,len);

}

/*
** ===================================================================
**     Método      :  gpio_unexport
*/
/*!
**     @resumen
**          Exporta el gpio especificado al Sysfs
**     @param
**          str_num    	   	- String del número del gpio a exportar
**     @param
**          len	    	   	- Largo en caracteres del str_num
**     @return
**          				- Estado de finalización
*/
/* ===================================================================*/
static void gpio_unexport(char* str_num, uint8_t len ){

	g_write_file("/sys/class/gpio/unexport",str_num,len);

}

/*
** ===================================================================
**     Método      :  gpio_set_dir
*/
/*!
**     @resumen
**          Define el GPIO especificado como una entreada o salida
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

	//Se incluye el GPIO en sysfs
	gpio_export(num_str,len);

	//Se define la ruta del archivo de dirección
	sprintf(ruta, "/sys/class/gpio/gpio%d/direction", num);

	if(!esEntrada){
		char buf[]="out";
		g_write_file(ruta,buf,3);
	}else{
		char buf[]="in";
		g_write_file(ruta,buf,2);		
	}

	//Se extrae el GPIO de sysfs
	gpio_unexport(num_str,len);

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

	//Se combierte el entero a string
	char* num_str=u8toa(num);
	uint8_t len;

	//Se determina el tamaño y la validez del número dado
	if(num<=9){
		len=1;
	}else if (num<=55){
		len=2;
	}else{
		printf("El número de GPIO especificado, no es válido");
	}
	
	//Se incluye el GPIO en sysfs
	gpio_export(num_str,len);
	
	//Se define la ruta del archivo de value
	char* ruta=malloc(40);
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
	gpio_unexport(num_str,len);

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

	//Se combierte el entero a string
	char* num_str=u8toa(num);
	char* buf;
	uint8_t len;

	//Se determina el tamaño y la validez del número dado
	if(num<=9){
		len=1;
	}else if (num<=55){
		len=2;
	}else{
		printf("El número de GPIO especificado, no es válido");
	}
	
	//Se incluye el GPIO en sysfs
	if(gpio_export(num_str,len)!=0){
		return GPIO_ERROR
	}
	
	//Se define la ruta del archivo de value
	char* ruta=malloc(40);
	sprintf(ruta, "/sys/class/gpio/gpio%d/value", num);


	//Dependiendo de el valor especificado, se escribe 
	//en value
	
	if(g_read_file(ruta,buf,1)!=0){
		return GPIO_ERROR;
	}
	
	*value = itoa(buf);
	
	//Se extrae el GPIO de sysfs
	if(gpio_unexport(num_str,len)!=0){
		return GPIO_ERROR
	}
	return GPIO_OK;
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
	uint8_t mux_val;

	/*Checkeo de multiplexores*/
	if(num==50){
		gpio_gal_get(40,mux_val);
		if(mux_val!=HIGH){
			printf("El gpio %d\n no está habilitado por los multiplexores correspondientes ",num);
			return GPIO_ERROR;
		}
	}else if(num==51){
		gpio_gal_get(41,mux_val);
		if(mux_val!=HIGH){
			printf("El gpio %d\n no está habilitado por los multiplexores correspondientes ",num);
			return GPIO_ERROR;
		}
	}else if(num==14){
		gpio_gal_get(31,mux_val);
		if(mux_val!=LOW){
			printf("El gpio %d\n no está habilitado por los multiplexores correspondientes ",num);
			return GPIO_ERROR;
		}
	}else if(num==32){
		gpio_gal_get(31,mux_val);
		if(mux_val!=HIGH){
			printf("El gpio %d\n no está habilitado por los multiplexores correspondientes ",num);
			return GPIO_ERROR;
		}
	}else if(num==10){
		gpio_gal_get(42,mux_val);
		if(mux_val!=LOW){
			printf("El gpio %d\n no está habilitado por los multiplexores correspondientes ",num);
			return GPIO_ERROR;
		}
	}else if(num==42){
		gpio_gal_get(42,mux_val);
		if(mux_val!=HIGH){
			printf("El gpio %d\n no está habilitado por los multiplexores correspondientes ",num);
			return GPIO_ERROR;
		}
	}else if(num==25){
		gpio_gal_get(43,mux_val);
		if(mux_val!=HIGH){
			printf("El gpio %d\n no está habilitado por los multiplexores correspondientes ",num);
			return GPIO_ERROR;
		}
	}else if(num==38){
		gpio_gal_get(54,mux_val);
		if(mux_val!=HIGH){
			printf("El gpio %d\n no está habilitado por los multiplexores correspondientes ",num);
			return GPIO_ERROR;
		}
	}else if(num==39){
		gpio_gal_get(55,mux_val);
		if(mux_val!=HIGH){
			printf("El gpio %d\n no está habilitado por los multiplexores correspondientes ",num);
			return GPIO_ERROR;
		}
	}else if(num==44){
		gpio_gal_get(37,mux_val);
		if(mux_val!=HIGH){
			printf("El gpio %d\n no está habilitado por los multiplexores correspondientes ",num);
			return GPIO_ERROR;
		}
	}else if(num==45){
		gpio_gal_get(36,mux_val);
		if(mux_val!=HIGH){
			printf("El gpio %d\n no está habilitado por los multiplexores correspondientes ",num);
			return GPIO_ERROR;
		}
	}else if(num==46){
		gpio_gal_get(23,mux_val);
		if(mux_val!=HIGH){
			printf("El gpio %d\n no está habilitado por los multiplexores correspondientes ",num);
			return GPIO_ERROR;
		}
	}else if(num==47){
		gpio_gal_get(22,mux_val);
		if(mux_val!=HIGH){
			printf("El gpio %d\n no está habilitado por los multiplexores correspondientes ",num);
			return GPIO_ERROR;
		}
	}else if(num==48){
		gpio_gal_get(21,mux_val);
		if(mux_val!=HIGH){
			printf("El gpio %d\n no está habilitado por los multiplexores correspondientes ",num);
			return GPIO_ERROR;
		}else{
			gpio_gal_get(29,mux_val);
			if(mux_val!=HIGH){
				printf("El gpio %d\n no está habilitado por los multiplexores correspondientes ",num);
				return GPIO_ERROR;
			}
		}
	}else if(num==49){
		gpio_gal_get(20,mux_val);
		if(mux_val!=HIGH){
			printf("El gpio %d\n no está habilitado por los multiplexores correspondientes ",num);
			return GPIO_ERROR;
		}else{
			gpio_gal_get(29,mux_val);
			if(mux_val!=HIGH){
				printf("El gpio %d\n no está habilitado por los multiplexores correspondientes ",num);
				return GPIO_ERROR;
			}
		}
	}

	/*Despues de la comprovación*/
	if(gpio_gal_value(num,HIGH)!=0){
		return GPIO_ERROR
	}
	return GPIO_OK
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
	uint8_t mux_val;

	/*Checkeo de multiplexores*/
	if(num==50){
		gpio_gal_get(40,mux_val);
		if(mux_val!=HIGH){
			printf("El gpio %d\n no está habilitado por los multiplexores correspondientes ",num);
			return GPIO_ERROR;
		}
	}else if(num==51){
		gpio_gal_get(41,mux_val);
		if(mux_val!=HIGH){
			printf("El gpio %d\n no está habilitado por los multiplexores correspondientes ",num);
			return GPIO_ERROR;
		}
	}else if(num==14){
		gpio_gal_get(31,mux_val);
		if(mux_val!=LOW){
			printf("El gpio %d\n no está habilitado por los multiplexores correspondientes ",num);
			return GPIO_ERROR;
		}
	}else if(num==32){
		gpio_gal_get(31,mux_val);
		if(mux_val!=HIGH){
			printf("El gpio %d\n no está habilitado por los multiplexores correspondientes ",num);
			return GPIO_ERROR;
		}
	}else if(num==10){
		gpio_gal_get(42,mux_val);
		if(mux_val!=LOW){
			printf("El gpio %d\n no está habilitado por los multiplexores correspondientes ",num);
			return GPIO_ERROR;
		}
	}else if(num==42){
		gpio_gal_get(42,mux_val);
		if(mux_val!=HIGH){
			printf("El gpio %d\n no está habilitado por los multiplexores correspondientes ",num);
			return GPIO_ERROR;
		}
	}else if(num==25){
		gpio_gal_get(43,mux_val);
		if(mux_val!=HIGH){
			printf("El gpio %d\n no está habilitado por los multiplexores correspondientes ",num);
			return GPIO_ERROR;
		}
	}else if(num==38){
		gpio_gal_get(54,mux_val);
		if(mux_val!=HIGH){
			printf("El gpio %d\n no está habilitado por los multiplexores correspondientes ",num);
			return GPIO_ERROR;
		}
	}else if(num==39){
		gpio_gal_get(55,mux_val);
		if(mux_val!=HIGH){
			printf("El gpio %d\n no está habilitado por los multiplexores correspondientes ",num);
			return GPIO_ERROR;
		}
	}else if(num==44){
		gpio_gal_get(37,mux_val);
		if(mux_val!=HIGH){
			printf("El gpio %d\n no está habilitado por los multiplexores correspondientes ",num);
			return GPIO_ERROR;
		}
	}else if(num==45){
		gpio_gal_get(36,mux_val);
		if(mux_val!=HIGH){
			printf("El gpio %d\n no está habilitado por los multiplexores correspondientes ",num);
			return GPIO_ERROR;
		}
	}else if(num==46){
		gpio_gal_get(23,mux_val);
		if(mux_val!=HIGH){
			printf("El gpio %d\n no está habilitado por los multiplexores correspondientes ",num);
			return GPIO_ERROR;
		}
	}else if(num==47){
		gpio_gal_get(22,mux_val);
		if(mux_val!=HIGH){
			printf("El gpio %d\n no está habilitado por los multiplexores correspondientes ",num);
			return GPIO_ERROR;
		}
	}else if(num==48){
		gpio_gal_get(21,mux_val);
		if(mux_val!=HIGH){
			printf("El gpio %d\n no está habilitado por los multiplexores correspondientes ",num);
			return GPIO_ERROR;
		}else{
			gpio_gal_get(29,mux_val);
			if(mux_val!=HIGH){
				printf("El gpio %d\n no está habilitado por los multiplexores correspondientes ",num);
				return GPIO_ERROR;
			}
		}
	}else if(num==49){
		gpio_gal_get(20,mux_val);
		if(mux_val!=HIGH){
			printf("El gpio %d\n no está habilitado por los multiplexores correspondientes ",num);
			return GPIO_ERROR;
		}else{
			gpio_gal_get(29,mux_val);
			if(mux_val!=HIGH){
				printf("El gpio %d\n no está habilitado por los multiplexores correspondientes ",num);
				return GPIO_ERROR;
			}
		}
	}

	if(gpio_gal_value(num,0)!=0){
		return GPIO_ERROR;
	}
	return GPIO_OK;
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
	i2c_result=i2c_open("/dev/i2c-0",addr);
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
