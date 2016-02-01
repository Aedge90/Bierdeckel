#include "hx711.h"
#include <Arduino.h>
static uint32_t _offset;
static float _scale;

void hx711_init(void)
{
	pinMode(SCLK, OUTPUT);
	pinMode(SDI, INPUT);

	//reset HX711
	digitalWrite(SCLK, HIGH);
	delayMicroseconds(100);
	digitalWrite(SCLK, LOW);

	hx711_averageValue(32);
	//callibration. _offset is weight of empty glas
	_offset = hx711_averageValue(32);
	_scale = .5*742.0f;
}

uint32_t hx711_averageValue(uint8_t times)
{
	uint8_t i;
	uint32_t sum = 0;
	for (i = 0; i < times; i++)
	{
		sum += hx711_getValue();
	}

	return sum / times;
}

uint32_t hx711_getValue()
{
	uint8_t i = 0, j = 0;
	//uint32_t data = 0;
	uint8_t data[3] = {0,0,0};
	//When output data isnt ready for retrieval digital output pin DOUT (=SDI) is high
	while (digitalRead(SDI));
	for (i = 24;i--;)
	{
		//	digitalWrite(SCLK, HIGH);
		//	data |= (digitalRead(SDI))<<i;
		//	digitalWrite(SCLK, LOW);
	}
	//read data bit by bit by sending 24 positive clock pulses to PD_SCK pin
	for (j = 3; j--;)
	{
		for (i = 8; i--;)
		{
			digitalWrite(SCLK, HIGH);
			data[j] |= (digitalRead(SDI))<<i;
			digitalWrite(SCLK, LOW);
		}
	}

	//25th pulse sets DOUT (=SDI) pin back to HIGH
	digitalWrite(SCLK, HIGH);
	digitalWrite(SCLK, LOW);

	//no other pulses are sent so we will be using input channel A with a gain of 128

	data[2] ^= 0x80;
	
	return ((uint32_t) data[2] << 16) | ((uint32_t) data[1] << 8) | (uint32_t) data[0];
}

float hx711_getGram(void)
{
	int32_t val = (hx711_averageValue(32) - _offset);
	return ((float) val) / _scale;
}

int32_t hx711_get_mg(void)
{
	int32_t val = (hx711_averageValue(32) - _offset);
	return 1000 * ( val / _scale);
}
