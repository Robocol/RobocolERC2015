#include "lmp_robocol.h"
lmp_dev lmp;

/*Inicio del programa*/
	

int main(){

	uint8_t* buff=malloc(sizeof(uint8_t));
	uint32_t* txADC=malloc(sizeof(uint8_t));
	uint32_t* rxADC=malloc(sizeof(uint8_t));


	/*---------------Inicialización LMPs---------------*/

	lmp.pin_cs=PIN7;


	build_lmp(&(lmp));



	printf("Bienvenido al programa de prueba de la Libreria lmp_robocol\n");
	printf("\n--------------------------------------------------------------------------------------\n");

	*buff=0x41;
	/*Encendiendo Leds (Si es que no se queman)*/
	if(write_reg_lmp(&lmp,0x0E, buff,1)){
		printf("Error configurando dirección de GPIOs de la AFE \n");
	}

	*buff=0x01;
	if(write_reg_lmp(&lmp,0x0F, buff,1)){
		printf("Error configurando asignando valor de GPIOs de la AFE \n");
	}


	/*Configuración de Mediciones*/
	printf("\n--------------------------------------------------------------------------------------\n");
	printf("Configurando mediciones\n\n");

	//Background Calibration
	 //Modo 2:offset correction/gain correction
	*buff=0x02;
	if(write_reg_lmp(&lmp,0x10, buff,1)){
		printf("Error configurando asignando valor de GPIOs de la AFE \n");
	}

	// Detección y Configuración de Clk. Configuración de fuentes de corriente
	printf("Detección y Configuración de Clk y fuentes de corriente:");

	//Deshabilitada detección de CLK - Seleccionado EXCLK -Corriente 1mA
	*buff=0x3A;
	if(write_reg_lmp(&lmp,0x12, buff,1)){
		printf("Error configurando detección de clock y fuentes de corriente de la AFE \n");
	}



	printf("Bienvenido al test de medición de dos canales por medio de LMP90100 (Intrumentación Electrónica 201510).\n Utilice una de los siguientes comandos:\n" 
	"\t 0\t\t-Realiza una única lectura del canal de medición 0 del AFE (VIN0-VIN1)\n"
	"\t 1\t\t-Realiza una única lectura del canal de medición 0 del AFE (VIN2-VIN3)\n"
	"\t q\t\t-Izquierda\n\n"
	"En caso de comando inválido, se mostrará nuevamente este menu como ayuda al usuario\n");
	

	/*----------Mediciones iterativas----------*/

	while(1){

		printf("Mediciónes de AFE \n");
		*buff=0x00;
		if(write_reg_lmp(&(lmp),0x1F,buff,1)){
			printf("Error configurando detección de clock y fuentes de corriente de la AFE \n");
		}

		if(read_ADC_lmp(&lmp,rxADC)){
			printf("Error medición de ADC.\n");
		}
		printf("Valor ADC Temp 1: ",*rxADC);

		*buff=0x09;
		if(write_reg_lmp(&lmp,0x1F,buff,1)){
			printf("Error configurando detección de clock y fuentes de corriente de la AFE \n");
		}
		if(read_ADC_lmp(&lmp,rxADC)){
			printf("Error medición de ADC .\n");
		}

		printf("Valor ADC Temp 2: ",*rxADC);
	}
}