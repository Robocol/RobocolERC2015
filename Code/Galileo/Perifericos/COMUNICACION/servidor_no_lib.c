/*
** =====================================================================================
**     USO DEL DRIVER
*/
/*!
**     @resumen
**          Esta librería incluye los métodos de alto nivel que manejan las comunicaciones internas del ROVER  y las comunicaciones
**		del ROVER con el cuarto de control. El ROVER tiene 4 modos de operación: 
**
**          mode 			- Modo de inicio (INIT): En este modo las Galileo se encienden, se ejecutan sus scripts de inicio y **					se sincroniza su reloj. Una vez sincronizados los relojs la tarjeta cambia a estado SAFE. En este **					estado se ejecuta el main que utiliza esta librería.
**
**					- Modo de operación segura (SAFE): Es el estado incial del ROVER después del encendido. En 
**					este modo el ROVER inicia todos los drivers y realiza un diagnóstico general de su compor-
**					tamiento. En este modo NO se puede mover ningún actuador o motor pero sí se puede verificar la
**					comunicación con todas las tarjetas. Adicionalmente, en este modo el ROVER está a la expectativa
**					de una orden por cualquiera de las alternativas de comunicación externa que lo haga cambiar a modo
**					normal. Si hay alguna falla en la comunicación el ROVER lo informará a nivel de la interfaz; **					dependiendo de la falla se podrá o no pasar a modo NORMAL. En este modo se pueden hacer los cambios **					de configuración necesarios en cada tarjeta: Negociar los diccionarios
**
**					- Modo de operación normal (NORMAL): En este modo el ROVER cumple con un ciclo de operación que 
**					se encarga de adquirir, compilar y transmitir los diagnósticos. Además actualiza los setpoints de
**					los controladores con base en lo que determine el usuario a nivel de la interfaz de usuario. La **					operación en este modo es cíclica. Es decir, cada cierto tiempo se adquieren los datos de diagnóstico
**					y cada cierto tiempo se actualizan los setpoints.
**
**					- Modo de recuperación de fallas (FAIL): En este modo el ROVER sigue operando ya que la falla que 
**					pudo haberse producido no afecta la operación completa del ROVER. La falla se indica a nivel de la **					interfaz de monitoreo y el operador de dicha interfaz puede tomar acciones de recuperación mientras **					que el operador principal maneja el ROVER. En caso que la falla sea una falla total, el sistema **					ingresa a modo de operación segura y proporciona un diagnóstico. En resumen, en este modo el ROVER 
**					funciona de forma acíclica: no envía diagnósticos ciclicamente ya que permite pequeñas interrupciones
**					en el ciclo para recuperar las fallas parciales.
**
**
*/
/* =====================================================================================*/
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
#include "traccion_robocol.h"

#define CLEAR(x) memset(&(x), 0, sizeof(x))

#define PORT_NUM "50002"        /* Port number for server */

#define INT_LEN 40              /* Size of string able to hold largest
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

/* Modos de operación de cada tarjeta */
typedef char galileo_role;
#define MASTER 0x01
#define SLAVE  0x02


/* Modos de operacion del brazo */
typedef char arm_mode;
#define MODO_AUTO 0X01
#define MODO_NORMAL 0x02

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
galileo_role rol_net = SLAVE;
uint8_t giro,res;


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

/*
** ===================================================================
**     Método      :  logClose
**
**     @resumen
**          Cierra el archivo que almacena el log. 
**			
**
** ===================================================================*/
static void logClose(void)
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
			logMessage("Velocidad: %s",velocidad);	

			/*TODO Comandos para mover adelante */
			if(tr_forward(velocidad)){
				logMessage("Error moviendo hacia adelante");
			}
					
		}else if(strcmp(direccion,"b")==0){
			logMessage("Mover atras");
			logMessage("Velocidad: %s",velocidad);

			/*TODO Comandos para mover atras */
			if(tr_backward(velocidad)){
				logMessage("Error moviendo hacia atras");
			}

		}else if(strcmp(direccion,"r")==0){
			logMessage("girar derecha");
			logMessage("Velocidad: %s",velocidad);						

			if(giro==1){
				logMessage("Giro sobre el eje\n");
				res=tr_spin(TR_TURN_RIGHT, tr_device.vel_pwm);	
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

		}else if(strcmp(modo,"l")==0){
			logMessage("Girar izquierda");
			logMessage("Velocidad: %s",velocidad);

			/*TODO Comandos para girar a la izquierda */
			if(giro==1){
				logMessage("Giro sobre el eje\n");
				res=tr_spin(TR_TURN_LEFT, tr_device.vel_pwm);	
			}else if(giro==2){
				logMessage("Giro diferencial\n");
				res=tr_diffTurn(TR_TURN_LEFT, velocidad);	
			}else if(giro==3){
				logMessage("Giro diferencial diagonal\n");
				res=tr_diagonalDiffTurn(TR_TURN_LEFT, velocidad);	
			}
			if(res){
				printf("Error girando a la izquierda\n");
			}			

		}else if(strcmp(modo,"c")==0){

			if(velocidad==1){
				printf("Escogido: Giro sobre el eje\n");
				giro=velocidad;
			}else if(velocidad==2){
				printf("Escogido: Giro diferencial\n");
				giro=velocidad;
			}else if(velocidad==3){
				printf("Escogido: Giro diferencial diagonal\n");
				giro=velocidad;
			}else{
				printf("Error: En valor ingresado %d no corresponde a ningún giro\n",velocidad);
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
		fd=open("/home/root/diagnostico_brazo.txt",O_RDONLY);				
		if(fd==-1){
			logMessage("Error: No se pudo abrir el archivo que contiene el diagnostico");
			return -1;
		}

		// Se obtiene el tamanho del archivo
		sz=fsize("/home/root/diagnostico_brazo.txt");
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
		
		// Se abre el archivo que contiene el diagnostico
		fd=open("/home/root/diagnostico_traccion.txt",O_RDONLY);				
		if(fd==-1){
			logMessage("Error: No se pudo abrir el archivo que contiene el diagnostico");
			return -1;
		}

		// Se obtiene el tamanho del archivo
		sz=fsize("/home/root/diagnostico_traccion.txt");
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

int main(int argc, char *argv[]) {
	
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
