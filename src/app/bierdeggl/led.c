#include "led.h"
#include "timer.h"
#include <avr/io.h>

#define LEDStrich1 (1<<PD5)
#define LEDStrich2 (1<<PD6)
#define LEDStrich3 (1<<PD7)
#define LEDStrich4 (1<<PB0)
#define LEDZaun1 (1<<PC5)
#define LEDZaun2 (1<<PC4)
#define LEDZaun3 (1<<PD3)
#define LEDZaun4 (1<<PD4)


void led_init(void){
	DDRD |= 0b11111000;
	DDRC |= 0b00110000;
	DDRB |= 0b00000001;
}

void led_show(uint8_t n) {
	int zaun = n/5;
	int strich = n%5;
	switch(zaun){
		default:
			strich = 5;
		case(4):
			PORTD |= LED8;
		case(3):
			PORTD |= LED7;
		case(2):
			PORTC |= LED6;
		case(1):
			PORTC |= LED5;
			break;
		case(0):
			PORTD &= ~(LED8 | LED7);
			PORTC &= ~(LED6 | LED5);
	}
	switch(strich){
		default:
		case(4):
			PORTB |= LEDStrich4;
		case(3):
			PORTB |= LEDStrich3;
		case(2):
			PORTD |= LEDStrich2;
		case(1):
			PORTD |= LEDStrich1;
			break;
		case(0):
			PORTB &= ~(LEDStrich4 | LEDStrich3);
			PORTD &= ~(LEDStrich2 | LEDStrich1);
	}
}

void led_blink(uint8_t n) {
	sleep16m(10);
	led_show(0);
	sleep16m(10);
	led_show(n);
}
