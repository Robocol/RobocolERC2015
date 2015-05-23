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
#include "lmp_robocol.h"
#include "math.h"

const uint32_t 	RESOLUTION=8388608;
const uint8_t 	NUM_AFE=14;
const float		VREF=5;
const float		THRESHOLD=50;


/*---------------Declaración de Variables de Medición---------------*/


//Temperatura
//float TA0[,TA1,TA2,TA3,TB0,TB1,TB2,TB3,TC0,TC1,TC2,TC3,TD0,TD1,TD2,TD3,TE0,TE1,TE2,TE3,TF0,TF1,TF2,TF3,TG0,TG1,TG2,TG3];
double T[28];


//Turbidez
//float UA0,UA1,UA2,UA3,UC0,UC1,UC2,UC3,UE0,UE1,UE2,UE3;
float U[12];


//PH
float P[2];


/*---------------Dispisitivos LMP---------------*/

lmp_dev lmp[14];

/*Inicio del programa*/
	

double adc_to_temp(int32_t raw){
	float volts; 
	volts=(((double)raw)*VREF)/(RESOLUTION);
	return -25.64815*log(volts) + 25.2765;
}

int main(){

	uint8_t* 	tx=malloc(sizeof(uint8_t));
	uint32_t* 	rxADC=malloc(sizeof(int32_t));


	/*---------------Inicialización LMPs---------------*/

	lmp[0].pin_cs=PIN0;
	lmp[1].pin_cs=PIN1;
	lmp[2].pin_cs=PIN2;
	lmp[3].pin_cs=PIN3;
	lmp[4].pin_cs=PIN4;
	lmp[5].pin_cs=PIN5;
	lmp[6].pin_cs=PIN6;
	lmp[7].pin_cs=PIN7;
	lmp[8].pin_cs=PINA0;
	lmp[9].pin_cs=PINA1;
	lmp[10].pin_cs=PINA2;
	lmp[11].pin_cs=PINA3;
	lmp[12].pin_cs=PINA4;
	lmp[13].pin_cs=PINA5;

	int i=0;
	for (i=0;i<14;i++){
		build_lmp(&(lmp[i]));
	}



	printf("Bienvenido al programa de control del Sistema para Caracterización del Proceso de Fermentación del Café\n");
	printf("\n--------------------------------------------------------------------------------------\n");


	/*Encendiendo Leds (Si es que no se queman)*/
	*tx=0x41;
	for (i=0;i<NUM_AFE;i++){
		if(write_reg_lmp(&(lmp[i]),0x0E, tx,1)){
			printf("Error configurando dirección de GPIOs de la AFE %d\n",i);
		}
	}

	*tx=0x01;
	for (i=0;i<NUM_AFE;i++){
		if(write_reg_lmp(&(lmp[i]),0x0F,tx,1)){
			printf("Error configurando asignando valor de GPIOs de la AFE %d\n",i);
		}
	}


	/*Configuración de Mediciones*/
	printf("\n--------------------------------------------------------------------------------------\n");
	printf("Configurando mediciones\n\n");

	//Background Calibration
	*tx=0x02;
	for (i=0;i<NUM_AFE;i++){
		//Modo 2:offset correction/gain correction
		if(write_reg_lmp(&(lmp[i]),0x10, tx,1)){
			printf("Error configurando asignando valor de GPIOs de la AFE %d\n",i);
		}
	}

	// Detección y Configuración de Clk. Configuración de fuentes de corriente
	printf("Detección y Configuración de Clk y fuentes de corriente:");
	*tx=0x3A;
	for (i=0;i<NUM_AFE;i++){
		//Deshabilitada detección de CLK - Seleccionado EXCLK -Corriente 1mA
		if(write_reg_lmp(&(lmp[i]),0x12,tx,1)){
			printf("Error configurando detección de clock y fuentes de corriente de la AFE %d\n",i);
		}
	}



	printf("Bienvenido al test de medición de dos canales por medio de LMP90100 (Intrumentación Electrónica 201510).\n Utilice una de los siguientes comandos:\n" 
	"\t 0\t\t-Realiza una única lectura del canal de medición 0 del AFE (VIN0-VIN1)\n"
	"\t 1\t\t-Realiza una única lectura del canal de medición 0 del AFE (VIN2-VIN3)\n"
	"\t q\t\t-Quit\n\n"
	"En caso de comando inválido, se mostrará nuevamente este menu como ayuda al usuario\n");
	

	/*----------Mediciones iterativas----------*/

	while(getchar()!='q'){
		for(i=0;i<NUM_AFE;i++){

			printf("Mediciónes de AFE %d.\n",i);
			if(i<12){

				//Configuración de Channel 0
				*tx=0x00;
				if(write_reg_lmp(&(lmp[i]),0x1F,tx,1)){
					printf("Error configurando detección de clock y fuentes de corriente de la AFE %d\n",i);
				}
				//ADC_RESTART
				*tx=0x01;
				if(write_reg_lmp(&(lmp[i]),0x0B, tx,1)){
					printf("Error configurando detección de clock y fuentes de corriente de la AFE %d\n",i);
				}

				usleep(50000);
				//Medición
				if(read_ADC_lmp(&lmp[i],rxADC)){
					printf("Error medición t%d.\n",i);
				}
				T[2*i]=adc_to_temp(*rxADC);
				printf("T%d: %f\n",2*i,T[2*i]);



				//Configuración de Channel 1
				*tx=0x09;
				if(write_reg_lmp(&(lmp[i]),0x1F,tx,1)){
					printf("Error configurando detección de clock y fuentes de corriente de la AFE %d\n",i);
				}
				//ADC_RESTART
				*tx=0x01;
				if(write_reg_lmp(&(lmp[i]),0x0B, tx,1)){
					printf("Error configurando detección de clock y fuentes de corriente de la AFE %d\n",i);
				}

				usleep(50000);
				//Medición
				if(read_ADC_lmp(&lmp[i],rxADC)){
					printf("Error medición t%d.\n",i);
				}
				T[2*i+1]=adc_to_temp(*rxADC);
				printf("T%d: %f\n",2*i+1,T[2*i+1]);



				//Configuración de Channel 2
				*tx=0x12;
				if(write_reg_lmp(&(lmp[i]),0x1F,tx,1)){
					printf("Error configurando detección de clock y fuentes de corriente de la AFE %d\n",i);
				}
				//ADC_RESTART
				*tx=0x01;
				if(write_reg_lmp(&(lmp[i]),0x0B, tx,1)){
					printf("Error configurando detección de clock y fuentes de corriente de la AFE %d\n",i);
				}

				usleep(50000);
				//Medición
				if(read_ADC_lmp(&lmp[i],rxADC)){
					printf("Error medición t%d.\n",i);
				}
				U[2*i]=((float)(*rxADC)*VREF)/RESOLUTION;
				printf("U%d: %f\n",i,T[i]);

			}else{

				//Configuración de Channel 0
				*tx=0x00;
				if(write_reg_lmp(&(lmp[i]),0x1F,tx,1)){
					printf("Error configurando detección de clock y fuentes de corriente de la AFE %d\n",i);
				}
				//ADC_RESTART
				*tx=0x01;
				if(write_reg_lmp(&(lmp[i]),0x0B, tx,1)){
					printf("Error configurando detección de clock y fuentes de corriente de la AFE %d\n",i);
				}

				usleep(50000);
				//Medición
				if(read_ADC_lmp(&lmp[i],rxADC)){
					printf("Error medición t%d.\n",i);
				}
				T[2*i]=adc_to_temp(*rxADC);
				printf("T%d: %f\n",2*i,T[2*i]);



				//Configuración de Channel 1
				*tx=0x09;
				if(write_reg_lmp(&(lmp[i]),0x1F,tx,1)){
					printf("Error configurando detección de clock y fuentes de corriente de la AFE %d\n",i);
				}
				//ADC_RESTART
				*tx=0x01;
				if(write_reg_lmp(&(lmp[i]),0x0B, tx,1)){
					printf("Error configurando detección de clock y fuentes de corriente de la AFE %d\n",i);
				}

				usleep(50000);
				//Medición
				if(read_ADC_lmp(&lmp[i],rxADC)){
					printf("Error medición t%d.\n",i);
				}
				T[2*i+1]=adc_to_temp(*rxADC);
				printf("T%d: %f\n",2*i+1,T[2*i+1]);



				//Configuración de Channel 2
				*tx=0x12;
				if(write_reg_lmp(&(lmp[i]),0x1F,tx,1)){
					printf("Error configurando detección de clock y fuentes de corriente de la AFE %d\n",i);
				}
				//ADC_RESTART
				*tx=0x01;
				if(write_reg_lmp(&(lmp[i]),0x0B, tx,1)){
					printf("Error configurando detección de clock y fuentes de corriente de la AFE %d\n",i);
				}

				usleep(50000);
				//Medición
				if(read_ADC_lmp(&lmp[i],rxADC)){
					printf("Error medición t%d.\n",i);
				}
				P[i-12]=((float)(*rxADC)*VREF)/RESOLUTION;
				printf("pH%d: %f\n",i-12,P[i-12]);
			}
		}
	}
}

