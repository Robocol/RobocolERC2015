#include "pwmg_robocl.h"

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
	char* str_pwm=malloc(1);
	uint8_t duty_len=2;
	if(pwm_n>7 || pwm_n==2 || pwm_n<1 ){
		printf("El número de pwm ingresado no es valido.\nRealice revisión de documentación de Galileo Gen 1 para observar los valores válidos\n");
		return PWM_ERROR;
	}
	str_pwm=u8toa(pwm_n);
	if(duty<0 || duty>100){
		printf("El número de duty cycle ingresado no es valido.\nIngrese un valor entre 0 y 100\n");
		return PWM_ERROR;
	}else{
		if(duty<10){
			duty_len=1;
		}else if(duty==100){
			duty_len=3;
		}
	}
	char* route=malloc(49);
	sprintf(route,"/sys/class/pwm/pwmchip0/pwm%d/duty_cycle",pwm_n);
	char* str_duty=u8toa(duty);
	pwm_export(str_pwm);
	g_write_file(route,str_duty_len);
	free(str_duty);
	free(str_duty);
	pwm_unexport(str_pwm);
	free(str_pwm);
	return PWM_OK;
}

pwm_st pwm_set_period(pwm_dev* dev_ptr, uint32_t period){
	char* str_pwm=malloc(1);
	uint8_t period_len=2;
	if(pwm_n>7 || pwm_n==2 || pwm_n<1 ){
		printf("El número de pwm ingresado no es valido.\nRealice revisión de documentación de Galileo Gen 1 para observar los valores válidos\n");
		return PWM_ERROR;
	}
	str_pwm=u8toa(pwm_n);
	if(period<0 || duty>100){
		printf("El número de duty cycle ingresado no es valido.\nIngrese un valor entre 0 y 100\n");
		return PWM_ERROR;
	}else{
		if(duty<10){
			duty_len=1;
		}else if(duty==100){
			duty_len=3;
		}
	}
	char* route=malloc(48+duty_len);
	sprintf(route,"/sys/class/pwm/pwmchip0/pwm%d/period",pwm_n);
	char* str_duty=u8toa(duty);
	pwm_export(str_pwm);
	g_write_file(route,str_duty_len);
	free(str_duty);
	free(str_duty);
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