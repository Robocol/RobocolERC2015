#ifndef PWMG_ROBOCOL_H
#define PWMG_ROBOCOL_H

#include "gpio_robocol.h"


#define PWM_ERROR 	0x01
#define PWM_OK 	0x00
#define ENABLE 0x01
#define DISABLE 0x00

typedef uint8_t pwm_st;

static pwm_st pwm_export(char* str_num );
static pwm_st pwm_unexport(char* str_num);
static pwm_st handle_muxes(uint8_t pwm_n);
pwm_st pwm_build(uint8_t pwm_n,uint32_t period, uint8_t duty);
pwm_st pwm_enable(uint8_t pwm_num);
pwm_st pwm_set_duty(uint8_t pwm_num, uint8_t duty);
pwm_st pwm_set_period(uint8_t pwm_num, uint32_t period);
pwm_st pwm_disable(uint8_t pwm_num);
int8_t pwm_get_dev(uint8_t pwm_num);
uint8_t getu32_len(uint32_t uint32);


#endif