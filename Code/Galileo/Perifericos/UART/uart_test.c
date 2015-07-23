#include "uart_robocol.h"
#define BAUDRATE B115200
#define BUFF_SIZE 2

int main(int argc, char const *argv[]){
	char buf_tx[BUFF_SIZE];
	char buf_rx[BUFF_SIZE];
	struct termios newtio;
	newtio.c_cflag = BAUDRATE | CS8 | CLOCAL | CREAD ;
	newtio.c_iflag = ICRNL;
	newtio.c_oflag = 0;
	newtio.c_lflag = 0;
	newtio.c_cc[VMIN]=1;
	newtio.c_cc[VTIME]=0;

	uart_open("/dev/ttyS0",&newtio);

	while (1) {

		printf("Escriba un mensaje:\n");
		fgets(buf_tx,BUFF_SIZE,stdin);
		printf("%s\n", buf_tx);
		uart_write(buf_tx, BUFF_SIZE);

		if (getWaitFlag()==FALSE ){
			printf("Recibido:\n");
			usleep(100000);
			uart_read(buf_rx,BUFF_SIZE);
			buf_rx[BUFF_SIZE-1]='\0';
			printf("E: %s",buf_rx);
		}

		setWaitFlag(TRUE);
	}

	uart_close();

}

