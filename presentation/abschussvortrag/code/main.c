int main(void)
{
	init();
	uint8_t n = 0;
	bool empty = false;
	// blink once to indicate initialization has finished
	led_show(25); timer_wait(1); led_show(0);
	for(;;){
		int32_t valg;
		if (isEmpty((valg = hx711_get_mg()/1000))){
			sleep16m(1); //prevent false empty indication while drinking	
			// blink while glass is empty
			while (isEmpty((valg = hx711_get_mg()/1000))){
				empty = true;
				led_blink(n);
			}
		}
		if (empty && isFull(valg)){
			++n;
			empty = false;
		}
		led_show(n);
		sleep(1);
	}
	return 0;
}
