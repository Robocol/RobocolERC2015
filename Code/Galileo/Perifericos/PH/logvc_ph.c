#include "ph_robocol.h"
int main(int argc, char const *argv[])
{
	FILE *fdl;
	size_t size=40;
	char* line=malloc(size);
	uint8_t medicion=1;
	uint8_t motor=1;
	char* nombref=malloc(13);
	ph_dev* devptr1;						//puntero a puente h
	ph_dev* devptr2;						//puntero a puente h
	ph_dev dev1;
	ph_dev dev2;

	uint8_t* buffmed1;
	uint32_t counter=0;

	dev1.pin_cs=PINA0;		
	dev1.pin_in_a=0;
	dev1.pin_in_b=1;
	dev1.pin_enable=4;
	dev1.pwm=0;
	dev1.addr=0b0111000;				//Creación de segundo puente h con pines ina=2, inb=3, enable=4
	devptr1=&dev1;						//Inicialmente el puntero se asigna al segundo puente h


	dev2.pin_cs=PINA1;		
	dev2.pin_in_a=2;
	dev2.pin_in_b=3;
	dev2.pin_enable=4;
	dev2.pwm=0;
	dev2.addr=0b0111000;				//Creación de segundo puente h con pines ina=2, inb=3, enable=4
	devptr2=&dev2;						//Inicialmente el puntero se asigna al segundo puente h

	printf("Iniciando creación de puenteH\n");

	if(ph_build(devptr1)){
		printf("Error en la creación de puenteH 2 \n");
		return 1;
	}
	if(ph_build(devptr2)){
		printf("Error en la creación de puenteH 2 \n");
		return 1;
	}

	printf("Bienvenido al menu de medicion de corriente y velocidad de puentes H(ERC 2015-ROBOCOL).\n Utilice una de los siguientes comandos:\n" 
			"\t motor\n"
			"\t cambiar-medicion\n"
			"\t iniciar\n");


	while(1){

			printf("Ingrese un comando\n");
			getline(&line,&size,stdin);
			printf("El comando ingresado fue: %s \n",line);

			//Mediciones
			if(!strcmp(line,"motor\n")){
				printf("Ingrese 1 o 2 segun el puente h que desea seleccionar\n");
				getline(&line,&size,stdin);
				printf("El comando ingresado fue: %s \n",line);	
				if(!strcmp(line,"1\n")){
					motor=1;
					printf("Configuración de motor exitosa\n");
				}else if(!strcmp(line,"2\n")){
					motor=2;
					printf("Configuración de motor exitosa\n");
				}
				else{
					printf("El numero de motor ingresdo por parámtro no es válido\n");
				}
			}else if(!strcmp(line,"cambiar-medicion\n")){
				printf("Ingrese 1 para corriente o 2 para velocidad\n");
				getline(&line,&size,stdin);
				printf("El comando ingresado fue: %s \n",line);	
				if(!strcmp(line,"1\n")){
					medicion=MEDIR_CORRIENTE;
					printf("Configuración de medicion exitosa\n");
				}else if(!strcmp(line,"2\n")){
					medicion=MEDIR_VELOCIDAD;
					printf("Configuración de medicion exitosa\n");
				}
				else{
					printf("El numero de medición ingresado por parámtro no es válido\n");
				}
			}else if(!strcmp(line,"iniciar\n")){

				printf("Iniciando Medicion\n");

				if (medicion==1)
				{
					printf("Medicion de corriente\n");
					sprintf(nombref,"corriente.log");
				}else if(medicion==2){
					printf("Medicion de velocidad\n");
					sprintf(nombref,"velocidad.log");
				}else{
					printf("La medición registrada para este momento no es válida\n");
					return 1;
				}

				printf("Realizando creación de archivo para registro de medicion\n");
				if((fdl=fopen(nombref,"w+"))<0){
					printf("Error en apertura de archivo para impresion de log\n");
					perror("Causa:");
					exit(1);
				}
				printf("Creación de archivo exitosa\n");

				if(motor==1){

					printf("----Medicion para puente H 1----\n");
					
					// ph_dev dev1={PINA0,0,1,4};			//Creación de primer puente h con pines ina=0, inb=1, enable=4
					// dev1.addr=0b0111000;
					// devptr1=&dev1;				//Inicialmente el puntero se asigna al primer puente h
					// printf("Iniciando creación de puenteH\n");
					
					// if(ph_build(devptr1)){
					// 	printf("Error en la creación de puenteH 1 \n");
					// 	return 1;
					// }	
					fprintf(fdl, "//--------PUENTE H MOTOR1--------\\\\\n");			
					fprintf(fdl, "//--------%s--------\\\\\n",nombref);
					free(nombref);

					if (medicion==1)
					{
						printf("Dentro del loop de medicion\n");
						while(1){
							ph_getCorriente(devptr1,buffmed1);
							fprintf(fdl, "Corriente%d: %d \n",counter,*buffmed1);
							counter++;
						}
					}else{
						printf("Dentro del loop de medicion\n");
						while(1){
							ph_getVelocidad(devptr1,buffmed1);
							fprintf(fdl, "Velocidad%d:%d \n",counter,*buffmed1);
							counter++;
						}
					}			

				}else if(motor==2){

					printf("----Medicion para puenteH 2----\n");


					fprintf(fdl, "//--------PUENTE H MOTOR2--------\\\\\n");
					fprintf(fdl, "//--------%s--------\\\\\n",nombref);

					free(nombref);

					if (medicion==1)
					{
						printf("Dentro del loop de medición\n");
						while(1){
							ph_getCorriente(devptr2,buffmed1);
							fprintf(fdl, "Corriente: %d \n",counter,*buffmed1);
							counter++;
						}
					}else{
						printf("Dentro del loop de medición\n");
						while(1){
							ph_getVelocidad(devptr2,buffmed1);
							fprintf(fdl, "Velocidad%d:%d \n",counter,*buffmed1);
							counter++;
						}
					}				
				}
			}else{
			printf("Bienvenido al menu de medicion de corriente y velocidad de puentes H(ERC 2015-ROBOCOL).\n Utilice una de los siguientes comandos:\n" 
					"\t motor\t\t\tCambiar Motor(1 por default)\n"
					"\t cambiar-medicion\t\t\t0 para corriente.\t1 para velocidad\n(medicion en 0 por default)");
			printf("--------------------------------------------------------------\n");
		}

	}

	return 0;

}