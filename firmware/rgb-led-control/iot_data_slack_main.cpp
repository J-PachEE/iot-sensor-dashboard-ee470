//-----------------------------
// Title: iot_data_slack_main.cpp
//-----------------------------
// Purpose: This program connects an ESP8266 to WiFi and sends an HTTPS request to an
// IFTTT webhook when a physical button is pressed. The webhook can be used to trigger
// an external notification service such as Slack, allowing the ESP8266 to report button
// events to an online service.
// Dependencies: Arduino.h, ESP8266WiFi.h, WiFiClientSecure.h
// Compiler/Interpreter: PlatformIO with VS Code using the ESP8266 Arduino framework
// Author: Jair Pacheco
// OUTPUTS: HTTPS webhook request, Serial Monitor status messages
// INPUTS: Physical button on D5
// Versions:
//      V1.0: 11/03/2025 - Initial working version
//-----------------------------
// Initialization

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

// WiFi credentials
const char* ssid = "YOUR_WIFI_NAME";
const char* password = "YOUR_WIFI_PASSWORD";

// IFTTT webhook configuration
const char* host = "maker.ifttt.com";
const int httpsPort = 443;
const char* requestPath = "/trigger/Button_pressed/with/key/YOUR_IFTTT_WEBHOOK_KEY";

// Pin assignments
const int buttonPin = D5;

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
// Function: sendWebhookRequest
//-----------------------------
// Purpose: Sends an HTTPS PUT request to the configured IFTTT webhook.
//-----------------------------
void sendWebhookRequest() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi disconnected. Reconnecting...");
    connectWiFi();
  }

  WiFiClientSecure client;
  client.setInsecure();

  if (client.connect(host, httpsPort)) {
    String url = String(requestPath) + "?value1=Button+Pressed";

    client.print(
      String("PUT ") + url + " HTTP/1.1\r\n" +
      "Host: " + host + "\r\n" +
      "Connection: close\r\n\r\n"
    );

    Serial.println("Webhook request sent.");
  } else {
    Serial.println("Webhook connection failed.");
  }
}

//-----------------------------
// Function: setup
//-----------------------------
// Purpose: Initializes serial communication, the button input pin, and WiFi connection.
//-----------------------------
void setup() {
  Serial.begin(9600);

  pinMode(buttonPin, INPUT);

  connectWiFi();
}

//-----------------------------
// Function: loop
//-----------------------------
// Purpose: Detects button presses and sends the webhook request after debounce timing.
//-----------------------------
void loop() {
  static bool lastButtonState = LOW;

  bool currentButtonState = digitalRead(buttonPin);
  unsigned long currentTime = millis();

  if (
    lastButtonState == LOW &&
    currentButtonState == HIGH &&
    currentTime - lastPressTime >= debounceDelayMs
  ) {
    lastPressTime = currentTime;

    Serial.println("Button pressed. Sending webhook request...");
    sendWebhookRequest();
  }

  lastButtonState = currentButtonState;
}
