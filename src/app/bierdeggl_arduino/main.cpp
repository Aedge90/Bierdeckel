#include "hx711.h"
#include <avr/io.h>
#include <util/delay.h>
#include <Arduino.h>
#include <HardwareSerial.h>

//Hx711 scale(A2, A3);

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
	//DDRB = 0xff;
	Serial.begin(19200);
	hx711_init();
	int n = 0;
	bool empty = false;
	for(;;){
		float fval = /*scale.getGram();*/hx711_getGram();
		Serial.print(fval, 1);
		Serial.print(" g -> ");
		empty |= isEmpty(fval);
		if (empty && isFull(fval)){
			++n;
			empty = false;
		}
		show(n);
		if(empty) Serial.print(" LEER ");
		else if (isFull(fval)) Serial.print(" VOLL ");
		else Serial.print(" halb ");
		Serial.print(" -> ");
		Serial.print(n, 1);
		Serial.println(" Bier");
	//	int32_t val = hx711_get_mg();
	//	empty |= isEmpty(val);
	//	if (empty && isFull(val)){
	//		++n;
	//		empty = false;
	//	}
	//	show(n);
	//	Serial.print(val/1000, 1);
	//	Serial.print("g -> ");
	//	if(empty) Serial.print(" LEER ");
	//	else if (isFull(val)) Serial.print(" VOLL ");
	//	else Serial.print(" halb ");
	//	Serial.print(" -> ");
	//	Serial.print(n, 1);
	//	Serial.println(" Bier");
//		_delay_ms(500);
	}
	return 0;
}




// > 0 -> unbenutzt
// = 0 -> leer
// < 0 -> nicht leer
