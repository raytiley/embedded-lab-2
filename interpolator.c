#include "interpolator.h"

extern long g_target_position;
extern long g_current_position;
extern int g_velocity;
extern long g_velocity_last_position;
extern long g_motor_output;
extern long g_prop_gain;
extern long g_der_gain;
extern char logging;
extern long pd_target;

#define STEP_SIZE 50

void interoplate_task() {
	char reversed = 0;
	long delta = g_target_position - g_current_position;
	
	if(delta < 0) {
		delta = delta * -1;
		reversed = 1;
	}

	if(delta > STEP_SIZE) {
		if(reversed)
			pd_target = g_current_position - STEP_SIZE;
		else
			pd_target = g_current_position + STEP_SIZE;
	} else {
		pd_target = g_target_position;
	}
}

void set_target(long position) {
	g_target_position += position;
}