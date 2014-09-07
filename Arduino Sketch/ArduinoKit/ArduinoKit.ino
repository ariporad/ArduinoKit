// ArduinoSketch.ino

#include <EEPROM.h>

#define HEARTBEAT_TRIGGER_TIME 700
#define HEARTBEAT_CUTOFF_TIME 115

int hearbeat_received_at = millis();

void setup() {
	Serial.begin(9600);
	connect();
}

void loop() {
	if(Serial.available()){
	   	char input[15];
	   	Serial.readBytesUntil('\n', input, 15);
	   	parseInput(input);
	}
}

void connect() {
	while(!Serial.available()) {
	    Serial.println("ArduinoKit\n");
	}
	Serial.read();
	Serial.println(EEPROM.read(0));
	Serial.println("-");
	hearbeat_received_at = millis();
}

void parseInput(char* input){
	if(&input[0] == "-"){
	    connect();
	} else if(&input[0] == "0"){
	    heartbeat();
	} else if(&input[0] == "1"){
	    parseCommand(input);
	} else if(&input[0] == "2"){
	    parsePinMode(input);
	}
}

void heartbeat() {
	hearbeat_received_at = millis();
}

void parseCommand(char* command) {
	bool write = parseBool(command[1]);
	bool digital = parseBool(command[2]);
	char pinStr[3] = {(command[3]), (command[4]), 0};
	int pin = atoi(pinStr);
	if(write){
		char valueStr[5] = {(command[5]), (command[6]), (command[7]), (command[8]), 0};
	    int newValue = atoi(valueStr);
	    (digital) ? digitalWrite(pin, newValue) : analogWrite(pin, newValue);
	} else {
		Serial.println((digital) ? digitalRead(pin) : analogRead(pin));
	}
}

void parsePinMode(char* command) {
	char pinNumStr[3] = { command[1], command[2], 0 };
	int pin = atoi(pinNumStr);
	char inputMode[2] = { command[3], 0 };
	int mode = atoi(inputMode);
	int modes[3] = { OUTPUT, INPUT, INPUT_PULLUP };
	pinMode(pin, modes[mode]);
}

bool parseBool(char b) { return &b == "1"; }