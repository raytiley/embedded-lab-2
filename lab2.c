#include <pololu/orangutan.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "menu.h"
#include "motor.h"
#include "timer.h"
#include "pd_controller.h"
#include "logging.h"
#include "interpolator.h"

volatile char g_pd_release = 0;
volatile char g_velocity_release = 0;
volatile char g_encoder_release = 0;
volatile char g_log_release = 0;
volatile char g_interpolate_release = 0;

volatile long g_prop_gain = 5;
volatile long g_der_gain = 0;
volatile long g_target_position = 0;
volatile long pd_target = 0;
volatile long g_current_position = 0;
volatile int g_velocity = 0;
volatile long g_velocity_last_position = 0;
volatile long g_motor_output = 0;
volatile char logging = 0;



int main(void) {

	init_menu();
	
	clear();
	init_motor();
	init_timers();
	encoders_init(IO_D2, IO_D3, IO_C4, IO_C5);

	sei();
	while (1) 
	{
		if(g_pd_release) {
			pd_task();
			g_pd_release = 0;
		}

		if(g_velocity_release) {
			velocity_task();
			g_velocity_release = 0;
		}

		if(g_encoder_release) {
			encoder_task();
			g_encoder_release = 0;
		}

		if(g_log_release) {
			log_task();
			g_log_release = 0;
		}

		if(g_interpolate_release) {
			interoplate_task();
			g_interpolate_release = 0;
		}

		serial_check();
		check_for_new_bytes_received();		
	}
}