	
#include "timer.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>


// The Overflow Interrupt Handler is called when TCNT2 is set from 255 to 0 wechselt (256 Cycles)
// So this is called F_CPU / 256 / Prescaler per second
// (F_CPU, which was set in the Makefile = the Clock Speed of the external Oscillator)
// (256, because TCNT2 is 8 bit; Prescaler, which divides the input clock)
ISR(TIMER2_OVF_vect){
	ticks++;
	// prescaler is 1024, so this is called 4194304/256/1024 = 16 times per second
	seconds = ticks/16;
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
	TCCR2B = (1<<CS22) | (1<<CS21) | (1<<CS20); // /1024

	TIMSK2 |= 1<<TOIE2;	
	
	seconds = 0;
	ticks = 0;
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
	uint32_t now = seconds;
	set_sleep_mode(_SLEEP_MODE);
	cli();
	while (seconds < now + t){
		sleep_enable();
		sleep_bod_disable();
		sei();
		sleep_cpu();
		sleep_disable();
	}
	sei();
}

