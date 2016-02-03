#ifndef _TIMER2_H_
#define _TIMER2_H_

#include <inttypes.h>

volatile uint32_t systime;
volatile uint32_t ticks;
volatile uint8_t qms;

void timer_init(void);
// t: 1s
void sleep(uint32_t t);
void sleep16m(uint32_t t);
void timer_wait(uint32_t t);
void timer_wait_16ms(uint32_t t);
uint32_t timer_get(void);
uint32_t ticks_get(void);

#endif
