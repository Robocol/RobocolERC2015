#include "servidor_robocol.h"

int main(int argc, char *argv[]) {
	char* line;
	//Variables locales servidor
	char comando[INT_LEN];	/* Buffer para almacenar lo que llega de un cliente */
	struct sockaddr_storage claddr;	/* Buffer para almacenar lo que llegs del cliente */	
	int cfd,lfd;//
	socklen_t addrlen;
	char* comando_filtrado;
	char* comando_analizar;
	char* param1_comando;
	char* gps_line=malloc(60*sizeof(char));
	uint8_t estado;


	gps_build("/home/german/GitHub/RobocolERC2015/Code/Galileo/Perifericos/GPS/gps.log");
	// gps_start();
	// sleep(2);
	// printf("Stopping\n");
	// gps_stop();

	gps_report(gps_line);
	printf("%s\n",gps_line );
	
	if (argc==2)
	{
		if (!strcmp(argv[1],"l")){
			printf("ESTOY EN LEFT :)\n");
			if(tr_build(TR_SLAVE,TR_LEFT_SIDE)){
				printf("Error al Construir el dispositivo de traccion.\n");
				return -1;
			}
		}else if(!strcmp(argv[1],"r")){
			printf("ESTOY EN RIGHT :)\n");
			if(tr_build(TR_SLAVE,TR_RIGHT_SIDE)){
				printf("Error al Construir el dispositivo de traccion.\n");
				return -1;
			}
		}else{
			printf("Argumento ingresado no válido. Ingrese l o r para seleccionar el tipo de la tracción\n");
			return -1;
		}
	}else{
		printf("Argumento ingresado no válido. Ingrese l o r para seleccionar el tipo de la tracción\n");
		return -1;
	}

//Configuración servidor
	tr_setCtlState(TR_AUTO);

	/* El proceso se convierte en un demonio */
	// if(becomeDaemon(0) == -1){
	// 	printf("\nEl proceso del servidor no pudo volverse un demonio\n");
	// }
	
	logOpen(LOG_FILE);
	

	/* Ignore the SIGPIPE signal, so that we find out about broken connection
	errors via a failure from write(). */
	if (signal(SIGPIPE, SIG_IGN) == SIG_ERR){
	        logMessage("Fail ignoring SIGPIPE ");
		exit(EXIT_FAILURE);
	}

	/* Se configura el servidor */
	lfd = configurar_servidor();
	if(lfd==-1){
		logMessage("No se pudo configurar el servidor");
		exit(EXIT_FAILURE);
	}


	for (;;) {                  /* Loop infinito que  maneja las peticiones de los clientes */
		logMessage("Estado de Traccion: %d",tr_device.ctl_state);
		ph_getEstado( tr_device.front_ph, &estado);
		logMessage("Estado Puente H front: %d",estado);
		ph_getEstado( tr_device.back_ph, &estado);
		logMessage("Estado Puente H back: %d",estado);

		/* Acepta la conexion de un cliente */
		addrlen = sizeof(struct sockaddr_storage);
		cfd = accept(lfd, (struct sockaddr *) &claddr, &addrlen);
		if (cfd == -1) {
			logMessage("Accept error (%s)", strerror(errno));
			continue;
		}
		logMessage("Acepto una conexion de un cliente");

		/* Lee la peticion del cliente y envia una respuesta o si no cierra el socket 
		y espera una nueva conexion */
		if (readLine(cfd, comando, INT_LEN) <= 0) {
			close(cfd);
			logMessage("No hay fin de línea");	
			continue;                   
        	}

		/* Imprime el texto que llegó */
		logMessage("Comando: %s",comando);
		
		strcpy(comando_analizar,comando);
		comando_filtrado=strtok(comando_analizar, "/");
		logMessage("Comando filtrado: %s",comando_filtrado);
		param1_comando=strtok(NULL,"/");
		if(param1_comando!=NULL){
			/* Imprime el tipo de comando que llegó */
			logMessage("Comando complejo");

			/* Determina la acción del comando */
			if(strcmp(comando_filtrado,"mover")==0){
				/* Analiza el comando y realiza una acción */
				if(parser_comandos_mov(comando, cfd)==-1){
					logMessage("No se ejecuto el comando adecuadamente");
				}
			}else if(strcmp(comando_filtrado,"diagnosticar")==0){
				/* Analiza el comando y realiza una acción */
				if(parser_comandos_diag(comando, cfd)==-1){
					logMessage("No se ejecuto el comando adecuadamente");
				}
			}else{
				logMessage("Accion desconocida");
			}


		
		}else{
			/* Imprime el tipo de comando que llegó */
			logMessage("Comando normal");

			/* Analiza el comando y realiza una acción */
			if(parser_comandos(comando, cfd)==-1)
				logMessage("No se ejecuto el comando adecuadamente");			
		}

		
		
	}		 

      	if (close(cfd) == -1)           /* Close connection */
		logMessage("Close error (%s)", strerror(errno));	
	
	logMessage("Finalizo rutina\n");
    	

}
