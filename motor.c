#include "motor.h"
#include <pololu/orangutan.h>  

void init_motor() {

    TCCR2A |= (1 << WGM20);
    TCCR2A |= (1 << WGM21);

    TCCR2A |= (1 << COM2B1);

    TCCR2B |= (1 << CS21);
    TCCR2B |= (1 << CS22);

	DDRD |= (1 << DDD6);
	DDRC |= (1 << DDC6);
}

void set_speed(long speed)
{
	char reversed = 0;
	if(speed < 0) {
		speed = speed * -1;
		reversed = 1;
	}

	if(reversed)
	{
		PORTC &= ~(1 << PORTC6);
	}
	else
	{
		PORTC |= (1 << PORTC6);
	}


	if(speed > 255)
		speed = 255;

	if(speed > 0 && speed < 25)
		speed = 25;

	OCR2B = speed;	// pwm one driver input
}