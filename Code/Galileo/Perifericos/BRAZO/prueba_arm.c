#include "arm_robocol.h"

arm_st prueba_ph(ph_dev* dev);
arm_st prueba_st(stp_device* dev);

int main(int argc, char const *argv[]){

stp_device* stepper;
ph_dev* ph;
uint8_t finished=0;
size_t size=40;
char* line=malloc(size);


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
				if(arm_get_st(SUP,stepper)){
					printf("Error en la captura del stp_device con el número %s asociado\n",*line );
				}
				prueba_st(stepper);
			}else if(!strcmp(line,"2\n")){
				if(arm_get_st(WRIST,stepper)){
					printf("Error en la captura del stp_device con el número %s asociado\n",*line );
				}
				prueba_st(stepper);
			}else if(!strcmp(line,"3\n")){
				if(arm_get_st(CLAW,stepper)){
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
				if(arm_get_st(BACTUATOR,ph)){
					printf("Error en la captura del stp_device con el número %s asociado\n",*line );
				}
				prueba_ph(ph);
			}else if(!strcmp(line,"2\n")){
				if(arm_get_st(UACTUATOR,ph)){
					printf("Error en la captura del stp_device con el número %s asociado\n",*line );
				}
				prueba_ph(ph);
			}else if(!strcmp(line,"3\n")){
				if(arm_get_st(BMOTOR,ph)){
					printf("Error en la captura del stp_device con el número %s asociado\n",*line );
				}
				prueba_ph(ph);
			}else if(!strcmp(line,"bck\n")){
				finished=1;
			}else{
				printf("El comando ingresado no es válido. Intente de nuevo\n");
			}

		}
		finished=0;
	}else if(!strcmp(line,"exit\n")){
		finished=1;
	}
}
return 0;
}