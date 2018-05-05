# -U lfuse:w:0x6d:m -U hfuse:w:0xdd:m -U efuse:w:0xf9:m

all:
#	avrdude -c avrisp -p atmega88P -P /dev/ttyUSB0 -b 4800 -U lfuse:w:0x6d:m -U hfuse:w:0xdd:m -U efuse:w:0xf9:m

	avrdude -c avrisp -p atmega88P -P /dev/ttyUSB0 -b 19200 -U flash:w:sketch_may05a.ino_atmega88p_8000000L.hex

