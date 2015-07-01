/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2010.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU Affero General Public License as published   *
* by the Free Software Foundation, either version 3 or (at your option)   *
* any later version. This program is distributed without any warranty.    *
* See the file COPYING.agpl-v3 for details.                               *
\*************************************************************************/

/* Listing 59-6 */

/* is_seqnum_sv.c

   A simple Internet stream socket server. Our service is to provide
   unique sequence numbers to clients.

   Usage:  is_seqnum_sv [init-seq-num]
                        (default = 0)

   See also is_seqnum_cl.c.
*/
#define _BSD_SOURCE             /* To get definitions of NI_MAXHOST and
                                   NI_MAXSERV from <netdb.h> */
#include <netdb.h>
 
#include <netinet/in.h>
#include <sys/socket.h>
#include <signal.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <getopt.h>             /* getopt_long() */

#include <fcntl.h>              /* low-level i/o */
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <sys/inotify.h>
#include <sys/sendfile.h>
#include <limits.h>
#include <sys/stat.h>
#include <time.h>
#include <syslog.h>

#include <stdarg.h>

#define CLEAR(x) memset(&(x), 0, sizeof(x))

#define PORT_NUM "50002"        /* Port number for server */

#define INT_LEN 30              /* Size of string able to hold largest
                                   integer (including terminating '\n') */

#define BACKLOG 50

#define MAX_FILE 1000000

#define BUF_LEN (10 * (sizeof(struct inotify_event) + NAME_MAX + 1))



#ifndef true
	typedef int bool;
	#define true 1
	#define false 0
#endif


/* Modos de operación del ROVER */
typedef uint8_t rover_status;
#define NC 0X00
#define INIT 0X01
#define SAFE 0x02
#define NORMAL 0x03
#define FAULT_RECOVERY  0x04


/* Bit-mask values for 'flags' argument of becomeDaemon() */

#define BD_NO_CHDIR           01    /* Don't chdir("/") */
#define BD_NO_CLOSE_FILES     02    /* Don't close all open files */
#define BD_NO_REOPEN_STD_FDS  04    /* Don't reopen stdin, stdout, and
                                       stderr to /dev/null */
#define BD_NO_UMASK0         010    /* Don't do a umask(0) */

#define BD_MAX_CLOSE  8192          /* Maximum file descriptors to close if
                                       sysconf(_SC_OPEN_MAX) is indeterminate */


//Variables globales*************************************************************
bool archivo1=false;//Indica si el archivo 1 se modifico
bool archivo2=false;//Indica si el archivo 2 se modifico
static FILE *logfp;                 /* Log file stream */

static const char *LOG_FILE = "/tmp/log_servidor.log";

struct buffer {
        void   *start;
        size_t length;
};

off_t fsize(const char *filename) {
    struct stat st; 

    if (stat(filename, &st) == 0)
        return st.st_size;

    return -1; 
}

/*	Variables Globales	*/
rover_status status = NC;


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
	for (;;) {

	        numRead = read(fd, &ch, 1);

		if (numRead == -1) {
			if (errno == EINTR)         /* Interrupted --> restart read() */
		                continue;
			else
		                return -1;              /* Some other error */

		} else if (numRead == 0) {      /* EOF */
			if (totRead == 0)           /* No bytes read; return 0 */
		                return 0;
			else                        /* Some bytes read; add '\0' */
				break;

		} else {                        /* 'numRead' must be 1 if we get here */
			if (totRead < n - 1) {      /* Discard > (n - 1) bytes */
				totRead++;
				*buf++ = ch;
			}

			if (ch == '\n')
		                break;
		}
	}

	*buf = '\0';
	return totRead;
}


int becomeDaemon(int flags)
{
    int maxfd, fd;

    switch (fork()) {                   /* Become background process */
    case -1: return -1;
    case 0:  break;                     /* Child falls through... */
    default: _exit(EXIT_SUCCESS);       /* while parent terminates */
    }

    if (setsid() == -1)                 /* Become leader of new session */
        return -1;

    switch (fork()) {                   /* Ensure we are not session leader */
    case -1: return -1;
    case 0:  break;
    default: _exit(EXIT_SUCCESS);
    }

    if (!(flags & BD_NO_UMASK0))
        umask(0);                       /* Clear file mode creation mask */

    if (!(flags & BD_NO_CHDIR))
        chdir("/");                     /* Change to root directory */

    if (!(flags & BD_NO_CLOSE_FILES)) { /* Close all open files */
        maxfd = sysconf(_SC_OPEN_MAX);
        if (maxfd == -1)                /* Limit is indeterminate... */
            maxfd = BD_MAX_CLOSE;       /* so take a guess */

        for (fd = 0; fd < maxfd; fd++)
            close(fd);
    }

    if (!(flags & BD_NO_REOPEN_STD_FDS)) {
        close(STDIN_FILENO);            /* Reopen standard fd's to /dev/null */

        fd = open("/dev/null", O_RDWR);

        if (fd != STDIN_FILENO)         /* 'fd' should be 0 */
            return -1;
        if (dup2(STDIN_FILENO, STDOUT_FILENO) != STDOUT_FILENO)
            return -1;
        if (dup2(STDIN_FILENO, STDERR_FILENO) != STDERR_FILENO)
            return -1;
    }

    return 0;
}

static void logMessage(const char *format, ...)
{
    va_list argList;
    const char *TIMESTAMP_FMT = "%F %X";        /* = YYYY-MM-DD HH:MM:SS */
#define TS_BUF_SIZE sizeof("YYYY-MM-DD HH:MM:SS")       /* Includes '\0' */
    char timestamp[TS_BUF_SIZE];
    time_t t;
    struct tm *loc;

    t = time(NULL);
    loc = localtime(&t);
    if (loc == NULL ||
           strftime(timestamp, TS_BUF_SIZE, TIMESTAMP_FMT, loc) == 0)
        fprintf(logfp, "???Unknown time????: ");
    else
        fprintf(logfp, "%s: ", timestamp);

    va_start(argList, format);
    vfprintf(logfp, format, argList);
    fprintf(logfp, "\n");
    va_end(argList);
}

/* Open the log file 'logFilename' */

static void logOpen(const char *logFilename)
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

/* Close the log file */

static void logClose(void)
{
    logMessage("Closing log file");
    fclose(logfp);
}

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

int apagar_tarjeta(void){
	//Se ejecuta la orden poweroff	
	if(system("poweroff")<0){
		logMessage("Error al apagar la tarjeta");
		return -1;
	}
	return 0;
}

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
	sprintf(formato_fecha, "date --set \"%s-%s-%s %s:%s:%s\"",anho, mes, dia, hora, minutos, segundos);
	logMessage("EL comando es: %s", formato_fecha);

	//Se ejecuta la orden date	
	if(system(formato_fecha)<0){
		logMessage("No se pudo cambiar la hora");
		return -1;
	}
	logMessage("Se cambio la hora exitosamente");
	return 0;


}

int parser_comandos(char* comando, int cfd){
	char str[18];
	
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
	}
	return 0;

}



int main(int argc, char *argv[]) {
	
	//Variables locales servidor
	char comando[INT_LEN];	/* Buffer para almacenar lo que llega de un cliente */
	struct sockaddr_storage claddr;	/* Buffer para almacenar lo que llegs del cliente */	
	int cfd,lfd;//
	socklen_t addrlen;
	char str[15];

	ssize_t escritos; // Bytes escritos
	
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

		/* Analiza el comando y realiza una acción */
		if(parser_comandos(comando, cfd)==-1)
			logMessage("No se respondio al comando");		
		
	}		 

      	if (close(cfd) == -1)           /* Close connection */
		logMessage("Close error (%s)", strerror(errno));	
	
	logMessage("Finalizo rutina\n");
    	

}
