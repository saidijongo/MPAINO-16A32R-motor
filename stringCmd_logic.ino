void processCommand(String command) {
  char separator = ',';

  // Find the opening parenthesis in the command
  int index = command.indexOf('(');

  // Check if an opening parenthesis was found
  if (index != -1) {
    // Extract the motor type from the command
    String motorType = command.substring(1, index);
    motorType.trim(); // Remove any leading/trailing whitespace

    // Check if the extracted motor type is valid
    if (motorType == "PUMPMOTOR_OPERATION" ||
        motorType == "STEPPERMOTOR_OPERATION" ||
        motorType == "SERVOMOTOR_OPERATION" ||
        motorType == "LEDSTRIP_OPERATION") {

      // Find the closing parenthesis in the command
      int endIndex = command.indexOf(')', index + 1);

      // Check if a closing parenthesis was found
      if (endIndex != -1) {
        // Extract the motor data (parameters) from the command
        String motorData = command.substring(index + 1, endIndex);
        
        // Find the comma in the motor data
        int commaIndex = motorData.indexOf(separator);

        // Check if a comma was found
        if (commaIndex != -1) {
          // Extract the first parameter from the motor data
          int param1 = motorData.substring(0, commaIndex).toInt();
          
          // Extract the second parameter from the motor data
          int param2 = motorData.substring(commaIndex + 1).toInt();

          // Perform actions based on the motor type and parameters
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
