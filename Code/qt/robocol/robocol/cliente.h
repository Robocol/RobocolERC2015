#ifndef CLIENTE
#define CLIENTE

#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <signal.h>

#include <sys/stat.h>
#include <fcntl.h>
#include <ctype.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <sys/types.h>

#include <unistd.h>
#include <errno.h>

#include <getopt.h>             /* getopt_long() */

#include <sys/time.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

#define INT_LEN 30

#define PORT_NUM "50002"        /* Port number for server */

#define MAX_READ 614400

#define IP_LLANTAS_IZQUIERDA "10.5.5.102"
#define IP_LLANTAS_DERECHA "192.168.0.103"
#define IP_BRAZO "10.5.5.104"

//unsigned char buffer[MAX_READ];

//unsigned char buffer_temp[MAX_READ];

/* Read characters from 'fd' until a newline is encountered. If a newline
  character is not encountered in the first (n - 1) bytes, then the excess
  characters are discarded. The returned string placed in 'buf' is
  null-terminated and includes the newline character if it was read in the
  first (n - 1) bytes. The function return value is the number of bytes
  placed in buffer (which includes the newline character if encountered,
  but excludes the terminating null byte). */


ssize_t readLine(int fd, char *buffer1, size_t n);
int conectarServidor(char* ip_servidor);
int enviarComando(char* comando, int cfd);
int cerrarConexion(int sfd);



#endif // CLIENTE

