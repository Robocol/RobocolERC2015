#include "uart_robocol.h"

uart_st uart_open(const char* path ,struct termios * termios_p ){
 	/*APERTURA DEL FILE DESCRIPTOR*/
 	if((u_dev.fd=open(path ,O_RDWR | O_NOCTTY | O_NONBLOCK))<0){
 		perror(path);
 		return UART_ERROR;
 	}
 	printf("Opened file descriptor %d\n",u_dev.fd );
 	u_dev.saio.sa_handler=sig_handler;
 	if(sigemptyset(&u_dev.saio.sa_mask)<0){
 		perror("Error setting the mask to signal handler in uart_open");
 		return UART_ERROR;
 	}
  printf("sigemptyset successful\n");
 	u_dev.saio.sa_flags = 0;
  u_dev.saio.sa_restorer = NULL;
  	if(sigaction(SIGIO,&u_dev.saio,NULL)<0){
  		perror("Error changing signal action in uart_open");
  		return UART_ERROR;
  	}
  printf("sigaction successful\n");

  	if(fcntl(u_dev.fd, F_SETOWN, getpid())<0){
  		perror("Error allowing the process to receive SIGIO");
  		return UART_ERROR;
  	}

  printf("fcntl successful\n");

	if(fcntl(u_dev.fd, F_SETFL,  O_ASYNC)<0){
		perror("Error setting the process to asynchronous in uart_open");
		return UART_ERROR;
	}

  if(tcgetattr(u_dev.fd,&u_dev.oldt)<0){
  		perror("Error getting the current terminal parameters in uart_open");
	  return UART_ERROR;	
  }
  	u_dev.newt=*termios_p;

	if(tcflush(u_dev.fd, TCIFLUSH)<0){
	  perror("Error cleaning data referred by the process' file descriptor");
    return UART_ERROR;
	}

  if(tcsetattr(u_dev.fd,TCSANOW,&u_dev.newt)<0){
    perror("Error setting parameters from termios passed by parameter to the process' file descriptor");
    return UART_ERROR;
  }
	return UART_OK;
}

uart_st uart_close(void){
  if (tcsetattr(u_dev.fd,TCSANOW,&u_dev.oldt)<0)
  {
    perror("Error setting old termios attributes");
    return UART_ERROR;
  }
 	if(close(u_dev.fd)<0){
 		perror("Error closing the process' file descriptor");
 		return UART_ERROR;
 	}
 	return UART_OK;
}

uart_st uart_write(const void* buff, int size){
    if(write(u_dev.fd,buff,size)<0){
      perror("Error en escritura de información hacia canal UART");
      return UART_ERROR;
    }
    return UART_OK;
}

uart_st uart_read(void* buff, int size){
	uint8_t len;
    if(len=read(u_dev.fd,buff,size)<0){
      perror("Error en lectura de información desde canal UART");
      return UART_ERROR;
    }
	buff[len]='\0';
    return UART_OK;
}

uint8_t getWaitFlag(void){
  return WAIT_FLAG;  
}

uart_st setWaitFlag(uint8_t flag){
  if (flag!=0 && flag!=1)
  {
    printf("El valor ingresado para WAIT_FLAG no es válido . Debe ser 0 o 1\n");
    return UART_ERROR;
  }
  WAIT_FLAG=flag;
  return UART_OK;
}


void sig_handler(int status)
{
	// Se pone a dormir el proceso para que el buffer tenga tiempo para llenarse
	usleep(10000);
	WAIT_FLAG=FALSE;
}
