#ifndef HX711_H_
#define HX711_H_

#include "Arduino.h"

#define SDI A2
#define SCLK A3

#include <avr/io.h>
#include <util/delay.h>

void hx711_init(void);
uint32_t hx711_getValue(void);
uint32_t hx711_averageValue(uint8_t times);
float hx711_getGram(void);
int32_t hx711_get_mg(void);

//class Hx711
//{
//public:
//	Hx711(uint8_t pin_din, uint8_t pin_slk);
//	virtual ~Hx711();
//	long getValue();
//	long averageValue(byte times = 32);
//	void setOffset(long offset);
//	void setScale(float scale = .5*742.f);
//	float getGram();
//
//private:
//	const uint8_t _pin_dout;
//	const uint8_t _pin_slk;
//	long _offset;
//	float _scale;
//};

#endif /* HX711_H_ */
