#ifndef ADC_ROBOCOL_H
#define ADC_ROBOCOL_H
#include "gpio_robocol.h"
static const uint8_t ADC_ERROR=1;
static const uint8_t ADC_OK=0;

static const uint8_t AD0 =37;
static const uint8_t AD1 =36;
static const uint8_t AD2 =23;
static const uint8_t AD3 =22;
static const uint8_t AD4 =21;
static const uint8_t AD5 =20;
static const uint8_t GPIO29 =29;


typedef struct adc_dev{
	uint8_t adc_num;
	uint16_t* data;
	int fd;
}adc_dev;

typedef uint8_t adc_st;
adc_st build_adc(adc_dev* devptr);
adc_st get_adc_value(adc_dev* devptr);
adc_st digital_filter(void);
adc_st parse_adc_gpio(uint8_t adc_num ,uint8_t* gpio_adc );
adc_st parse_adc_route(uint8_t adc_num ,char* route_adc );
adc_st close_adc(adc_dev* devptr);

#endif 