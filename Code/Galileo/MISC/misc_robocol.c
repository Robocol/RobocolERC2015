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
		printf("Error en la apertura del file descriptor para escritura en archivo.(misc_robocol.c)\n");
		perror("Descripción:");
		close(fd);
		return MISC_ERROR;
	}
	printf("Llamada a escritura en archivo para valor de gpio.(misc_robocol.c>g_write_file)\n");
	//Se escribe en el archivo el número del GPIO a modificar
	if(write(fd,buff,len)!=len)
	{
		//En caso de error, se notifica y se cierra el archivo			
		printf("Error al leer en%s\n",ruta );
		perror("Descripción:\n");
		close(fd);
		return MISC_ERROR;
	}
	printf("Exitoso\n");
	// Se cierra el archivo
	close(fd);
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
		close(fd);
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
		close(fd);
		return MISC_ERROR;
	}
	// Se cierra el archivo
	close(fd);
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
	
	uint8_t* ptr = malloc(sizeof(uint8_t)*4);
	memcpy(ptr, &in, sizeof in);
    return ptr;
}
