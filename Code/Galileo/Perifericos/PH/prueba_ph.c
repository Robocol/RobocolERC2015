#include <string.h>
#include "ph_robocol.h"

int main(){
const uint8_t STEP_SIZE=10;
size_t size=40;
char* line=malloc(size);
uint8_t temp, vel,corr,est,debug;
ph_dev* devptr;
int buf;
uint8_t pwmval;
uint8_t selected=1;
int8_t steps=0;
uint8_t curr_pwm=0;
int8_t step;
uint8_t i=0;

temp, vel,corr,est,debug=0x5A;

printf("----------Prueba Driver Puente H----------\n");


ph_dev dev1={PINA0,0,1,4,0};
ph_dev dev2={PINA1,2,3,4,0};

ph_build(&dev1);
ph_build(&dev2);


while(selected){
	printf("Bienvenido al test de funcionamiento de Puente H (URC 2015-ROBOCOL).\n "
		"Seleccione el motor con el cual desea trabajar:\n"
		"\t1\tSelecciona el motor1\n"
		"\t2\tSelecciona el motor2\n");
	printf("Ingrese el numero del motor\n");
	getline(&line,&size,stdin);
	printf("El comando ingresado fue: %s \n",line);
	if(!strcmp(line,"1\n")){
		devptr=&dev1;
		printf("Motor 1 seleccionado\n");
		selected=0;
	}else if (!strcmp(line,"2\n")){
		devptr=&dev2;
		printf("Motor 2 seleccionado\n");
		selected=0;
	}else{
		printf("Numero de motor ingresado no valido.\n");
	}
}
if(ph_setEstado(devptr,32)){
	printf("Error en set de Estado a 32(prueba_ph.c)\n");
	perror("Descripción");		
}
if(ph_setPWM(devptr,0)){
	printf("Error en set de PWM(prueba_ph.c)\n");
	perror("Descripción");	
}



printf("Test de funcionamiento de Puente H (URC 2015-ROBOCOL).\n Utilice una de los siguientes comandos:\n" 
		"\t enable\t\t\t-Habilita o deshabilita el puente H. Valores de entrada 1 o 0\n"
		"\t getvel\t-Imprime la velocidad actual del motor\n "
		"\t gettemp\t-Imprime la temperatura actual del motor\n "
		"\t getcorr\t-Imprime la corriente actual que atravieza el motor\n"
		"\t getstate\t \t-Imprime el estado de operación actual del motor\n"
		"\t pwm\t\t-Cambia ciclo útil del PWM. Valor de entrada entre 0 y 255\n"
		"\t dir\t-Cambia la dirección de giro del motor. Valor de entrada 1 o 0\n"
		"\t setvel\t-Cambia la velocidad objetivo del controlador. Valor de entrada entre ------\n"
		"\t setstate\t \t-Cambia el estado de operación del puente H. VAlor de entrada entre 16 o 32 TODO: Parser\n"
		"\t kic\t\t-Cambia el valor de KIC. Valor de entrada entre 0 y 255\n"
		"\t kpc\t\t-Cambia el valor de KPC. Valor de entrada entre 0 y 255\n"
		"\t kiv\t\t-Cambia el valor de KIV. Valor de entrada entre 0 y 255\n"
		"\t kpv\t\t-Cambia el valor de KPV. Valor de entrada entre 0 y 255\n"
		"\t sp_vel\t\t\t-Cambia el valor de SP_VELOCIDAD. Valor de entrada entre 0 y 255\n"
		"\t sp_cor\t\t\t-Cambia el valor de SP_VELOCIDAD. Valor de entrada entre 0 y 255\n"
		"\t motor\t\t\t-Cambia el motor. Valor de entrada 1 o 2\n"
		"\t debug\t\t\t-Permite rápido envia de CAMBIAR_PWM con un valor de 0x5A\n");

while(1){

		printf("Ingrese un comando\n");
		getline(&line,&size,stdin);
		printf("El comando ingresado fue: %s \n",line);

		//Mediciones
		if(!strcmp(line,"gettemp\n")){
			getTemperatura(devptr,&temp);
			printf("Temperatura: %d \n",temp);

		}else if(!strcmp(line,"getvel\n")){
			getVelocidad(devptr,&vel);
			printf("Velocidad: %d \n",vel);

		}else if(!strcmp(line,"getcorr\n")){
			getCorriente(devptr,&corr);
			printf("Corriente: %d \n",corr);

		}else if(!strcmp(line,"getstate\n")){
			getEstado(devptr,&est);
			printf("Estado: %d \n",est);
		
		//Setters
		}else if(!strcmp(line,"pwm\n")){
			printf("Ingrese el PWM deseado:\n");
			getline(&line,&size,stdin);
			buf=atoi(line);
			if(setPWMSmooth(devptr, (uint8_t)buf, (uint8_t)STEP_SIZE)){
				printf("Error en set de pwm\n");
			}

		}else if(!strcmp(line,"dir\n")){
			printf("Ingrese la direccion deseada ( 1 o 0):\n");
			getline(&line,&size,stdin);
			buf=atoi(line);
			printf("Cambiando direccion a: %d \n",buf);
			ph_setDireccion(devptr,buf);

		}else if(!strcmp(line,"setvel\n")){
			printf("Ingrese la velocidad deseada:\n");
			getline(&line,&size,stdin);
			buf=atoi(line);
			printf("Cambiando velocidad a: %d \n",buf);
			ph_setVel(devptr,buf);

		}else if(!strcmp(line,"setstate\n")){
			printf("Ingrese el estado deseado:\n");
			getline(&line,&size,stdin);
			buf=atoi(line);
			printf("Cambiando estado a: %d \n",buf);
			ph_setEstado(devptr,buf);

		}else if(!strcmp(line,"kpv\n")){
			printf("Ingrese el KPV deseado:\n");
			getline(&line,&size,stdin);
			buf=atoi(line);
			printf("Cambiando KPV a: %d \n",buf);
			ph_setKPV(devptr,buf);

		}else if(!strcmp(line,"kiv\n")){
			printf("Ingrese el KIV deseado:\n");
			getline(&line,&size,stdin);
			buf=atoi(line);
			printf("Cambiando KIV a: %d \n",buf);
			ph_setKIV(devptr,buf);

		}else if(!strcmp(line,"kpc\n")){
			printf("Ingrese el KPC deseado:\n");
			getline(&line,&size,stdin);
			buf=atoi(line);
			printf("Cambiando KPC a: %d \n",buf);
			ph_setKPC(devptr,buf);

		}else if(!strcmp(line,"kic\n")){
			printf("Ingrese el KIC deseado:\n");
			getline(&line,&size,stdin);
			buf=atoi(line);
			printf("Cambiando KIC a: %d \n",buf);
			ph_setKIC(devptr,buf);

		}else if(!strcmp(line,"sp_vel\n")){
			printf("Ingrese el SP_VELOCIDAD deseado:\n");
			getline(&line,&size,stdin);
			buf=atoi(line);
			printf("Cambiando SP_VELOCIDAD a: %d \n",buf);
			ph_setKIC(devptr,buf);

		}else  if(!strcmp(line,"sp_cor\n")){
			printf("Ingrese el SP_CORRIENTE deseado:\n");
			getline(&line,&size,stdin);
			buf=atoi(line);
			printf("Cambiando SP_CORRIENTE a: %d \n",buf);
			ph_setKIC(devptr,buf);

		}else  if(!strcmp(line,"enable\n")){
			printf("Ingrese 1 para activar, 0 para desactivar:\n");
			getline(&line,&size,stdin);
			buf=atoi(line);
			printf("Cambiando enable a: %d \n",buf);
			if(buf){
				ph_enable(devptr);
			}else{
				ph_disable(devptr);
			}

		}else if(!strcmp(line,"motor\n")){
			printf("Ingrese 1 para Motor1, 2 para Motor2:\n");
			getline(&line,&size,stdin);
			buf=atoi(line);
			printf("Cambiando Motor a: %d \n",buf);
			if(buf==1){
				devptr=&dev1;
				printf("%d\n", (*devptr).pin_in_b);
			}else{
				devptr=&dev2;
				printf("%d\n", (*devptr).pin_in_b);
			}

		}else if(!strcmp(line,"exit\n")){
			printf("Cerrando el programa. Adiós\n");
			break;
		}else if(!strcmp(line,"debug\n")){
			while(getchar()!='q'){
				printf("%c\n",debug);
				ph_setPWM(devptr,debug);
			}

		}else{
		printf("Bienvenido al test de funcionamiento de Puente H (URC 2015-ROBOCOL).\n Utilice una de los siguientes comandos:\n" 
				"\t enable\t\t\t-Habilita o deshabilita el puente H. Valores de entrada 1 o 0\n"
				"\t getvel\t-Imprime la velocidad actual del motor\n "
				"\t gettemp\t-Imprime la temperatura actual del motor\n "
				"\t getcorr\t-Imprime la corriente actual que atravieza el motor\n"
				"\t getstate\t \t-Imprime el estado de operación actual del motor\n"
				"\t pwm\t\t-Cambia ciclo útil del PWM. Valor de entrada entre 0 y 255\n"
				"\t dir\t-Cambia la dirección de giro del motor. Valor de entrada 1 o 0\n"
				"\t setvel\t-Cambia la velocidad objetivo del controlador. Valor de entrada entre ------\n"
				"\t setstate\t \t-Cambia el estado de operación del puente H. VAlor de entrada entre 16 o 32 TODO: Parser\n"
				"\t kic\t\t-Cambia el valor de KIC. Valor de entrada entre 0 y 255\n"
				"\t kpc\t\t-Cambia el valor de KPC. Valor de entrada entre 0 y 255\n"
				"\t kiv\t\t-Cambia el valor de KIV. Valor de entrada entre 0 y 255\n"
				"\t kpv\t\t-Cambia el valor de KPV. Valor de entrada entre 0 y 255\n"
				"\t sp_vel\t\t\t-Cambia el valor de SP_VELOCIDAD. Valor de entrada entre 0 y 255\n"
				"\t sp_cor\t\t\t-Cambia el valor de SP_VELOCIDAD. Valor de entrada entre 0 y 255\n"
				"\t motor\t\t\t-Cambia el motor. Valor de entrada 1 o 2\n"
				"\t debug\t\t\t-Permite rápido envia de CAMBIAR_PWM con un valor de 0x5A\n");
		}
		printf("--------------------------------------------------------------\n");
	}

}