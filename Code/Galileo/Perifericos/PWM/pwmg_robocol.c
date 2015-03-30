#include "pwmg_robocol.h"

pwm_st pwm_build(uint8_t pwm_n,uint32_t period, uint8_t duty){
	pwm_set_period(pwm_n,period);
	pwm_set_duty(pwn_n,duty);
}

pwm_st pwm_enable(uint8_t pwm_n){
	char* route=malloc(35);
	char* str_pwm=malloc(1);
	char* enable=malloc(1);
	if(pwm_n>7 || pwm_n==2 || pwm_n<1 ){
		printf("El número de pwm ingresado no es valido.\nRealice revisión de documentación de Galileo Gen 1 para observar los valores válidos\n");
		return PWM_ERROR;
	}
	str_pwm=u8toa(pwm_n);
	enable=u8toa(ENABLE);
	sprintf(route, "/sys/class/pwm/pwmchip0/pwm%d/enable", pwm_n);

	pwm_export(str_pwm);
	g_write_file(route,enable,1);
	free(route);
	free(enable);
	pwm_unexport(str_pwm);
	free(str_pwm);
	return PWM_OK;
}

pwm_st pwm_disable(uint8_t pwm_n){
	char* route=malloc(35);
	char* str_pwm=malloc(1);
	char* disable=malloc(1);
	if(pwm_n>7 || pwm_n==2 || pwm_n<1 ){
		printf("El número de pwm ingresado no es valido.\nRealice revisión de documentación de Galileo Gen 1 para observar los valores válidos\n");
		return PWM_ERROR;
	}

	str_pwm=u8toa(pwm_n);
	disable=u8toa(DISABLE);
	sprintf(route, "/sys/class/pwm/pwmchip0/pwm%d/enable", pwm_n);

	pwm_export(str_pwm);
	g_write_file(route,disable,1);
	free(route);
	free(disable);
	pwm_unexport(str_pwm);
	free(str_pwm);
	return PWM_OK;
}



pwm_st pwm_set_duty(uint8_t pwm_n, uint8_t duty){
	char* str_pwm=u8toa(pwm_n);
	uint32_t period;
	pwm_dev* pwm;

	if(pwm_n>7 || pwm_n==2 || pwm_n<1 ){
		printf("El número de pwm ingresado no es valido.\nRealice revisión de documentación de Galileo Gen 1 para observar los valores válidos\n");
		return PWM_ERROR;
	}
	
	if(duty<0 || duty>100){
		printf("El número de duty cycle ingresado no es valido.\nIngrese un valor entre 0 y 100\n");
		return PWM_ERROR;
	}
	
	if((pwm=pwn_get_dev(pwm_n))==NULL){
		printf("No ha inicializado el dispositivo pwm. Se iniciliza el dispositivo con frecuencia por defecto de 200Hz.\n");
		period=5000000;
		*pwm={period,duty};
	}else{
		period=(*pwm).period;
		(*pwm).duty=duty;
	}
	uint32_t duty_ns=(uint32_t)(period*duty/100);
	char* str_duty=u32toa(duty_ns);
	uint8_t duty_len=getu32_len(duty_ns);

	// Se construye la ruta para la escritura del duty cycle
	char* route=malloc(39);
	sprintf(route,"/sys/class/pwm/pwmchip0/pwm%d/duty_cycle",pwm_n);

	pwm_export(str_pwm);
	g_write_file(route,str_duty,duty_len);
	free(str_duty);
	free(route);
	pwm_unexport(str_pwm);
	free(str_pwm);
	return PWM_OK;
}

pwm_st pwm_set_period(uint8_t pwm_n, uint32_t period){
	char* str_pwm=malloc(1);
	pwm_dev* pwm;

	if(pwm_n>7 || pwm_n==2 || pwm_n<1 ){
		printf("El número de pwm ingresado no es valido.\nRealice revisión de documentación de Galileo Gen 1 para observar los valores válidos\n");
		return PWM_ERROR;
	}
	str_pwm=u8toa(pwm_n);
	if(period<0 || period>1000000){
		printf("El número de periodo ingresado no es valido.\nIngrese un valor entre 0 y 1000000\n");
		return PWM_ERROR;
	}

	if((pwm=pwn_get_dev(pwm_n))==NULL){
		printf("No ha inicializado el dispositivo pwm. Se iniciliza el dispositivo con frecuencia por defecto de 200Hz.\n");
		*pwm={period,0};
	}else{
		(*pwm).period=period;
	}

	char* route=malloc(35);
	sprintf(route,"/sys/class/pwm/pwmchip0/pwm%d/period",pwm_n);
	char* str_period=u32toa(period);
	uint8_t period_len=getu32_len(period);
	pwm_export(str_pwm);
	g_write_file(route,str_period,period_len);
	free(str_duty);
	free(route);
	pwm_unexport(str_pwm);
	free(str_pwm);
	return PWM_OK;
}

static void pwm_export(char* str_num ){
	g_write_file("/sys/class/pwm/pwmchip0/export",str_num,1);
}
static void pwm_unexport(char* str_num){
	g_write_file("/sys/class/pwm/pwmchip0/unexport",str_num,1);
}

uint8_t getu32_len(uint32_t uint32){
	if (uint32<10){
		return  (uint8_t)1;
	}else if(uint32<100){
		return  (uint8_t)2;
	}else if(uint32<1000){
		return  (uint8_t)3;
	}else if(uint32<10000){
		return  (uint8_t)4;
	}else if(uint32<100000){
		return  (uint8_t)5;
	}else if(uint32<1000000){
		return  (uint8_t)6;
	}else if(uint32<10000000){
		return  (uint8_t)7;
	}else if(uint32<100000000){
		return  (uint8_t)8;
	}else if(uint32<1000000000){
		return  (uint8_t)9;
	}else if(uint32<4294967296){
		return  (uint8_t)10;
	}else{
		return PWM_ERROR;
	}
}

pwm_dev* pwm_get_dev(uint8_t pwm_num){
	switch ( pwm_num) {
	case 3:
	  return pwms.pwm3;
	case 5:
	  return pwms.pwm5;
	case 6:
	  return pwms.pwm6;
	case 1:
	  return pwms.pwm1;
	case 7:
	  return pwms.pwm7;
	case 4:
	  return pwms.pwm4;	  	 	  	
	default:
	  printf("El pwm_num ingresado a pwm_get_dev en libreria de pwmg_robocl no es un numero válido de pwm.\n");
	  return PWM_ERROR;
	}
}