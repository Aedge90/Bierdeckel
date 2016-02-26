#ifndef HX711_H_
#define HX711_H_

#define DDR_HX DDRC
#define PORT_HX PORTC
#define PIN_HX PINC
#define SCK (1<<PC1)
#define DT (1<<PC0)

#include <avr/io.h>
#include <util/delay.h>

void hx711_init(void);
uint32_t hx711_getValue(void);
uint32_t hx711_averageValue(uint8_t times);
int32_t hx711_get_g(void);
int32_t hx711_get_mg(void);

#endif /* HX711_H_ */
