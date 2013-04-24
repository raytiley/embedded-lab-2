#include <pololu/orangutan.h>
#include <avr/io.h>         //gives us names for registers
#include <avr/interrupt.h>
#include "timer.h"

#define VELOCITY_PERIOD 50
#define ENCODER_PERIOD 10 

volatile uint32_t g_timer_ticks = 0;
extern char g_pd_release;
extern char g_velocity_release;
extern char g_encoder_release;
extern char g_log_release;
extern char g_interpolate_release;

void init_timers() {
	// WGM30 = 0
	// WGM31 = 0
	// WGM32 = 1
	// WGM33 = 0
	TCCR3A &= ~(0 << WGM00);
	TCCR3A &= ~(0 << WGM31);
	TCCR3B |= (1 << WGM32);
	TCCR3B &= ~(0 << WGM33);
	
	// Using pre-scaler 1024
	// CS30 = 1
	// CS31 = 0
	// CS32 = 1
	TCCR3B |= (1 << CS30);
	TCCR3B &= ~(0 << CS31);
	TCCR3B |= (1 << CS32);
	
	// Set TOP
	OCR3A = 19;

	//Enable output compare match interrupt on timer 3A
	TIMSK3 |= (1 << OCIE3A);
}

ISR(TIMER3_COMPA_vect) {
	//This should get called 1000 times a second.
	g_timer_ticks++;

	// We basically want to release the pd controller task 1Khz
	// so every time this interput is called
	g_pd_release = 1;

	if ( ( g_timer_ticks % VELOCITY_PERIOD ) == 0 ) {
		g_velocity_release = 1;
	}

	if (( g_timer_ticks % ENCODER_PERIOD) == 0) {
		g_encoder_release = 1;
	}

	if (( g_timer_ticks % 500) == 0) {
		g_log_release = 1;
	}

	if (( g_timer_ticks % 2) == 0) {
		g_interpolate_release = 1;
	}
}