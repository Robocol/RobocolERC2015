#include <string.h>
#include "ph_robocol.h"

size_t size=40;						//Tamaño del buffer para nueavas lineas
char* line=malloc(size);			//Puntero al buffer de linea
uint8_t temp,vel,corr,est,debug;	//Buffers para los diferentes mediciones
uint8_t addr=0b0111000;				//Direccion expansor I2C
ph_dev* devptr;						//puntero a puente h
int buf;							//buff para envío de mensajes

int main(){

temp, vel, corr, est, debug=0x5A;	

printf("----------Prueba Driver Puente H----------\n");
spi_device com1;					//Canal de comunicación del primer puente h
ph_dev dev1={&com1,0,1,4};			//Creación de primer puente h con pines ina=0, inb=1, enable=4

spi_device com2;					//Canal de comunicación del segundo puente h
ph_dev dev2={&com2,2,3,4};			//Creación de segundo puente h con pines ina=2, inb=3, enable=4

build_expander(addr);

spi_start("/dev/spidev1.0",100000);	//Inicialización de SPI
spi_create_device(&com1,0,PINA0);	//Instanciando SPI del primer puente h con CS en el pi A0
spi_create_device(&com2,0,PINA1);	//Instanciando SPI del segundo puente h con CS en el pi A1

devptr=&dev1;				//Inicialmente el puntero se asigna al primer puente h


	//Instrucciones de uso para el usuario
		printf("Bienvenido al test de funcionamiento de Puente H (URC 2015-ROBOCOL).\n Utilice una de los siguientes comandos:\n" 
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
				"\t debug\t\t\t-Permite rápido envia de CAMBIAR_PWM con un valor de 0x5A\n");

while(1){

		printf("Ingrese un comando\n");
		getline(&line,&size,stdin);			//Lectura de comando
		printf("El comando ingresado fue: %s \n",line);

		//Mediciones
		if(!strcmp(line,"medir-temperatura\n")){
			getTemperatura(devptr,&temp);
			printf("Temperatura: %d \n",temp);

		}else if(!strcmp(line,"medir-velocidad\n")){
			getTemperatura(devptr,&vel);
			printf("Velocidad: %d \n",vel);

		}else if(!strcmp(line,"medir-corriente\n")){
			getCorriente(devptr,&corr);
			printf("Corriente: %d \n",corr);

		}else if(!strcmp(line,"dar-estado\n")){
			getEstado(devptr,&est);
			printf("Estado: %d \n",est);
		
		//Setters
		}else if(!strcmp(line,"cambiar-pwm\n")){
			printf("Ingrese el PWM deseado:\n");
			getline(&line,&size,stdin);
			buf=atoi(line);
			printf("Cambiando PWM a: %d \n",buf);
			setPWM(devptr,buf);
 
		}else if(!strcmp(line,"cambiar-direccion\n")){
			printf("Ingrese la direccion deseada ( 1 o 0):\n");
			getline(&line,&size,stdin);
			buf=atoi(line);
			printf("Cambiando direccion a: %d \n",buf);
			setDireccion(devptr,buf);

		}else if(!strcmp(line,"cambiar-velocidad\n")){
			printf("Ingrese la velocidad deseada:\n");
			getline(&line,&size,stdin);
			buf=atoi(line);
			printf("Cambiando velocidad a: %d \n",buf);
			setVel(devptr,buf);

		}else if(!strcmp(line,"cambiar-estado\n")){
			printf("Ingrese el estado deseado:\n");
			getline(&line,&size,stdin);
			buf=atoi(line);
			printf("Cambiando estado a: %d \n",buf);
			setEstado(devptr,buf);

		}else if(!strcmp(line,"cambiar-kpv\n")){
			printf("Ingrese el KPV deseado:\n");
			getline(&line,&size,stdin);
			buf=atoi(line);
			printf("Cambiando KPV a: %d \n",buf);
			setKPV(devptr,buf);

		}else if(!strcmp(line,"cambiar-kiv\n")){
			printf("Ingrese el KIV deseado:\n");
			getline(&line,&size,stdin);
			buf=atoi(line);
			printf("Cambiando KIV a: %d \n",buf);
			setKIV(devptr,buf);

		}else if(!strcmp(line,"cambiar-kpc\n")){
			printf("Ingrese el KPC deseado:\n");
			getline(&line,&size,stdin);
			buf=atoi(line);
			printf("Cambiando KPC a: %d \n",buf);
			setKPC(devptr,buf);

		}else if(!strcmp(line,"cambiar-kic\n")){
			printf("Ingrese el KIC deseado:\n");
			getline(&line,&size,stdin);
			buf=atoi(line);
			printf("Cambiando KIC a: %d \n",buf);
			setKIC(devptr,buf);

		}else if(!strcmp(line,"enable\n")){
			printf("Ingrese 1 para activar, 0 para desactivar:\n");
			getline(&line,&size,stdin);
			buf=atoi(line);
			printf("Cambiando enable a: %d \n",buf);
			ph_enable(dev,buf);
		//Cambio de puente h asignado al puntero
		}else if(!strcmp(line,"cambiar-motor\n")){
			printf("Ingrese 1 para Motor1, 2 para Motor2:\n");
			getline(&line,&size,stdin);
			buf=atoi(line);
			printf("Cambiando Motor a: %d \n",buf);
			if(buf==1){
				devptr=&dev1;
				printf("%d\n", (*devptr).pin_dir_b);
			}else{
				devptr=&dev2;
				printf("%d\n", (*devptr).pin_dir_b);
			}
		//Salida del programa
		}else if(!strcmp(line,"exit\n")){
			printf("Cerrando el programa. Adiós\n");
			break;
		//Debug, permite enviar mensajes SPI rápido para observar más facilmente la señal
		}else if(!strcmp(line,"debug\n")){
			while(getchar()!='q'){
				printf("%c\n",debug);
				setPWM(devptr,debug);
			}
		//Instrucciones para el usuario en caso de comando incorrecto
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