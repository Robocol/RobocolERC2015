#ifndef PWMG_ROBOCOL_H
#define PWMG_ROBOCOL_H

#include "gpio_robocol.h"

#define PWM_ERROR 	0x01
#define PWM_OK 	0x00

typedef uint8_t pwm_st;
typedef struct pwm_dev{
	uint8_t pwm_n;
	uint32_t period;
	uint8_t duty_cycle;
}pwm_dev;




pwm_st build_pwm(pwm_dev* dev_ptr);
pwm_st pwm_change_duty(pwm_dev* dev_ptr, uint8_t duty);
pwm_st pwm_change_period(pwm_dev* dev_ptr, uint32_t period);



#endif