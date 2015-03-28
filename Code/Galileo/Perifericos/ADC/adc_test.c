#include "adc_robocol.h"
int main(int argc, char const *argv[])
{
	uint16_t* data=malloc(sizeof(uint16_t));
	adc_dev dev={2,data,0};
	adc_dev* devptr=&dev;
	build_adc(devptr);
	while(1){
		get_adc_value(devptr);
		printf("Valor Adc: %d\n",*((*devptr).data));	/* code */
	}
	return 0;
}