#include <string.h>
#include "ph_robocol.h"

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


stp_dev dev1={PIN6,PIN2,0,3,1};

stp_build(&dev1);


devptr=&dev1;


		printf("Bienvenido al test de funcionamiento de Puente H (ERC 2015-ROBOCOL).\n Utilice una de los siguientes comandos:\n" 
				"\t enable\t\t\t-Habilita el puente H\n"
				"\t disable\t\t\t-Deshabilita el puente H\n"
				"\t medir-velocidad\t-Imprime la velocidad actual del motor\n "
				"\t medir-temperatura\t-Imprime la temperatura actual del motor\n "
				"\t medir-corriente\t-Imprime la corriente actual que atravieza el motor\n"
				"\t dar-estado\t \t-Imprime el estado de operación actual del motor\n"
				"\t cambiar-pwm\t\t-Cambia ciclo útil del PWM. Valor de entrada entre 0 y 255\n"
				"\t cambiar-direccion\t-Cambia la dirección de giro del motor. Valor de entrada 1 o 0\n"
				"\t cambiar-velocidad\t-Cambia la velocidad objetivo del controlador. Valor de entrada entre ------\n"
				"\t cambiar-estado\t \t-Cambia el estado de operación del puente H. VAlor de entrada entre 16 o 32 TODO: Parser\n"
				"\t cambiar-kic\t\t-Cambia el valor de KIC. Valor de entrada entre 0 y 255\n"
				"\t cambiar-kpc\t\t-Cambia el valor de KPC. Valor de entrada entre 0 y 255\n"
				"\t cambiar-kiv\t\t-Cambia el valor de KIV. Valor de entrada entre 0 y 255\n"
				"\t cambiar-kpv\t\t-Cambia el valor de KPV. Valor de entrada entre 0 y 255\n"
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
			printf("Alarm: %d \n",alarm);

		}else if(!strcmp(line,"get-step\n")){
			stp_getStepSel(devptr,&step);
			printf("Step: %X \n",step);

		}else if(!strcmp(line,"get-ocdt\n")){
			stp_getOCDT(devptr,&ocd);
			printf("Estado: %d \n",est);
		
		//Setters
		}else if(!strcmp(line,"pwm\n")){
			printf("Ingrese el periodo del paso en ns:\n");
			getline(&line,&size,stdin);
			buf=atoi(line);
			printf("Cambiando PWM a: %d \n",buf);
			stp_setStepPeriod(devptr,buf);
 
		}else if(!strcmp(line,"set_posicion\n")){
			printf("Ingrese el marcador de la posición actual:\n");
			getline(&line,&size,stdin);
			buf=atoi(line);
			printf("Cambiando posición a: %d \n",buf);
			ph_setDireccion(devptr,buf);

		}else if(!strcmp(line,"set-tval\n")){
			printf("Ingrese el TVAL deseado:\n");
			getline(&line,&size,stdin);
			buf=atoi(line);
			printf("Cambiando TVAL a: %d \n",buf);
			ph_setTVal(devptr,buf);

		}else if(!strcmp(line,"set-tonmin\n")){
			printf("Ingrese el TONMIN deseado:\n");
			getline(&line,&size,stdin);
			buf=atoi(line);
			printf("Cambiando TONMIN a: %d \n",buf);
			ph_setTOnMin(devptr,buf);

		}else if(!strcmp(line,"set-toffmin\n")){
			printf("Ingrese el TOFFMIN deseado:\n");
			getline(&line,&size,stdin);
			buf=atoi(line);
			printf("Cambiando TOFFMIN a: %d \n",buf);
			ph_setOffMin(devptr,buf);

		}else if(!strcmp(line,"set-ocdt\n")){
			printf("Ingrese el OCDT deseado:\n");
			getline(&line,&size,stdin);
			buf=atoi(line);
			printf("Cambiando OCDT a: %d \n",buf);
			ph_setOCDT(devptr,buf);

		}else if(!strcmp(line,"enable\n")){
			printf("Ingrese 1 para activar, 0 para desactivar:\n");
			getline(&line,&size,stdin);
			buf=atoi(line);
			printf("Cambiando enable a: %d \n",buf);
			if(buf){
				ph_enable(devptr);
			}else{
				ph_disable(devptr);
			}

		}else if(!strcmp(line,"exit\n")){
			printf("Cerrando el programa. Adiós\n");
			break;
		}else{
			printf("Comando inválido. Utilice una de las siguientes opciones:\n" 
				"\t enable\t\t\t-Habilita o deshabilita el puente H. Valores de entrada 1 o 0\n"
				"\t medir-velocidad\t-Imprime la velocidad actual del motor\n "
				"\t medir-temperatura\t-Imprime la temperatura actual del motor\n "
				"\t medir-corriente\t-Imprime la corriente actual que atravieza el motor\n"
				"\t dar-estado\t \t-Imprime el estado de operación actual del motor\n"
				"\t cambiar-pwm\t\t-Cambia ciclo útil del PWM. Valor de entrada entre 0 y 255\n"
				"\t cambiar-direccion\t-Cambia la dirección de giro del motor. Valor de entrada 1 o 0\n"
				"\t cambiar-velocidad\t-Cambia la velocidad objetivo del controlador. Valor de entrada entre ------\n"
				"\t cambiar-estado\t \t-Cambia el estado de operación del puente H. VAlor de entrada entre 16 o 32 TODO: Parser\n"
				"\t cambiar-kic\t\t-Cambia el valor de KIC. Valor de entrada entre 0 y 255\n"
				"\t cambiar-kpc\t\t-Cambia el valor de KPC. Valor de entrada entre 0 y 255\n"
				"\t cambiar-kiv\t\t-Cambia el valor de KIV. Valor de entrada entre 0 y 255\n"
				"\t cambiar-kpv\t\t-Cambia el valor de KPV. Valor de entrada entre 0 y 255\n"
				"\t debug\t\t-Permite rápido envia de CAMBIAR_PWM con un valor de 0x5A\n");
		}
		printf("--------------------------------------------------------------\n");
	}

}