/*
 * motor.h
 *
 *  Created on: Mar 18, 2014
 *      Author: Anthony
 */

#ifndef MOTOR_H_
#define MOTOR_H_

#include <stdint.h>
#include <stdbool.h>
#include <inc/hw_memmap.h>
#include <driverlib/pwm.h>

#include "system.h"

// CPU/MOTOR Configurations
#define CLK_FREQ 80000000
#define PWM_CLK_DIV 64

#define PWM_TIMER_FREQ (CLK_FREQ/PWM_CLK_DIV) // Hz
// NOTE: About 52000 us is the high limit for period!
#define MOTOR_PERIOD 10000

// Holds PWM information for each motor
typedef struct {
	uint32_t pwm_base_module;
	uint32_t pwm_generator;
	uint32_t pwm_pin;
	micromouse_gpio_name_t dir_pins[2];
} motor_config_t;

typedef enum  {
	RIGHT_MOTOR,
	LEFT_MOTOR,
	NUM_MOTORS
} motor_index_t;

typedef enum  {
	CW,
	CCW,
	BRAKE,
	STOP
} motor_direction_t;

// Define config information for the motors
static motor_config_t motors[NUM_MOTORS] = {
	{// Right Motor (PF2)
		PWM1_BASE,
		PWM_GEN_3,
		PWM_OUT_6,
		{RIGHT_MOTOR_DIR_1, RIGHT_MOTOR_DIR_2}
	},
	{// Left Motor (PF3)
		PWM1_BASE,
		PWM_GEN_3,
		PWM_OUT_7,
		{LEFT_MOTOR_DIR_1, LEFT_MOTOR_DIR_2}
	}
};

/*
 * Initialize all pins and register with system using GPIO libraries.
 * NOTE: If a new motor is added to the pwm_motors list, please make
 * 		 it is initialized in this function.
 */
void motors_init(void);

/*
 * Sets the duty cycle to the closest approximation of uSec that can be set.
 * After the function returns, the uSec variable will be changed to the approximation
 * that the pulse width was set to.
 */
void set_pulse_width(uint8_t motorIndex, uint32_t *uSec);

void update_motor(motor_index_t index, motor_direction_t direction, uint16_t duty_tenths_perc);



#endif /* MOTOR_H_ */
