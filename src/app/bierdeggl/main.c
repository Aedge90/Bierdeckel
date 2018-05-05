#include "hx711.h"
#include "uart.h"
#include "led.h"
#include "timer.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define bool uint8_t
#define true 1
#define false 0

//#define DEBUG

// > 0 -> unbenutzt
// ~ 0 -> leer
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
	led_init();
	hx711_init();
#ifdef DEBUG
	uart_init();
#endif
	timer_init();
	sei();
	uint8_t n = 0;
	bool empty = false;
	// blink once to indicate initilisation has finished
	for(;;) {
		sleep16m(1);
		led_show(25);
		sleep16m(1);
		led_show(0);
	}
	for(;;){
		int32_t valg;
		// blink while glass is empty
		// if no LEDs are avail, blink LED8

		if (isEmpty((valg = hx711_get_mg()/1000))){
			sleep16m(1);
			while (isEmpty((valg = hx711_get_mg()/1000))){
				empty |= 1;
				if (0 == n) PORTD |= 1<<PD4;
				led_blink(n);
				if (0 == n) PORTD &= ~(1<<PD4);
			}
		}
		if (empty && isFull(valg)){
			++n;
			empty = false;
		}
		led_show(n);
#ifdef DEBUG
		if(empty) 
			printf("%4ldg -> LEER -> %2u Bier\n", valg ,n);
		else if (isFull(valg)) 
			printf("%4ldg -> VOLL -> %2u Bier\n", valg ,n);
		else
			printf("%4ldg -> halb -> %2u Bier\n", valg ,n);
		//sleep mode seems to interfere with serial output \n...
		_delay_ms(1000);
#else
		sleep(1);
#endif
	}
	return 0;
}
