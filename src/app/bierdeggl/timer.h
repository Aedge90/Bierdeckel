#ifndef _TIMER2_H_
#define _TIMER2_H_

#include <inttypes.h>

volatile uint32_t seconds;
volatile uint32_t ticks;

void timer_init(void);
void timer_wait(uint32_t t);
uint32_t timer_get(void);
uint32_t ticks_get(void);

#endif
