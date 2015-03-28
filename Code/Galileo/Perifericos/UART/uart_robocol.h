#ifndef UART_ROBOCOL
#define UART_ROBOCOL

#include <stdint.h>                                                          
#include <stdlib.h> 
#include <stdio.h>                                                         
#include <linux/types.h>                                                      
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/signal.h>
#include <sys/types.h>
#define _POSIX_SOURCE 1 /* POSIX compliant source */
#define FALSE 0
#define TRUE 1
#define UART_ERROR 		0x01;
#define UART_OK 	 	0x00;
static volatile char WAIT_FLAG=TRUE ;
typedef unsigned char uart_st;

typedef struct uart_dev
{
	int fd;
	struct termios oldt;
	struct termios newt;
	struct sigaction saio;
}uart_dev;

struct uart_dev u_dev;

uart_st uart_open(const char * path,struct termios * termios_p );
uart_st uart_close(void);
uart_st uart_write(const void* buff, int size);
uart_st uart_read(void* buff, int size);
void sig_handler(int status);
uint8_t getWaitFlag(void);
uart_st setWaitFlag(uint8_t flag);


//		TODO: PROVE MEMORY DEPLETION WHEN CLOSING DEVICE.

#endif /*UART_ROBOCOL*/