/*
 * afe_multi_in.c
 *
 *  Created on: 	May  2015
 *  Authors:		Daniel Ochoa
 * 					Germán Giraldo    
 */

#include <stdio.h>
#include <sys/times.h>
#include <sys/types.h>
#include <unistd.h>
#include <math.h>
#include "spi_robocol.h"
#include "misc_robocol.h"


const uint32_t 	RESOLUTION=8388608;
const uint8_t 	NUM_AFE=14;
const float		VREF=5;
const float		THRESHOLD=50;
long	 		CLK_TCK;
char nombref[13];


FILE *fdl;

int32_t		raw;
double		init;
double		elapsed;

struct tms *ts;

/*---------------Declaración de Variables de Medición---------------*/




//Temperatura
//float TA0[,TA1,TA2,TA3,TB0,TB1,TB2,TB3,TC0,TC1,TC2,TC3,TD0,TD1,TD2,TD3,TE0,TE1,TE2,TE3,TF0,TF1,TF2,TF3,TG0,TG1,TG2,TG3];
double T[28];


//Turbidez
//float UA0,UA1,UA2,UA3,UC0,UC1,UC2,UC3,UE0,UE1,UE2,UE3;
float U[14];



/*---------------Dispisitivos LMP---------------*/

spi_device lmp[14];



double adc_to_temp(int32_t raw){
	float volts; 
	volts=(((double)raw)*VREF)/(RESOLUTION);
	return -25.64815*log(volts) + 25.2765;
}

void print_log(){
	int it;
	if((fdl=fopen(nombref,"a"))<0){
		printf("Error en apertura de archivo para impresion de log\n");
		perror("Causa:");
	}

	if(fprintf(fdl,"-----------------------------------\n")<0){
			perror("Error imprimiendo");
	}
	if(fprintf(fdl, "%.2f \n",elapsed)<0){
		perror("Error imprimiendo");
	}

	for(it=0;it<28;it++){
		if(fprintf(fdl,"%.2f,",T[it])<0){
			perror("Error imprimiendo");
		}
	}
	if(fprintf(fdl,"\n")<0){
			perror("Error imprimiendo");
	}

	for(it=0;it<14;it++){
		if(fprintf(fdl,"%.2f,",U[it])<0){
			perror("Error imprimiendo");
		}
	}

	if(fprintf(fdl,"\n")<0){
			perror("Error imprimiendo");
	}
	fclose(fdl);
}
/*Inicio del programa*/

int main(){


	CLK_TCK=sysconf(_SC_CLK_TCK);
	sprintf(nombref,"cafe.log");

	// if((fdl=fopen(nombref,"a"))<0){
	// 	printf("Error en apertura de archivo para impresion de log\n");
	// 	perror("Causa:");
	// }

	uint8_t* 	tx=malloc(2*sizeof(uint8_t));
	uint8_t* 	rx=malloc(2*sizeof(uint8_t));
	uint8_t* 	txADC=malloc(4*sizeof(uint8_t));
	uint8_t* 	rxADC=malloc(4*sizeof(uint8_t));
	int i=0;

	float adc,temperature;
	spi_start("/dev/spidev1.0",100000);

	spi_create_device( &lmp[0],0,PIN0);
	spi_create_device( &lmp[1],0,PIN1);
	spi_create_device( &lmp[2],0,PIN2);
	spi_create_device( &lmp[3],0,PIN3);
	spi_create_device( &lmp[4],0,PIN4);
	spi_create_device( &lmp[5],0,PIN5);
	spi_create_device( &lmp[6],0,PIN6);
	spi_create_device( &lmp[7],0,PIN7);
	spi_create_device( &lmp[8],0,PINA0);
	spi_create_device( &lmp[9],0,PINA1);
	spi_create_device( &lmp[10],0,PINA2);
	spi_create_device( &lmp[11],0,PINA3);
	spi_create_device( &lmp[12],0,PINA4);
	spi_create_device( &lmp[13],0,PINA5);



	printf("Bienvenido al Test de Funcionamiento de Modulo de Medición de Temperatura para Motores \n");
	printf("\n--------------------------------------------------------------------------------------\n");


	/*Test de Comunicaciones*/
	printf("Inicio de Diagnóstico de Sistema\n");
	printf("Por favor esperar mientras se realiza el diagnóstico...\n");


	for(i=0;i<NUM_AFE;i++){
		printf("Changing upper address\n");
		tx[0]=0x10;
		tx[1]=0x00; //Upper Address
		if(spi_rw(&lmp[i],tx,rx,2)){
			printf("Error en set de upper Address para Configuración de Background Calibration\n");
			return 1;
		}
		printf("Requesting static register information\n");
		tx[0]=0x89;
		tx[1]=0x00; //Upper Address
		if(spi_rw(&lmp[i],tx,rx,2)){
			printf("Error en set de upper Address para Configuración de Background Calibration\n");
			return 1;
		}

		if(rx[1]==0x1A){
			printf("Test de Comunicación Exitoso. SPI en funcionamiento \n" );
		}else{
			printf("Ocurrió un error al comunicarse con la Tarjeta de Medición Nun%d. Test de comunicación FALLIDO\n",i);
			printf("Verifique las conexiones SPI y la alimentación\n");
		}
	}

	for(i=0;i<NUM_AFE;i++){
		//D6 as Output
		tx[0]=0x10;
		tx[1]=0x00; //Upper Address
		if(spi_rw(&lmp[i],tx,rx,2)){
			printf("Error en set de upper Address para set de D6DIR\n");
			return 1;

		}
		tx[0]=0x8E;
		tx[1]=0x41; //Upper Address
		if(spi_rw(&lmp[i],tx,rx,2)){
			printf("Error en set de upper Address para set D6\n");
			return 1;

		}

		tx[0]=0x8F; //
		tx[1]=0x01;
		if(spi_rw(&lmp[i],tx,rx,2)){
			printf("Error en set de upper Address para Configuración de Background Calibration\n");
			return 1;

		}
	}


	/*Configuración de Mediciones*/
	printf("\n--------------------------------------------------------------------------------------\n");
	printf("Configurando mediciones\n\n");

//Background Calibration
	for(i=0;i<NUM_AFE;i++){
		tx[0]=0x10;
		tx[1]=0x01; //Upper Address
		if(spi_rw(&lmp[i],tx,rx,2)){
			printf("Error en set de upper Address para Configuración de Background Calibration\n");
			return 1;
		}

		tx[0]=0x00;
		tx[1]=0x02; //Modo 2:offset correction/gain correction 
		if(spi_rw(&lmp[i],tx,rx,2)){
			printf("Error en transacción con lower Address para Background Calibration\n");
			return 1;
		}
	}



// Configuración interrupción DRDY
	// printf("Configuración Interrupción Data ready:");
	// //Interrupción por nuevo dato disponible
	// tx[0]=0x01; //Lower Address
	// tx[1]=0x80; //Activación de interrupción
	// if(spi_rw(&lmp[i],tx,rx,2)){
	// 	printf("Error en set para interrupción de Data ready\n");
	// 	return 1;
	// }

	// // Lectura de registro escrito
	// tx[0]=0x81; //Lower Address
	// tx[1]=0x00; //Activación de interrupción
	// if(spi_rw(&lmp[i],tx,rx,2)){
	// 	printf("Error en set para interrupción de Data ready\n");
	// 	return 1;
	// }
	// if(rx[1]==0x80){
	// 	printf("Done\n" );
	// }else{
	// 	printf("Failed\n" );
	// }

// Detección y Configuración de Clk Lower Address
	printf("Detección y Configuración de Clk y fuentes de corriente:");
	for(i=0;i<NUM_AFE;i++){
		//Output Data Rate
		tx[0]=0x02; //Lower Address
		tx[1]=0x3A; //Deshabilitada detección de CLK - Seleccionado EXCLK -Corriente 1mA
		if(spi_rw(&lmp[i],tx,rx,2)){
			printf("Error en transacción lower Address Configuración de Clk\n");
		}
	}



	for(i=0;i<NUM_AFE;i++){
		tx[0]=0x10;
		tx[1]=0x01; //Upper Address
		if(spi_rw(&lmp[i],tx,rx,2)){
			printf("Error en transacción para Configuración de Upper Address\n");
		}
	}

	init=times(ts)/((double)CLK_TCK);

	printf("Inicio de Mediciones");
	
	while(1){

		for(i=0;i<NUM_AFE;i++){
			printf("Configurando Medición CH0.\n");

			//Configuración de Channel 0
			tx[0]=0x0F; //Lower Address
			tx[1]=0x00; //ScanMode1 - Single Scan CH0
			if(spi_rw(&lmp[i],tx,rx,2)){
				printf("Error en transacción para Configuración de Channel 0.\n");
				return 1;
			}

			//adc_restart
			tx[0]=0x10;
			tx[1]=0x00; //Upper Address
			if(spi_rw(&lmp[i],tx,rx,2)){
				printf("Error en transacción para Configuración de Upper Address\n");
				return 1;
			}
			tx[0]=0x0B; //Lower Address
			tx[1]=0x01; //SRESTART
			if(spi_rw(&lmp[i],tx,rx,2)){
				printf("Error en transacción para Configuración de Channel 0.\n");
				return 1;
			}

			tx[0]=0x10;
			tx[1]=0x01; //Upper Address
			if(spi_rw(&lmp[i],tx,rx,2)){
				printf("Error en transacción para Configuración de Upper Address\n");
				return 1;
			}


			printf("Espere un monento...\n");
			usleep(50000);

			txADC[0]=0xCA;
			txADC[1]=0x00;
			txADC[2]=0x00;
			txADC[3]=0x00;
			if(spi_rw(&lmp[i],txADC,rxADC,3)){
				printf("Error en transacción para lectura de ADC\n");
			}


			//Checking for negatives
			if((rxADC[1]&0x80)){
				rxADC[0]=0xFF;
			}else{
				rxADC[0]=0x00;
			}

			T[2*i]=adc_to_temp(*rxADC);
			printf("T%d: %f\n",2*i,T[2*i]);
			adc=((float)(array_to_i32(rxADC,4,0))*VREF)/(RESOLUTION);
			printf("RAW: %d\t",array_to_i32(rxADC,4,0));
			printf("ADC[Volts]: %.4f\t\n",adc);




			printf("Configurando Medición CH1.\n");
			//Configuración de Channel 1
			tx[0]=0x0F; //Lower Address
			tx[1]=0x09; //ScanMode1 - Single Scan CH1
			if(spi_rw(&lmp[i],tx,rx,2)){
				printf("Error en transacción para Configuración de Channel 1.\n");
				return 1;
			}


			//adc_restart
			tx[0]=0x10;
			tx[1]=0x00; //Upper Address
			if(spi_rw(&lmp[i],tx,rx,2)){
				printf("Error en transacción para Configuración de Upper Address\n");
				return 1;
			}
			tx[0]=0x0B; //Lower Address
			tx[1]=0x01; //RESTART
			if(spi_rw(&lmp[i],tx,rx,2)){
				printf("Error en transacción para Configuración de Channel 0.\n");
				return 1;
			}

			tx[0]=0x10;
			tx[1]=0x01; //Upper Address
			if(spi_rw(&lmp[i],tx,rx,2)){
				printf("Error en transacción para Configuración de Upper Address\n");
				return 1;
			}

			printf("Espere un monento...\n");

			usleep(50000);

			txADC[0]=0xCA;
			txADC[1]=0x00;
			txADC[2]=0x00;
			txADC[3]=0x00;
			if(spi_rw(&lmp[i],txADC,rxADC,3)){
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
			T[2*i+1]=adc_to_temp(*rxADC);
			printf("T%d: %f\n",2*i+1,T[2*i+1]);
			printf("RAW: %d\t",raw);
			printf("ADC[Volts]: %.4f\t\n",adc);




			printf("Configurando Medición CH2.\n");
			//Configuración de Channel 2
			tx[0]=0x0F; //Lower Address
			tx[1]=0x12; //ScanMode1 - Single Scan CH2
			if(spi_rw(&lmp[i],tx,rx,2)){
				printf("Error en transacción para Configuración de Channel 1.\n");
				return 1;
			}


			//adc_restart
			tx[0]=0x10;
			tx[1]=0x00; //Upper Address
			if(spi_rw(&lmp[i],tx,rx,2)){
				printf("Error en transacción para Configuración de Upper Address\n");
				return 1;
			}
			tx[0]=0x0B; //Lower Address
			tx[1]=0x01; //ScanMode1 - Single Scan CH0
			if(spi_rw(&lmp[i],tx,rx,2)){
				printf("Error en transacción para Configuración de Channel 0.\n");
				return 1;
			}

			tx[0]=0x10;
			tx[1]=0x01; //Upper Address
			if(spi_rw(&lmp[i],tx,rx,2)){
				printf("Error en transacción para Configuración de Upper Address\n");
				return 1;
			}
			
			printf("Espere un monento...\n");

			usleep(50000);

			txADC[0]=0xCA;
			txADC[1]=0x00;
			txADC[2]=0x00;
			txADC[3]=0x00;
			if(spi_rw(&lmp[i],txADC,rxADC,3)){
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
			U[i]=adc_to_temp(*rxADC);
			printf("U%d: %f\n",i,T[i]);
			printf("RAW: %d\t",raw);
			printf("ADC[Volts]: %.4f\t\n",adc);

		}
		elapsed=times(ts)/CLK_TCK-init;
		print_log();
		printf("Elapsed: %f\n", elapsed);

		if(elapsed>72000){
			fclose(fdl);
			break;
		}
		sleep(51);
	}
}