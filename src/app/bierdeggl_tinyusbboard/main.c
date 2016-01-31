#include "hx711.h"
#include "usbprint.h"

#define bool uint8_t
#define false 0
#define true 1


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

bool isEmpty(int32_t val){
	const int epsilon = 5000;
	if (val > -epsilon && val < epsilon)
		return true;
	return false;
}
bool isFull(int32_t val){
	const int32_t full = 480000;
	if (val > full)
		return true;
	return false;
}
int main(void)
{
	usbprint_setup();
//	while(!(PINB & (1<<PB0)));
	putString(" Hello annoying World\0");
	putInt(1234);
	putString("\n\0");
		usbprint_flush();
	//Taster
	// LEDs...
	DDRB = 0xff;

	hx711_init();
	int n = 0;
	bool empty = false;
	for(;;){
	putString(" Pre get g\0");
	putString("\n\0");
		usbprint_flush();
		float fval = hx711_getGram();
	putString(" Got g\0");
	putString("\n\0");
		usbprint_flush();
		putInt(fval);
		putString(" mg -> ");
		if(empty) putString(" LEER ");
		else if (isFull(fval)) putString(" VOLL ");
		else putString(" halb ");
		putString(" -> ");
		putInt(n);
		putString(" Bier\n\0");
		usbprint_flush();
	putString(" Pre get mg\0");
	putString("\n\0");
		usbprint_flush();
		int32_t val = hx711_get_mg();
	putString(" Got mg\0");
	putString("\n\0");
		usbprint_flush();
		empty |= isEmpty(val);
		if (empty && isFull(val)){
			++n;
			empty = false;
		}
		show(n);
		putInt(val);
		putString(" mg -> ");
		if(empty) putString(" LEER ");
		else if (isFull(val)) putString(" VOLL ");
		else putString(" halb ");
		putString(" -> ");
		putInt(n);
		putString(" Bier\n\0");
		usbprint_flush();
	}
	return 0;
}




// > 0 -> unbenutzt
// = 0 -> leer
// < 0 -> nicht leer
