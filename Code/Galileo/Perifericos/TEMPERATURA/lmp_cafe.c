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

const uint32_t 	RESOLUTION=8388608;
const uint8_t 	NUM_AFE=14;
const float		VREF=5;
const float		THRESHOLD=50;


/*---------------Declaración de Variables de Medición---------------*/


//Temperatura
//float TA0[,TA1,TA2,TA3,TB0,TB1,TB2,TB3,TC0,TC1,TC2,TC3,TD0,TD1,TD2,TD3,TE0,TE1,TE2,TE3,TF0,TF1,TF2,TF3,TG0,TG1,TG2,TG3];
float T[28];


//Turbidez
//float UA0,UA1,UA2,UA3,UC0,UC1,UC2,UC3,UE0,UE1,UE2,UE3;
float U[12];


//PH
float P[2];


/*---------------Dispisitivos LMP---------------*/

lmp_dev lmp[14];

/*Inicio del programa*/
	

int main(){

	uint8_t* 	tx=malloc(2*sizeof(uint8_t));
	uint8_t* 	rx=malloc(2*sizeof(uint8_t));
	uint8_t* 	txADC=malloc(4*sizeof(uint8_t));
	uint8_t* 	rxADC=malloc(4*sizeof(uint8_t));


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

	int i=0
	for (i=0;i<14;i++){
		build_lmp(&(lmp[i]));
	}



	printf("Bienvenido al programa de control del Sistema para Caracterización del Proceso de Fermentación del Café\n");
	printf("\n--------------------------------------------------------------------------------------\n");


	/*Encendiendo Leds (Si es que no se queman)*/
	for (i=0;i<NUM_AFE;i++){
		if(write_reg_lmp(&(lmp[i]),0x0E, 0x41,1)){
			printf("Error configurando dirección de GPIOs de la AFE %d\n",i);
		}
	}
	for (i=0;i<NUM_AFE;i++){
		if(write_reg_lmp(&(lmp[i]),0x0F, 0x01,1)){
			printf("Error configurando asignando valor de GPIOs de la AFE %d\n",i);
		}
	}


	/*Configuración de Mediciones*/
	printf("\n--------------------------------------------------------------------------------------\n");
	printf("Configurando mediciones\n\n");

	//Background Calibration
	for (i=0;i<NUM_AFE;i++){
		 //Modo 2:offset correction/gain correction
		if(write_reg_lmp(&(lmp[i]),0x10, 0x02,1)){
			printf("Error configurando asignando valor de GPIOs de la AFE %d\n",i);
		}
	}

	// Detección y Configuración de Clk. Configuración de fuentes de corriente
	printf("Detección y Configuración de Clk y fuentes de corriente:");
	for (i=0;i<NUM_AFE;i++){
		//Deshabilitada detección de CLK - Seleccionado EXCLK -Corriente 1mA
		if(write_reg_lmp(&(lmp[i]),0x12, 0x3A,1)){
			printf("Error configurando detección de clock y fuentes de corriente de la AFE %d\n",i);
		}
	}



	printf("Bienvenido al test de medición de dos canales por medio de LMP90100 (Intrumentación Electrónica 201510).\n Utilice una de los siguientes comandos:\n" 
	"\t 0\t\t-Realiza una única lectura del canal de medición 0 del AFE (VIN0-VIN1)\n"
	"\t 1\t\t-Realiza una única lectura del canal de medición 0 del AFE (VIN2-VIN3)\n"
	"\t q\t\t-Izquierda\n\n"
	"En caso de comando inválido, se mostrará nuevamente este menu como ayuda al usuario\n");
	

	/*----------Mediciones iterativas----------*/

	while(){
		for(i=0;i<NUM_AFE;i++){

			printf("Mediciónes de AFE %d.\n",i);
			if(i<12){

				//Configuración de Channel 0
				if(write_reg_lmp(&(lmp[i]),0x1F, 0x00,1)){
					printf("Error configurando detección de clock y fuentes de corriente de la AFE %d\n",i);
				}
				if(read_ADC_lmp(lmp[i],rxADC)){
					printf("Error medición t%d.\n",i);
				}
				T[2*i]=rxADC*VREF/RESOLUTION;

				//Configuración de Channel 0
				if(write_reg_lmp(&(lmp[i]),0x1F, 0x09,1)){
					printf("Error configurando detección de clock y fuentes de corriente de la AFE %d\n",i);
				}
				if(read_ADC_lmp(lmp[i],rxADC)){
					printf("Error medición t%d.\n",i);
				}
				T[2*i+1]=rxADC*VREF/RESOLUTION;


				//Configuración de Channel 0
				if(write_reg_lmp(&(lmp[i]),0x1F, 0x00,1)){
					printf("Error configurando detección de clock y fuentes de corriente de la AFE %d\n",i);
				}
				if(read_ADC_lmp(lmp[i],rxADC)){
					printf("Error medición t%d.\n",i);
				}
				U[i]=rxADC*VREF/RESOLUTION;

			}else{

				if(read_ADC_lmp(lmp[i],rxADC)){
					printf("Error medición t%d.\n",i);
				}
				T[2*i]=rxADC*VREF/RESOLUTION;

				if(read_ADC_lmp(lmp[i],rxADC)){
					printf("Error medición t%d.\n",i);
				}
				T[2*i+1]=rxADC*VREF/RESOLUTION;

				if(read_ADC_lmp(lmp[i],rxADC)){
					printf("Error medición t%d.\n",i);
				}
				P[i-12]=rxADC*VREF/RESOLUTION;
			}
		}
	}

}