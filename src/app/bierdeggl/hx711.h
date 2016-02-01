#ifndef HX711_H_
#define HX711_H_

#define DDR_HX DDRC
#define PORT_HX PORTC
#define PIN_HX PINC
#define SDI (1<<PC0)
#define SCLK (1<<PC1)

#include <avr/io.h>
#include <util/delay.h>

void hx711_init(void);
uint32_t hx711_getValue(void);
uint32_t hx711_averageValue(uint8_t times);
float hx711_getGram(void);
int32_t hx711_get_mg(void);

#endif /* HX711_H_ */