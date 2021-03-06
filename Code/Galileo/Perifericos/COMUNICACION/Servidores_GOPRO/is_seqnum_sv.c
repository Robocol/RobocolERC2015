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

#include <linux/videodev2.h>
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

//Variables globales*************************************************************
bool archivo1=false;//Indica si el archivo 1 se modifico
bool archivo2=false;//Indica si el archivo 2 se modifico

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

int main(int argc, char *argv[]) {
//Variables locales servidor
	char reqLenStr[INT_LEN];            /* Length of requested sequence */
	struct sockaddr_storage claddr;
	int lfd, cfd, optval;//
	socklen_t addrlen;
	struct addrinfo hints; //
	struct addrinfo *result, *rp;//
	#define ADDRSTRLEN (NI_MAXHOST + NI_MAXSERV + 10)
	char addrStr[ADDRSTRLEN];
	char host[NI_MAXHOST];
	char service[NI_MAXSERV];
	ssize_t escritos;
	char str[15];
	bool inicio=true;
	struct tm* clock1;				// Crea una estructura de tiempo
	struct stat attrib1;			// crea una estrusctura de estadisticas	
	
	struct tm* clock2;				// Crea una estructura de tiempo
	struct stat attrib2;			// crea una estrusctura de estadisticas	
	
	struct tm* clock3;				// Crea una estructura de tiempo
	struct stat attrib3;			// crea una estrusctura de estadisticas	
	
	struct tm* clock4;				// Crea una estructura de tiempo
	struct stat attrib4;			// crea una estrusctura de estadisticas	
	
	struct tm* clock0;				// Crea una estructura de tiempo
	struct tm* clock;				// Crea una estructura de tiempo


//Variables para notificacion
	int fd, sz;
	char* frame="2";
	char* imagen_rec;
	long tiempo;
	long tiempo_ant=0;
	
//Verifica los parametros ingresados por terminal
	if (argc > 2 && strcmp(argv[1], "--help") == 0){
		printf("%s [init-seq-num]\n", argv[0]);
		exit(EXIT_FAILURE);
	}

//Configuración servidor

	/* Ignore the SIGPIPE signal, so that we find out about broken connection
	errors via a failure from write(). */
	if (signal(SIGPIPE, SIG_IGN) == SIG_ERR){
	        fprintf(stderr,"signal error");
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

	if (getaddrinfo(NULL, PORT_NUM, &hints, &result) != 0){
		fprintf(stderr,"getaddrinfo error");
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
			fprintf(stderr,"setsockopt error");
			exit(4);
		}

	        if (bind(lfd, rp->ai_addr, rp->ai_addrlen) == 0)
        	    break;                      /* Success */

        	/* bind() failed: close this socket and try next address */

        	close(lfd);
	}

	if (rp == NULL){
		fprintf(stderr,"Could not bind socket to any address");
		exit(5);
	}

	if (listen(lfd, BACKLOG) == -1){
		fprintf(stderr,"listen error");
		exit(6);
	}

	freeaddrinfo(result);


	for (;;) {                  /* Handle clients iteratively */

	        /* Accept a client connection, obtaining client's address */

        	addrlen = sizeof(struct sockaddr_storage);
        	cfd = accept(lfd, (struct sockaddr *) &claddr, &addrlen);
        	if (cfd == -1) {
			fprintf(stderr,"accept error");
			continue;
        	}
		printf("6. Acepto cliente\n");
/*
	        if (getnameinfo((struct sockaddr *) &claddr, addrlen,host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0)
		        snprintf(addrStr, ADDRSTRLEN, "(%s, %s)", host, service);
	        else
        		snprintf(addrStr, ADDRSTRLEN, "(?UNKNOWN?)");
        		printf("Connection from %s\n", addrStr);

*/
		printf("7. Adquirio la información del cliente\n");

		/* Lee la peticion del cliente y envia una respuesta o si no cierra el socket 
		y espera una nueva conexion */

	        if (readLine(cfd, reqLenStr, INT_LEN) <= 0) {
			close(cfd);
			printf("No hay fin de línea\n");	
			continue;                   
        	}

		printf(reqLenStr);

		/* Si hay una peticion de imagen adquiere y envia la imagen */
		if(strcmp(reqLenStr,"imagen\n")==0){
			printf("Ingreso al método para adquirir la imagen\n");
			
			printf("Leyo el estado:\n");
			printf(frame);
			printf("\n");

			/* Se verifica cual imagen es mas reciente */
			stat("./imagen1.mp4", &attrib1);	// get the attributes of afile.txt
			clock1 = localtime(&(attrib1.st_ctime));	// Get the last modified time
			tiempo=attrib1.st_mtim.tv_nsec; 
			imagen_rec="imagen1.mp4";
/*			stat("./imagen2.mp4", &attrib2);	// get the attributes of afile.txt
			clock2 = localtime(&(attrib2.st_ctime));	// Get the last modified time
			printf("mtime = %lld.%.9ld\n",(long long)attrib2.st_mtim.tv_sec, attrib1.st_mtim.tv_nsec);
			stat("./imagen3.mp4", &attrib3);	// get the attributes of afile.txt
			clock3 = localtime(&(attrib3.st_ctime));	// Get the last modified time 
			printf("mtime = %lld.%.9ld\n",(long long)attrib3.st_mtim.tv_sec, attrib1.st_mtim.tv_nsec);
			stat("./imagen4.mp4", &attrib4);	// get the attributes of afile.txt
			clock4 = localtime(&(attrib4.st_ctime));	// Get the last modified time
			printf("mtime = %lld.%.9ld\n",(long long)attrib4.st_mtim.tv_sec, attrib1.st_mtim.tv_nsec);

			printf("sec 1:%i \n",clock1->tm_sec);
			printf("sec 2:%i \n",clock2->tm_sec);
			printf("sec 3:%i \n",clock3->tm_sec);
			printf("sec 4:%i \n",clock4->tm_sec);

			printf("Obtuvo estadisiticas de los archivos\n");

			if(clock1->tm_sec > clock2->tm_sec){
				printf("1>2\n");
				if(clock1->tm_sec > clock3->tm_sec){
					printf("1>3\n");
					if(clock1->tm_sec >= clock4->tm_sec){
						printf("1>=4\n");
						imagen_rec="imagen1.mp4";
						clock=clock1;
						if(inicio){
							clock0=clock2;
							inicio=false;
						}
					}else{
						printf("4>1\n");
						imagen_rec="imagen4.mp4";
						clock=clock4;
						if(inicio){
							clock0=clock2;
							inicio=false;
						}
					}
				}else{
					printf("3>=1\n");					
					if(clock3->tm_sec > clock4->tm_sec){
						printf("3>4\n");
						imagen_rec="imagen3.mp4";
						clock=clock3;
						if(inicio){
							clock0=clock2;
							inicio=false;
						}
					}else{
						printf("4>=3\n");					
						imagen_rec="imagen4.mp4";
						clock=clock4;
						if(inicio){
							clock0=clock2;
							inicio=false;
						}
					}
				}	
			}else{
				printf("2>=1\n");				
				if(clock2->tm_sec > clock3->tm_sec){
					printf("2>3\n");
					if(clock2->tm_sec > clock4->tm_sec){
						printf("2>4\n");
						imagen_rec="imagen2.mp4";
						clock=clock2;
						if(inicio){
							clock0=clock3;
							inicio=false;
						}
					}else{
						printf("4>=2\n");
						imagen_rec="imagen4.mp4";
						clock=clock4;
						if(inicio){
							clock0=clock3;
							inicio=false;
						}						
					}
				}else{
					printf("3>=2\n");
					if(clock3->tm_sec > clock4->tm_sec){
						printf("3>4\n");					
						imagen_rec="imagen3.mp4";
						clock=clock3;
						if(inicio){
							clock0=clock4;
							inicio=false;
						}
					}else{
						printf("4>=3\n");					
						imagen_rec="imagen4.mp4";
						clock=clock4;
						if(inicio){
							clock0=clock2;
							inicio=false;
						}
					}
				}		
			}	
			
			printf("Archivo mas reciente:");
			printf(imagen_rec);
			printf("\n");
*/			
			

			/* Verifica si el archivo no se ha enviado antes */
			if(tiempo!=tiempo_ant){
				printf("El archivo no se envio antes\n");				
				tiempo_ant=tiempo;
				printf("Comparo frame\n");
				/* Abre el archivo de la imagen */				
				fd=open(imagen_rec,O_RDONLY);				
				if(fd==-1){
					fprintf(stderr,"No se pudo abrir el archivo de la imagen");
					exit(9);
				}
				printf("Archivo abierto\n");
				
				sz=fsize(imagen_rec);				
				
				printf("tamanho = %i", sz);				

				/* Envia el tamanho de la imagen */
				sprintf(str, "%d", sz);
				escritos = write(cfd,str,15);
				if(escritos==-1){
					fprintf(stderr,"No se pudo escribir en el socket");
					exit(9);
				}

				escritos = write(cfd,"\n",1);		
				if(escritos==-1){
					fprintf(stderr,"No se pudo escribir en el socket");
					exit(9);
				}

				// Lee una linea de respuesta  
	        		if (readLine(cfd, reqLenStr, INT_LEN) <= 0) {
        				close(cfd);
     		    			printf("No hay fin de línea\n");	
					continue;                   
        			}

				printf(reqLenStr);

				/* Compara la respuesta y envia la imagen */
				if(strcmp(reqLenStr,"tamanho\n")==0){
					escritos = sendfile(cfd, fd, NULL, sz);
	        			if (escritos == -1){
						fprintf(stderr,"No se pudo escribir en el socket");
						exit(9);
					}
				}

				/* Cierra el archivo de la imagen */
				close(fd);				
		
			}else{
				printf("No hay una imagen nueva\n");				
				escritos = write(cfd,"No hay imagen\n",14);		
				if(escritos==-1){
					fprintf(stderr,"No se pudo escribir en el socket");
					exit(9);
				}
			}
						
			
        	}		 

        	if (close(cfd) == -1)           /* Close connection */
			fprintf(stderr,"close error");		
		
		printf("Finalizo rutina\n");
    	}

}
