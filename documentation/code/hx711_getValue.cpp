 uint32_t hx711_getValue()
 {
    uint8_t i = 0;
    uint32_t data = 0;

    //When output data isnt ready
    //for retrieval, digital output pin
    //DOUT (=SDI) is high
    while (PIN_HX & SDI);

    //read data bit by bit by sending 24
    //positive clock pulses to PD_SCK pin
    for (i = 24;i--;)
    {
        PORT_HX |= SCLK;
        data |= 
            (uint32_t)((PINC & SDI)?1:0)<<i;
        PORT_HX &= ~SCLK;
    }
 
    //25th pulse sets DOUT (=SDI) 
    //pin back to HIGH
    PORT_HX |= SCLK;
    PORT_HX &= ~SCLK;
 
    //no other pulses are sent 
    //so we will be using input channel A
    //with a gain of 128
 
    data ^= 0x800000;
 
    return data;	
 }
