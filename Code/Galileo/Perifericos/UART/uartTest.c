#include <stdint.h>                                                          
#include <stdlib.h>                                                          
#include <getopt.h>                                                          
#include <linux/types.h>                                                      
#include <termios.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/signal.h>
#include <sys/types.h>
#include <string.h>

#define BUFFSIZE 2
#define BAUDRATE B115200
#define MODEMDEVICE "/dev/ttyS0"
#define _POSIX_SOURCE 1 /* POSIX compliant source */
#define FALSE 0
#define TRUE 1

volatile int STOP=FALSE; 

void signal_handler_IO (int status);   /* definition of signal handler */
int wait_flag=TRUE;                    /* TRUE while no signal received */

main()
{
int fd,c, res;
struct termios oldtio,newtio;
struct sigaction saio;           /* definition of signal action */
char buf_tx[BUFFSIZE];
char buf_rx[BUFFSIZE];


/* open the device to be non-blocking (read will return immediatly) */
fd = open(MODEMDEVICE, O_RDWR | O_NOCTTY | O_NONBLOCK);
if (fd <0) {
  perror(MODEMDEVICE); abort(); 
}

/* install the signal handler before making the device asynchronous */
saio.sa_handler = signal_handler_IO;
sigemptyset(&saio.sa_mask);
saio.sa_flags = 0;
saio.sa_restorer = NULL;
sigaction(SIGIO,&saio,NULL);
  
/* allow the process to receive SIGIO */
fcntl(fd, F_SETOWN, getpid());
/* Make the file descriptor asynchronous (the manual page says only 
   O_APPEND and O_NONBLOCK, will work with F_SETFL...) */
fcntl(fd, F_SETFL, FNDELAY|FASYNC);

tcgetattr(fd,&oldtio); /* save current port settings */
/* set new port settings for canonical input processing */
newtio.c_cflag = BAUDRATE | CS8 | CLOCAL | CREAD ;
newtio.c_iflag = ICRNL;
newtio.c_oflag = 0;
newtio.c_lflag = 0;
newtio.c_cc[VMIN]=1;
newtio.c_cc[VTIME]=0;
tcflush(fd, TCIFLUSH);
tcsetattr(fd,TCSANOW,&newtio);
 
/* loop while waiting for input. normally we would do something
   useful here */ 
while (STOP==FALSE) {
  
  fgets (buf_tx, BUFFSIZE, stdin);
  if(write(fd, &buf_tx, BUFFSIZE)<0){
    perror("Error en escritura");abort();
  }
  usleep(1000);
  /* after receiving SIGIO, wait_flag = FALSE, input is available
     and can be read */
  if (wait_flag==FALSE) {
    if((res = read(fd,&buf_rx,BUFFSIZE))<0){
      perror("Error en lectura");abort();
    }
    printf("%s",buf_rx);
    if (res==1)STOP=TRUE; /* stop loop if only a CR was input */
    wait_flag = TRUE;      /* wait for new input */
    }
  memset(&buf_tx,0,sizeof(buf_tx));
  memset(&buf_rx,0,sizeof(buf_rx));
}
/* restore old port settings */
tcsetattr(fd,TCSANOW,&oldtio);
z}


/***************************************************************************
*  handler. sets wait_flag to FALSE, to indicate above loop that     *
* characters have been received.                                           *
***************************************************************************/

void signal_handler_IO (int status)
{
wait_flag = FALSE;
}