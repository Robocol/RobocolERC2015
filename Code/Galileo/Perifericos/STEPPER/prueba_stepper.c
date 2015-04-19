#include <string.h>
#include "stepper_robocol.h"

int main(){
size_t size=40;
char* line=malloc(size);
uint32_t position,status,config=0;
uint8_t step,ocd,tval;
uint8_t alarm,corr,debug;
stp_device* devptr;
int32_t buf;

debug=0x5A;

printf("----------Prueba Driver Stepper----------\n");
stp_device dev1;
dev1.pin_cs=PIN7;
dev1.pin_dir=PIN5;
dev1.pin_stndby=1;
dev1.pin_flag=0;
dev1.exp_n=2;
dev1.pin_pwm=3;
dev1.period=5000000;

printf("Pin del stepper_device para chip select de Spi: %d\n",dev1.pin_cs);
if(stp_build(&dev1)){
	printf("Error en la construccion de stepper\n" );
}
printf("Tras construccion de stepper\n");
printf("Pin del stepper_device para chip select de Spi: %d\n",(*dev1.spi).pin);


devptr=&dev1;


		printf("Bienvenido al test de funcionamiento de Puente H (ERC 2015-ROBOCOL).\n Utilice una de los siguientes comandos:\n" 
				"\t enable\t\t\t-Habilita el stepper\n"
				"\t disable\t\t\t-Deshabilita el stepper\n"
				"\t stat\t\t\t\t-Imprime el STATUS register\n "
				"\t get-config\t\t\t-Imprime el CONFIG register\n "
				"\t get-step\t\t\t-Imprime el estado actual del selector de paso\n"
				"\t get-alarm\t\t\t-Imprime el registro de alarmas\n"
				"\t get-ocdt\t\t\t-Imprime el Overcurrent Detection Threshold actual\n"
				"\t get-tval\t\t\t-Retorna TVAL\n"
				"\t periodo\t\t\t-Cambia el periodo del step\n"
				"\t set-config\t\t\t-Imprime el CONFIG register\n "
				"\t set-step\t\t\t-Imprime el estado actual del selector de paso\n"
				"\t set-alarm\t\t\t-Imprime el registro de alarmas\n"
				"\t set-ocdt\t\t\t-Imprime el Overcurrent Detection Threshold actual\n"
				"\t set-tval\t\t\t-Imprime el TVAL\n"
				"\t debug\t\t\t-Permite rápido envia de CAMBIAR_PWM con un valor de 0x5A\n");
while(1){

		printf("Ingrese un comando\n");
		getline(&line,&size,stdin);
		printf("El comando ingresado fue: %s \n",line);

		//Mediciones
		if(!strcmp(line,"status\n")){
			stp_getStatus(devptr,&status);
			printf("STATUS: %X \n",status);

		}else if(!strcmp(line,"get-config\n")){
			stp_getConfig(devptr,&config);
			printf("Config: %X \n",config);

		}else if(!strcmp(line,"get-alarm\n")){
			stp_getAlarmEn(devptr,&alarm);
			printf("Alarm: %X \n",alarm);

		}else if(!strcmp(line,"get-step\n")){
			stp_getStepSel(devptr,&step);
			printf("Step: %X \n",step);

		}else if(!strcmp(line,"get-posicion\n")){
			stp_getPosition(devptr,&position);
			printf("Posición: %X \n",position);

		}else if(!strcmp(line,"get-ocdt\n")){
			stp_getOCDT(devptr,&ocd);
			printf("OCD: %d \n",ocd);
		}else if(!strcmp(line,"get-tval\n")){
			stp_getTVAL(devptr,&tval);
			printf("TVAL: %d \n",tval);
		//Setters
		}else if(!strcmp(line,"set-step\n")){
			printf("Ingrese el divisor del paso para paso de 1.8 grados:\n");
			getline(&line,&size,stdin);
			buf=atoi(line);
			printf("Cambiando divisor de paso a: %d \n",buf);
			if(stp_setStepSel(devptr,buf)){
				printf("Error en el cambio de divisor de paso.\n");

			}
		}else if(!strcmp(line,"periodo\n")){
			printf("Ingrese el periodo del paso en ns:\n");
			getline(&line,&size,stdin);
			buf=atoi(line);
			printf("Cambiando periodo a: %d \n",buf);
			stp_period(devptr,buf);
 
		}else if(!strcmp(line,"dir\n")){
			printf("Ingresela dirección: 0 - clockwise; 1 - counterclockwise:\n");
			getline(&line,&size,stdin);
			buf=atoi(line);
			printf("Cambiando direccipon a a: %d \n",buf);
			stp_period(devptr,buf);
	
		}else if(!strcmp(line,"set-posicion\n")){
			printf("Ingrese el marcador de la posición actual:\n");
			getline(&line,&size,stdin);
			buf=atoi(line);
			printf("Cambiando posición a: %d \n",buf);
			stp_setPosition(devptr,buf);

		}else if(!strcmp(line,"set-tval\n")){
			printf("Ingrese el TVAL deseado:\n");
			getline(&line,&size,stdin);
			buf=atoi(line);
			printf("Cambiando TVAL a: %d \n",buf);
			stp_setTVal(devptr,buf);
		}else if(!strcmp(line,"set-tonmin\n")){
			printf("Ingrese el TONMIN deseado:\n");
			getline(&line,&size,stdin);
			buf=atoi(line);
			printf("Cambiando TONMIN a: %d \n",buf);
			stp_setTOnMin(devptr,buf);

		}else if(!strcmp(line,"set-toffmin\n")){
			printf("Ingrese el TOFFMIN deseado:\n");
			getline(&line,&size,stdin);
			buf=atoi(line);
			printf("Cambiando TOFFMIN a: %d \n",buf);
			stp_setTOffMin(devptr,buf);

		}else if(!strcmp(line,"set-ocdt\n")){
			printf("Ingrese el OCDT deseado:\n");
			getline(&line,&size,stdin);
			buf=atoi(line);
			printf("Cambiando OCDT a: %d \n",buf);
			stp_setOCDT(devptr,buf);

		}else if(!strcmp(line,"enable\n")){
			printf("Habilitando stepper:\n");
			printf("Gpio para chip select de spi numero  %d\n",(*(*devptr).spi).pin );
			stp_enable(devptr);
	
		}else if(!strcmp(line,"disable\n")){
			printf("Deshabilitando stepper:\n");
			
			stp_disable(devptr);
	
		}else if(!strcmp(line,"exit\n")){
			printf("Cerrando el programa. Adiós\n");
			break;
		}else if(!strcmp(line,"debug\n")){
			while(1){
				stp_getTVAL(devptr,&tval);
				printf("TVAL: %d \n",tval);
			}
		}else{
		printf("Bienvenido al test de funcionamiento de Puente H (ERC 2015-ROBOCOL).\n Utilice una de los siguientes comandos:\n" 
				"\t enable\t\t\t-Habilita el stepper\n"
				"\t disable\t\t\t-Deshabilita el stepper\n"
				"\t stat\t\t\t\t-Imprime el STATUS register\n "
				"\t get-config\t\t\t-Imprime el CONFIG register\n "
				"\t get-step\t\t\t-Imprime el estado actual del selector de paso\n"
				"\t get-alarm\t\t\t-Imprime el registro de alarmas\n"
				"\t get-ocdt\t\t\t-Imprime el Overcurrent Detection Threshold actual\n"
				"\t get-tval\t\t\t-Imprime el registro de alarmas\n"
				"\t period\t\t\t-Imprime el registro de alarmas\n"
				"\t set-config\t\t\t-Imprime el CONFIG register\n "
				"\t set-step\t\t\t-Imprime el estado actual del selector de paso\n"
				"\t set-alarm\t\t\t-Imprime el registro de alarmas\n"
				"\t set-ocdt\t\t\t-Imprime el Overcurrent Detection Threshold actual\n"
				"\t set-tval\t\t\t-Imprime el registro de alarmas\n"
				"\t debug\t\t\t-Permite rápido envia de CAMBIAR_PWM con un valor de 0x5A\n");
		
		}
		printf("--------------------------------------------------------------\n");
	}

}