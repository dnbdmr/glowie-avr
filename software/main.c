#include <avr/io.h>
#include <util/delay.h>
#include <avr/power.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include "ws2812_config.h"
#include "light_ws2812.h"

/* change this for multiple lights */
#define SEED	1
/* timings in powers of 2 - 1 */
#define MAXCYCLE	0x3F	// total max up/down count of 16.07s
#define MAXHOLD		0xFF	// 255ms
#define MAXWAIT		0xFFF	// 4.096ms

#define GAMMA(a) (pgm_read_byte_near(gamma8 + a)) // CIE Lightness loopup table function
const uint8_t gamma8[] PROGMEM = {
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
	1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,
	2,  3,  3,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4,  5,  5,  5,
	5,  6,  6,  6,  6,  7,  7,  7,  7,  8,  8,  8,  9,  9,  9, 10,
	10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
	17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
	25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
	37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
	51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
	69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
	90, 92, 93, 95, 96, 98, 99,101,102,104,105,107,109,110,112,114,
	115,117,119,120,122,124,126,127,129,131,133,135,137,138,140,142,
	144,146,148,150,152,154,156,158,160,162,164,167,169,171,173,175,
	177,180,182,184,186,189,191,193,196,198,200,203,205,208,210,213,
	215,218,220,223,225,228,231,233,236,239,241,244,247,249,252,255 };

void init(void) {
	clock_prescale_set(clock_div_1);
	DDRB |= _BV(PB0);
}

void variable_delay_ms(uint32_t i) {
	while (i--)
		_delay_ms(1);
}

int main(void) {
	init();

	srand(SEED);

	struct cRGB led[1]; 		// used for counting
	struct cRGB maxled[1];		// holds max values
	struct cRGB sendled[1];		// used for sending gamma corrected led[]
	uint8_t cycledelay;

	uint8_t ledreg = 0; // 0b00000RGB

	while(1) {
		maxled[0].r = rand() & 0xFF;
		maxled[0].g = rand() & 0xFF;
		maxled[0].b = rand() & 0xFF;
		cycledelay = rand() & MAXCYCLE; 

		led[0].r = led[0].g = led[0].b = 0;
		ws2812_setleds(led, 1);
		variable_delay_ms(cycledelay);

		while (ledreg < 7) { 			//count up
			if (led[0].r < maxled[0].r)
				led[0].r++;
			else
				ledreg |= (1<<2);
			if (led[0].g < maxled[0].g)
				led[0].g++;
			else 
				ledreg |= (1<<1);
			if (led[0].b < maxled[0].b)
				led[0].b++;
			else
				ledreg |= (1<<0);

			sendled[0].r = GAMMA(led[0].r);
			sendled[0].g = GAMMA(led[0].g);
			sendled[0].b = GAMMA(led[0].b);
			ws2812_setleds(sendled, 1);
			variable_delay_ms(cycledelay);
		}

		variable_delay_ms(rand() & MAXHOLD);

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

			sendled[0].r = GAMMA(led[0].r);
			sendled[0].g = GAMMA(led[0].g);
			sendled[0].b = GAMMA(led[0].b);
			ws2812_setleds(sendled, 1);
			variable_delay_ms(cycledelay);
		}

		variable_delay_ms(rand() & MAXWAIT);
	}
}
