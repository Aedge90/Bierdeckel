 void hx711_init(void)
 {
    DDR_HX |= SCLK;
    DDR_HX &= ~SDI;
 
    //reset HX711
    PORT_HX |= SCLK;
    _delay_us(100);
    PORT_HX &= ~SCLK;
 
    hx711_averageValue(32);
    //callibration
    //_offset is weight of empty glas
    offset = hx711_averageValue(32);
    _scale = 371;
 }

