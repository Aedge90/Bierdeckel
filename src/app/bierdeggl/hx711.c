#include "hx711.h"
static uint32_t _offset;
static int32_t  _scale;

void hx711_init(void)
{
	DDR_HX |= SCLK;
	DDR_HX &= ~SDI;

	//reset HX711
	PORT_HX |= SCLK;
	_delay_us(100);
	PORT_HX &= ~SCLK;

	hx711_averageValue(32);
	//callibration. _offset is weight of empty glas
	_offset = hx711_averageValue(32);
	_scale = 371;//.5*742.0f;
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
	uint8_t i = 0;
	uint32_t data = 0;
	//When output data isnt ready for retrieval digital output pin DOUT (=SDI) is high
	while (PIN_HX & SDI);
	//read data bit by bit by sending 24 positive clock pulses to PD_SCK pin
	for (i = 24;i--;)
	{
		PORT_HX |= SCLK;
		data |= (uint32_t)((PINC & SDI)?1:0)<<i;
		PORT_HX &= ~SCLK;
	}

	//25th pulse sets DOUT (=SDI) pin back to HIGH
	PORT_HX |= SCLK;
	PORT_HX &= ~SCLK;

	//no other pulses are sent so we will be using input channel A with a gain of 128
	
	data ^= 0x800000;

	return data;	
}

float hx711_getGram(void)
{
	int32_t val = (hx711_averageValue(32) - _offset);
	return ((float) val) / _scale;
}

int32_t hx711_get_mg(void)
{
	int32_t val = (hx711_averageValue(32) - _offset);
	return (1000 *  val) / _scale;
}
