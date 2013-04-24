#include "motor.h"
#include "pd_controller.h"
#include <pololu/orangutan.h>

extern long g_target_position;
extern long g_current_position;
extern int g_velocity;
extern long g_velocity_last_position;
extern long g_motor_output;
extern long g_prop_gain;
extern long g_der_gain;
extern char logging;
extern long pd_target;


void pd_task() {
	g_motor_output = ((g_prop_gain * (pd_target - g_current_position)) - (g_der_gain * g_velocity));
	set_speed(g_motor_output);
}

void velocity_task() {
	g_velocity = (g_current_position - g_velocity_last_position);
	g_velocity_last_position = g_current_position;
}

void encoder_task() {
	g_current_position = encoders_get_counts_m1();

}
void decrement_proportional_gain(long value) {
	g_prop_gain -= value;
}

void increment_proportional_gain(long value) {
	g_prop_gain += value;
}

void decrement_derivative_gain(long value) {
	g_der_gain -= value;
}
void increment_derivative_gain(long value) {
	g_der_gain += value;
}