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
st_misc g_write_file(char* ruta, char* buff, uint8_t len ){
	//Se abre el archivo para incluir el GPIO en sysfs

	int fd;
	if((fd=open(ruta,O_WRONLY))<0){
		//printf("Error en la apertura del file descriptor para escritura en archivo.(misc_robocol.c)\n");
		perror("Descripción:");
		if(close(fd)<0){
			printf("Error en close de fd.(g_write_file)\n");
		}
		return MISC_ERROR;
	}
	//printf("Llamada a escritura en archivo para valor de gpio.(misc_robocol.c>g_write_file)\n");
	//Se escribe en el archivo el número del GPIO a modificar
	if(write(fd,buff,len)!=len)
	{
		//En caso de error, se notifica y se cierra el archivo			
		//printf("Error al leer en%s\n",ruta );
		perror("Descripción:\n");
		if(close(fd)<0){
			printf("Error en close de fd.(g_write_file)\n");
		}
		return MISC_ERROR;
	}
	//printf("Exitoso\n");
	// Se cierra el archivo
	if(close(fd)<0){
		printf("Error en close de fd.(g_write_file)\n");
	}
	return MISC_OK;
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
st_misc g_read_file(char* ruta, char* buff, uint8_t len ){
	//Se abre el archivo para incluir el GPIO en sysfs
	int fd;
	if((fd=open(ruta,O_RDONLY))<0){
		printf("Error al abrir archivo %s\n",ruta );
		perror("Descripción");
		if(close(fd)<0){
			printf("Error en close de fd.(g_read_file)\n");
		}
		return MISC_ERROR;
	}
	printf("File Descriptor: %d\n",fd );
	printf("Llamada a lectura en archivo para valor de gpio.(misc_robocol.c>g_read_file)\n");
	//Se escribe en el archivo el número del GPIO a modificar
	if(read(fd,buff,len)!=len)
	{
		//En caso de error, se notifica y se cierra el archivo			
		printf("Error al leer de %s\n",ruta );
		perror("Descripción");
		if(close(fd)<0){
			printf("Error en close de fd.(g_read_file)\n");
		}
		return MISC_ERROR;
	}
	// Se cierra el archivo
	if(close(fd)){
		printf("Error en close de fd.(g_read_file)\n");
	}
	return MISC_OK;
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
**     Método      array_to_i32
*/
/*!
**     @resumen
**          Convierte un a int32_t un arraglo de bytes
**     @param
**          array 	    	   	- Arreglo de bytes a convertir
**     @param
**          len 	    	   	- Tamaño del arreglo a convertir
**     @param
**          little_endian 	    - Indicador de little_endian o big_endian.
**
**     @preturn
**          				- Numero convertido a int32_t.
*/
/* ===================================================================*/
int32_t array_to_i32(uint8_t* array, uint8_t len, uint8_t little_endian) {
	int32_t res=0;
	uint8_t iterator;
	if(len>4){
		printf("Precaución: El arreglo contiene más de 32bits. Puede haber pérdida de información\n");
	}

	if (little_endian)
	{
		iterator=len;
		while (iterator){
			iterator--;
			res = res * 256 + array[iterator];
		}

	}else{
		iterator=0;
		while(iterator<len){
			res = res * 256 + array[iterator];
			iterator++;
		}

	}
	


    return res;
}

uint8_t* i32_to_array(uint32_t in) {
	
	uint8_t* ptr = malloc(sizeof(uint8_t)*4);
	memcpy(ptr, &in, sizeof in);
    return ptr;
}
