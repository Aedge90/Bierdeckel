#include "hx711.h"
#include "usbprint.h"
#include <util/delay.h>

void hx711_init(void)
{
	DDRB |= 1<<PB0;
	PORTB &= ~(1<<PB0);
	putString("HX711Init\n\0");
	usbprint_flush();
	DDRC |= SCL;
	DDRC &= SDI;
	PORTC |= SDI;

	PORTC |= SCL;
	_delay_ms(100);
	PORTC &= ~SCL;

	hx711_averageValue(3);
	_offset = hx711_averageValue(3);
	_scale = 742.0f;
	putString("HX711Initfinished\n\0");
	usbprint_flush();
}


uint32_t hx711_averageValue(uint8_t times)
{
	putString("HX711avgval\n\0");
	usbprint_flush();
	uint8_t i;
	uint32_t sum = 0;
	for (i = 0; i < times; i++)
	{
		sum += hx711_getValue();
	}
	putString("HX711avgvalfinished\n\0");
	usbprint_flush();

	return sum / times;
}

uint32_t hx711_getValue()
{
	putString("HX711getval\n\0");
	usbprint_flush();
	uint8_t i = 0;
	uint32_t data = 0;

	putString("Start\n\0");
	usbprint_flush();
	PORTC |= SCL;
	_delay_ms(100);
	PORTC &= ~SCL;
	uint8_t pr = 1;
	//while (PINC & SDI){
	while (bit_is_set(PINC ,PC0/*SDI*/)){
		if (i++%2 == 0) PORTB ^= (1<<PB0);
		//if (pr){
		//	putInt(PORTC);
		//	//putInt(PINC);
		//	putString("\b\0");
		//	usbprint_flush();
		//}
	}
	//putString("HX711getvaldataready\n\0");
	//usbprint_flush();

	for (i = 24;i ;i--)
	{
		PORTC |= SCL;
		_delay_us(1);
		data |= ((PINC & SDI)<<i);
		PORTC &= ~SCL;
	}

	PORTC |= SCL;
	_delay_us(1);
	PORTC &= ~SCL;

	data ^= 0x800000;
	putInt(data);
	usbprint_flush();

	return data;
	}

	float hx711_getGram(void)
	{
		putString("HX711Gramm\n\0");
		usbprint_flush();
		int32_t val = (hx711_averageValue(3) - _offset);
		return (float) val / _scale;
	}
	int32_t hx711_get_mg(void)
	{
		putString("HX711mg\n\0");
		usbprint_flush();
		int32_t val = (hx711_averageValue(3) - _offset);
		return (1000 * val) / _scale;
	}
