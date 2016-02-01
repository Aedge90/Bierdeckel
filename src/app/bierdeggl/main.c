#include "hx711.h"
#include "uart.h"
#include <avr/io.h>
#include <util/delay.h>

#define bool uint8_t
#define true 1
#define false 0


void show(uint8_t n) {
	int zaun = n/5;
	int strich = n%5;
	switch(zaun){
		default:
			strich = 5;
		case(4):
			PORTB |= 0x80;
		case(3):
			PORTB |= 0x40;
		case(2):
			PORTB |= 0x20;
		case(1):
			PORTB |= 0x10;
			break;
		case(0):
			PORTB &= 0x0f;
	}
	switch(strich){
		default:
		case(4):
			PORTB |= 0x08;
		case(3):
			PORTB |= 0x04;
		case(2):
			PORTB |= 0x02;
		case(1):
			PORTB |= 0x01;
			break;
		case(0):
			PORTB &= 0xf0;
	}
}

// > 0 -> unbenutzt
// = 0 -> leer
// < 0 -> nicht leer
bool isEmpty(int32_t val){
	const int epsilon = 50;
	if (val > -epsilon && val < epsilon)
		return true;
	return false;
}
bool isFull(int32_t val){
	const int32_t full = 480;
	if (val > full)
		return true;
	return false;
}
int main(void)
{
	// LEDs...

	hx711_init();
	uart_init();
	uint8_t n = 0;
	bool empty = false;
	for(;;){
		int32_t valmg = hx711_get_mg();
		int32_t valg = valmg/1000;
		empty |= isEmpty(valg);
		if (empty && isFull(valg)){
			++n;
			empty = false;
		}
		show(n);
		if(empty) 
			printf("%ldg -> LEER -> %u Bier\n", valg ,n);
		else if (isFull(valg)) 
			printf("%ldg -> VOLL -> %u Bier\n", valg ,n);
		else
			printf("%ldg -> halb -> %u Bier\n", valg ,n);
//		_delay_ms(500);
	}
	return 0;
}
