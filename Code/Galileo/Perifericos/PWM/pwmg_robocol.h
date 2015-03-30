#ifndef PWMG_ROBOCOL_H
#define PWMG_ROBOCOL_H

#include "gpio_robocol.h"


#define PWM_ERROR 	0x01
#define PWM_OK 	0x00
#define ENABLE 0x01
#define DISABLE 0x00

typedef uint8_t pwm_st;

pwm_st pwm_enable(uint8_t pwm_num);
pwm_st pwm_set_duty(pwm_dev* dev_ptr, uint8_t duty);
pwm_st pwm_set_period(pwm_dev* dev_ptr, uint32_t period);
pwm_st pwm_disable(uint8_t pwm_num);



#endif