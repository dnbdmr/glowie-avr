#define F_CPU 1000000L
#include <avr/io.h>
#include <util/delay.h>
#include <avr/power.h>
#include <avr/pgmspace.h>
#include <stdlib.h>

void init(void) {
	clock_prescale_set(clock_div_8);
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
