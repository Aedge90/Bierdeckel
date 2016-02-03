#include "hx711.h"
static uint32_t _offset;
static int32_t  _scale;

void hx711_init(void)
{
	DDR_HX |= SCLK;
	DDR_HX &= ~SDI;

	PORT_HX |= SCLK;
	_delay_us(100);
	PORT_HX &= ~SCLK;

	hx711_averageValue(32);
	_offset = hx711_averageValue(32);
	_scale = 371; // TODO: adjust
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
	while (PIN_HX & SDI);
	for (i = 24;i--;)
	{
		PORT_HX |= SCLK;
		data |= (uint32_t)((PINC & SDI)?1:0)<<i;
		PORT_HX &= ~SCLK;
	}

	PORT_HX |= SCLK;
	PORT_HX &= ~SCLK;

	data ^= 0x800000;

	return data;	
}

int32_t hx711_getGram(void)
{
	int32_t val = (hx711_averageValue(32) - _offset);
	return val / _scale;
}

int32_t hx711_get_mg(void)
{
	int32_t val = (hx711_averageValue(32) - _offset);
	return (1000 *  val) / _scale;
}
