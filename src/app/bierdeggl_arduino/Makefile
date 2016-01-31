BOARD=uno
# Version-specific settings
ARD_HOME=/opt/arduino-1.6.7
ARD_BOARDS = $(ARD_HOME)/hardware/arduino/avr/boards.txt
ARD_SRC_DIR = $(ARD_HOME)/hardware/arduino/avr/cores/arduino
ARD_MAIN = $(ARD_SRC_DIR)/main.cpp

# Standard macros.
SKETCH = $(notdir $(CURDIR))
BUILD_DIR = build
VPATH = $(LIB_DIRS)

# Macros derived from boards.txt
MCU := $(shell sed -n 's/$(BOARD)\.build\.mcu=\(.*\)/\1/p' < $(ARD_BOARDS))
F_CPU := $(shell sed -n 's/$(BOARD)\.build\.f_cpu=\(.*\)/\1/p' < $(ARD_BOARDS))
UPLOAD_SPEED := \
	$(shell sed -n 's/$(BOARD)\.upload\.speed=\(.*\)/\1/p' < $(ARD_BOARDS))

# CPP_SOURCE_FILES is a previously-defined variable, holding my project's source files
CPP_SOURCE_FILES += $(filter-out $(ARD_SRC_DIR)/main.cpp $(ARD_SRC_DIR)/Stream.cpp $(ARD_SRC_DIR)/Tone.cpp, $(wildcard $(ARD_SRC_DIR)/*.cpp))
C_SOURCE_FILES += $(filter-out $(ARD_SRC_DIR)/wiring_pulse.c, $(wildcard $(ARD_SRC_DIR)/*.c))

DEVICE=atmega328p
PROGRAMMER=arduino
PORT=/dev/ttyUSB0
MON_CMD=screen
MON_SPEED=19200
AVRDUDE = avrdude -c $(PROGRAMMER) -p $(DEVICE)
# The two lines above are for "avrdude" and the STK500 programmer connected
# to an USB to serial converter to a Mac running Mac OS X.
# Choose your favorite programmer and interface.

COMPILE = avr-gcc -Wall -Os -Iusbdrv -I. -I$(ARD_SRC_DIR) -mmcu=$(DEVICE) -DF_CPU=16000000 -DDEBUG_LEVEL=0
COMPILE_FLAGS = -Wall -Os -Iusbdrv -I. -I$(ARD_SRC_DIR) -l-android-core-d -mmcu=$(DEVICE) -DF_CPU=16000000 -DDEBUG_LEVEL=0
# NEVER compile the final product with debugging! Any debug output will
# distort timing so that the specs can't be met.

OBJECTS =
OBJECTS += hx711.o
OBJECTS += main.o
OBJECTS += $(CPP_SOURCE_FILES)
OBJECTS += $(C_SOURCE_FILES)
#OBJECTS += $(ARD_SRC_DIR)/HardwareSerial*.cpp
#OBJECTS += $(ARD_SRC_DIR)/Print.cpp

# symbolic targets:
all:	main.hex

.c.o:
	echo avr-gcc $(COMPILE_FLAGS) -c $< -o $@
	avr-gcc $(COMPILE_FLAGS) -c $< -o $@

.cpp.o:
	echo avr-g++ $(COMPILE_FLAGS) -c $< -o $@
	avr-g++ $(COMPILE_FLAGS) -c $< -o $@

.S.o:
	echo avr-gcc $(COMPILE_FLAGS) -x assembler-with-cpp -c $< -o $@
	avr-gcc $(COMPILE_FLAGS) -x assembler-with-cpp -c $< -o $@
# "-x assembler-with-cpp" should not be necessary since this is the default
# file type for the .S (with capital S) extension. However, upper case
# characters are not always preserved on Windows. To ensure WinAVR
# compatibility define the file type manually.

.c.s:
	echo avr-gcc $(COMPILE) -S $< -o $@
	avr-gcc $(COMPILE) -S $< -o $@

.cpp.s:
	echo avr-g++ $(COMPILE) -S $< -o $@
	avr-g++ $(COMPILE) -S $< -o $@

flash:	all
	- pkill -f '$(MON_CMD).*$(PORT)'
	- sleep 1
	- stty -F $(PORT) hupcl
	- $(AVRDUDE) -P$(PORT) -b$(UPLOAD_SPEED) -U flash:w:main.hex:i


# Fuse high byte:
# 0xdd = 1 1 0 1   1 1 0 1
#        ^ ^ ^ ^   ^ \-+-/ 
#        | | | |   |   +------ BODLEVEL 2..0 (brownout trigger level -> 2.7V)
#        | | | |   +---------- EESAVE (preserve EEPROM on Chip Erase -> not preserved)
#        | | | +-------------- WDTON (watchdog timer always on -> disable)
#        | | +---------------- SPIEN (enable serial programming -> enabled)
#        | +------------------ DWEN (debug wire enable)
#        +-------------------- RSTDISBL (disable external reset -> enabled)
#
# Fuse low byte:
# 0xe1 = 1 1 1 0   0 0 0 1
#        ^ ^ \+/   \--+--/
#        | |  |       +------- CKSEL 3..0 (clock selection -> HF PLL)
#        | |  +--------------- SUT 1..0 (BOD enabled, fast rising power)
#        | +------------------ CKOUT (clock output on CKOUT pin -> disabled)
#        +-------------------- CKDIV8 (divide clock by 8 -> don't divide)
#fuse:
#	$(AVRDUDE) -U hfuse:w:0xde:m -U lfuse:w:0xff:m -U efuse:w:0xf9:m
#	$(AVRDUDE) -U hfuse:w:0xdd:m -U lfuse:w:0xff:m -U efuse:w:0xfe:m
#	$(AVRDUDE) -U hfuse:w:0xdd:m -U lfuse:w:0xe1:m

readcal:
	$(AVRDUDE) -U calibration:r:/dev/stdout:i | head -1

monitor :
	$(MON_CMD) $(PORT) $(MON_SPEED)


clean:
	rm -f main.hex main.lst main.obj main.cof main.list main.map main.eep.hex main.bin *.o usbdrv/*.o main.s usbdrv/oddebug.s usbdrv/usbdrv.s *.o

# file targets:
main.bin:	$(OBJECTS)
	$(COMPILE) -o main.bin $(OBJECTS)

main.hex:	main.bin
	rm -f main.hex main.eep.hex
	avr-objcopy -j .text -j .data -O ihex main.bin main.hex
	avr-size -C --mcu=$(DEVICE) main.bin
# do the checksize script as our last action to allow successful compilation
# on Windows with WinAVR where the Unix commands will fail.

disasm:	main.bin
	avr-objdump -d main.bin

cpp:
	$(COMPILE) -E main.c
