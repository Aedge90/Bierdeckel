#ifndef _HX711_H_
#define _HX711_H_

#define SDI (1<<PC0)
#define SCL (1<<PC1)
//#define SDI (1<<PC2)
//#define SCL (1<<PC3)

#include <avr/io.h>
#include <util/delay.h>

void hx711_init(void);
uint32_t hx711_getValue(void);
uint32_t hx711_averageValue(uint8_t times);
float hx711_getGram(void);
int32_t hx711_get_mg(void);

uint32_t _offset;
float _scale;

#endif /* HX711_H_ */
