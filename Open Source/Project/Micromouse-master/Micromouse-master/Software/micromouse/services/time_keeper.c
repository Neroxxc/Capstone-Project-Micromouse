#include "time_keeper.h"

#include <ti/sysbios/family/arm/lm4/Timer.h>

Timer_Struct time_keeper_timer;
Timer_Params time_keeper_params;

static sys_time_t curr_time = {0,0,0,0};

//Function definitions
void timer_update (UArg inc_usec);

void time_keeper_init(void) {
	// Create a timer that this module will use
	Timer_Params_init(&time_keeper_params);
	time_keeper_params.arg = (UArg)MICROSECONDS_PER_UPDATE;
	time_keeper_params.period = MICROSECONDS_PER_UPDATE;
	Timer_construct(&time_keeper_timer, Timer_ANY, timer_update, &time_keeper_params, NULL);
}

void get_current_time(sys_time_handle time_handle) {
	time_handle->hours = curr_time.hours;
	time_handle->minutes = curr_time.minutes;
	time_handle->seconds = curr_time.seconds;
	time_handle->uSec = curr_time.uSec;
}

uint64_t get_curr_time_us(void) {
	return 1000000 * (60 * (60 * curr_time.hours + curr_time.minutes) + curr_time.seconds) + curr_time.uSec;
}

uint64_t get_time_lapsed_us(sys_time_handle time_handle) {
	uint64_t time_us = 1000000 * (60 * (60 * time_handle->hours + time_handle->minutes) + time_handle->seconds) + time_handle->uSec;
	uint64_t curr_time_us = 1000000 * (60 * (60 * curr_time.hours + curr_time.minutes) + curr_time.seconds) + curr_time.uSec;
	return curr_time_us - time_us;
}

// Timer Interrupt Service Routine
void timer_update (UArg inc_usec) {
	if(curr_time.uSec + inc_usec < 1000000) {
		curr_time.uSec += inc_usec;
	}
	else { // uSeconds are rolling over, increment seconds
		curr_time.uSec = (curr_time.uSec + inc_usec) - 1000000;
		if(curr_time.seconds + 1 < 60){
			curr_time.seconds++;
		}
		else { // seconds are rolling over, increment minutes
			curr_time.seconds = 0;
			if(curr_time.minutes + 1 < 60){
				curr_time.minutes++;
			}
			else { // minutes are rolling over, increment hours
				curr_time.minutes = 0;
				curr_time.hours++;
			}
		}
	}
}
