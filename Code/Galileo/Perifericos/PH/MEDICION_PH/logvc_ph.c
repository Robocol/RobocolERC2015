#include "ph_robocol.h"
#define CORR 1
#define VEL 2
int main(int argc, char const *argv[])
{
	FILE *fdl;
	uint8_t medicion=0;
	char* nombref=malloc(13);
	ph_dev* devptr1;						//puntero a puente h
	ph_dev* devptr2;						//puntero a puente h
	uint8_t addr=0b0111000;				//Direccion expansor I2C
	uint8_t* buffmed1;
	uint8_t* buffmed2;


	if (argc!=2){
		fprintf(stderr,"Uso:%s <MEDICION>\nMEDICION:\n  corriente\n  velocidad",argv[0] );
		exit(1);
	}else{
		if(strcmp(argv[1],"corriente")){
			medicion=CORR;
		}else if(strcmp(argv[1],"velocidad")){
			medicion=VEL;
		}else{
			fprintf(stderr,"Ingrese un valor valido para <MEDICION>\nVALORES VALIDOS:\n  corriente\n  velocidad");
			exit(1);
		}
	}

	if (medicion==1)
	{
		sprintf(nombref,"corriente.log");
	}else{
		sprintf(nombref,"velocidad.log");
	}

	if((fdl=fopen(nombref,"w+"))<0){
		printf("Error en apertura de archivo para impresion de log\n");
		perror("Causa:");
		exit(1);
	}
	ph_dev dev1={PINA0,0,1,4};			//Creación de primer puente h con pines ina=0, inb=1, enable=4
	ph_dev dev2={PINA1,2,3,4};			//Creación de segundo puente h con pines ina=2, inb=3, enable=4

	devptr1=&dev1;				//Inicialmente el puntero se asigna al primer puente h
	devptr2=&dev2;				//Inicialmente el puntero se asigna al segundo puente h

	ph_build(devptr1);
	ph_build(devptr2);

	fprintf(fdl, "//--------%s--------\\\\\n",nombref);
	free(nombref);

	if (medicion==1)
	{
		while(1){
			getCorriente(devptr1,buffmed1);
			getCorriente(devptr2,buffmed2);
			fprintf(fdl, "Motor1: %d .... Motor2: %d \n",*buffmed1,*buffmed2);
		}
	}else{
		while(1){
			getVelocidad(devptr1,buffmed1);
			getVelocidad(devptr2,buffmed2);
			fprintf(fdl, "Motor1: %d .... Motor2: %d \n",*buffmed1,*buffmed2);
		}
	}

	return 0;
}