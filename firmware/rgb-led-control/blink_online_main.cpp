//-----------------------------
// Title: blink_online_main.cpp
//-----------------------------
// Purpose: This program connects an ESP8266 to WiFi and checks a remote web server for
// an LED status stored in JSON format. When the physical switch is pressed, the ESP8266
// sends an HTTPS request, parses the returned JSON data, and updates the LED state based
// on the server response.
// Dependencies: Arduino.h, ESP8266WiFi.h, ESP8266HTTPClient.h, WiFiClientSecure.h,
//               ArduinoJson.h
// Compiler/Interpreter: PlatformIO with VS Code using the ESP8266 Arduino framework
// Author: Jair Pacheco
// OUTPUTS: LED on D2, Serial Monitor status messages
// INPUTS: Physical switch on D1, remote JSON file from web server
// Versions:
//      V1.0: 11/03/2025 - Initial working version
//      V1.1: 11/03/2025 - Added JSON parsing for remote LED status
//      V1.2: 11/03/2025 - Added switch debounce and HTTPS support
//-----------------------------
// Initialization

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>

// WiFi credentials
const char* ssid = "YOUR_WIFI_NAME";
const char* password = "YOUR_WIFI_PASSWORD";

// Remote server file
const char* statusUrl = "https://jairpacheco.com/results.txt";

// Pin assignments
const int ledPin = D2;
const int switchPin = D1;

// Debounce settings
const unsigned long debounceDelayMs = 1000;
unsigned long lastPressTime = 0;

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
// Function: updateLedFromServer
//-----------------------------
// Purpose: Requests the remote JSON file, parses the LED status, and updates the LED output.
//-----------------------------
void updateLedFromServer() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi disconnected. Reconnecting...");
    connectWiFi();
  }

  WiFiClientSecure client;
  HTTPClient http;

  client.setInsecure();

  if (!http.begin(client, statusUrl)) {
    Serial.println("Unable to start HTTPS request.");
    return;
  }

  int httpCode = http.GET();

  Serial.print("HTTP code: ");
  Serial.println(httpCode);

  if (httpCode == HTTP_CODE_OK) {
    String payload = http.getString();

    Serial.print("Payload: ");
    Serial.println(payload);

    StaticJsonDocument<200> doc;
    DeserializationError error = deserializeJson(doc, payload);

    if (!error) {
      const char* ledStatus = doc["led"];

      if (ledStatus != nullptr && strcmp(ledStatus, "ON") == 0) {
        digitalWrite(ledPin, HIGH);
      } else {
        digitalWrite(ledPin, LOW);
      }
    } else {
      Serial.println("JSON parse error.");
    }
  } else {
    Serial.println("Server request failed.");
  }

  http.end();
}

//-----------------------------
// Function: setup
//-----------------------------
// Purpose: Initializes serial communication, pin modes, and WiFi connection.
//-----------------------------
void setup() {
  Serial.begin(9600);

  pinMode(ledPin, OUTPUT);
  pinMode(switchPin, INPUT);

  digitalWrite(ledPin, LOW);

  connectWiFi();
}

//-----------------------------
// Function: loop
//-----------------------------
// Purpose: Detects switch presses and checks the server for the latest LED state.
//-----------------------------
void loop() {
  static bool lastSwitchState = LOW;

  bool currentSwitchState = digitalRead(switchPin);
  unsigned long currentTime = millis();

  if (
    lastSwitchState == LOW &&
    currentSwitchState == HIGH &&
    currentTime - lastPressTime >= debounceDelayMs
  ) {
    lastPressTime = currentTime;

    Serial.println("Switch pressed. Checking LED status...");
    updateLedFromServer();
  }

  lastSwitchState = currentSwitchState;
}
