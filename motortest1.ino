
#include <Arduino.h>
#include <Servo.h>

const int motorPins[] = {54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85};
const int numPumps = sizeof(motorPins) / sizeof(motorPins[0]);
const int FLOW_RATE = 1.5; // in ml/minute

Servo servoMotor;
const int stepperMotorPUL = 1;
const int stepperMotorDIR = 2;
const int servoMotorPwm = 3;
const int ledStripPwm = 4;

unsigned long pumpStartTimes[numPumps] = {0};

void activateMotor(int pin) {
  digitalWrite(pin, HIGH);
}

void deactivateMotor(int pin) {
  digitalWrite(pin, LOW);
}

void activateServo(int angle, int speed) {
  int pwmValue = map(angle, 0, 180, 544, 2400);
  servoMotor.writeMicroseconds(pwmValue);
  delay(speed);
}

void activateStepper(int angle, int speed) {
  digitalWrite(stepperMotorDIR, angle < 0 ? HIGH : LOW);

  for (int i = 0; i < abs(angle); i++) {
    digitalWrite(stepperMotorPUL, HIGH);
    delayMicroseconds(speed);
    digitalWrite(stepperMotorPUL, LOW);
    delayMicroseconds(speed);
  }
}

void activateLEDStrip(int pattern, int onOff) {
  // Add logic to control LED strip based on pattern and onOff values
}

void processCommand(String command) {
  char separator = ',';

  int index = command.indexOf('(');

  if (index != -1) {
    String motorType = command.substring(1, index);

    if (motorType == "PUMPMOTOR_OPERATION" ||
        motorType == "STEPPERMOTOR_OPERATION" ||
        motorType == "SERVOMOTOR_OPERATION" ||
        motorType == "LEDSTRIP_OPERATION") {

      int endIndex = command.indexOf(')', index + 1);

      if (endIndex != -1) {
        String motorData = command.substring(index + 1, endIndex);
        int commaIndex = motorData.indexOf(separator);

        if (commaIndex != -1) {
          int param1 = motorData.substring(0, commaIndex).toInt();
          int param2 = motorData.substring(commaIndex + 1).toInt();

          if (motorType == "PUMPMOTOR_OPERATION" && param1 >= 54 && param1 <= 85) {
            activateMotor(motorPins[param1 - 54]);
            pumpStartTimes[param1 - 54] = millis() + param2; // Set the time when the pump should be turned off
            Serial.print("Mixing pump: ");
            Serial.println(param1);
          } else if (motorType == "STEPPERMOTOR_OPERATION") {
            activateStepper(param1, param2);
          } else if (motorType == "SERVOMOTOR_OPERATION") {
            activateServo(param1, param2);
          } else if (motorType == "LEDSTRIP_OPERATION") {
            activateLEDStrip(param1, param2);
          } else {
            Serial.println("Invalid motor type or parameters");
          }
        } else {
          Serial.println("Invalid command format");
        }
      } else {
        Serial.println("Invalid command format");
      }
    } else {
      Serial.println("Invalid motor type");
    }
  }
}

void setup() {
  for (int i = 0; i < numPumps; i++) {
    pinMode(motorPins[i], OUTPUT);
  }

  servoMotor.attach(servoMotorPwm);
  pinMode(stepperMotorPUL, OUTPUT);
  pinMode(stepperMotorDIR, OUTPUT);
  
  Serial.begin(115200);
}

void loop() {
  for (int i = 0; i < numPumps; i++) {
    if (millis() >= pumpStartTimes[i] && digitalRead(motorPins[i]) == HIGH) {
      deactivateMotor(motorPins[i]); // Turn off the pump when the run time has elapsed
    }
  }

  if (Serial.available() > 0) {
    String data = Serial.readStringUntil('\n');
    processCommand(data);
  }
}
