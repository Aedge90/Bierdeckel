#include "hx711.h"
#include "uart.h"
#include "led.h"
#include <avr/io.h>
#include <util/delay.h>

#define bool uint8_t
#define true 1
#define false 0

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
	led_init();
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
		led_show(n);
		if(empty) 
			printf("%4ldg -> LEER -> %2u Bier\n", valg ,n);
		else if (isFull(valg)) 
			printf("%4ldg -> VOLL -> %2u Bier\n", valg ,n);
		else
			printf("%4ldg -> halb -> %2u Bier\n", valg ,n);
//		_delay_ms(500);
	}
	return 0;
}
