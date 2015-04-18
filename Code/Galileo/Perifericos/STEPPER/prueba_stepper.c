#include <string.h>
#include "stepper_robocol.h"

int main(){
size_t size=40;
char* line=malloc(size);
uint32_t status,config=0;
int8_t step;
uint8_t temp, vel,corr,est,debug;
ph_dev* devptr;
int32_t buf;

temp, vel,corr,est,debug=0x5A;

printf("----------Prueba Driver Puente H----------\n");


stp_dev dev1={PIN6,PIN2,0,1,3,5000000};

stp_build(&dev1);


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
		if(!strcmp(line,"stat\n")){
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
			stp_getPosition(devptr,&step);
			printf("Posición: %X \n",step);

		}else if(!strcmp(line,"get-ocdt\n")){
			stp_getOCDT(devptr,&ocd);
			printf("Estado: %d \n",est);
		
		//Setters
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
			printf("Cambiando periodo a: %d \n",buf);
			stp_period(devptr,buf);
	
		}else if(!strcmp(line,"set_posicion\n")){
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
			stp_setOffMin(devptr,buf);

		}else if(!strcmp(line,"set-ocdt\n")){
			printf("Ingrese el OCDT deseado:\n");
			getline(&line,&size,stdin);
			buf=atoi(line);
			printf("Cambiando OCDT a: %d \n",buf);
			stp_setOCDT(devptr,buf);

		}else if(!strcmp(line,"enable\n")){
			printf("Habilitando stepper:\n");
			stp_enable(devptr);
	
		}else if(!strcmp(line,"disable\n")){
			printf("Deshabilitando stepper:\n");
			
			stp_disable(devptr);
	
		}else if(!strcmp(line,"exit\n")){
			printf("Cerrando el programa. Adiós\n");
			break;
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