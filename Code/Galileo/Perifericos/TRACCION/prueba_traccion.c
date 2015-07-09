#include "traccion_robocol.h"

tr_st prueba_ph(ph_dev* devptr);

size_t size;
char* line;
FILE *fdl;

int32_t position,status,config=0;
uint8_t step,ocd,tval;
uint8_t corr,debug;
int32_t buf,buf2,arg;
double fbuf;

uint8_t temp, vel,corr,est;
uint8_t vel_pwm,giro,res;
uint8_t i=0;

int main(int argc, char const *argv[]){

	ph_dev* ph;
	size=40;
	vel_pwm=50;
	line=malloc(size);
	giro=1;
	uint8_t finished=0;

	printf("Test de funcionamiento del sistema de traccion (ERC 2015-ROBOCOL).\n Seleccione el lado del robot que utilizará:\n"
			"\tr\t\t\t Right side\n"
			"\tl\t\t\t Left side.\n"
			"\texit\t\t\tSalir del programa\n");

	getline(&line,&size,stdin);
	if(!strcmp(line,"l\n")){
		printf("before built right\n");
		if(tr_build(TR_SLAVE,TR_LEFT_SIDE)){
			printf("Error al Construir el dispositivo de traccion.\n");
		}
		printf("built left\n");
	}if(!strcmp(line,"r\n")){
		printf("before built right\n");
		if(tr_build(TR_SLAVE,TR_RIGHT_SIDE)){
			printf("Error al Construir el dispositivo de traccion.\n");
		}
		printf("built right\n");
	}


	while(!finished){
	printf("Test de funcionamiento del sistema de traccion (ERC 2015-ROBOCOL).\n Utilice una de los siguientes comandos:\n"
			"\tw\t\t\t Adelante.\n"
			"\ts\t\t\t Atras\n"
			"\ta\t\t\t Girar a izquierda\n"
			"\td\t\t\t Girar a derecha\n"
			"\tx\t\t\t Detener\n"
			"\te\t\t\t Freno de Emergencia\n"
			"\tvelpwm\t\t Cambia la velocidad de todos los motores a la ingresada\n"
			"\tselGiro\t\t Selecciona el tipo de giro\n"
			"\tdiffArg\t\t Selecciona la differencia de velocidades para los giros diferenciales\n"
			);

		printf("Ingrese un comando\n");
		getline(&line,&size,stdin);
		printf("El comando ingresado fue: %s \n",line);
		if(!strcmp(line,"ph\n")){
			while(!finished){
				printf("Ingrese el número del puente H que desea utilizar:\n"
						"\t1\t\t\tPuente H del motor frontal\n"
						"\t2\t\t\tPuente H del motor trasero\n"
						"\tbck\t\t\tRegresar al anterior menú.\n");
				printf("Ingrese un comando\n");
				getline(&line,&size,stdin);

				if (!strcmp(line,"1\n")){
					prueba_ph(tr_device.front_ph);
				}else if(!strcmp(line,"2\n")){
					prueba_ph(tr_device.back_ph);
				}else if(!strcmp(line,"bck\n")){
					finished=1;
				}else{
					printf("El comando ingresado no es válido. Intente de nuevo\n");
				}
			}
			finished=0;
		}else if(!strcmp(line,"w\n")){
			printf("Moviendo hacia adelante..\n");
			tr_forward(vel_pwm);

		}else if(!strcmp(line,"s\n")){
			printf("Moviendo hacia atrás...\n");
			tr_backward(vel_pwm);

		}else if(!strcmp(line,"x\n")){
			printf("Deteniendo\n");
			tr_forward(0);

		}else if(!strcmp(line,"e\n")){
			printf("Freno de Emergencia!");
			tr_eBrake();

		}else if(!strcmp(line,"a\n")){
			printf("Girando a la izquierda...\n");
			tr_setVP(vel_pwm);
			if(giro==1){
				printf("Giro sobre el eje\n");
				res=tr_spin(TR_TURN_LEFT, vel_pwm);	
			}else if(giro==2){
				printf("Giro diferencial\n");
				res=tr_diffTurn(TR_TURN_LEFT, arg);	
			}else if(giro==3){
				printf("Giro diferencial diagonal\n");
				res=tr_diagonalDiffTurn(TR_TURN_LEFT, arg);	
			}
			if(res){
				printf("Error girando a la izquierda\n");
			}
		}else if(!strcmp(line,"d\n")){
			printf("Girando a la derecha...\n");
			tr_setVP(vel_pwm);
			if(giro==1){
				printf("Giro sobre el eje\n");
				res=tr_spin(TR_TURN_RIGHT, vel_pwm);	
			}else if(giro==2){
				printf("Giro diferencial\n");
				res=tr_diffTurn(TR_TURN_RIGHT, arg);	
			}else if(giro==3){
				printf("Giro diferencial diagonal\n");
				res=tr_diagonalDiffTurn(TR_TURN_RIGHT, arg);	
			}

			if(res){
				printf("Error girando a la derecha\n");
			}
		}else if(!strcmp(line,"selGiro\n")){
			printf("Selecciones una de las siguientes opciones:\n"
			"\t1\t\t\t Giro sobre el eje\n"
			"\t2\t\t\t Giro diferencial\n"
			"\t3\t\t\t Giro diferencial diagonal\n");


			getline(&line,&size,stdin);
			giro=atoi(line);
			if(giro==1){
				printf("Escogido: Giro sobre el eje\n");
			}else if(giro==2){
				printf("Escogido: Giro diferencial\n");
			}else if(giro==3){
				printf("Escogido: Giro diferencial diagonal\n");
			}else{
				printf("Error: En valor ingresado no es válido. Intente nuevamente\n");
			}

		}else if(!strcmp(line,"velpwm\n")){
			printf("Ingrese la velocidad/pwm deseado:\n");
			getline(&line,&size,stdin);
			vel_pwm=atoi(line);
			printf("Cambiando velocidad/pwm a: %d \n",vel_pwm);
			tr_setVP(vel_pwm);

		}else if(!strcmp(line,"diffArg\n")){
			printf("Ingrese la diferencia de velocidad/pwm para el giro diferencial deseado:\n");
			getline(&line,&size,stdin);
			arg=atoi(line);
			printf("Cambiando velocidad/pwm diferencial a: %d \n",arg);

		}else if(!strcmp(line,"exit\n")){
			finished=1;
		}
	}
	return 0;
}

tr_st prueba_ph(ph_dev* devptr){

	build_expander((*devptr).addr);

	printf("pinEnable: %d\n",(*devptr).pin_enable);
	printf("Test de funcionamiento de Puente H.\n Utilice una de los siguientes comandos:\n" 
		"\t enable\t\t\t-Habilita o deshabilita el puente H. Valores de entrada 1 o 0\n"
		"\t getvel\t-Imprime la velocidad actual del motor\n "
		"\t gettemp\t-Imprime la temperatura actual del motor\n "
		"\t getcorr\t-Imprime la corriente actual que atravieza el motor\n"
		"\t getstate\t \t-Imprime el estado de operación actual del motor\n"
		"\t pwm\t\t-Cambia ciclo útil del PWM. Valor de entrada entre 0 y 255\n"
		"\t dir\t-Cambia la dirección de giro del motor. Valor de entrada 1 o 0\n"
		"\t setvel\t-Cambia la velocidad objetivo del controlador. Valor de entrada entre ------\n"
		"\t setstate\t \t-Cambia el estado de operación del puente H. Valor de entrada entre 16 o 32 TODO: Parser\n"
		"\t kic\t\t-Cambia el valor de KIC. Valor de entrada entre 0 y 255\n"
		"\t kpc\t\t-Cambia el valor de KPC. Valor de entrada entre 0 y 255\n"
		"\t kiv\t\t-Cambia el valor de KIV. Valor de entrada entre 0 y 255\n"
		"\t kpv\t\t-Cambia el valor de KPV. Valor de entrada entre 0 y 255\n"
		"\t sp_vel\t\t\t-Cambia el valor de SP_VELOCIDAD. Valor de entrada entre 0 y 255\n"
		"\t sp_cor\t\t\t-Cambia el valor de SP_CORRIENTE. Valor de entrada entre 0 y 255\n"
		"\t debug\t\t\t-Permite rápido envia de CAMBIAR_PWM con un valor de 0x5A\n");

while(1){

		printf("Ingrese un comando\n");
		getline(&line,&size,stdin);
		printf("El comando ingresado fue: %s \n",line);

		//Mediciones
		if(!strcmp(line,"gettemp\n")){
			ph_getTemperatura(devptr,&temp);
			printf("Temperatura: %d \n",temp);

		}else if(!strcmp(line,"getvel\n")){
			ph_getVelocidad(devptr,&vel);
			printf("Velocidad: %d \n",vel);

		}else if(!strcmp(line,"getcorr\n")){
			ph_getCorriente(devptr,&corr);
			printf("Corriente: %d \n",corr);

		}else if(!strcmp(line,"getstate\n")){
			ph_getEstado(devptr,&est);
			printf("Estado: %d \n",est);
		
		//Setters
		}else if(!strcmp(line,"pwm\n")){
			printf("Ingrese el PWM deseado:\n");
			getline(&line,&size,stdin);
			buf=atoi(line);
			if(ph_setPWM(devptr,(uint8_t)buf)){
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
			ph_setVel(devptr,buf);

		}else  if(!strcmp(line,"sp_cor\n")){
			printf("Ingrese el SP_CORRIENTE deseado:\n");
			getline(&line,&size,stdin);
			buf=atoi(line);
			printf("Cambiando SP_CORRIENTE a: %d \n",buf);
			ph_setCorriente(devptr,buf);

		}else  if(!strcmp(line,"move\n")){
			printf("Ingrese la posición deseada:\n");
			getline(&line,&size,stdin);
			buf=atoi(line);
			printf("Cambiando posición a: %d \n",buf);
			ph_moveToAngle(devptr,buf);

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

		}else if(!strcmp(line,"exit\n")){
			break;
		}else if(!strcmp(line,"debug\n")){
			while(getchar()!='q'){
				printf("%c\n",debug);
				ph_setPWM(devptr,debug);
			}
		}else{
		printf("Test de funcionamiento de Puente H (ERC 2015-ROBOCOL).\n Utilice una de los siguientes comandos:\n" 
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
				"\t debug\t\t\t-Permite rápido envia de CAMBIAR_PWM con un valor de 0x5A\n");
		}
		printf("--------------------------------------------------------------\n");
	}
}


