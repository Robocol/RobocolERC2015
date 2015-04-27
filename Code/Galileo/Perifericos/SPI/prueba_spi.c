#include <stdio.h>
#include "spi_robocol.h"

int main(){

	uint8_t tx,rx;
	
	spi_start("/dev/spidev1.0",100000);

	spi_device dev;

	spi_create_device( &dev,0,PIN6 );

	tx=0xee;
	rx=0x00;
	printf("TX es: %x\n", tx);

	while(1){
		spi_rw(&dev,&tx,&rx,1);
		printf("Envié esto: %x\n", tx);
		spi_rw(&dev,&tx,&rx,1);
		printf("Recibí esto: %x\n", rx);

		if(getchar()=='q'){
			break;
		}
	}
	return 0;
}