#include "terminal_robocol.h"


term_st term_open(term_device* dev, char* route_in, char* route_out){
	FILE *input,*output;

	if (!(input=fopen(route_in,"r"))){
		printf("Error en la ruta de archivo ingresada para entrada de la terminal.(term_open -> term_robocol.c)\n");
		perror("Descripción: ");
		return TERM_ERROR;
	}

	if (!(output=fopen(route_out,"w"))){
		printf("Error en la ruta de archivo ingresada para salida de la terminal.(term_open -> term_robocol.c)\n");
		perror("Descripción: ");
		return TERM_ERROR;
	}

	(*dev).input=input;
	(*dev).output=output;

	return TERM_OK;
}

term_st term_write(term_device* dev, char* word){
	if (!(*dev).output){
		printf("File descriptor del output file para el dispositivo pasado por parámetro no es válido.(term_write -> term_robocol.c)\n");
		return TERM_ERROR;
	}

	if(fprintf((*dev).output,"%s\n",word)<0){
		printf("Error en el parámetro ingresado para escritura\n");
		perror("Descripción: ");
		return TERM_ERROR;
	}

	return TERM_OK;
}

term_st term_readln(term_device* dev, char** buffer){
	size_t length=0;
	char* line=NULL;
	if (!(*dev).input){
		printf("File descriptor del input file para el dispositivo pasado por parámetro no es válido.(term_read -> term_robocol.c)\n");
		return TERM_ERROR;
	}

	if(getline(&line,&length,(*dev).input)<0){
		printf("Error en el parámetro ingresado para escritura.getline no exitoso.(term_read -> term_robocol.c)\n");
		perror("Descripción: ");
		return TERM_ERROR;
	}

	*buffer=line;
	free(line);

	return TERM_OK;
}
