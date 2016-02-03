#include "led.h"
#include "timer.h"
#include <avr/io.h>

#define LED1 (1<<PD5)
#define LED2 (1<<PD6)
#define LED3 (1<<PB0)
#define LED4 (1<<PB1)
#define LED5 (1<<PC5)
#define LED6 (1<<PC4)
#define LED7 (1<<PD3)
#define LED8 (1<<PD4)


void led_init(void){
	DDRD |= 0b01111000;
	DDRC |= 0b00110000;
	DDRB |= 0b00000011;
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
			PORTB |= LED4;
		case(3):
			PORTB |= LED3;
		case(2):
			PORTD |= LED2;
		case(1):
			PORTD |= LED1;
			break;
		case(0):
			PORTB &= ~(LED4 | LED3);
			PORTD &= ~(LED2 | LED1);
	}
}

void led_blink(uint8_t n) {
	sleep16m(10);
	led_show(0);
	sleep16m(10);
	led_show(n);
}
