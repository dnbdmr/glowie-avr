#define F_CPU 4000000

#include <avr/io.h>
#include <util/delay.h>
#include <avr/power.h>
#include <avr/pgmspace.h>
#include <stdlib.h>
#include "ws2812_config.h"
#include "light_ws2812.h"

void init(void) {
	clock_prescale_set(clock_div_2);
}

void variable_delay_ms(uint16_t i) {
	while (i--)
		_delay_ms(1);
}

int main(void) {
	init();
	while(1) {
	}
}
