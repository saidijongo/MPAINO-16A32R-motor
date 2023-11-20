
#include <Servo.h>
#include <Stepper.h>
#include <Arduino.h>


int motorPins[] = {54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85};

#define PUL 1
#define DIR 2
#define SERVO_PIN 3 
#define LED_PIN 4

Stepper stepper(200, PUL, DIR);
Servo servo;

void setup() {
  for(int i = 0; i < 85; i++) {
    pinMode(motorPins[i], OUTPUT);
  }

  pinMode(PUL, OUTPUT);
  pinMode(DIR, OUTPUT); 
  pinMode(SERVO_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);

  servo.attach(SERVO_PIN);

  Serial.begin(115200);
}

void loop() {
  if(Serial.available() > 0) {
    String data = Serial.readStringUntil('\n');
    Serial.println(data);
  
    String command = data.substring(0, data.indexOf(")"));

    if(command == "('PUMPMOTOR_OPERATION')") 
      controlPumps(data);
    else if(command == "('STEPPERMOTOR_OPERATION')")
      controlStepper(data);
    else if(command == "('SERVOMOTOR_OPERATION')")   
      controlServo(data);
    else if(command == "('LEDSTRIP_OPERATION')")
      controlLED(data);
  }
}

void controlPumps(String params){

  String pumpsStr = params.substring(params.indexOf(")")+2);

  while(pumpsStr.length() > 0) {

    String pumpPair = pumpsStr.trim().substring(0,pumpsStr.indexOf("("));

    int pump = pumpPair.toInt();
    int duration = pumpPair.substring(pumpPair.indexOf(",")+1).toInt();

    Serial.println(pump);
    Serial.println(duration);

    digitalWrite(motorPins[pump], HIGH);
    delay(duration); 
    digitalWrite(motorPins[pump], LOW);

    pumpsStr = pumpsStr.substring(pumpsStr.indexOf(")")+1);
  }

}

void controlStepper(String params){

  String stepperParams = params.substring(params.indexOf(")")+2);

  while(stepperParams.length() > 0){

    String stepPair = stepperParams.substring(0,stepperParams.indexOf("("));

    int angle = stepPair.toInt();
    int speed = stepPair.substring(stepPair.indexOf(",")+1).toInt();

    stepper.setSpeed(speed); 
    stepper.step(angle);

    stepperParams = stepperParams.substring(stepperParams.indexOf(")")+1);

  }

}

void controlServo(String params){

  String servoParams = params.substring(params.indexOf(")")+2);

  while(servoParams.length() > 0){

    String servoPair = servoParams.substring(0,servoParams.indexOf("("));

    int angle = servoPair.toInt();
    int speed = servoPair.substring(servoPair.indexOf(",")+1).toInt();

    Serial.println(angle);
    Serial.println(speed);

    servo.write(angle);
    delay(speed);

    servoParams = servoParams.substring(servoParams.indexOf(")")+1);

  }

}

void controlLED(String params){

  String ledParams = params.substring(params.indexOf(")")+2);

  while(ledParams.length() > 0){

    String ledPair = ledParams.substring(0,ledParams.indexOf("("));

    int pattern = ledPair.toInt();
    int state = ledPair.substring(ledPair.indexOf(",")+1).toInt();

    analogWrite(LED_PIN, state*255);

    ledParams = ledParams.substring(ledParams.indexOf(")")+1);

  }

}

Let me know if you have any other questions!
