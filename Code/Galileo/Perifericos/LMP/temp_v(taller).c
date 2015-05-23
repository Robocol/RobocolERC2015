/*
 * --------------------------------------------------------------------------------------
 * temp_v(taller).c (0.9)
 *
 *  Created on: May, 2015
 *
 *      Authors:	Daniel Ochoa
 *					Germán Giraldo
 * -------------------------------------------------------------------------------------
 */
#include <stdio.h>
#include "spi_robocol.h"
#include "misc_robocol.h"

const uint32_t 	RESOLUTION=8388608;
const float		VREF=1.167;
const float		THRESHOLD=50;



	/*Variables de Diagnóstico*/
	uint8_t		upperLimit=30;
	uint8_t		lowerLimit=10;
	
	uint8_t 	overtemp=FALSE;

	float		avrTemp;

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
	tx[1]=0x01; //Upper Address
	if(spi_rw(&dev,tx,rx,2)){
		printf("Error en set de upper Address para Configuración de Background Calibration\n");
		return 1;
	}
	printf("Requesting static register information\n");
	tx[0]=0x8F;
	tx[1]=0x00; //Upper Address
	if(spi_rw(&dev,tx,rx,2)){
		printf("Error en set de upper Address para Configuración de Background Calibration\n");
		return 1;
	}

	if(rx[1]==0x30){
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
	tx[0]=0x8F;
	tx[1]=0x00; //Upper Address
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
		printf("Done\n" );
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


//Configuración de Channel 0
	printf("Configuración de Channel 0\n");
	tx[0]=0x01; //Lower Address
	tx[1]=0x06; //Gain = 8 - Buffer enabled
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
	if(rx[1]==0x06){
		printf("Done\n" );
	}else{
		printf("Failed\n" );
	};


	tx[0]=0x10;
	tx[1]=0x01;
	if(spi_rw(&dev,tx,rx,2)){
		printf("Error en el set de Upper Address para lectura ADC\n");
		return 1;
	}

	printf("¿Desea iniciar el Test de Mediciones?(y/N)\n");
	if(getchar()=='y'||getchar()=='Y'){
	/*Test de Mediciones*/
		printf("\n--------------------------------------------------------------------------------------\n");
		printf("Iniciando Test de Mediciones\n\n");

		printf("Por favor esperar\n");
		sleep(1);

		
		avrTemp=0;

		uint8_t i;
		for(i=0;i<10;i++){

			txADC[0]=0xCA;
			txADC[1]=0x00;
			txADC[2]=0x00;
			txADC[3]=0x00;
			if(spi_rw(&dev,txADC,rxADC,3)){
				printf("Error en transacción para lectura de ADC\n");
			}

			rxADC[0]=0x00;
			adc=((float)(array_to_i32(rxADC,4,0)-1792)*VREF)/(8*RESOLUTION);
			temperature=(adc/0.978-0.101)/(0.0003851);

			avrTemp+=temperature/10;
			printf("Temperature[C]: %f\n",temperature);
			//printf("ADC[Volts]: %f\n",adc);
			sleep(1);

		}

		printf("\nTEMPERATURA: %f\n", temperature);
	

		if(avrTemp<upperLimit && avrTemp>lowerLimit){
			printf("Test de Medición Exitoso. Valores de temperatura dentro de rango esperado\n" );
		}else{
			printf("Valores obtenidos por fuera del rango esperado. Test de medición FALLIDO\n" );
			printf("Verifique las conexiones de la RTD y el voltaje de referencia\n");
			printf("Abortando...\n");
			abort();
		}
	}
	/*Test de Mediciones*/
	printf("\n--------------------------------------------------------------------------------------\n");
	printf("DIAGNOSTICO FINALIZADO. LMP90100 ONLINE\n\n");

	while(1){
		txADC[0]=0xCA;
		txADC[1]=0x00;
		txADC[2]=0x00;
		txADC[3]=0x00;
		if(spi_rw(&dev,txADC,rxADC,3)){
			printf("Error en transacción para lectura de ADC\n");
		}

		rxADC[0]=0x00;
		adc=((float)(array_to_i32(rxADC,4,0))*VREF)/(8*RESOLUTION);
		temperature=(adc/0.978-0.101)/(0.0003851);
		printf("RAW: %d\t",array_to_i32(rxADC,4,0));
		printf("ADC[Volts]: %.4f\t",adc);
		printf("Temperature[C]: %.2f\n",temperature);
		

		if(temperature>THRESHOLD && !overtemp){
			
			overtemp=TRUE;
			gpio_gal_set(PIN7);

			// tx[0]=0x10;
			// tx[1]=0x00; //Upper Address
			// if(spi_rw(&dev,tx,rx,2)){
			// 	printf("Error en set de upper Address para Configuración de Background Calibration\n");
			// 	return 1;

			// }
			// tx[0]=0x8F;
			// tx[1]=0x7F; //Upper Address
			// if(spi_rw(&dev,tx,rx,2)){
			// 	printf("Error en set de upper Address para Configuración de Background Calibration\n");
			// 	return 1;

			// }

			// tx[0]=0x10;
			// tx[1]=0x01; //Upper Address
			// if(spi_rw(&dev,tx,rx,2)){
			// 	printf("Error en set de upper Address para Configuración de Background Calibration\n");
			// 	return 1;

			// }
		}else if(temperature<THRESHOLD && overtemp){
			overtemp=FALSE;
			gpio_gal_clear(PIN7);
		}

		if(overtemp){
			printf("SOBRETEMPERATURA\n");
		}

		if(temperature<-5 || temperature>125){
			printf("Error en Medición: Valor Fuera de rango\n");
			printf("Posible falla en conexión\n");
		}

		sleep(1);
	}
	return 0;
}