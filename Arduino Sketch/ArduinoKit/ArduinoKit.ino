// ArduinoSketch.ino

#include <EEPROM.h>

#define HEARTBEAT_TRIGGER_TIME 700
#define HEARTBEAT_CUTOFF_TIME 1150

const int PIN_MODES[3] = { OUTPUT, INPUT, INPUT_PULLUP };
const int A_PINS[6]    = { A0, A1, A2, A3, A4, A5 };
const int D_PINS[14]    = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13 };
const int D_STATES[2]  = { LOW, HIGH };
int heartbeat_received_at = millis();
int heartbeat_sent_at = millis();

void setup() {
  Serial.begin(9600);
  pinMode(13, OUTPUT);
  connect();
}

char data[9];
int index = 0;

void loop() {
  if((millis() - heartbeat_received_at) > (HEARTBEAT_CUTOFF_TIME * 3)){
//    connect(); // Dissconnected, So Connect.
  }
  if(millis() - heartbeat_sent_at > HEARTBEAT_TRIGGER_TIME){
    sendHeartbeat();
  }
  if(Serial.peek() != -1){
    if (Serial.peek() == 'h'){
      Serial.read();
      parseInput("h");
      return;
    }
    if (Serial.peek() == '-'){
      Serial.read();
      parseInput("-");
      return;
    }
    if (Serial.peek() == ';'){
      Serial.read();
      parseInput(data);
      setAllIndexesTo(0, data);
      index = 0;
    } else {
      data[index] = Serial.read();
      index++;
    }
  }
}

void connect() {
        setAllPinsToLow(2); // Start with 2, because 1 & 0 are being used for Serial.
	while(!Serial.available()) {
            digitalWrite(13, LOW);
            delay(50);
	    Serial.println("ArduinoKit");
            digitalWrite(13, HIGH);
            delay(50);
	}
	if (Serial.read() != '-') {
          connect();
          return;
        }
	Serial.println(EEPROM.read(0));
	Serial.println("-");
	heartbeat_received_at = millis();
	heartbeat_sent_at = millis();
}

void parseInput(char* input){
        char firstChar = (input[0]);
	if(firstChar == '-'){
	    connect();
	} else if(firstChar == 'h'){
	    heartbeat();
	} else if(firstChar == '0'){
	    parseCommand(input);
	} else if(firstChar == '1'){
	    parsePinMode(input);
	}
}

void heartbeat() {
  heartbeat_received_at = millis(); 
}

void sendHeartbeat() {
  Serial.println("h");
  heartbeat_sent_at = millis();
}

void setAllPinsToLow(int startingPin){
  for(int i = startingPin; i < sizeof(D_PINS) - 1; i++) {
    digitalWrite(D_PINS[i], LOW);
  }
}

void parseCommand(char* command) {
	bool w = parseBool(command[1]);
	bool digital = !parseBool(command[2]);
	char pinStr[3] = {(command[3]), (command[4]), 0};
	int pin = atoi(pinStr);
	if(w){
	    char valueStr[5] = {(command[5]), (command[6]), (command[7]), (command[8]), 0};
	    int newValue = atoi(valueStr);
            (digital) ? digitalWrite(pin, D_STATES[newValue]) : analogWrite(pin, newValue/4);
	} else {
	    Serial.println((digital) ? digitalRead(pin) : analogRead(A_PINS[pin]));
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
