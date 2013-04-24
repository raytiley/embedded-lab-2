#include <avr/io.h>         //gives us names for registers
#include <avr/interrupt.h>

void init_motor();

void set_speed(long speed);