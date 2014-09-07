// SetId.ino

#include <EEPROM.h>

void setup() {
	Serial.begin(9600);
	randomSeed(analogRead(0));
	EEPROM.write(0, random(1,255));
}

void loop() {
	Serial.println("ID Set To: ");
	Serial.println(EEPROM.read(0));
	while (true) { delay(99999); }
}


