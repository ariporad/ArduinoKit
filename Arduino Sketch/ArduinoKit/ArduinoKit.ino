// ArduinoSketch.ino

#include <EEPROM.h>

#define HEARTBEAT_TRIGGER_TIME 700
#define HEARTBEAT_CUTOFF_TIME 115

const int PIN_MODES[3] = { OUTPUT, INPUT, INPUT_PULLUP };
const int A_PINS[6]    = { A0, A1, A2, A3, A4, A5 };
const int D_STATES[2]  = { LOW, HIGH };
int hearbeat_received_at = millis();

void setup() {
	Serial.begin(9600);
	connect();
}

char data[9];
int index = 0;

void loop() {
  if(Serial.peek() != -1){
    if (Serial.peek() == ';'){
      Serial.read();
      parseInput(data);
      setAllIndexesTo(0, data);
      index = 0;
    } else {
      data[index] = Serial.read();
      index++;
      Serial.println(data);
    }
  }
}

void connect() {
	while(!Serial.available()) {
	    Serial.println("ArduinoKit;");
	}
	Serial.read();
	Serial.print(EEPROM.read(0));
        Serial.println(";");
	Serial.print("-");
        Serial.println(";");
	hearbeat_received_at = millis();
}

void parseInput(char* input){
        char firstChar = (input[0]);
	if(firstChar == '-'){
            Serial.println("diss");
	    connect();
	} else if(firstChar == '0'){
            Serial.println("beat");
	    heartbeat();
	} else if(firstChar == '1'){
            Serial.println("cmd");
	    parseCommand(input);
	} else if(firstChar == '2'){
            Serial.println("pmd");
	    parsePinMode(input);
	} else {
            Serial.println("else");
            Serial.println(firstChar);
        }
}

void heartbeat() {
	hearbeat_received_at = millis();
}

void parseCommand(char* command) {
	bool w = parseBool(command[1]);
	bool digital = !parseBool(command[2]);
	char pinStr[3] = {(command[3]), (command[4]), 0};
	int pin = atoi(pinStr);
	if(w){
	    char valueStr[5] = {(command[5]), (command[6]), (command[7]), (command[8]), 0};
	    int newValue = atoi(valueStr);
            Serial.print("I am going to write ");
            Serial.print(newValue);
            Serial.print(" to ");
            Serial.print(((digital) ? "digital" : "anolouge"));
            Serial.print(" pin ");
	    Serial.println(pin);
            (digital) ? digitalWrite(pin, D_STATES[newValue]) : analogWrite(pin, newValue/4);
	} else {
	    Serial.print((digital) ? digitalRead(pin) : analogRead(A_PINS[pin]));
            Serial.println(";");
	}
}

void parsePinMode(char* command) {
	char pinNumStr[3] = { command[1], command[2], 0 };
	int pin = atoi(pinNumStr);
	char inputMode[2] = { command[3], 0 };
	int mode = atoi(inputMode);
	pinMode(pin, PIN_MODES[mode]);
}

bool parseBool(char b) { return b == '1'; }

void setAllIndexesTo(char d, char* a) {
  for(int i = 0; i < sizeof(a) - 1; i++){
    a[i] = d;
  }
}
