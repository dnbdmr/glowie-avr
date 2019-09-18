#include <avr/io.h>
#include <util/delay.h>
#include <avr/power.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include "ws2812_config.h"
#include "light_ws2812.h"

struct cRGB led[1];

void init(void) {
	clock_prescale_set(clock_div_1);
	DDRB |= _BV(PB1);

	DDRB |= _BV(PB0);
}

void variable_delay_ms(uint16_t i) {
	while (i--)
		_delay_ms(1);
}

int main(void) {
	init();
	while(1) {
		PORTB |= _BV(PB1);
		led[0].r=10;led[0].g=0;led[0].b=0;    // Write red to array
		ws2812_setleds(led,1);
		_delay_ms(500);                         // wait for 500ms.

		PORTB &= ~(_BV(PB1));
		led[0].r=0;led[0].g=10;led[0].b=0;			// green
		ws2812_setleds(led,1);
		_delay_ms(500);

		led[0].r=0;led[0].g=00;led[0].b=10;		// blue
		ws2812_setleds(led,1);
		_delay_ms(500);
	}
}
