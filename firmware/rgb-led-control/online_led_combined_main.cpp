//-----------------------------
// Title: online_led_combined_main.cpp
//-----------------------------
// Purpose: This program connects an ESP8266 to WiFi and combines two online IoT features.
// When one button is pressed, the ESP8266 checks remote server files for LED and RGB values,
// parses the responses, and updates the connected LED and RGB LED outputs. When a second
// button is pressed, the ESP8266 sends an HTTPS webhook request to an external notification
// service through IFTTT.
// Dependencies: Arduino.h, ESP8266WiFi.h, ESP8266HTTPClient.h, WiFiClientSecure.h,
//               ArduinoJson.h
// Compiler/Interpreter: PlatformIO with VS Code using the ESP8266 Arduino framework
// Author: Jair Pacheco
// OUTPUTS: LED on D2, RGB LED on D7, D6, and D5, HTTPS webhook request,
//          Serial Monitor status messages
// INPUTS: Status sync button on D1, webhook button on D0, remote LED and RGB server files
// Versions:
//      V1.0: 11/03/2025 - Initial working version
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

// Remote server files
const char* ledUrl = "https://jairpacheco.com/results.txt";
const char* rgbUrl = "https://jairpacheco.com/rgb.txt";

// IFTTT webhook configuration
const char* webhookHost = "maker.ifttt.com";
const int httpsPort = 443;
const char* webhookPath = "/trigger/Button_pressed/with/key/YOUR_IFTTT_WEBHOOK_KEY";

// Pin assignments
const int ledPin = D2;
const int redPin = D7;
const int greenPin = D6;
const int bluePin = D5;
const int buttonStatusPin = D1;
const int buttonWebhookPin = D0;

// Debounce settings
const unsigned long debounceDelayMs = 250;
unsigned long lastStatusPressTime = 0;
unsigned long lastWebhookPressTime = 0;

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
// Function: syncLEDandRGB
//-----------------------------
// Purpose: Reads remote LED and RGB values from the web server and updates local outputs.
//-----------------------------
void syncLEDandRGB() {
  ensureWiFiConnected();

  WiFiClientSecure client;
  HTTPClient http;

  client.setInsecure();

  if (http.begin(client, ledUrl)) {
    int ledHttpCode = http.GET();

    if (ledHttpCode == HTTP_CODE_OK) {
      String payload = http.getString();

      StaticJsonDocument<200> doc;
      DeserializationError error = deserializeJson(doc, payload);

      if (!error) {
        String ledStatus = doc["led"];

        digitalWrite(ledPin, ledStatus == "ON" ? HIGH : LOW);

        Serial.print("LED status: ");
        Serial.println(ledStatus);
      } else {
        Serial.println("Failed to parse LED JSON.");
      }
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
      int redValue = http.getString().toInt();

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
// Function: sendWebhookMessage
//-----------------------------
// Purpose: Sends an HTTPS POST request to the configured IFTTT webhook.
//-----------------------------
void sendWebhookMessage() {
  ensureWiFiConnected();

  WiFiClientSecure client;
  client.setInsecure();

  if (!client.connect(webhookHost, httpsPort)) {
    Serial.println("Webhook connection failed.");
    return;
  }

  String payload = "{\"value1\":\"ESP Triggered\"}";

  String request =
    String("POST ") + webhookPath + " HTTP/1.1\r\n" +
    "Host: " + webhookHost + "\r\n" +
    "Content-Type: application/json\r\n" +
    "Content-Length: " + String(payload.length()) + "\r\n" +
    "Connection: close\r\n\r\n" +
    payload;

  client.print(request);

  Serial.println("Webhook request sent.");
}

//-----------------------------
// Function: setup
//-----------------------------
// Purpose: Initializes serial communication, output pins, input buttons, and WiFi connection.
//-----------------------------
void setup() {
  Serial.begin(9600);

  pinMode(ledPin, OUTPUT);
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);

  pinMode(buttonStatusPin, INPUT);
  pinMode(buttonWebhookPin, INPUT);

  digitalWrite(ledPin, LOW);
  analogWrite(redPin, 0);
  analogWrite(greenPin, 0);
  analogWrite(bluePin, 0);

  connectWiFi();
}

//-----------------------------
// Function: loop
//-----------------------------
// Purpose: Checks both button inputs, syncs online LED/RGB data, and sends webhook events.
//-----------------------------
void loop() {
  static bool lastStatusState = HIGH;
  static bool lastWebhookState = HIGH;

  bool currentStatusState = digitalRead(buttonStatusPin);
  bool currentWebhookState = digitalRead(buttonWebhookPin);
  unsigned long currentTime = millis();

  if (
    lastStatusState == HIGH &&
    currentStatusState == LOW &&
    currentTime - lastStatusPressTime >= debounceDelayMs
  ) {
    lastStatusPressTime = currentTime;
    syncLEDandRGB();
  }

  if (
    lastWebhookState == HIGH &&
    currentWebhookState == LOW &&
    currentTime - lastWebhookPressTime >= debounceDelayMs
  ) {
    lastWebhookPressTime = currentTime;
    sendWebhookMessage();
  }

  lastStatusState = currentStatusState;
  lastWebhookState = currentWebhookState;
}
