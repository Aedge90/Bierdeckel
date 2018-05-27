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


