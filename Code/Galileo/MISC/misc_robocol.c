#include "misc_robocol.h"

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
**     Método      :  little_endian_to_u32
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
int32_t array_to_i32(uint8_t* array, uint8_t len) {

	if(len>4){
		printf("Precaución: El arreglo contiene más de 32bits. Puede haber pérdida de información\n");
	}
	
	int32_t res=0;
	while (len){
		len--;
		res = res * 256 + array[len];
	}

    return res;
}

uint8_t* i32_to_array(uint32_t in) {

	ptr = malloc(sizeof uint8_t*4);
	memcpy(ptr, &in, sizeof in);
    return ptr;
}
