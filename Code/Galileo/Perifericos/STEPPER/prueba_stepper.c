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

if(stp_master_disable(devptr)){
	printf("Error en Master Disable inicial\n");
}


if(stp_driver_enable(devptr)){
	printf("Error en driver enable inicial\n" );
}


if(stp_setStepSel(devptr,0)){
	printf("Error en setStepSel inicial\n" );
}

printf("------------------Motor de pasos configurado------------------\n");
		printf("Bienvenido al test de funcionamiento de Stepper(ERC 2015-ROBOCOL).\n Utilice una de los siguientes comandos:\n" 
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

		}else if(!strcmp(line,"getalarm\n")){
			stp_getAlarmEn(devptr,&alarm);
			printf("Alarm: %X \n",alarm);

		}else if(!strcmp(line,"getstep\n")){
			stp_getStepSel(devptr,&step);
			printf("Step: %X \n",step);

		}else if(!strcmp(line,"getpos\n")){
			stp_getPosition(devptr,&position);
			printf("Posición: %X \n",position);

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
			printf("Ingresela dirección: 0 - clockwise; 1 - counterclockwise:\n");
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
			break;
		}else if(!strcmp(line,"debug\n")){
			while(getchar()!='q'){
				stp_getTVAL(devptr,&tval);
				printf("TVAL: %d \n",tval);
			}
		}else{
		printf("Bienvenido al test de funcionamiento de Puente H (ERC 2015-ROBOCOL).\n Utilice una de los siguientes comandos:\n" 
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