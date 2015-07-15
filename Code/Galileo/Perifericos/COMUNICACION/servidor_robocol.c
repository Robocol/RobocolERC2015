#include "servidor_robocol.h"


off_t fsize(const char *filename) {
    struct stat st; 

    if (stat(filename, &st) == 0){
        return st.st_size;
    }

    return -1; 
}

/* Read characters from 'fd' until a newline is encountered. If a newline
  character is not encountered in the first (n - 1) bytes, then the excess
  characters are discarded. The returned string placed in 'buf' is
  null-terminated and includes the newline character if it was read in the
  first (n - 1) bytes. The function return value is the number of bytes
  placed in buffer (which includes the newline character if encountered,
  but excludes the terminating null byte). */
ssize_t readLine(int fd, void *buffer, size_t n){
	ssize_t numRead;                    /* # of bytes fetched by last read() */
	size_t totRead;                     /* Total bytes read so far */
	char *buf;
	char ch;

	if (n <= 0 || buffer == NULL) {
	        errno = EINVAL;
	        return -1;
	}

	buf = buffer;                       /* No pointer arithmetic on "void *" */

	totRead = 0;
	while(1){
	    numRead = read(fd, &ch, 1);

		if(numRead <= -1){
			if (errno == EINTR){        /* Interrupted --> restart read() */
		        continue;
			}else{
		        return -1;              /* Some other error */
			}

		}else if(numRead == 0){      /* EOF */
			if (totRead == 0){          /* No bytes read; return 0 */
		        return 0;
			}else{                        /* Some bytes read; add '\0' */
				break;
			}

		}else{                  /* 'numRead' must be 1 if we get here */
			if (totRead < n - 1) {      /* Discard > (n - 1) bytes */
				totRead++;
				*buf++= ch;
			}

			if (ch == '\n'){
				break;
			}
		}
	}

	*buf = '\0';
	return totRead;
}

/*
** ===================================================================
**     Método      :  becomeDaemon
**
**     @resumen
**          Vuelve al proceso actual un proceso demonio. 
**			
**     @param
**          flags     	   	- Opciones del demonio
**     @return
**                         	- Estado de salida del método (-1 error). 
**
** ===================================================================*/
int becomeDaemon(int flags)
{
	int maxfd, fd;


	switch (fork()) {                   /* Become background process */
		case -1: return -1;
		case 0:  break;                     /* Child falls through... */
		default: _exit(EXIT_SUCCESS);       /* while parent terminates */
	}

	if (setsid() == -1){                 /* Become leader of new session */
		return -1;
	}

	switch (fork()) {                   /* Ensure we are not session leader */
		case -1: return -1;
		case 0:  break;
		default: _exit(EXIT_SUCCESS);
	}

	if (!(flags & BD_NO_UMASK0)){
		umask(0);                       /* Clear file mode creation mask */
	}

	if (!(flags & BD_NO_CHDIR)){
		chdir("/");                     /* Change to root directory */
	}

	if (!(flags & BD_NO_CLOSE_FILES)) { /* Close all open files */
		maxfd = sysconf(_SC_OPEN_MAX);
		if (maxfd == -1){                /* Limit is indeterminate... */
			maxfd = BD_MAX_CLOSE;       /* so take a guess */
		}

		for (fd = 0; fd < maxfd; fd++){
			close(fd);
		}
	}

	if (!(flags & BD_NO_REOPEN_STD_FDS)) {
		close(STDIN_FILENO);            /* Reopen standard fd's to /dev/null */

		fd = open("/dev/null", O_RDWR);

		if (fd != STDIN_FILENO){        /* 'fd' should be 0 */
			return -1;
		}
		if (dup2(STDIN_FILENO, STDOUT_FILENO) != STDOUT_FILENO){
			return -1;
		}
		if (dup2(STDIN_FILENO, STDERR_FILENO) != STDERR_FILENO){
			return -1;
		}
	}

	return 0;
}

/*
** ===================================================================
**     Método      :  logMessage
**
**     @resumen
**          Imprime un mensaje en el log. 
**			
**     @param
**          flags     	   	- Mensaje a imprimir
**
** ===================================================================*/
void logMessage(const char *format, ...)
{
	va_list argList;
	const char *TIMESTAMP_FMT = "%F %X";        /* = YYYY-MM-DD HH:MM:SS */
	char timestamp[TS_BUF_SIZE];
	time_t t;
	struct tm *loc;

	t = time(NULL);
	loc = localtime(&t);
	if (loc == NULL || strftime(timestamp, TS_BUF_SIZE, TIMESTAMP_FMT, loc) == 0)
		fprintf(logfp, "???Unknown time????: ");
	else
		fprintf(logfp, "%s: ", timestamp);

	va_start(argList, format);
	vfprintf(logfp, format, argList);
	fprintf(logfp, "\n");
	va_end(argList);
}

/*
** ===================================================================
**     Método      :  logOpen
**
**     @resumen
**          Abre un archivo para almacenar el log. 
**			
**     @param
**          flags     	   	- Ruta del archivo que contiene el log
**
** ===================================================================*/
void logOpen(const char *logFilename)
{
	mode_t m;

	m = umask(077);
	logfp = fopen(logFilename, "a");
	umask(m);

	/* If opening the log fails we can't display a message... */

	if (logfp == NULL)
		exit(EXIT_FAILURE);

	setbuf(logfp, NULL);                    /* Disable stdio buffering */

	logMessage("Opened log file");
}

/*
** ===================================================================
**     Método      :  logClose
**
**     @resumen
**          Cierra el archivo que almacena el log. 
**			
**
** ===================================================================*/
void logClose(void)
{
	logMessage("Closing log file");
	fclose(logfp);
}

/*
** ===================================================================
**     Método      :  configurar_servidor
**
**     @resumen
**          Configura el servidor y lo deja listo para aceptar solicitudes. 
**			
**     @return
**                         	- Estado de salida del método (-1 error). 
**
** ===================================================================*/
int configurar_servidor(void){

	struct addrinfo hints; // Estructura de informacion del socket y la direccion
	struct addrinfo *result, *rp;//
	int lfd, cfd, optval;//

	/* Call getaddrinfo() to obtain a list of addresses that
	we can try binding to */
	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_canonname = NULL;
	hints.ai_addr = NULL;
	hints.ai_next = NULL;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_family = AF_UNSPEC;        /* Allows IPv4 or IPv6 */
	hints.ai_flags = AI_PASSIVE | AI_NUMERICSERV;
                       /* Wildcard IP address; service name is numeric */

	// Identifica la informacion de las direcciones IP del computador
	if (getaddrinfo(NULL, PORT_NUM, &hints, &result) != 0){
		logMessage("Fail getting address info (%s)", strerror(errno));
		exit(3);
	}

	/* Walk through returned list until we find an address structure
       that can be used to successfully create and bind a socket */
	optval = 1;
	for (rp = result; rp != NULL; rp = rp->ai_next) {
		lfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
	        if (lfd == -1)
	            continue;                   /* On error, try next address */

	        if (setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval))== -1){		
			logMessage("Fallo al configurar las opciones del socket (%s)", strerror(errno));
			return -1;
		}

	        if (bind(lfd, rp->ai_addr, rp->ai_addrlen) == 0)
        	    break;                      /* Success */

        	/* bind() failed: close this socket and try next address */

        	close(lfd);
	}

	if (rp == NULL){
		logMessage("No se pudo enlazar a ningun socket (%s)", strerror(errno));
		return -1;
	}

	if (listen(lfd, BACKLOG) == -1){
		logMessage("Fallo escuchando (%s)", strerror(errno));
		return -1;
	}
	return lfd;

	logMessage("Se configuro el servidor exitosamente");

	freeaddrinfo(result);
}

/*
** ===================================================================
**     Método      :  escribir_respuesta
**
**     @resumen
**          Escribe un String en el socket y añade un fin de línea. 
**			
**     @param
**          respuesta  	   	- String a escribir en el socket
**     @param
**                         	- File Descriptor del Socket. 
**     @return
**                         	- Estado de salida del método (-1 error). 
**
** ===================================================================*/
int escribir_respuesta(char* respuesta, int cfd){

	if(write(cfd,respuesta,strlen(respuesta))<0){
		logMessage("Fallo al escribir en el Socket (%s)", strerror(errno));
		return -1;
	}

	if(write(cfd,"\n",1)<0){
		logMessage("Fallo al escribir en el Socket un ENTER(%s)", strerror(errno));
		return -1;
	}
	return 0;
}

/*
** ===================================================================
**     Método      :  apagar_tarjeta
**
**     @resumen
**          Apaga la tarjeta Galileo. 
**
**     @return
**                         	- Estado de salida del método (-1 error). 
**
** ===================================================================*/
int apagar_tarjeta(void){
	//Se ejecuta la orden poweroff	
	if(system("poweroff")<0){
		logMessage("Error al apagar la tarjeta");
		return -1;
	}
	return 0;
}

/*
** ===================================================================
**     Método      :  configurar_fecha
**
**     @resumen
**          Configura la fecha de la tarjeta Galileo. 
**			
**     @param
**          fecha_hora     	- Fecha en formato default (ver interfaz)
**
**     @return
**                         	- Estado de salida del método (-1 error). 
**
** ===================================================================*/
int configurar_fecha(char* fecha_hora){
	char* anho;
	char* mes;
	char* dia;
	char* hora;
	char* minutos;
	char* segundos;
	char formato_fecha[40];
	
	//Se separa el string con la fecha y la hora
	anho = strtok(fecha_hora, "-");
	logMessage("anho: %s",anho);
	mes = strtok(NULL, "-");
	logMessage("mes: %s",mes);
	dia = strtok(NULL, "-");
	logMessage("dia: %s",dia);
	hora = strtok(NULL, "-");
	logMessage("hora: %s",hora);
	minutos = strtok(NULL, "-");
	logMessage("minutos: %s",minutos);
	segundos = strtok(NULL, "-");
	logMessage("segundos: %s",segundos);
	
	//Se crea el comando de configuracion de la hora en BASH
	sprintf(formato_fecha, "date --set \"%s-%s-%s %s:%s\"",anho, mes, dia, hora, minutos);
	logMessage(formato_fecha);

	//Se ejecuta la orden date	
	if(system(formato_fecha)!=0){
		logMessage("No se pudo cambiar la hora");
		return -1;
	}
	logMessage("Se cambio la hora exitosamente");
	return 0;


}

/*
** ===================================================================
**     Método      :  parser_comandos
**
**     @resumen
**          Analiza el comando que llegó y ejecuta una acción. Los
**		comandos analizados son comandos simples. Es decir no
**		tienen parámetros después del comando.
**			
**     @param
**          comando     	- comando a ejecutar
**     @param
**          cfd		     	- File Descriptor del Socket
**
**     @return
**                         	- Estado de salida del método (-1 error). 
**
** ===================================================================*/
int parser_comandos(char* comando, int cfd){
	char str[18];
	rover_status status = NC;
	/* Si hay una peticion de conectar */
	if(strcmp(comando,"conectar\n")==0){ //Comando que retorna el estado de la tarjeta y pasa a estado SAFE
		logMessage("Llego el comando conectar");

		//Cambia el estado de la Galileo
		status=SAFE;
		logMessage("Paso a estado Safe");

		//Imprime el status en la variable str
		sprintf(str, "%u", status);
		if(escribir_respuesta(str,cfd)==-1){
			logMessage("Error al responder");
			return -1;
		}

	}else if(strcmp(comando,"fecha\n")==0){ //Comando que cambia la fecha y la hora
		logMessage("Llego el comando fecha");

		//Informa al cliente que llegó la petición
		if(escribir_respuesta("OK",cfd)==-1){
			logMessage("Error al responder");
			return -1;
		}

		//Lee la fecha y la hora
		if (readLine(cfd, str, 19) <= 0) {
			close(cfd);
			logMessage("No hay fin de línea");	
			return -1;
        	}
		logMessage(str);

		//Configura la fecha y la hora
		if(configurar_fecha(str)==-1){
			logMessage("No se pudo configurar la fecha y la hora");
			return -1;
		}

	}else if(strcmp(comando,"estado\n")==0){ //Comando que retorna el estado de la tarjeta
		logMessage("Llego el comando estado");
		
		//Imprime el status en la variable str
		sprintf(str, "%u", status);
		logMessage(str);
		if(escribir_respuesta(str,cfd)==-1){
			logMessage("Error al responder");
			return -1;
		}
	}else if(strcmp(comando,"desconectar\n")==0){ //Comando que apaga la tarjeta
		logMessage("Llego el comando desconectar");
		
		//Imprime el status en la variable str
		if(apagar_tarjeta()==-1){
			logMessage("No se pudo apagar");
			return -1;
		}
	}else{ //Comando no válido
		logMessage("Comando no valido");
		return -1;
	}
	return 0;

}

/*
** ===================================================================
**     Método      :  parser_comandos_mov
**
**     @resumen
**          Analiza el comando que llegó y ejecuta una acción. Los
**		comandos analizados son comandos de movimiento. Es 
**		decir tiene parámetros después del comando. Los pa-
**		rámetros definen el movimiento
**			
**     @param
**          comando     	- comando a ejecutar
**     @param
**          cfd		     	- File Descriptor del Socket
**
**     @return
**                         	- Estado de salida del método (-1 error). 
**
** ===================================================================*/

int parser_comandos_mov(char* comando, int cfd){
	char str[18];
	char* 	accion;
	char* 	parte;
	char* 	modo;
	char* 	direccion;
	char* 	s_velocidad;
	uint8_t	velocidad;
	char* 	angulo1;
	char* 	angulo2;
	char* 	angulo3;
 	uint8_t giro;
 	uint8_t res;

	char formato_fecha[40];

	logMessage("Comando que ingreso al metodo: %s",comando);
	
	//Se separa la accion del string que contiene el comando
	accion = strtok(comando, "/");
	logMessage("accion: %s",accion);
	if(strcmp(accion,"mover")!=0){
		logMessage("Error: El metodo se llamo incorrectamente");
		return -1;
	}

	//Se separa la parte a mover del string que contiene el comando
	parte = strtok(NULL, "/");
	logMessage("Parte a mover: %s",parte);

	//Se determina que parte se va a mover, se obtienen los parámetros y se mueve
	if(strcmp(parte,"brazo")==0){
		logMessage("Se va a mover el brazo");

		modo = strtok(NULL, "/");
		logMessage("Modo de operacion:%s",modo);

		//Se determinan los parámetros
		angulo1 = strtok(NULL, "/");
		angulo2 = strtok(NULL, "/");
		angulo3 = strtok(NULL, "/");
		
		// Se ejecuta la accion asociada al modo de operación del brazo
		if(strcmp(modo,"auto")==0){
			logMessage("Accion de modo automatico");
			logMessage("Angulos: %s, %s, %s",angulo1,angulo2,angulo3);

			/*TODO Posicionar brazo */

						
		}else if(strcmp(modo,"normal")==0){
			logMessage("Accion de modo normal");
			logMessage("Angulos: %s, %s, %s",angulo1,angulo2,angulo3);

			/*TODO Mover brazo de a pasos pequeños */

		}else{
			logMessage("Error: Modo desconocido");
			return -1;	
		}
	}else if(strcmp(parte,"traccion")==0){
		logMessage("Se va a mover la tracción");

		direccion = strtok(NULL, "/");
		logMessage("Direccion del ROVER:%s",direccion);

		//Se determinan los parámetros
		s_velocidad = strtok(NULL, "/");
		velocidad=atoi(s_velocidad);

		// Se ejecuta la accion asociada al modo de operación del brazo
		if(strcmp(direccion,"f")==0){
			logMessage("Mover adelante");
			logMessage("Velocidad: %d",velocidad);	

			/*TODO Comandos para mover adelante */
			if(tr_forward(velocidad)){
				logMessage("Error moviendo hacia adelante");
			}
					
		}else if(strcmp(direccion,"b")==0){
			logMessage("Mover atras");
			logMessage("Velocidad: %d",velocidad);

			/*TODO Comandos para mover atras */
			if(tr_backward(velocidad)){
				logMessage("Error moviendo hacia atras");
			}

		}else if(strcmp(direccion,"r")==0){
			logMessage("girar derecha");
			logMessage("Velocidad: %d",velocidad);						

			if(giro==1){
				logMessage("Giro sobre el eje\n");
				res=tr_spin(TR_TURN_RIGHT, velocidad);	
			}else if(giro==2){
				logMessage("Giro diferencial\n");
				res=tr_diffTurn(TR_TURN_RIGHT, velocidad);	
			}else if(giro==3){
				logMessage("Giro diferencial diagonal\n");
				res=tr_diagonalDiffTurn(TR_TURN_RIGHT, velocidad);
			}
			if(res){
				printf("Error girando a la derecha\n");
			}

			/*TODO Comandos para girar a la derecha */

		}else if(strcmp(direccion,"l")==0){
			logMessage("Girar izquierda");
			logMessage("Velocidad: %d",velocidad);

			/*TODO Comandos para girar a la izquierda */
			if(giro==1){
				logMessage("Giro sobre el eje\n");
				res=tr_spin(TR_TURN_LEFT, velocidad);	
			}else if(giro==2){
				logMessage("Giro diferencial\n");
				res=tr_diffTurn(TR_TURN_LEFT, velocidad);	
			}else if(giro==3){
				logMessage("Giro diferencial diagonal\n");
				res=tr_diagonalDiffTurn(TR_TURN_LEFT, velocidad);	
			}
			if(res){
				logMessage("Error girando a la izquierda\n");
				return -1:
			}			

		}else if(strcmp(direccion,"c")==0){

			if(velocidad==1){
				logMessage("Escogido: Giro sobre el eje\n");
				giro=velocidad;
			}else if(velocidad==2){
				logMessage("Escogido: Giro diferencial\n");
				giro=velocidad;
			}else if(velocidad==3){
				logMessage("Escogido: Giro diferencial diagonal\n");
				giro=velocidad;
			}else{
				logMessage("Error: En valor ingresado %d no corresponde a ningún giro\n",velocidad);
				return -1;
			}
			

		}else if(!strcmp(direccion,"e")){
			printf("Freno de Emergencia!");
			if(tr_eBrake()){
				logMessage("Error en tr_eBrake para freno de Emergencia");
				return -1;
			}

		}else if(!strcmp(direccion,"x")){
			printf("Deteniendo\n");
			if(tr_setVP(0)){
				logMessage("Error en tr_setVP para freno común");
				return -1;
			}

		}else if(!strcmp(direccion,"i")){
			printf("Estado a %d\n",velocidad);
			if(tr_setCtlState(velocidad)={
				logMessage("Error en tr_setCtlState");
				return -1;
			}

		}else{
			logMessage("Error: Direccion desconocida");
			return -1;	
		}
		
	}else{
		logMessage("Error: Parte a mover desconocida");
		return -1;
	}

	return 0;

}
/*
** ===================================================================
**     Método      :  parser_comandos_diag
**
**     @resumen
**          Analiza el comando que llegó y ejecuta una acción. Los
**		comandos analizados son comandos de diagnostico. 
**			
**     @param
**          comando     	- comando a ejecutar
**     @param
**          cfd		     	- File Descriptor del Socket
**
**     @return
**                         	- Estado de salida del método (-1 error). 
**
** ===================================================================*/
int parser_comandos_diag(char* comando, int cfd){
	char str[18];
	char* accion;
	char* parte;
	char respuesta[INT_LEN];
	int fd;
	int sz;
	int escritos;

	char formato_fecha[40];
	
	//Se separa la accion del string que contiene el comando
	accion = strtok(comando, "/");
	logMessage("accion: %s",accion);
	if(strcmp(accion,"diagnosticar")!=0){
		logMessage("Error: El metodo se llamo incorrectamente");
		return -1;
	}

	//Se separa la parte a diagnosticar del string que contiene el comando
	parte = strtok(NULL, "/");
	logMessage("Parte a diagnosticar: %s",parte);
	logMessage("Comando: %s",comando);

	//Se determina que parte se va a diagnosticar, se obtienen los parámetros y se envían
	if(strcmp(parte,"brazo\n")==0){
		logMessage("Se va a diagnosticar el brazo");
		
		// Se abre el archivo que contiene el diagnostico
		fd=open("/home/root/LOGS/diagnostico_brazo.txt",O_RDONLY);				
		if(fd==-1){
			logMessage("Error: No se pudo abrir el archivo que contiene el diagnostico");
			return -1;
		}

		// Se obtiene el tamanho del archivo
		sz=fsize("/home/root/LOGS/diagnostico_brazo.txt");
		logMessage("Tamanho del archivo = %i", sz);

		// Se envia el tamanho del archivo
		sprintf(str, "%d", sz);
		if(escribir_respuesta(str,cfd)==-1){
			logMessage("Error: no se envio el tamanho");
			return -1;
		}

		// Lee una linea de respuesta  
       	if (readLine(cfd, respuesta, INT_LEN) <= 0) {
 			logMessage("Error: No hay fin de línea");
 			return -1;
		}

		logMessage("Llego: %s",respuesta);

		// Compara la respuesta y envia el archivo de diagnóstico
		if(strcmp(respuesta,"ok\n")==0){
			escritos = sendfile(cfd, fd, NULL, sz);
			if (escritos == -1){
				logMessage("Error: No se pudo enviar el archivo");
				return -1;				
			}
			logMessage("Se envio correctamente el archivo");
		}

		// Cierra el archivo del diagnostico
		close(fd);
	}else if(strcmp(parte,"traccion\n")==0){
		logMessage("Se va a diagnosticar la traccion");
		if (tr_diagnostico()){
			logMessage("Error en el diagnóstico de tracción");
			return -1;
		}
		// Se abre el archivo que contiene el diagnostico
		fd=open(TR_DIAGNOSTIC_PATH,O_RDONLY);				
		if(fd==-1){
			logMessage("Error: No se pudo abrir el archivo que contiene el diagnostico");
			return -1;
		}

		// Se obtiene el tamanho del archivo
		sz=fsize(TR_DIAGNOSTIC_PATH);
		logMessage("Tamanho del archivo = %i", sz);

		// Se envia el tamanho del archivo
		sprintf(str, "%d", sz);
		if(escribir_respuesta(str,cfd)==-1){
			logMessage("Error: no se envio el tamanho");
			return -1;
		}

		// Lee una linea de respuesta  
       	if (readLine(cfd, respuesta, INT_LEN) <= 0) {
			logMessage("Error: No hay fin de línea");
			return -1;	
		}

		logMessage("Llego: %s",respuesta);

		// Compara la respuesta y envia el archivo de diagnóstico
		if(strcmp(respuesta,"ok\n")==0){
			escritos = sendfile(cfd, fd, NULL, sz);
			if (escritos == -1){
				logMessage("Error: No se pudo enviar el archivo");
				return -1;				
			}
			logMessage("Se envio correctamente el archivo");
		}

		// Cierra el archivo del diagnostico
		close(fd);
	
	}else{
		logMessage("Error: Parte a diagnosticar desconocida");
		return -1;
	}

	return 0;

}