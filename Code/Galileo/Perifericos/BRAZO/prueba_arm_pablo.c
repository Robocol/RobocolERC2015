#include <signal.h>
#include <string.h>
#include <sys/times.h>
#include <sys/types.h>
#include <unistd.h>
#include "arm_robocol.h"

arm_st prueba_ph(ph_dev* devptr);
arm_st prueba_st(stp_device* devptr);
arm_st writeLog(char *title);
void stop_measure(int sig);

size_t size;
char *line,*title;
FILE *fdl;

int32_t position,status,config=0;
uint8_t step,ocd,tval;
uint8_t corr,debug;
int32_t buf,buf2;
double fbuf;
float		init;
float	elapsed;
long	 	CLK_TCK;

struct tms *ts;

uint8_t temp, vel,corr,est;
uint8_t pwmval;
int8_t steps=0;
uint8_t curr_pwm=0;
uint8_t i=0;

uint8_t medir = FALSE;

int main(int argc, char const *argv[]){

	CLK_TCK=sysconf(_SC_CLK_TCK);
	init=times(ts)/((double)CLK_TCK);

	stp_device* stepper;
	ph_dev* ph;
	uint8_t finished=0;
	size=40;
	line=malloc(size);
	title=malloc(size);


	//(void) signal(SIGINT, stop_measure);

	if(arm_build()){
		printf("Error al Construir el dispositivo brazo.\n");
	}

	if(arm_get_ph(BACTUATOR,&ph)){
		printf("Error en la captura del stp_device con el número %s asociado\n",*line);
	}
	printf("pinEnable: %d\n",(*ph).pin_enable);
	
	if(ph_setDireccion(ph,1)){
		printf("Error en la captura del stp_device con el número %s asociado\n",*line);
	}
	usleep(200000);
	if(ph_setEstado(ph,16)){
		printf("Error en la captura del stp_device con el número %s asociado\n",*line);
	}	

	if(arm_get_ph(UACTUATOR,&ph)){
		printf("Error en la captura del stp_device con el número %s asociado\n",*line);
	}
	printf("pinEnable: %d\n",(*ph).pin_enable);
	
	if(ph_setDireccion(ph,1)){
		printf("Error en la captura del stp_device con el número %s asociado\n",*line);
	}

	if(ph_setEstado(ph,16)){
		printf("Error en la captura del stp_device con el número %s asociado\n",*line);
	}


	if(arm_get_stp(SUP,&stepper)){
		printf("Error en la captura del stp_device con el número %s asociado\n",*line );
	}

	if(stp_master_disable(stepper)){
	printf("Error en Master Disable inicial\n");
	}

	if(stp_driver_enable(stepper)){
		printf("Error en driver enable inicial\n" );
	}

	if(stp_setStepSel(stepper,2)){
		printf("Error en setStepSel inicial\n" );
	}

	while(!finished){
	printf("Test de funcionamiento de Brazo (URC 2015-ROBOCOL).\n Utilice una de los siguientes comandos:\n"
			"\tstp\t\t\tSeleccionar el stepper que desea mover.\n"
			"\tph\t\t\tSeleccionar el puente h a movilizar.\n"
			"\texit\t\t\tSalir del programa\n");

		printf("Ingrese un comando\n");
		getline(&line,&size,stdin);
		printf("El comando ingresado fue: %s \n",line);
		if (!strcmp(line,"stp\n")){

			while(!finished){
				printf("Ingrese el número del stepper que desea utilizar:\n"
						"\t1\t\t\tMotor encargado de la supinación\n"
						"\t2\t\t\tMotor encargado de elevación de la garra\n"
						"\t3\t\t\tMotor encargado de la apertura de la garra\n"
						"\tbck\t\t\tRegresar al anterior menú.\n");
				printf("Ingrese un comando\n");
				getline(&line,&size,stdin);

				if (!strcmp(line,"1\n")){
					if(arm_get_stp(SUP,&stepper)){
						printf("Error en la captura del stp_device con el número %s asociado\n",*line );
					}
					printf("stndby: %d\n",(*stepper).pin_stndby);
					prueba_st(stepper);
				}else if(!strcmp(line,"2\n")){
					if(arm_get_stp(WRIST,&stepper)){
						printf("Error en la captura del stp_device con el número %s asociado\n",*line );
					}
					prueba_st(stepper);
				}else if(!strcmp(line,"3\n")){
					if(arm_get_stp(CLAW,&stepper)){
						printf("Error en la captura del stp_device con el número %s asociado\n",*line );
					}
					prueba_st(stepper);
				}else if(!strcmp(line,"bck\n")){
					finished=1;
				}else{
					printf("El comando ingresado no es válido. Intente de nuevo\n");
				}

			}
			finished=0;
		}else if(!strcmp(line,"ph\n")){
			while(!finished){
				printf("Ingrese el número del puente H que desea utilizar:\n"
						"\t1\t\t\tPuente H del actuador lineal inferior\n"
						"\t2\t\t\tPuente H del actuador lineal superior\n"
						"\t3\t\t\tMotor encargado del giro del brazo\n"
						"\tbck\t\t\tRegresar al anterior menú.\n");
				printf("Ingrese un comando\n");
				getline(&line,&size,stdin);

				if (!strcmp(line,"1\n")){
					if(arm_get_ph(BACTUATOR,&ph)){
						printf("Error en la captura del stp_device con el número %s asociado\n",*line);
					}
					printf("pinEnable: %d\n",(*ph).pin_enable);
					prueba_ph(ph);
				}else if(!strcmp(line,"2\n")){
					if(arm_get_ph(UACTUATOR,&ph)){
						printf("Error en la captura del stp_device con el número %s asociado\n",*line);
					}
					prueba_ph(ph);
				}else if(!strcmp(line,"3\n")){
					if(arm_get_ph(BMOTOR,&ph)){
						printf("Error en la captura del stp_device con el número %s asociado\n",*line);
					}
					prueba_ph(ph);
				}else if(!strcmp(line,"bck\n")){
					finished=1;
				}else{
					printf("El comando ingresado no es válido. Intente de nuevo\n");
				}
			}
			finished=0;
		}else if(!strcmp(line,"posA\n")){
			printf("Moviendo el brazo a la posición A...\n");
			sprintf(title,"To PosA");
			arm_moveToPos(posicionA);
			writeLog(title);

		}else if(!strcmp(line,"posB\n")){
			printf("Moviendo el brazo a la posición B...\n");
			sprintf(title,"To PosB");
			arm_moveToPos(posicionB);
			writeLog(title);

		}else if(!strcmp(line,"posC\n")){
			printf("Moviendo el brazo a la posición C...\n");
			sprintf(title,"To PosC");
			arm_moveToPos(posicionC);
			writeLog(title);

		}else if(!strcmp(line,"posD\n")){
			printf("Moviendo el brazo a la posición D...\n");
			sprintf(title,"To PosD");
			arm_moveToPos(posicionD);
			writeLog(title);

		}else if(!strcmp(line,"exit\n")){
			finished=1;
		}
	}
	return 0;
}





arm_st prueba_st(stp_device* devptr){
	build_expander((*devptr).exp);
	printf("pin_enable: %x\n",(*devptr).pin_stndby);
		printf("Test de funcionamiento de Stepper(ERC 2015-ROBOCOL).\n Utilice una de los siguientes comandos:\n" 
				"\t master-enable\t\t\t-Habilita el stepper\n"
				"\t en-clk\t\t\t\t-Habilita clk del stepper\n"
				"\t en-driver\t\t\t-Habilita diver del stepper\n"
				"\t en-out\t\t\t\t-Habilita salidas del stepper\n"
				"\t s\t\t\t\t-Deshabilita el stepper\n"
				"\t dis-clk\t\t\t-Deshabilita clk del stepper\n"
				"\t dis-driver\t\t\t-Deshabilita driver del stepper\n"
				"\t dis-out\t\t\t-Deshabilita salidas del stepper\n"
				"\t status\t\t\t\t-Imprime el STATUS register\n "
				"\t getconfig\t\t\t-Imprime el CONFIG register\n "
				"\t getstep\t\t\t-Imprime el estado actual del selector de paso\n"
				"\t getalarm\t\t\t-Imprime el registro de alarmas\n"
				"\t getpos\t\t\t-Imprime el registro posición absoluta\n"
				"\t getocdt\t\t\t-Imprime el Overcurrent Detection Threshold actual\n"
				"\t gettval\t\t\t-Retorna TVAL\n"
				"\t period\t\t\t-Cambia el periodo del step\n"
				"\t dir\t\t\t\t-Cambia la direccion de giro del stepper\n"
				"\t setconfig\t\t\t-Imprime el CONFIG register\n "
				"\t setstep\t\t\t-Asigna un valor al estado actual del selector de paso\n"
				"\t setpos\t\t\t-Asigna un valor al registro posición absoluta\n"
				"\t setocdt\t\t\t-Asigna un valor al Overcurrent Detection Threshold actual\n"
				"\t settval\t\t\t-Cambia TVAL\n"
				"\t debug\t\t\t\t-Permite rápido envia de getTVAL útil para econtrar fallas en comunicación SPI\n");
	while(1){
		printf("Ingrese un comando\n");
		getline(&line,&size,stdin);
		printf("El comando ingresado fue: %s \n",line);

		//Mediciones
		if(!strcmp(line,"status\n")){
			stp_getStatus(devptr,&status);
			printf("STATUS: %X \n",status);

		}else if(!strcmp(line,"getconfig\n")){
			stp_getConfig(devptr,&config);
			printf("Config: %X \n",config);

		}else if(!strcmp(line,"getstep\n")){
			stp_getStepSel(devptr,&step);
			printf("Step: %X \n",step);

		}else if(!strcmp(line,"getpos\n")){
			stp_getPosition(devptr,&position);
			printf("Posición: %d \n",position);

		}else if(!strcmp(line,"getocdt\n")){
			stp_getOCDT(devptr,&ocd);
			printf("OCD: %d \n",ocd);
		}else if(!strcmp(line,"gettval\n")){
			stp_getTVAL(devptr,&tval);
			printf("TVAL: %X \n",tval);
		//Setters
		}else if(!strcmp(line,"setstep\n")){
			printf("Ingrese el divisor del paso para paso de 1.8 grados:\n");
			getline(&line,&size,stdin);
			buf=atoi(line);
			printf("Cambiando divisor de paso a: %d \n",buf);
			if(stp_setStepSel(devptr,buf)){
				printf("Error en el cambio de divisor de paso.\n");

			}
		}else if(!strcmp(line,"period\n")){
			printf("Ingrese el periodo del paso en ns:\n");
			getline(&line,&size,stdin);
			buf=atoi(line);
			printf("Cambiando periodo a: %d \n",buf);
			stp_period(devptr,buf);
 
		}else if(!strcmp(line,"dir\n")){
			printf("Ingresela dirección: 0 - counterclockwise; 1 - clockwise:\n");
			getline(&line,&size,stdin);
			buf=atoi(line);
			printf("Cambiando direccipon a a: %d \n",buf);
			stp_dir(devptr,buf);
	
		}else if(!strcmp(line,"setpos\n")){
			printf("Ingrese el marcador de la posición actual:\n");
			getline(&line,&size,stdin);
			buf=atoi(line);
			printf("Cambiando posición a: %d \n",buf);
			stp_setPosition(devptr,buf);

		}else if(!strcmp(line,"settval\n")){
			printf("Ingrese el TVAL deseado:\n");
			getline(&line,&size,stdin);
			buf=atoi(line);
			printf("Cambiando TVAL a: %d \n",buf);
			stp_setTVal(devptr,buf);
		}else if(!strcmp(line,"settonmin\n")){
			printf("Ingrese el TONMIN deseado:\n");
			getline(&line,&size,stdin);
			buf=atoi(line);
			printf("Cambiando TONMIN a: %d \n",buf);
			stp_setTOnMin(devptr,buf);

		}else if(!strcmp(line,"settoffmin\n")){
			printf("Ingrese el TOFFMIN deseado:\n");
			getline(&line,&size,stdin);
			buf=atoi(line);
			printf("Cambiando TOFFMIN a: %d \n",buf);
			stp_setTOffMin(devptr,buf);

		}else if(!strcmp(line,"setocdt\n")){
			printf("Ingrese el OCDT deseado:\n");
			getline(&line,&size,stdin);
			buf=atoi(line);
			printf("Cambiando OCDT a: %d \n",buf);
			stp_setOCDT(devptr,buf);

		}else if(!strcmp(line,"ratio\n")){
			printf("Ingrese la nueva relación:\n");
			getline(&line,&size,stdin);
			fbuf=atof(line);
			printf("Cambiando relación a: %f \n",fbuf);
			(*devptr).gear_ratio=fbuf;

		}else if(!strcmp(line,"rel\n")){
			printf("Ingrese dirección:\n");
			getline(&line,&size,stdin);
			buf=atoi(line);
			printf("Ingrese grados hexadecimales:\n");
			getline(&line,&size,stdin);
			buf2=atoi(line);
			printf("Cambiando posición a: %d \n",buf);
			stp_moveRelAngle(devptr,buf2,buf);

		}else if(!strcmp(line,"master-enable\n")){
			printf("Habilitando stepper:\n");
			stp_master_enable(devptr);
		}else if(!strcmp(line,"en-clk\n")){
			printf("Habilitando clk del stepper:\n");
			stp_clk_enable(devptr);
	
		}else if(!strcmp(line,"en-driver\n")){
			printf("Habilitando driver del stepper:\n");
			stp_driver_enable(devptr);
		}else if(!strcmp(line,"en-out\n")){
			printf("Habilitando salidas del stepper:\n");
			stp_output_enable(devptr);
	
		}else if(!strcmp(line,"master-disable\n")){
			printf("Deshabilitando stepper:\n");
			stp_master_disable(devptr);
	
		}else if(!strcmp(line,"dis-clk\n")){
			printf("Deshabilitando clk del stepper:\n");
			stp_clk_disable(devptr);
	
		}else if(!strcmp(line,"dis-driver\n")){
			printf("Deshabilitando driver del stepper:\n");
			stp_driver_disable(devptr);
	
		}else if(!strcmp(line,"dis-out\n")){
			printf("Deshabilitando salidas del stepper:\n");
			stp_output_disable(devptr);
	
		}else if(!strcmp(line,"exit\n")){
			printf("Cerrando el programa. Adiós\n");
			stp_returnToZero(devptr);
			stp_master_disable(devptr);
			break;
		}else if(!strcmp(line,"debug\n")){
			while(getchar()!='q'){
				stp_getTVAL(devptr,&tval);
				printf("TVAL: %d \n",tval);
			}
		}else if(!strcmp(line,"s\n")){
			printf("Deshabilitando stepper:\n");
			stp_master_disable(devptr);
	
		}else if(!strcmp(line,"cero\n")){
			printf("Volviendo a casa:\n");
			stp_returnToZero(devptr);
		}else{
		printf("Test de funcionamiento de Stepper (ERC 2015-ROBOCOL).\n Utilice una de los siguientes comandos:\n" 
				"\t master-enable\t\t\t-Habilita el stepper\n"
				"\t en-clk\t\t\t\t-Habilita clk del stepper\n"
				"\t en-driver\t\t\t-Habilita diver del stepper\n"
				"\t en-out\t\t\t\t-Habilita salidas del stepper\n"
				"\t master-disable\t\t\t-Deshabilita el stepper\n"
				"\t dis-clk\t\t\t-Deshabilita clk del stepper\n"
				"\t dis-driver\t\t\t-Deshabilita driver del stepper\n"
				"\t dis-out\t\t\t-Deshabilita salidas del stepper\n"
				"\t status\t\t\t\t-Imprime el STATUS register\n "
				"\t getconfig\t\t\t-Imprime el CONFIG register\n "
				"\t getstep\t\t\t-Imprime el estado actual del selector de paso\n"
				"\t getalarm\t\t\t-Imprime el registro de alarmas\n"
				"\t getpos\t\t\t-Imprime el registro posición absoluta\n"
				"\t getocdt\t\t\t-Imprime el Overcurrent Detection Threshold actual\n"
				"\t gettval\t\t\t-Retorna TVAL\n"
				"\t period\t\t\t-Cambia el periodo del step\n"
				"\t dir\t\t\t\t-Cambia la direccion de giro del stepper\n"
				"\t setconfig\t\t\t-Imprime el CONFIG register\n "
				"\t setstep\t\t\t-Asigna un valor al estado actual del selector de paso\n"
				"\t setalarm\t\t\t-Asigna un valor al registro de alarmas\n"
				"\t setpos\t\t\t-Asigna un valor al registro posición absoluta\n"
				"\t setocdt\t\t\t-Asigna un valor al Overcurrent Detection Threshold actual\n"
				"\t settval\t\t\t-Cambia TVAL\n"
				"\t debug\t\t\t\t-Permite rápido envia de getTVAL útil para econtrar fallas en comunicación SPI\n");
		
		}
		printf("--------------------------------------------------------------\n");
	}	
}





arm_st prueba_ph(ph_dev* devptr){

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

arm_st writeLog(char *title){
	
	// uint8_t cor1,cor2,pwm1,pwm2,i;

	// if((fdl=fopen("brazo.log","a"))<0){
	// 	printf("Error en apertura de archivo para impresion de log\n");
	// 	perror("Causa:");
	// 	exit(1);
	// }

	// fprintf(fdl, title);
	// fprintf(fdl, "\n");
	
	// while(medir){
	// //for (i = 0; i < 200; ++i){
	// 	ph_getCorriente((*armdev).b_actuator,&cor1);
	// 	usleep(20000);
	// 	ph_getCorriente((*armdev).u_actuator,&cor2);
	// 	usleep(20000);

	// 	elapsed=times(ts)/((double)CLK_TCK)-init;

	// 	ph_getTemperatura((*armdev).b_actuator,&pwm1);
	// 	usleep(20000);
	// 	ph_getTemperatura((*armdev).u_actuator,&pwm2);
	// 	usleep(20000);

	// 	fprintf(fdl,"%.3f,%d,%d,%d,%d\n",elapsed,cor1,pwm1,cor2,pwm2);
	// 	sleep(4);
	// }
	
	// fclose(fdl);
}

void stop_measure(int sig){
	if(medir){
		medir=FALSE;
	}else{
		abort();
	}
}