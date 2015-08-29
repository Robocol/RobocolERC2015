#include "adc_robocol.h"

int main(int argc, char const *argv[])
{
	size_t size=40;
	uint8_t selected=0;
	float* voltage=malloc(sizeof(float));
	char* line=malloc(size);
	uint16_t* data=malloc(sizeof(uint16_t));
	adc_dev* dev=malloc(sizeof(adc_dev));
	(*dev).data=data;
	(*dev).v_ref=5;
	while(!selected){
		printf("Bienvenido al test de funcionamiento de ADC (ERC 2015-ROBOCOL).\n Seleccione el ADC que desea utilizar:\n" 
				"\t 0\t\t\tPara seleccionar el adc 0\n"
				"\t 1\t\t\tPara seleccionar el adc 1\n"
				"\t 2\t\t\tPara seleccionar el adc 2\n"
				"\t 3\t\t\tPara seleccionar el adc 3\n"
				"\t 4\t\t\tPara seleccionar el adc 4\n"
				"\t 5\t\t\tPara seleccionar el adc 5\n");
		getline(&line,&size,stdin);
		printf("Comando ingresado: %s\n",line );

		if (!strcmp(line,"0\n")){
			(*dev).adc_num=0;
			selected=1;
		}else if(!strcmp(line,"1\n")){
			(*dev).adc_num=1;
			selected=1;
		}else if(!strcmp(line,"2\n")){
			(*dev).adc_num=2;
			selected=1;
		}else if(!strcmp(line,"3\n")){
			(*dev).adc_num=3;
			selected=1;		
		}else if(!strcmp(line,"4\n")){
			(*dev).adc_num=4;
			selected=1;		
		}else if(!strcmp(line,"5\n")){
			(*dev).adc_num=5;
			selected=1;		
		}else{
			printf("Comando inválido, ingrese uno de los comandos listas\n");
		}
	}

	build_adc(dev);
	while(1){
		get_voltage(dev,voltage);
		
		printf("Valor(V): %f\n",*(voltage));	/* code */

		sleep(1);
	}
	return 0;
}