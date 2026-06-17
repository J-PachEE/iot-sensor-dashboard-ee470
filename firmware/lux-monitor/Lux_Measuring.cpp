//-----------------------------
// Title: Lux_Measuring.cpp
//-----------------------------
// Purpose: This program measures light intensity using a light sensor connected to an analog
// input. The system activates a buzzer when the measured light level is too low and adjusts
// an RGB LED output when the measured light level is too high.
// Dependencies: Arduino.h, math.h
// Compiler/Interpreter: PlatformIO with VS Code using the ESP8266 Arduino framework
// Author: Jair Pacheco
// OUTPUTS: Buzzer on D4, RGB LED on D5, D6, and D7, Serial Monitor lux readings
// INPUTS: Light sensor on A0, Serial Monitor input
// Versions:
//      V1.0: 9/28/2025 - Initial version
//      V1.1: 9/28/2025 - RGB color values hardcoded for reliable LED control
//      V1.2: 9/28/2025 - Calibrated using a lux measurement application as reference
//-----------------------------
// Initialization

#include <Arduino.h>
#include <math.h>

// Lux threshold values
const int luxLowerLimit = 50;
const int luxBuzzerOffLimit = 100;

// Pin assignments
const int luxSensorPin = A0;
const int buzzerPin = D4;
const int rgbRedPin = D5;
const int rgbGreenPin = D6;
const int rgbBluePin = D7;

// Global variables
int luxAdcValue = 0;
int rgbPwmValue = 0;
float luxVoltage = 0.0;

String serialInput = "";

//-----------------------------
// Function: setup
//-----------------------------
// Purpose: Initializes sensor input, RGB LED outputs, buzzer output, and serial communication.
//-----------------------------
void setup() {
  pinMode(luxSensorPin, INPUT);
  pinMode(rgbRedPin, OUTPUT);
  pinMode(rgbGreenPin, OUTPUT);
  pinMode(rgbBluePin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);

  analogWrite(rgbRedPin, 1023);
  analogWrite(rgbGreenPin, 1023);
  analogWrite(rgbBluePin, 1023);

  Serial.begin(9600);
}

//-----------------------------
// Function: calculateLux
//-----------------------------
// Purpose: Converts the measured analog voltage into an estimated lux value using calibration data.
//-----------------------------
int calculateLux(float voltage) {
  if (voltage <= 0.0) {
    return 0;
  }

  return (int)(log(voltage / 3852.0) / -3.9);
}

//-----------------------------
// Function: updateBuzzer
//-----------------------------
// Purpose: Activates the buzzer when the light level is below the lower threshold.
//-----------------------------
void updateBuzzer(int luxValue) {
  if (luxValue < luxLowerLimit) {
    digitalWrite(buzzerPin, HIGH);
  }

  if (luxValue >= luxBuzzerOffLimit) {
    digitalWrite(buzzerPin, LOW);
  }
}

//-----------------------------
// Function: updateRgbLed
//-----------------------------
// Purpose: Adjusts the RGB LED brightness based on the measured lux value.
//-----------------------------
void updateRgbLed(int luxValue) {
  if (luxValue > 900) {
    rgbPwmValue = 0;
  }
  else if (luxValue > 750) {
    rgbPwmValue = 50;
  }
  else if (luxValue > 600) {
    rgbPwmValue = 100;
  }
  else if (luxValue > 450) {
    rgbPwmValue = 200;
  }
  else if (luxValue > 300) {
    rgbPwmValue = 600;
  }
  else if (luxValue > 150) {
    rgbPwmValue = 900;
  }
  else {
    rgbPwmValue = 1023;
  }

  analogWrite(rgbBluePin, rgbPwmValue);
  analogWrite(rgbGreenPin, rgbPwmValue);
}

//-----------------------------
// Function: checkSerialInput
//-----------------------------
// Purpose: Allows the buzzer to be manually activated through the Serial Monitor.
//-----------------------------
void checkSerialInput() {
  if (Serial.available() > 0) {
    serialInput = Serial.readStringUntil('\n');
    serialInput.trim();

    if (serialInput == "B") {
      digitalWrite(buzzerPin, HIGH);
      delay(5000);
      digitalWrite(buzzerPin, LOW);
    }
  }
}

//-----------------------------
// Function: loop
//-----------------------------
// Purpose: Continuously reads the light sensor, calculates lux, updates outputs, and checks
// for Serial Monitor commands.
//-----------------------------
void loop() {
  luxAdcValue = analogRead(luxSensorPin);
  luxVoltage = luxAdcValue * (3.3 / 1023.0);

  int luxValue = calculateLux(luxVoltage);

  Serial.println(luxValue);

  updateBuzzer(luxValue);
  updateRgbLed(luxValue);
  checkSerialInput();

  delay(500);
}
