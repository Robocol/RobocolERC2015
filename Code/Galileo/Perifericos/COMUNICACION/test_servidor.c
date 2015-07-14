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
	
	/* El proceso se convierte en un demonio */
	if(becomeDaemon(0) == -1)
		printf("\nEl proceso del servidor no pudo volverse un demonio\n");
	logOpen(LOG_FILE);


	
//Configuración servidor

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
				if(parser_comandos_mov(comando, cfd)==-1)
					logMessage("No se ejecuto el comando adecuadamente");
			}else if(strcmp(comando_filtrado,"diagnosticar")==0){
				/* Analiza el comando y realiza una acción */
				if(parser_comandos_diag(comando, cfd)==-1)
					logMessage("No se ejecuto el comando adecuadamente");
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
