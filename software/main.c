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

void variable_delay_ms(uint32 i) {
	while (i--)
		_delay_ms(1);
}

int main(void) {
	init();

	struct cRGB  templed[1];
	led[0].r = 0xFF;
	led[0].b = 0x0;
	led[0].g = 0xFF;
	ws2812_setleds(led, 1);

	uint8_t ledreg = 0; // 0b00000RGB

	while(1) {
		PORTB |= _BV(PB1);

		templed[0].r = rand() & 0xFF;
		templed[0].g = rand() & 0xFF;
		templed[0].b = rand() & 0xFF;

		led[0].r = led[0].g = led[0].b = 0;
		ws2812_setleds(led, 1);
		_delay_ms(10);

		while (ledreg < 7) { 			//count up
			if (led[0].r < templed[0].r)
				led[0].r++;
			else
				ledreg |= (1<<2);
			if (led[0].g < templed[0].g)
				led[0].g++;
			else 
				ledreg |= (1<<1);
			if (led[0].b < templed[0].b)
				led[0].b++;
			else
				ledreg |= (1<<0);
			ws2812_setleds(led, 1);
			_delay_ms(10);
		}

		variable_delay_ms(rand() & 0xF);

		PORTB &= ~(_BV(PB1));

		while (ledreg > 0) { 			//count down
			if (led[0].r > 0)
				led[0].r--;
			else
				ledreg &= ~(1<<2);
			if (led[0].g > 0)
				led[0].g--;
			else 
				ledreg &= ~(1<<1);
			if (led[0].b > 0)
				led[0].b--;
			else
				ledreg &= ~(1<<0);
			ws2812_setleds(led, 1);
			_delay_ms(10);
		}

		variable_delay_ms(rand() & 0xFFF);
	}
}
