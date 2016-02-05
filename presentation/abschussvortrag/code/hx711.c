void hx711_init(void) {
	DDR_HX |= SCLK;
	DDR_HX &= ~SDI;
	//reset HX711
	PORT_HX |= SCLK;
	_delay_us(100);
	PORT_HX &= ~SCLK;
	//callibration _offset is weight of empty glass
	_offset = hx711_averageValue(32);
}
uint32_t hx711_getValue() {
	uint8_t i = 0;
	uint32_t data = 0;
	//Wait until data is ready
	while (PIN_HX & SDI);
	//read data bit by bit by sending 24 positive clock pulses to PD_SCK pin
	for (i = 24;i--;) {
		PORT_HX |= SCLK;
		data |= (uint32_t)((PINC & SDI)?1:0)<<i;
		PORT_HX &= ~SCLK;
	}
	//25th pulse sets DOUT (=SDI) pin back to HIGH
	PORT_HX |= SCLK;
	PORT_HX &= ~SCLK;
	return data;	
}
