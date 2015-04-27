#include "pwmg_robocol.h"
static volatile uint32_t periods[6];
static volatile uint8_t duties[6];
static volatile uint8_t initpwm[6];

pwm_st pwm_build(uint8_t pwm_n,uint32_t period, uint8_t duty){
	int8_t pwm;
	if((pwm=pwm_get_dev(pwm_n))<0){
		printf("Error en la obtencion de dispositivo\n");
		return PWM_ERROR;
	}

	initpwm[pwm]=1;

	if(pwm_set_period(pwm_n,period)!=0){
		initpwm[pwm]=1;
		return PWM_ERROR;
	}

	if(pwm_set_duty(pwm_n,duty)!=0){
		initpwm[pwm]=0;
		return PWM_ERROR;
	}

	return PWM_OK;
}

pwm_st pwm_enable(uint8_t pwm_n){
	printf("%s\n");
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

	printf("Exporting pwm...\n");
	if(pwm_export(str_pwm)){
		printf("Error en  export durante habilitación de pwm %s (pwmg_robocol.c)\n",str_pwm);
	}

	printf("Escribiendo enable en archivo de manejo de pwm\n");
	if(g_write_file(route,enable,1)){
		printf("Error en la escritura del enable de pwm %s \n",str_pwm);
		pwm_unexport(str_pwm);
		return PWM_ERROR;
	}
	free(route);
	free(enable);
	
	printf("Unexporting pwm...\n");
	if(pwm_unexport(str_pwm)!=0){
		printf("Error en  unexport durante habilitación de pwm %s (pwmg_robocol.c)\n",str_pwm);
	}
	
	free(str_pwm);
	return PWM_OK;
}

pwm_st pwm_disable(uint8_t pwm_n){
	char* route=malloc(35);
	char* str_pwm=malloc(1);
	char* disable=malloc(1);
	if(pwm_n>7 || pwm_n==2 || pwm_n<1 ){
		printf("El número de pwm ingresado no es valido.\n Realice revisión de documentación de Galileo Gen 1 para observar los valores válidos\n");
		return PWM_ERROR;
	}

	str_pwm=u8toa(pwm_n);
	disable=u8toa(DISABLE);
	sprintf(route, "/sys/class/pwm/pwmchip0/pwm%d/enable", pwm_n);

	pwm_export(str_pwm);
	if(g_write_file(route,disable,1)!=0){
		printf("Error en la escritura del disable\n");
		pwm_unexport(str_pwm);
		return PWM_ERROR;
	}
	free(route);
	free(disable);
	pwm_unexport(str_pwm);
	free(str_pwm);
	return PWM_OK;
}



pwm_st pwm_set_duty(uint8_t pwm_n, uint8_t duty){
	char* str_pwm=u8toa(pwm_n);
	uint32_t duty_ns;
	char* str_duty;
	uint8_t duty_len;
	char* route=malloc(39);
	int8_t pwm;

	if(pwm_n>7 || pwm_n==2 || pwm_n<1 ){
		printf("El número de pwm ingresado no es valido.\nRealice revisión de documentación de Galileo Gen 1 para observar los valores válidos\n");
		return PWM_ERROR;
	}
	
	if(duty<0 || duty>100){
		printf("El número de duty cycle ingresado no es valido.\nIngrese un valor entre 0 y 100\n");
		return PWM_ERROR;
	}
	
	if((pwm=pwm_get_dev(pwm_n))<0){
		printf("Error en la obtencion del dispositivo de pwm en la libreria pwmg_robocol\n");
		return PWM_ERROR;
	}

	if(initpwm[pwm]!=1){
		printf("No ha inicializado el dispositivo pwm identificado con el numero pasado por parametro.\n");
		return PWM_ERROR;
	}

	duty_ns=(uint32_t)(periods[pwm]*duty/100);
	str_duty=u32toa(duty_ns);
	duty_len=getu32_len(duty_ns);

	sprintf(route,"/sys/class/pwm/pwmchip0/pwm%d/duty_cycle",pwm_n);

	pwm_export(str_pwm);
	if(g_write_file(route,str_duty,duty_len)!=0){
		printf("Error en la escritura del duty_cycle\n");
		pwm_unexport(str_pwm);
		return PWM_ERROR;
	}
	duties[pwm]=duty;
	free(str_duty);
	free(route);
	pwm_unexport(str_pwm);
	free(str_pwm);
	return PWM_OK;
}

pwm_st pwm_set_period(uint8_t pwm_n, uint32_t period){
	char* str_pwm=malloc(1);
	char* str_period;
	char* str_duty;
	char* routep=malloc(35);
	char* routed=malloc(39);
	uint32_t duty_ns;
	uint8_t duty_len;
	uint8_t period_len;
	int8_t pwm;
	if(pwm_n>7 || pwm_n==2 || pwm_n<1 ){
		printf("El número de pwm ingresado no es valido.\nRealice revisión de documentación de Galileo Gen 1 para observar los valores válidos\n");
		return PWM_ERROR;
	}
	str_pwm=u8toa(pwm_n);
	if(period<100000 || period>7999999){
		printf("El número de periodo ingresado no es valido.\nIngrese un valor entre 100000 y 799999\n");
		return PWM_ERROR;
	}

	if((pwm=pwm_get_dev(pwm_n))<0){
		printf("Error en la obtencion del dispositivo de pwm en la libreria pwmg_robocol\n");
		return PWM_ERROR;
	}	
	if(initpwm[pwm]!=1){
		printf("No ha inicializado el dispositivo pwm identificado con el numero pasado por parametro.\n");
		return PWM_ERROR;
	}

	sprintf(routed,"/sys/class/pwm/pwmchip0/pwm%d/duty_cycle",pwm_n);
	duty_ns=(uint32_t)(period*duties[pwm]/100);
	str_duty=u32toa(duty_ns);
	duty_len=getu32_len(duty_ns);

	sprintf(routep,"/sys/class/pwm/pwmchip0/pwm%d/period",pwm_n);
	str_period=u32toa(period);
	period_len=getu32_len(period);

	pwm_export(str_pwm);
	if (period>periods[pwm]){
		if(g_write_file(routep,str_period,period_len)!=0){
			printf("Error en la escritura del periodo\n");
			pwm_unexport(str_pwm);
			return PWM_ERROR;
		}

		if(g_write_file(routed,str_duty,duty_len)!=0){
			printf("Error en la escritura del duty_cycle\n");
			pwm_unexport(str_pwm);
			return PWM_ERROR;
		}
	}else{

		if(g_write_file(routed,str_duty,duty_len)!=0){
			printf("Error en la escritura del duty_cycle\n");
			pwm_unexport(str_pwm);
			return PWM_ERROR;
		}

		if(g_write_file(routep,str_period,period_len)!=0){
			printf("Error en la escritura del periodo\n");
			pwm_unexport(str_pwm);
			return PWM_ERROR;
		}
	}

	periods[pwm]=period;
	free(str_period);
	free(routed);
	free(routep);
	pwm_unexport(str_pwm);
	free(str_pwm);
	return PWM_OK;
}

static pwm_st pwm_export(char* str_num ){
	if(g_write_file("/sys/class/pwm/pwmchip0/export",str_num,1)!=0){
		printf("Error en la escritura del export\n");
		return PWM_ERROR;
	}
	return PWM_OK;
}
static pwm_st pwm_unexport(char* str_num){
	if(g_write_file("/sys/class/pwm/pwmchip0/unexport",str_num,1)){
		printf("Error en la escritura del unexport\n");
		return PWM_ERROR;
	}
	return PWM_OK;
}


uint8_t getu32_len(uint32_t i){
	if (i<10){
		return  (uint8_t)1;
	}else if(i<100){
		return  (uint8_t)2;
	}else if(i<1000){
		return  (uint8_t)3;
	}else if(i<10000){
		return  (uint8_t)4;
	}else if(i<100000){
		return  (uint8_t)5;
	}else if(i<1000000){
		return  (uint8_t)6;
	}else if(i<10000000){
		return  (uint8_t)7;
	}else if(i<100000000){
		return  (uint8_t)8;
	}else if(i<1000000000){
		return  (uint8_t)9;
	}else if(i<4294967296){
		return  (uint8_t)10;
	}else{
		return PWM_ERROR;
	}
}

int8_t pwm_get_dev(uint8_t pwm_num){
	switch (pwm_num) {
	case 1:
	  	return 0;
	case 3:
		return 1;
	case 4:
		return 2;
	case 5:
		return 3;
	case 6:
		return 4;
	case 7:
		return 5;
	default:
	  printf("El pwm_num ingresado a pwm_get_dev en libreria de pwmg_robocol no es un numero válido de pwm.\n");
	  return -1;
	}
}