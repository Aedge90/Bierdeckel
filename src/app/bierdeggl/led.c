#include "led.h"
#include "timer.h"
#include <avr/io.h>


#define LEDZaun1 (1<<PB0)
#define LEDZaun2 (1<<PD7)
#define LEDZaun3 (1<<PD6)
#define LEDZaun4 (1<<PD5)
#define LEDStrich1 (1<<PD4)
#define LEDStrich2 (1<<PD3)
#define LEDStrich3 (1<<PC4)
#define LEDStrich4 (1<<PC5)

// defines the data direction of the pins. 1 is output, 0 is input
void led_init(void){
	DDRD |= 0b11111000;
	DDRC |= 0b00110000;
	DDRB |= 0b00000001;
}

void led_show(uint8_t n) {
	int zaun = n/5;
	int strich = n-(zaun*5);
	switch(zaun){
		case(4):
			PORTD |= LEDZaun4;
		case(3):
			PORTD |= LEDZaun3;
		case(2):
			PORTD |= LEDZaun2;
		case(1):
			PORTB |= LEDZaun1;
			break;
		default:
			PORTD &= ~(LEDZaun4 | LEDZaun3 | LEDZaun2);
			PORTB &= ~(LEDZaun1);
	}
	switch(strich){
		case(4):
			PORTC |= LEDStrich4;
		case(3):
			PORTC |= LEDStrich3;
		case(2):
			PORTD |= LEDStrich2;
		case(1):
			PORTD |= LEDStrich1;
			break;
		default:
			PORTC &= ~(LEDStrich4 | LEDStrich3);
			PORTD &= ~(LEDStrich2 | LEDStrich1);
	}
}

void led_blink(uint8_t n) {
	timer_wait(1);
	led_show(0);
	timer_wait(1);
	led_show(n);
}
