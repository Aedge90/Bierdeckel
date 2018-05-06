	
#include "timer.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

// F_CPU / 2**8 / Prescaler
ISR(TIMER2_OVF_vect){
	if (++qms >= 61){ // 100ms -> 128/10
		qms = 0;
		++systime; //1 Hz
	}
	++ticks; //61 Hz
	//RFM_GetMode();
}

void timer_init(void){
	
	// When AS2 is written to one, Timer/Counter2 is clocked from a crystal Oscillator
	// connected to the Timer Oscillator 1 (TOSC1) pin
	ASSR |= (1<<AS2);
	
	// The three Clock Select bits select the clock source to be used by the Timer/Counter
	/*
	 * CS22 CS21 CS20 Prescaling
	 * 0    0    0    No clock source (Timer/Counter stopped)
	 * 0    0    1    clk_T2S/1 (no prescaling)
	 * 0    1    0    clk_T2S/8
	 * 0    1    1    clk_T2S/32
	 * 1    0    0    clk_T2S/64
	 * 1    0    1    clk_T2S/128
	 * 1    1    0    clk_T2S/256
	 * 1    1    1    clk_T2S/1024
	*/
	TCCR2B = (1<<CS20);

	TIMSK2 |= 1<<TOIE2;	
	
	systime = 0;
	ticks = 0;
	qms = 0;	
}

/*
Sleep Modes:
    #define SLEEP_MODE_IDLE         0
    #define SLEEP_MODE_PWR_DOWN     1
    #define SLEEP_MODE_PWR_SAVE     2
    #define SLEEP_MODE_ADC          3
    #define SLEEP_MODE_STANDBY      4
    #define SLEEP_MODE_EXT_STANDBY  5
*/

//Not sure if this would work without ext oszi..
//..in this case use SLEEP_MODE_EXT_STANDBY
#define _SLEEP_MODE SLEEP_MODE_PWR_SAVE  

// t: Time in seconds
void timer_wait(uint32_t t){
	uint32_t now = timer_get();
	set_sleep_mode(_SLEEP_MODE);
	cli();
	while (timer_get() < now + t){
		sleep_enable();
		sleep_bod_disable();
		sei();
		sleep_cpu();
		sleep_disable();
	}
	sei();
}

uint32_t timer_get(void){
	return systime;
}

uint32_t ticks_get(void){
	return ticks;
}

