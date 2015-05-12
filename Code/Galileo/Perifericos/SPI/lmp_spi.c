#include <stdio.h>
#include "spi_robocol.h"
#include "misc_robocol.h"

const uint32_t RESOLUTION=16777216;

int main(){

	uint8_t* tx=malloc(2*sizeof(uint8_t));
	uint8_t* rx=malloc(2*sizeof(uint8_t));
	uint8_t* txADC=malloc(4*sizeof(uint8_t));
	uint8_t* rxADC=malloc(4*sizeof(uint8_t));
	float adc,temperature;
	spi_start("/dev/spidev1.0",100000);
	spi_device dev;
	spi_create_device( &dev,0,PIN9 );

	//Background Calibration
	printf("Background Calibration Upper Address\n");
	tx[0]=0x10;
	tx[1]=0x01; //Upper Address
	if(spi_rw(&dev,tx,rx,2)){
		printf("Error en set de upper Address para Configuración de Background Calibration\n");
		return 1;
	}

	printf("Background Calibration Configuration\n");
	tx[0]=0x00;
	tx[1]=0x02; //Modo 2:offset correction/gain correction 
	if(spi_rw(&dev,tx,rx,2)){
		printf("Error en transacción con lower Address para Background Calibration\n");
		return 1;
	}
	printf("Recibido de transacción: %x%x\n", rx[0],rx[1]);

// Lectura de registro escrito
	tx[0]=0x80;
	tx[1]=0x00; //Modo 2:offset correction/gain correction 
	if(spi_rw(&dev,tx,rx,2)){
		printf("Error en transacción con lower Address para Background Calibration\n");
		return 1;
	}
	printf("\t\t Background Calibration Register: %x%x\n", rx[0],rx[1]);

// Configuración interrupción DRDY
	printf("Configuración Interrupción Data ready\n");
	//Interrupción por nuevo dato disponible
	tx[0]=0x01; //Lower Address
	tx[1]=0x80; //Activación de interrupción
	if(spi_rw(&dev,tx,rx,2)){
		printf("Error en set para interrupción de Data ready\n");
		return 1;
	}

// Lectura de registro escrito
	tx[0]=0x81; //Lower Address
	tx[1]=0x00; //Activación de interrupción
	if(spi_rw(&dev,tx,rx,2)){
		printf("Error en set para interrupción de Data ready\n");
		return 1;
	}
	printf("\t\tDataRdy Register: %x%x\n", rx[0],rx[1]);

	printf("Detección y Configuración de Clk Lower Address\n");
	//Output Data Rate
	tx[0]=0x02; //Lower Address
	tx[1]=0x3A; //Deshabilitada detección de CLK - Seleccionado EXCLK -Corriente 1mA
	if(spi_rw(&dev,tx,rx,2)){
		printf("Error en transacción lower Address Configuración de Clk\n");
		return 1;
	}
	printf("Recibido de transacción: %x%x\n", rx[0],rx[1]);


// Lectura de registro escrito
	tx[0]=0x82; //Lower Address
	tx[1]=0x00; 
	if(spi_rw(&dev,tx,rx,2)){
		printf("Error en set para interrupción de Data ready\n");
		return 1;
	}
	printf("\t\tADC_AUX Register: %x%x\n", rx[0],rx[1]);


	printf("Configuración de Channel 0 Upper Address\n");
	//Configuración de Channel 0
	tx[0]=0x10;
	tx[1]=0x02; //Upper Address
	if(spi_rw(&dev,tx,rx,2)){
		printf("Error en transacción para Configuración de Upper Address\n");
		return 1;
	}

//Lectura registro escrito
	tx[0]=0x90;
	tx[1]=0x00; //Upper Address
	if(spi_rw(&dev,tx,rx,2)){
		printf("Error en transacción para Configuración de Upper Address\n");
		return 1;
	}
	printf("\t\tUpper Address Channel 0: %x%x\n", rx[0],rx[1]);

//Lower address channel 0
	printf("Configuración de Channel 0 Lower Address\n");
	tx[0]=0x01; //Lower Address
	tx[1]=0x06; //Gain = 8 - Buffer enabled
	if(spi_rw(&dev,tx,rx,2)){
		printf("Error en transacción para Configuración de Channel 0.\n");
		return 1;
	}
	printf("Recibido de transacción: %x%x\n", rx[0],rx[1]);

//Lectura registro escrito
	tx[0]=0x81;
	tx[1]=0x00;
	if(spi_rw(&dev,tx,rx,2)){
		printf("Error en transacción de lectura\n");
		return 1;
	}
	printf("\t\tRegistro de Configuración Channel 0: %x%x\n", rx[0],rx[1]);


	printf("Set de Upper Address para lectura de ADC\n");
	tx[0]=0x10;
	tx[1]=0x01;
	if(spi_rw(&dev,tx,rx,2)){
		printf("Error en el set de Upper Address para lectura ADC\n");
		return 1;
	}
	printf("Recibido de transacción: %x%x\n\n", rx[0],rx[1]);

	while(1){
		txADC[0]=0xCA;
		txADC[1]=0x00;
		txADC[2]=0x00;
		txADC[3]=0x00;
		if(spi_rw(&dev,txADC,rxADC,3)){
			printf("Error en transacción para lectura de ADC\n");
		}

		rxADC[0]=0x00;
		adc=((float)(array_to_i32(rxADC,4,0)-1792)*1.905)/(8*8388608);
		temperature=(adc-0.1)/0.000385;
		//printf("ADC[Volts]: %f\n",adc);
		printf("Temperature[C]: %f\n",temperature);
		if(getchar()=='q'){
			break;
		}
	}
	return 0;
}