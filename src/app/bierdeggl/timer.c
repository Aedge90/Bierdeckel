	
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
	ASSR |= (1<<AS2);
	/*
	 * CS 	Pre
	 * 000 	inf
	 * 001	1
	 * 010	8
	 * 011	32
	 * 100	64
	 * 101	128
	 * 110	256
	 * 111	1024
	*/
	TCCR2B = (1<<CS22) | (1<<CS21) | (1<<CS20); // /1024

	TIMSK2 |= 1<<TOIE2;	
	
	systime = 0;
	ticks = 0;
	qms = 0;	
}

/*
    #define SLEEP_MODE_IDLE         0
    #define SLEEP_MODE_PWR_DOWN     1
    #define SLEEP_MODE_PWR_SAVE     2
    #define SLEEP_MODE_ADC          3
    #define SLEEP_MODE_STANDBY      4
    #define SLEEP_MODE_EXT_STANDBY  5


    #include <avr/interrupt.h>
    #include <avr/sleep.h>

    ...
      set_sleep_mode(<mode>);
      cli();
      if (some_condition)
      {
        sleep_enable();
        sleep_bod_disable();
        sei();
        sleep_cpu();
        sleep_disable();
      }
      sei();
*/

// t: 1s
void timer_wait(uint32_t t){
	uint32_t now = timer_get();

        //set_sleep_mode(SLEEP_MODE_IDLE);
   	set_sleep_mode(SLEEP_MODE_EXT_STANDBY);
        //set_sleep_mode(SLEEP_MODE_PWR_SAVE);	//Not sure if this would work in final dev..
        cli();
	while (timer_get() < now + t){
        	sleep_enable();
	        //sleep_bod_disable();
        	sei();
	        sleep_cpu();
	        sleep_disable();
	}	
        sei();

}
// t: 8ms
void timer_wait_16ms(uint32_t t){
	uint32_t now = ticks_get();

        //set_sleep_mode(SLEEP_MODE_IDLE);
   	set_sleep_mode(SLEEP_MODE_EXT_STANDBY);
        //set_sleep_mode(SLEEP_MODE_PWR_SAVE);	//Not sure if this would work in final dev..
        cli();
	while (ticks_get() < now + t){
        	sleep_enable();
	        //sleep_bod_disable();
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
