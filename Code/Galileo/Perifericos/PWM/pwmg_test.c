#include "pwmg_robocol.h"


int main(int argc, char const *argv[]){
	int size =10;
	char* line=malloc(size);
	uint8_t end=0;
	uint8_t pwm_n=0;
	uint32_t periodo=0;
	uint8_t tfin=0;
	uint8_t duty=0;
	if(argc>1){
		printf("Uso %s \n",argv[0]);
		return 1;
	}

	printf("Bienvenido al test de funcionamiento de PWM(URC 2015-ROBOCOL).\nPor favor ingrese el numero del pwm sobre el cual desea trabajar\n");
	getline(&line,&size,stdin);
	if((pwm_n=(uint8_t)atoi(line))<0){
		printf("El valor ingresado no es un numero valido\n");
		return 1;
	}
	printf("El valor ingreado fue %d. Se trabajara con dicho valor de ahora en adelante\n",pwm_n );
	while(!end){
		printf("Ingrese uno de los siguientes comandos para actuar sobre pwm%d:\n" 
		"\t b\t Construye el dispositivo pwm\n"
		"\t sp\t Modificacion del periodo del pwm\n"
		"\t sd\t Modificacion duty_cycle\n"
		"\t e\t Habilita el dispositivo\n"
		"\t d\t Desabilita el dispositivo\n"
		"\t f\t Finalizar la operacion\n",pwm_n);
		getline(&line,&size,stdin);

		if (!strcmp(line,"b\n")){
			while(!tfin){
				printf("Ingrese el periodo deseado:\n\tperiodo:");
				getline(&line,&size,stdin);
				if((periodo=(uint32_t)atoi(line))<0){
					printf("Valor numerico no valido.\n");
				}else{
					while(!tfin){
						printf("Ingrese el duty_cycle en porcentaje deseado:\n\tduty:");
						getline(&line,&size,stdin);
						if((duty=(uint8_t)atoi(line))<0){
							printf("Valor numerico no valido.\n");								
						}else{
							if(pwm_build(pwm_n,periodo,duty)!=0){
								printf("Error en la construccion del dispositivo pwm\n");
								return 1;
							}else{
								printf("Transaccion exitosa. Construccion del pwm\n");
								tfin=1;									
							}
						}
					}
				}
			}
			tfin=0;

		}else if (!strcmp(line,"sp\n")){
			while(!tfin){
				printf("Ingrese el periodo deseado:\n\tperiodo:");
				getline(&line,&size,stdin);
				if((periodo=(uint32_t)atoi(line))<0){
					printf("Valor numerico no valido.\n");
				}else{
					if(pwm_set_period(pwm_n,periodo)!=0){
						printf("Error en el set del periodo\n");
						return 1;
					}else{
						printf("Transaccion exitosa: periodo=%d\n",periodo);
						tfin=1;
					}
				}
			}
			tfin=0;			

		}else if (!strcmp(line,"sd\n")){
			while(!tfin){
				printf("Ingrese el duty_cycle deseado:\n\tduty:");
				getline(&line,&size,stdin);
				if((duty=(uint8_t)atoi(line))<0){
					printf("Valor numerico no valido.\n");
				}else{
					if(pwm_set_duty(pwm_n,duty)!=0){
						printf("Error en el set del duty_cycle\n");
						return 1;
					}else{
						printf("Transaccion exitosa: duty_cycle=%d\n",duty);
						tfin=1;													
					}
				}
			}
			tfin=0;			
		}else if (!strcmp(line,"e\n")){
			if(pwm_enable(pwm_n)!=0){
				printf("Error en el enable del pwm\n");
				return 1;
			}else{
				printf("Transaccion exitosa. pwm enable\n");
			}
		}else if (!strcmp(line,"d\n")){
			if(pwm_disable(pwm_n)!=0){
				printf("Error en el enable del pwm\n");
				return 1;
			}else{
				printf("Transaccion exitosa. pwm disable\n");
			}
		}else if (!strcmp(line,"f\n")){
			printf("Finalizando programa...\n");
			return 0;
		}else{
			printf("Comando no reconocido.\n");
		}
	}
	return 0;
}
