#define _BSD_SOURCE             /* To get definitions of NI_MAXHOST and
                                   NI_MAXSERV from <netdb.h> */
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <signal.h>

#include <stdlib.h>
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

#define PORT_NUM "50002"        /* Port number for server */

#define INT_LEN 40              /* Size of string able to hold largest integer (including terminating '\n') */

#define BACKLOG 50


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
#define TS_BUF_SIZE sizeof("YYYY-MM-DD HH:MM:SS")       /* Includes '\0' */



///////////////////////////////////////////////////////////////////
/////////////////////*  VARIABLES  GLOBALES  */////////////////////
///////////////////////////////////////////////////////////////////
static FILE *logfp;                 /* Log file stream */

//Observar si es posible modificarlo
static const char *LOG_FILE = "/tmp/log_servidor.log";


///////////////////////////////////////////////////////////////////
/////////////////////////*  MÉTODOS  */////////////////////////////
///////////////////////////////////////////////////////////////////

off_t fsize(const char *filename);
ssize_t readLine(int fd, void *buffer, size_t n);
int becomeDaemon(int flags);
void logMessage(const char *format, ...);
void logOpen(const char *logFilename);
void logClose(void);
int configurar_servidor(void);
int escribir_respuesta(char* respuesta, int cfd);
int apagar_tarjeta(void);
int configurar_fecha(char* fecha_hora);
int parser_comandos(char* comando, int cfd);
int parser_comandos_mov(char* comando, int cfd);
int parser_comandos_diag(char* comando, int cfd);
