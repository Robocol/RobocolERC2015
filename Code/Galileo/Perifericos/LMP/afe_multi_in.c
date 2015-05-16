/*
 * afe_multi_in.c
 *
 *  Created on: 	May  2015
 *  Authors:		Daniel Ochoa
 * 					Germán Giraldo    
 */

#include <stdio.h>
#include "spi_robocol.h"
#include "misc_robocol.h"

const uint32_t 	RESOLUTION=8388608;
const float		VREF=3.32;
const float		THRESHOLD=50;



	/*Variables de Diagnóstico*/
	uint8_t		upperLimit=30;
	uint8_t		lowerLimit=10;
	
	uint8_t 	overtemp=FALSE;
	char		entrada='h';

	float		avrTemp;
	int32_t		raw;

int main(){

	uint8_t* 	tx=malloc(2*sizeof(uint8_t));
	uint8_t* 	rx=malloc(2*sizeof(uint8_t));
	uint8_t* 	txADC=malloc(4*sizeof(uint8_t));
	uint8_t* 	rxADC=malloc(4*sizeof(uint8_t));

	float adc,temperature;
	spi_start("/dev/spidev1.0",100000);
	spi_device dev;
	spi_create_device( &dev,0,PIN9 );
	gpio_set_dir(PIN7,OUT);
	gpio_gal_clear(PIN7);



	printf("Bienvenido al Test de Funcionamiento de Modulo de Medición de Temperatura para Motores \n");
	printf("\n--------------------------------------------------------------------------------------\n");


	/*Test de Comunicaciones*/
	printf("Inicio de Diagnóstico de Sistema\n");
	printf("Por favor esperar mientras se realiza el diagnóstico...\n");



	printf("Changing upper address\n");
	tx[0]=0x10;
	tx[1]=0x00; //Upper Address
	if(spi_rw(&dev,tx,rx,2)){
		printf("Error en set de upper Address para Configuración de Background Calibration\n");
		return 1;
	}
	printf("Requesting static register information\n");
	tx[0]=0x89;
	tx[1]=0x00; //Upper Address
	if(spi_rw(&dev,tx,rx,2)){
		printf("Error en set de upper Address para Configuración de Background Calibration\n");
		return 1;
	}

	if(rx[1]==0x1A){
		printf("Test de Comunicación Exitoso. SPI en funcionamiento \n" );
	}else{
		printf("Ocurrió un error al comunicarse con la Tarjeta de Medición. Test de comunicación FALLIDO\n" );
		printf("Verifique las conexiones SPI y la alimentación\n");
		printf("Abortando...\n");
		abort();
	}

	//D6 as Output
	tx[0]=0x10;
	tx[1]=0x00; //Upper Address
	if(spi_rw(&dev,tx,rx,2)){
		printf("Error en set de upper Address para set de D6DIR\n");
		return 1;

	}
	tx[0]=0x8E;
	tx[1]=0x7C; //Upper Address
	if(spi_rw(&dev,tx,rx,2)){
		printf("Error en set de upper Address para set D6\n");
		return 1;

	}

	//D6 
	tx[0]=0x10;
	tx[1]=0x00; //Upper Address
	if(spi_rw(&dev,tx,rx,2)){
		printf("Error en set de upper Address para Configuración de Background Calibration\n");
		return 1;
	}
	tx[0]=0x8F; //
	tx[1]=0x00;
	if(spi_rw(&dev,tx,rx,2)){
		printf("Error en set de upper Address para Configuración de Background Calibration\n");
		return 1;

	}



	/*Configuración de Mediciones*/
	printf("\n--------------------------------------------------------------------------------------\n");
	printf("Configurando mediciones\n\n");

//Background Calibration
	tx[0]=0x10;
	tx[1]=0x01; //Upper Address
	if(spi_rw(&dev,tx,rx,2)){
		printf("Error en set de upper Address para Configuración de Background Calibration\n");
		return 1;
	}

	printf("Background Calibration Configuration: ");
	tx[0]=0x00;
	tx[1]=0x02; //Modo 2:offset correction/gain correction 
	if(spi_rw(&dev,tx,rx,2)){
		printf("Error en transacción con lower Address para Background Calibration\n");
		return 1;
	}

// Lectura de registro escrito
	tx[0]=0x80;
	tx[1]=0x00; //Modo 2:offset correction/gain correction 
	if(spi_rw(&dev,tx,rx,2)){
		printf("Error en transacción con lower Address para Background Calibration\n");
		return 1;
	}

	if(rx[1]==0x02){
		printf("Done. LMP90100 Online\n" );
	}else{
		printf("Failed\n" );
	}

// Configuración interrupción DRDY
	// printf("Configuración Interrupción Data ready:");
	// //Interrupción por nuevo dato disponible
	// tx[0]=0x01; //Lower Address
	// tx[1]=0x80; //Activación de interrupción
	// if(spi_rw(&dev,tx,rx,2)){
	// 	printf("Error en set para interrupción de Data ready\n");
	// 	return 1;
	// }

	// // Lectura de registro escrito
	// tx[0]=0x81; //Lower Address
	// tx[1]=0x00; //Activación de interrupción
	// if(spi_rw(&dev,tx,rx,2)){
	// 	printf("Error en set para interrupción de Data ready\n");
	// 	return 1;
	// }
	// if(rx[1]==0x80){
	// 	printf("Done\n" );
	// }else{
	// 	printf("Failed\n" );
	// }

// Detección y Configuración de Clk Lower Address
	printf("Detección y Configuración de Clk Lower Address:");
	//Output Data Rate
	tx[0]=0x02; //Lower Address
	tx[1]=0x3A; //Deshabilitada detección de CLK - Seleccionado EXCLK -Corriente 1mA
	if(spi_rw(&dev,tx,rx,2)){
		printf("Error en transacción lower Address Configuración de Clk\n");
		return 1;
	}


	// Lectura de registro escrito
	tx[0]=0x82; //Lower Address
	tx[1]=0x00; 
	if(spi_rw(&dev,tx,rx,2)){
		printf("Error en set para interrupción de Data ready\n");
		return 1;
	}
	if(rx[1]==0x3A){
		printf("Done\n" );
	}else{
		printf("Failed\n" );
	};


	//Configuración de Channel 0
	tx[0]=0x10;
	tx[1]=0x02; //Upper Address
	if(spi_rw(&dev,tx,rx,2)){
		printf("Error en transacción para Configuración de Upper Address\n");
		return 1;
	}


	printf("Configuración de Channel 0\n");
	tx[0]=0x01; //Lower Address
	tx[1]=0x30; //Gain = 1 - Buffer enabled
	if(spi_rw(&dev,tx,rx,2)){
		printf("Error en transacción para Configuración de Channel 0.\n");
		return 1;
	}

	//Lectura registro escrito
	tx[0]=0x81;
	tx[1]=0x00;
	if(spi_rw(&dev,tx,rx,2)){
		printf("Error en transacción de lectura\n");
		return 1;
	}
	if(rx[1]==0x30){
		printf("Done\n" );

	}else{
		printf("Failed\n" );

	};


//Configuración de Channel 1
	printf("Configuración de Channel 1\n");
	tx[0]=0x02; //Lower Address
	tx[1]=0x1C; //VINP=VIN3; VINN=VIN4;
	if(spi_rw(&dev,tx,rx,2)){
		printf("Error en transacción para Configuración de Channel 1.\n");
		return 1;
	}

	//Lectura registro escrito
	tx[0]=0x82;
	tx[1]=0x00;
	if(spi_rw(&dev,tx,rx,2)){
		printf("Error en transacción de lectura\n");
		return 1;
	}
	if(rx[1]==0x1C){
		printf("Done\n" );

	}else{
		printf("Failed\n" );

	};

	tx[0]=0x03; //Lower Address
	tx[1]=0x30; //Gain = 1 - Buffer enabled
	if(spi_rw(&dev,tx,rx,2)){
		printf("Error en transacción para Configuración de Channel 1.\n");
		return 1;
	}

	//Lectura registro escrito
	tx[0]=0x83;
	tx[1]=0x00;
	if(spi_rw(&dev,tx,rx,2)){
		printf("Error en transacción de lectura\n");
		return 1;
	}
	if(rx[1]==0x30){
		printf("Done\n" );

	}else{
		printf("Failed\n" );

	};


	tx[0]=0x10;
	tx[1]=0x01; //Upper Address
	if(spi_rw(&dev,tx,rx,2)){
		printf("Error en transacción para Configuración de Upper Address\n");
		return 1;
	}




	printf("Bienvenido al test de medición de dos canales por medio de LMP90100 (Intrumentación Electrónica 201510).\n Utilice una de los siguientes comandos:\n" 
	"\t 0\t\t-Realiza una única lectura del canal de medición 0 del AFE (VIN0-VIN1)\n"
	"\t 1\t\t-Realiza una única lectura del canal de medición 0 del AFE (VIN2-VIN3)\n"
	"\t q\t\t-Izquierda\n\n"
	"En caso de comando inválido, se mostrará nuevamente este menu como ayuda al usuario\n");
	
	
	while((entrada=getchar()) != 'q'){
		if(entrada=='0'){

			printf("Configurando Medición CH0.\n");



			//Configuración de Channel 0
			tx[0]=0x0F; //Lower Address
			tx[1]=0x00; //ScanMode1 - Single Scan CH0
			if(spi_rw(&dev,tx,rx,2)){
				printf("Error en transacción para Configuración de Channel 0.\n");
				return 1;
			}

			//Lectura registro escrito
			tx[0]=0x8F;
			tx[1]=0x00;
			if(spi_rw(&dev,tx,rx,2)){
				printf("Error en transacción de lectura\n");
				return 1;
			}
			if(rx[1]==0x00){
				printf("Done\n" );

			}else{
				printf("Failed\n" );

			};

			printf("Espere un monento...\n");
			sleep(3);

			txADC[0]=0xCA;
			txADC[1]=0x00;
			txADC[2]=0x00;
			txADC[3]=0x00;
			if(spi_rw(&dev,txADC,rxADC,3)){
				printf("Error en transacción para lectura de ADC\n");
			}


			//Checking for negatives
			if((rxADC[1]&0x80)){
				rxADC[0]=0xFF;
			}else{
				rxADC[0]=0x00;
			}

			adc=((float)(array_to_i32(rxADC,4,0))*VREF)/(RESOLUTION);
			printf("RAW: %d\t",array_to_i32(rxADC,4,0));
			printf("ADC[Volts]: %.4f\t\n",adc);


		}else if(entrada=='1'){


			printf("Configurando Medición CH1.\n");
			//Configuración de Channel 1
			tx[0]=0x0F; //Lower Address
			tx[1]=0x09; //ScanMode1 - Single Scan CH1
			if(spi_rw(&dev,tx,rx,2)){
				printf("Error en transacción para Configuración de Channel 1.\n");
				return 1;
			}

			//Lectura registro escrito
			tx[0]=0x8F;
			tx[1]=0x0F;
			if(spi_rw(&dev,tx,rx,2)){
				printf("Error en transacción de lectura\n");
				return 1;
			}
			if(rx[1]==0x09){
				printf("Done\n" );

			}else{
				printf("Failed\n" );

			};

			printf("Espere un monento...\n");

			sleep(3);

			txADC[0]=0xCA;
			txADC[1]=0x00;
			txADC[2]=0x00;
			txADC[3]=0x00;
			if(spi_rw(&dev,txADC,rxADC,3)){
				printf("Error en transacción para lectura de ADC\n");
				abort();
			}

			//Checking for negatives
			if((rxADC[1]&0x80)){
				rxADC[0]=0xFF;
			}else{
				rxADC[0]=0x00;
			}

			raw=array_to_i32(rxADC,4,0);

			adc=((float)raw*VREF)/(RESOLUTION);
	
			printf("RAW: %d\t",raw);
			printf("ADC[Volts]: %.4f\t\n",adc);


		}else if(entrada!='\n') {
		printf("Comando inválido.Por favor utilice una de los siguientes comandos:\n" 
		"\t 0\t\t-Realiza una única lectura del canal de medición 0 del AFE (VIN0-VIN1)\n"
		"\t 1\t\t-Realiza una única lectura del canal de medición 0 del AFE (VIN2-VIN3)\n"
		"\t q\t\t-Izquierda\n\n");
		}
	}
}