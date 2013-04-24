#include "logging.h"
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "menu.h"
#include <pololu/orangutan.h>

extern char logging;
extern long g_target_position;
extern long g_current_position;
extern int g_velocity;
extern long g_velocity_last_position;
extern long g_motor_output;
extern long g_prop_gain;
extern long g_der_gain;
extern int g_velocity;

void log_task() {
	if(logging)
		log_print_log_row();

	log_update_lcd();
}

void log_print_header() {
	int length;
	char tempBuffer[256];
	length = sprintf(tempBuffer, "Kd, Kp, V, Pr, Pm, T\r\n");
	print_usb(tempBuffer, length);
}

void log_print_log_row() {
	// Used to pass to USB_COMM for serial communication
	int length;
	char tempBuffer[256];
	length = sprintf(tempBuffer, "%ld, %ld, %d, %ld, %ld, %ld\r\n", g_der_gain, g_prop_gain, g_velocity, g_target_position, g_current_position, g_motor_output);
	print_usb(tempBuffer, length);
}

void log_update_lcd() {

	clear();
	lcd_goto_xy(0,0);
	print("V:");
	print_long(g_velocity);
	print(" T:");
	print_long(g_motor_output);
	print(" P:");
	print_long(g_prop_gain);
	print(" D:");
	print_long(g_der_gain);

	lcd_goto_xy(0,1);
	print("P: ");
	print_long(g_current_position);
	print(" G: ");
	print_long(g_target_position);
}