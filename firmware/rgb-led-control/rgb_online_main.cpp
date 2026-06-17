//-----------------------------
// Title: rgb_online_main.cpp
//-----------------------------
// Purpose: This program connects an ESP8266 to WiFi and checks remote server files for LED
// and RGB values. The ESP8266 updates a standard LED and the red channel of an RGB LED based
// on the server responses. The system checks automatically every 30 seconds or immediately
// when the physical button is pressed.
// Dependencies: Arduino.h, ESP8266WiFi.h, ESP8266HTTPClient.h, WiFiClientSecure.h
// Compiler/Interpreter: PlatformIO with VS Code using the ESP8266 Arduino framework
// Author: Jair Pacheco
// OUTPUTS: LED on D2, RGB LED on D7, D6, and D5, Serial Monitor status messages
// INPUTS: Physical button on D1, remote LED status file, remote RGB value file
// Versions:
//      V1.0: 11/03/2025 - Initial working version
//-----------------------------
// Initialization

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>

// WiFi credentials
const char* ssid = "YOUR_WIFI_NAME";
const char* password = "YOUR_WIFI_PASSWORD";

// Remote server files
const char* ledUrl = "https://jairpacheco.com/results.txt";
const char* rgbUrl = "https://jairpacheco.com/rgb.txt";

// Pin assignments
const int ledPin = D2;
const int redPin = D7;
const int greenPin = D6;
const int bluePin = D5;
const int buttonPin = D1;

// Timing settings
const unsigned long checkIntervalMs = 30000;
const unsigned long debounceDelayMs = 1000;

unsigned long lastCheckTime = 0;
unsigned long lastButtonPressTime = 0;

//-----------------------------
// Function: connectWiFi
//-----------------------------
// Purpose: Connects the ESP8266 to the configured WiFi network.
//-----------------------------
void connectWiFi() {
  WiFi.begin(ssid, password);

  Serial.print("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi connected");
}

//-----------------------------
// Function: ensureWiFiConnected
//-----------------------------
// Purpose: Reconnects WiFi if the ESP8266 is disconnected.
//-----------------------------
void ensureWiFiConnected() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi disconnected. Reconnecting...");
    connectWiFi();
  }
}

//-----------------------------
// Function: syncOnlineOutputs
//-----------------------------
// Purpose: Reads the LED and RGB values from the web server and updates local outputs.
//-----------------------------
void syncOnlineOutputs() {
  ensureWiFiConnected();

  WiFiClientSecure client;
  HTTPClient http;

  client.setInsecure();

  if (http.begin(client, ledUrl)) {
    int ledHttpCode = http.GET();

    if (ledHttpCode == HTTP_CODE_OK) {
      String ledStatus = http.getString();
      ledStatus.trim();

      digitalWrite(ledPin, ledStatus == "ON" ? HIGH : LOW);

      Serial.print("LED status: ");
      Serial.println(ledStatus);
    } else {
      Serial.print("LED request failed. HTTP code: ");
      Serial.println(ledHttpCode);
    }

    http.end();
  } else {
    Serial.println("Unable to start LED HTTPS request.");
  }

  if (http.begin(client, rgbUrl)) {
    int rgbHttpCode = http.GET();

    if (rgbHttpCode == HTTP_CODE_OK) {
      String rgbValue = http.getString();
      int redValue = rgbValue.toInt();

      redValue = constrain(redValue, 0, 1023);

      analogWrite(redPin, redValue);
      analogWrite(greenPin, 0);
      analogWrite(bluePin, 0);

      Serial.print("RGB red value: ");
      Serial.println(redValue);
    } else {
      Serial.print("RGB request failed. HTTP code: ");
      Serial.println(rgbHttpCode);
    }

    http.end();
  } else {
    Serial.println("Unable to start RGB HTTPS request.");
  }
}

//-----------------------------
// Function: setup
//-----------------------------
// Purpose: Initializes serial communication, output pins, input button, and WiFi connection.
//-----------------------------
void setup() {
  Serial.begin(9600);

  pinMode(ledPin, OUTPUT);
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  pinMode(buttonPin, INPUT);

  digitalWrite(ledPin, LOW);
  analogWrite(redPin, 0);
  analogWrite(greenPin, 0);
  analogWrite(bluePin, 0);

  connectWiFi();
}

//-----------------------------
// Function: loop
//-----------------------------
// Purpose: Automatically checks the server at timed intervals or when the button is pressed.
//-----------------------------
void loop() {
  static bool lastButtonState = LOW;

  bool currentButtonState = digitalRead(buttonPin);
  unsigned long currentTime = millis();

  bool intervalElapsed = currentTime - lastCheckTime >= checkIntervalMs;
  bool buttonPressed = (
    lastButtonState == LOW &&
    currentButtonState == HIGH &&
    currentTime - lastButtonPressTime >= debounceDelayMs
  );

  if (intervalElapsed || buttonPressed) {
    lastCheckTime = currentTime;

    if (buttonPressed) {
      lastButtonPressTime = currentTime;
    }

    Serial.println("Checking LED and RGB status...");
    syncOnlineOutputs();
  }

  lastButtonState = currentButtonState;
}
