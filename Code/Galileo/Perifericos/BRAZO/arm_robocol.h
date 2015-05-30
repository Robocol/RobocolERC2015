#ifndef ARM_ROBOCOL
#define ARM_ROBOCOL
#include "ph_robocol.h"
#include "stepper_robocol.h"

#define ARM_OK 			0x00
#define ARM_ERROR 		0x01

typedef uint8_t arm_st;

typedef struct 
{
	ph_dev* b_motor;
	ph_dev* b_actuator;
	ph_dev* u_actuator;
	stp_device* sup;
	stp_device* wrist;
	stp_device* claw;
	
}arm_dev;

arm_st arm_build(arm_dev* dev);

// arm_st arm_ph();

// arm_st arm_st();

#endif