### Programing the Atmega

1. See https://www.heise.de/make/artikel/Arduino-Uno-als-In-System-Programmer-2769246.html?seite=2:
Für die Steuerung von MISO, MOSI und SCK dienen beim Arduino UNO die Pins 11, 12 und 13, den RST-Eingang des Mikrocontrollers steuert Pin 10.
(see arduino_uno_as_programmer.png)
Die Belegung 6-poliger und 10-poliger ISP-Anschlüsse für Atmel-Controller sind standardisiert.
Der unter dem Menüpunkt Beispiele verfügbare Sketch ArduinoISP steuert die Pins gemäß der Spezifizierung von Atmel.
Der Sketch wird über die normale Upload-Funktion auf den Arduino gebracht.
Beim Arduino Uno muss man für die weiteren Schritte einen Kondensator (Elko mit 10µF) zwischen GND und Reset legen. 
Er fängt den Reset-Impuls (Low) ab, den der USB-Seriell-Wandler auf dem Board des Arduino Uno beim Starten der seriellen Kommunikation sendet. 
Der Reset-Impuls führt nämlich dazu, dass der Mikrocontroller auf dem Arduino-Board seinen Bootloader zur Neuprogrammierung startet, statt den Arduino-Sketch zu starten.
Das Arduino-Board arbeitet jetzt selbst als Programmer.

2. set fusebits with "make fuse" (meaning of the fusebits used in the Makefile is described in used_fuse_bits_configuration_description.png)
3. run "make program"

### Debugging

The serial to USB(TTL) IC (either FTDI FT232RL or ATmega8U2) is connected to Arduino pins 0 and 1.
If we hold the processor in RESET mode, then all its GPIO pins will be in tri-state, effectively removing it from the circuit and you 
can directly interface the serial to USB IC with your device.
To hold the processor in RESET mode, just connect the RESET pin (which is active-low) with the ground.
This will hold the processor in RESET mode.
Now you can connect the serial device to the Arduino pins 0 and 1.
You should however note that the Rx pin (pin 0) goes to the Rx of the device and Tx pin (pin 1) goes to the Tx of the device and not the other way around.
In your computer the device will appear as a Virtual COM port. You can connect to it using programs like screen or hyper terminal.

1. run "make monitor" (runs the program "screen" (To end the session, press Ctrl+a followed by k))
