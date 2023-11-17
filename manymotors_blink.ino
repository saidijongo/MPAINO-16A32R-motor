
#include <Servo.h>
#include <Stepper.h>

int motorPins[] = {54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85};

#define PUL 1
#define DIR 2
#define SERVO 3
#define LED 4

Stepper stepper(200, PUL, DIR);
Servo servo;

void setup()
{

    for (int i = 0; i < 85; i++)
    {
        pinMode(motorPins[i], OUTPUT);
    }

    pinMode(PUL, OUTPUT);
    pinMode(DIR, OUTPUT);
    pinMode(SERVO, OUTPUT);
    pinMode(LED, OUTPUT);

    servo.attach(SERVO);

    Serial.begin(115200);
}

void loop()
{

    if (Serial.available() > 0)
    {

        String data = Serial.readStringUntil(',');

        Serial.println(data);

        String command = data.substring(0, data.indexOf(")"));

        if (command == "('PUMPMOTOR_OPERATION')")
            controlPumps(data);

        else if (command == "('STEPPERMOTOR_OPERATION')")
            controlStepper(data);

        else if (command == "('SERVOMOTOR_OPERATION')")
            controlServo(data);

        else if (command == "('LEDSTRIP_OPERATION')")
            controlLED(data);
    }
}

void controlPumps(String params)
{

    String pumpsStr = params.substring(params.indexOf(")") + 1);

    Serial.println(pumpsStr);

    while (pumpsStr.length() > 0)
    {

        String pumpPair = pumpsStr.substring(0, pumpsStr.indexOf("("));

        int pump = pumpPair.substring(0, pumpPair.indexOf(",")).toInt();
        // Serial.println(pump);
        //int pumpPin = params.substring(0, params.indexOf(",")).toInt();
        //Serial.println(pumpPin);

        int duration = pumpPair.substring(pumpPair.indexOf(",") + 1).toInt();

        Serial.println(pump);
        Serial.println(duration);

        digitalWrite(motorPins[pump], HIGH);
        delay(duration);
        digitalWrite(motorPins[pump], LOW);

        pumpsStr = pumpsStr.substring(pumpsStr.indexOf(")") + 1);
    }
}

void controlStepper(String params)
{

    String stepperParams = params.substring(params.indexOf(")") + 1);

    while (stepperParams.length() > 0)
    {

        String stepPair = stepperParams.substring(0, stepperParams.indexOf("("));

        int angle = stepPair.substring(0, stepPair.indexOf(",")).toInt();

        int speed = stepPair.substring(stepPair.indexOf(",") + 1).toInt();

        stepper.setSpeed(speed);
        stepper.step(angle);

        stepperParams = stepperParams.substring(stepperParams.indexOf(")") + 1);
    }
}

void controlServo(String params)
{

    int angle, speed;

    while (params.length() > 0)
    {
        //////
        angle = params.substring(0, params.indexOf(",")).toInt();
        speed = params.substring(params.indexOf(",") + 1).toInt();

        servo.write(angle);
        delay(speed);

        params = params.substring(params.indexOf(",") + 1);
        /////
    }
}

void controlLED(String params)
{

    int pattern, state;

    while (params.length() > 0)
    {
        ///////

        pattern = params.substring(0, params.indexOf(",")).toInt();
        state = params.substring(params.indexOf(",") + 1).toInt();

        analogWrite(LED, state * 255);

        params = params.substring(params.indexOf(",") + 1);

        /////
    }
}
