uint32_t hx711_averageValue(uint8_t times)
{
	uint8_t i;
	uint32_t sum = 0;
	for (i = 0; i < times; i++) {
		sum += hx711_getValue();
	}
	return sum / times;
}

int32_t hx711_get_mg(void) {
	int32_t val = (hx711_averageValue(32) - _offset);
	return (1000 *  val) / _scale; 
}
