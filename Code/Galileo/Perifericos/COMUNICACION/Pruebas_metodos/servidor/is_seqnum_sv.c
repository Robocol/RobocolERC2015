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

int main(int argc, char *argv[]) {

	//Variables locales servidor
	char reqLenStr[INT_LEN];	/* Buffer para almacenar lo que llega de un cliente */
	struct sockaddr_storage claddr;	/* Buffer para almacenar lo que llegs del cliente */	
	int lfd, cfd, optval;//
	socklen_t addrlen;
	struct addrinfo hints; // Estructura de informacion del socket y la direccion
	struct addrinfo *result, *rp;//
	#define ADDRSTRLEN (NI_MAXHOST + NI_MAXSERV + 10)
	ssize_t escritos;
	logOpen(LOG_FILE);

	if(becomeDaemon(0) == -1)
		logMessage("Fail becomeDaemon");



	
//Verifica los parametros ingresados por terminal
	if (argc > 2 && strcmp(argv[1], "--help") == 0){
		printf("%s [init-seq-num]\n", argv[0]);
		exit(EXIT_FAILURE);
	}

//Configuración servidor

	/* Ignore the SIGPIPE signal, so that we find out about broken connection
	errors via a failure from write(). */
	if (signal(SIGPIPE, SIG_IGN) == SIG_ERR){
	        logMessage("Fail ignoring SIGPIPE ");
		exit(EXIT_FAILURE);
	}

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
			logMessage("Fail setting socket options (%s)", strerror(errno));
			exit(4);
		}

	        if (bind(lfd, rp->ai_addr, rp->ai_addrlen) == 0)
        	    break;                      /* Success */

        	/* bind() failed: close this socket and try next address */

        	close(lfd);
	}

	if (rp == NULL){
		logMessage("Could not bind to any socket (%s)", strerror(errno));
		exit(5);
	}

	if (listen(lfd, BACKLOG) == -1){
		logMessage("Fail listening (%s)", strerror(errno));
		exit(6);
	}
	printf("Configura el servidor");
	logMessage("Configura el servidor");

	freeaddrinfo(result);


	for (;;) {                  /* Handle clients iteratively */

	        /* Accept a client connection, obtaining client's address */

        	addrlen = sizeof(struct sockaddr_storage);
        	cfd = accept(lfd, (struct sockaddr *) &claddr, &addrlen);
        	if (cfd == -1) {
			logMessage("Accept error (%s)", strerror(errno));
			continue;
        	}
		logMessage("acepto cliente ");

		printf("7. Adquirio la información del cliente\n");

		/* Lee la peticion del cliente y envia una respuesta o si no cierra el socket 
		y espera una nueva conexion */

	        if (readLine(cfd, reqLenStr, INT_LEN) <= 0) {
			close(cfd);
			logMessage("No hay fin de línea\n");	
			continue;                   
        	}

		printf(reqLenStr);

		/* Si hay una peticion de imagen adquiere y envia la imagen */
		if(strcmp(reqLenStr,"imagen\n")==0){
			logMessage("Ingreso al método para adquirir la imagen\n");
			
			logMessage("Leyo el estado:\n");
			
			logMessage("\n");

			/* Se verifica cual imagen es mas reciente 
			stat("./imagen1.mp4", &attrib1);	// get the attributes of afile.txt
			clock1 = localtime(&(attrib1.st_ctime));	// Get the last modified time
			tiempo=attrib1.st_mtim.tv_nsec; 
			imagen_rec="imagen1.mp4";

			
			

			 Envia el tamanho de la imagen */
			//sprintf(str, "%d", sz);
			escritos = write(cfd,"good",15);
			if(escritos==-1){
				logMessage("Failing writing socket (%s)", strerror(errno));
				exit(9);
			}

			escritos = write(cfd,"\n",1);		
			if(escritos==-1){
				logMessage("Failing writing socket (%s)", strerror(errno));
				exit(9);
			}

			// Lee una linea de respuesta  
        		if (readLine(cfd, reqLenStr, INT_LEN) <= 0) {
       				close(cfd);
	    			logMessage("No hay fin de linea (%s)", strerror(errno));	
				continue;                   
       			}

			logMessage(reqLenStr);
						
		
        	}		 

        	if (close(cfd) == -1)           /* Close connection */
			logMessage("Close error (%s)", strerror(errno));	
		
		logMessage("Finalizo rutina\n");
    	}

}
