#include "usbprint.h"
#include <avr/io.h> 
#include <avr/wdt.h>
#include <avr/eeprom.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

#include "usbdrv.h"
#include "oddebug.h"

/* ------------------------------------------------------------------------- */

static uchar    reportBuffer[2];    /* buffer for HID reports */
static uchar    idleRate;           /* in 4 ms units */

static uchar    adcPending;
static uchar    isRecording;

static uchar    valueBuffer[16];
static uchar    *nextDigit;

/* ------------------------------------------------------------------------- */

const PROGMEM char usbHidReportDescriptor[USB_CFG_HID_REPORT_DESCRIPTOR_LENGTH] = { /* USB report descriptor */
    0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
    0x09, 0x06,                    // USAGE (Keyboard)
    0xa1, 0x01,                    // COLLECTION (Application)
    0x05, 0x07,                    //   USAGE_PAGE (Keyboard)
    0x19, 0xe0,                    //   USAGE_MINIMUM (Keyboard LeftControl)
    0x29, 0xe7,                    //   USAGE_MAXIMUM (Keyboard Right GUI)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x25, 0x01,                    //   LOGICAL_MAXIMUM (1)
    0x75, 0x01,                    //   REPORT_SIZE (1)
    0x95, 0x08,                    //   REPORT_COUNT (8)
    0x81, 0x02,                    //   INPUT (Data,Var,Abs)
    0x95, 0x01,                    //   REPORT_COUNT (1)
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0x25, 0x65,                    //   LOGICAL_MAXIMUM (101)
    0x19, 0x00,                    //   USAGE_MINIMUM (Reserved (no event indicated))
    0x29, 0x65,                    //   USAGE_MAXIMUM (Keyboard Application)
    0x81, 0x00,                    //   INPUT (Data,Ary,Abs)
    0xc0                           // END_COLLECTION
};
/* We use a simplifed keyboard report descriptor which does not support the
 * boot protocol. We don't allow setting status LEDs and we only allow one
 * simultaneous key press (except modifiers). We can therefore use short
 * 2 byte input reports.
 * The report descriptor has been created with usb.org's "HID Descriptor Tool"
 * which can be downloaded from http://www.usb.org/developers/hidpage/.
 * Redundant entries (such as LOGICAL_MINIMUM and USAGE_PAGE) have been omitted
 * for the second INPUT item.
 */

static void buildReport(void)
{
uchar   key = 0;

    if(nextDigit != NULL){
        key = *nextDigit;
    }
    reportBuffer[0] = 0;    /* no modifiers */
    reportBuffer[1] = key;
}

static uchar    printing = 0;
char buffer[128];
uint8_t buf_wpos = 0;
uint8_t buf_rpos = 0;
void putChar(char c)
{

	if ((buf_wpos+1 == buf_rpos) || (buf_wpos == 127 && buf_rpos == 0)) usbprint_flush();	

	if (c == 'z') c = 'y';
	if (c == 'y') c = 'z';
	if (c == 'Z') c = 'Y';
	if (c == 'Y') c = 'Z';

    if (c >= 'A' && c <= 'Z')
	buffer[buf_wpos++] = c-61;
    else if (c >= 'a' && c <= 'z')
	buffer[buf_wpos++] = c-93;
    else if (c >= '0' && c <= '9')
	buffer[buf_wpos++] = c-19;
    else if (c == '\n'){
	buffer[buf_wpos++] = KEY_RETURN;
	usbprint_flush();
    }
    else if (c == '\b')
		buffer[buf_wpos++] = KEY_BACKSPACE;
    if (buf_wpos == 128) buf_wpos = 0;
    return;
}

void putInt(int32_t ival)
{
	uint32_t div, val;
	uint8_t base = 10;
    uchar   digit;
	if (0 > ival){
		putString("minus");
		val = -ival;
	} else {
		val = ival;
	}
	for (div = 1; val/div >= (uint32_t) base; div *= base); 
     for (; div > 0; div /= (uint32_t) base) {                              
         digit = val / div;                                                     
         if (digit < 10)                                                         
             putChar('0' + digit);                                                   
         else                                                                    
             putChar('a' + digit - 10);                                              
         val %= div;                                                            
     }   
	return;
}
void putString(char *str)
{
	char c;
	uint8_t pos = 0;
	while ('\0' != (c = *(str+pos))){
		putChar(c);
		if (++pos >=64) break; 
	}	
}
static void evaluateADC(unsigned int value)
{
    nextDigit = &valueBuffer[sizeof(valueBuffer)];
    *--nextDigit = 0xff;/* terminate with 0xff */
    *--nextDigit = 0;
    *--nextDigit = value;
    return;
}

/* ------------------------------------------------------------------------- */

static void setIsRecording(uchar newValue)
{
    isRecording = newValue;
    if(!isRecording){
        PORTB |= 1 << BIT_LED;      /* LED off */
    }else{
        PORTB &= ~(1 << BIT_LED);   /* LED on */
    }
}

/* ------------------------------------------------------------------------- */

//static void keyPoll(void)
//{
//static uchar    keyMirror;
//uchar           key;
//
//    key = PINC & (1 << BIT_KEY);
//	printing = key;
//    if(keyMirror != key){   /* status changed */
//        keyMirror = key;
//        if(!key){           /* key was pressed */
//printing ^= 1;
//            setIsRecording(!isRecording);
//        }
//    }
//}

static void adcPoll(void)
{
   static unsigned int vvv = 0;
    if(adcPending && !(ADCSRA & (1 << ADSC))){
        adcPending = 0;
        vvv ++;
	if ((buf_rpos == buf_wpos)) return;
        evaluateADC(buffer[buf_rpos++]);
    }
}

static void timerPoll(void)
{
static uchar timerCnt;

    if(TIFR0 & (1 << TOV0)){
        TIFR0 = (1 << TOV0); /* clear overflow */
        //keyPoll();
        //if(++timerCnt >= 32){       /* ~ 0.5 second interval */
        if(++timerCnt >= 8){       /* ~ 0.5 second interval */
            timerCnt = 0;
            if(1 /*|| isRecording*/){
                adcPending = 1;
                ADCSRA |= (1 << ADSC);  /* start next conversion */
            }
        }
    }
}

/* ------------------------------------------------------------------------- */

static void timerInit(void)
{
    TCCR0B = 0x0d;           /* select clock: 16.5M/1k -> overflow rate = 16.5M/256k = 62.94 Hz */
}

/* ------------------------------------------------------------------------- */
/* ------------------------ interface to USB driver ------------------------ */
/* ------------------------------------------------------------------------- */

uchar	usbFunctionSetup(uchar data[8])
{
usbRequest_t    *rq = (void *)data;

    usbMsgPtr = reportBuffer;
    if((rq->bmRequestType & USBRQ_TYPE_MASK) == USBRQ_TYPE_CLASS){    /* class request type */
        if(rq->bRequest == USBRQ_HID_GET_REPORT){  /* wValue: ReportType (highbyte), ReportID (lowbyte) */
            /* we only have one report type, so don't look at wValue */
            buildReport();
            return sizeof(reportBuffer);
        }else if(rq->bRequest == USBRQ_HID_GET_IDLE){
            usbMsgPtr = &idleRate;
            return 1;
        }else if(rq->bRequest == USBRQ_HID_SET_IDLE){
            idleRate = rq->wValue.bytes[1];
        }
    }else{
        /* no vendor specific requests implemented */
    }
	return 0;
}

void    usbEventResetReady(void)
{
    /* Disable interrupts during oscillator calibration since
     * usbMeasureFrameLength() counts CPU cycles.
     */
    //cli();
    //calibrateOscillator();
    //sei();
    //eeprom_write_byte(0, OSCCAL);   /* store the calibrated value in EEPROM */
}

void usbprint_setup(void)
{
    uchar   i;
    
    odDebugInit();
    usbDeviceDisconnect();
    for(i=0;i<20;i++){  /* 300 ms disconnect */
        _delay_ms(15);
    }
    usbDeviceConnect();
    DDRB |= 1 << BIT_LED;   /* output for LED */
    PORTB |= 1 << BIT_LED;  // led off
    //PORTC |= 1 << BIT_KEY;  /* pull-up on key input */
    wdt_enable(WDTO_1S);
    timerInit();
    usbInit();
    sei();

}

void usbprint_flush(void)
{
    while(buf_wpos != buf_rpos){
        wdt_reset();
        usbPoll();        
        if(usbInterruptIsReady() && nextDigit != NULL){ /* we can send another key */
            buildReport();
            usbSetInterrupt(reportBuffer, sizeof(reportBuffer));
            if(*++nextDigit == 0xff)    /* this was terminator character */
                nextDigit = NULL;
        }
        timerPoll();
        adcPoll();        
    }
	buf_wpos = 0;
	buf_rpos = 0;

}
