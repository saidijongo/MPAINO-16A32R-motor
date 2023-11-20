const int relayPins[] = {54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65};
const int motor1pin1 = 12;
const int motor1pin2 = 11;

void setup() {
  for (int i = 0; i < sizeof(relayPins) / sizeof(relayPins[0]); i++) {
    pinMode(relayPins[i], OUTPUT);
    digitalWrite(relayPins[i], LOW); 
  }

  pinMode(motor1pin1, OUTPUT);
  pinMode(motor1pin2, OUTPUT);

  Serial.begin(115200);
}

void loop() {
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');
    processCommand(command);
  }
}

void processCommand(String command) {
  if (command == "TURN_ON_RELAYS_CW") {
    turnOnRelays();
    rotateMotorCW();
  } else if (command == "TURN_ON_RELAYS_CCW") {
    turnOnRelays();
    rotateMotorCCW();
  } else if (command == "TURN_OFF_RELAYS_CW") {
    turnOffRelays();
    rotateMotorCW();
  } else if (command == "TURN_OFF_RELAYS_CCW") {
    turnOffRelays();
    rotateMotorCCW();
  } else {
    Serial.println("Invalid command");
  }
}

void turnOnRelays() {
  for (int i = 0; i < sizeof(relayPins) / sizeof(relayPins[0]); i++) {
    digitalWrite(relayPins[i], HIGH); // Turn on all relays
  }
}

void turnOffRelays() {
  for (int i = 0; i < sizeof(relayPins) / sizeof(relayPins[0]); i++) {
    digitalWrite(relayPins[i], LOW); // Turn off all relays
  }
}

void rotateMotorCW() {
  digitalWrite(motor1pin1, HIGH);
  digitalWrite(motor1pin2, LOW);
  delay(3000); // Adjust the delay as needed for your motor
  stopMotor();
}

void rotateMotorCCW() {
  digitalWrite(motor1pin1, LOW);
  digitalWrite(motor1pin2, HIGH);
  delay(3000); // Adjust the delay as needed for your motor
  stopMotor();
}

void stopMotor() {
  digitalWrite(motor1pin1, LOW);
  digitalWrite(motor1pin2, LOW);
}
