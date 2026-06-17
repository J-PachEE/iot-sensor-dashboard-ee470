//-----------------------------
// Title: ESP_HQTT.cpp
//-----------------------------
// Purpose: This program connects an ESP8266 to a WiFi network and an MQTT broker. The device
// subscribes to an MQTT topic to control an LED and publishes potentiometer readings as JSON
// data to another MQTT topic at regular intervals.
// Dependencies: Arduino.h, ESP8266WiFi.h, PubSubClient.h
// Compiler/Interpreter: PlatformIO with VS Code using the ESP8266 Arduino framework
// Author: Jair Pacheco
// OUTPUTS: LED on D5, MQTT topic testtopic/temp/outTopic/room1
// INPUTS: Potentiometer on A0, MQTT topic testtopic/temp/inTopic
// Versions:
//      V1.0: 11/25/2025 - Initial working version
//-----------------------------
// Initialization

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// WiFi credentials
const char* ssid = "YOUR_WIFI_NAME";
const char* password = "YOUR_WIFI_PASSWORD";

// MQTT broker configuration
const char* mqtt_server = "broker.hivemq.com";
const int mqtt_port = 1883;

// MQTT topics
const char* sub_topic = "testtopic/temp/inTopic";
const char* pub_topic = "testtopic/temp/outTopic/room1";

// Pin assignments
#define LED_PIN D5
#define POT_PIN A0

WiFiClient espClient;
PubSubClient client(espClient);

unsigned long lastPublish = 0;
const unsigned long publishIntervalMs = 15000;

//-----------------------------
// Function: callback
//-----------------------------
// Purpose: Handles incoming MQTT messages and controls the LED based on the received payload.
//-----------------------------
void callback(char* topic, byte* payload, unsigned int length) {
  String msg;

  for (unsigned int i = 0; i < length; i++) {
    msg += (char)payload[i];
  }

  msg.trim();

  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("]: ");
  Serial.println(msg);

  if (String(topic) == sub_topic) {
    if (msg == "1") {
      digitalWrite(LED_PIN, HIGH);
      Serial.println("LED ON (via MQTT)");
    } 
    else if (msg == "0") {
      digitalWrite(LED_PIN, LOW);
      Serial.println("LED OFF (via MQTT)");
    }
  }
}

//-----------------------------
// Function: reconnect
//-----------------------------
// Purpose: Reconnects the ESP8266 to the MQTT broker and subscribes to the LED control topic.
//-----------------------------
void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");

    String clientId = "ESP8266Client-" + String(ESP.getChipId(), HEX);

    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      client.subscribe(sub_topic);
    } 
    else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" retry in 2s");
      delay(2000);
    }
  }
}

//-----------------------------
// Function: setup
//-----------------------------
// Purpose: Initializes serial communication, pin modes, WiFi connection, and MQTT settings.
//-----------------------------
void setup() {
  Serial.begin(115200);

  pinMode(LED_PIN, OUTPUT);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected");

  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

//-----------------------------
// Function: loop
//-----------------------------
// Purpose: Maintains the MQTT connection and publishes potentiometer data every 15 seconds.
//-----------------------------
void loop() {
  if (!client.connected()) {
    reconnect();
  }

  client.loop();

  unsigned long now = millis();

  if (now - lastPublish >= publishIntervalMs) {
    lastPublish = now;

    int raw = analogRead(POT_PIN);
    float volts = (raw / 1023.0f) * 3.3f;

    char payload[64];
    snprintf(payload, sizeof(payload), "{\"raw\":%d,\"volts\":%.2f}", raw, volts);

    client.publish(pub_topic, payload);

    Serial.print("Published to ");
    Serial.print(pub_topic);
    Serial.print(" -> ");
    Serial.println(payload);
  }
}


//-----------------------------
// Troubleshooting and Test Reference
//-----------------------------
// Purpose: The following archived sections are not part of the active program. They are kept as
// reference tests for troubleshooting individual hardware and MQTT features. To use one of these
// tests, comment out the active program above and uncomment only the desired test section below.
//-----------------------------


/*
//-----------------------------
// Test Section A: Hardware Input and Output Test
//-----------------------------
// Purpose: Tests the ESP8266 LED, push button, potentiometer, and Serial Monitor input.
// This section can be used to confirm that the hardware wiring is correct before testing WiFi
// or MQTT communication.
//-----------------------------

#include <Arduino.h>

#define LED_PIN D5
#define SWITCH_PIN D2
#define POT_PIN A0

int lastSwitchState = LOW;

void setup() {
  Serial.begin(9600);

  pinMode(LED_PIN, OUTPUT);
  pinMode(SWITCH_PIN, INPUT);

  Serial.println("ESP8266 hardware test started.");
  Serial.println("Type 1 to turn the LED on or 0 to turn the LED off.");
}

void loop() {
  int potValue = analogRead(POT_PIN);
  float voltage = (potValue / 1023.0) * 3.3;

  Serial.print("Potentiometer raw: ");
  Serial.print(potValue);
  Serial.print(" | Voltage: ");
  Serial.println(voltage, 2);

  int switchState = digitalRead(SWITCH_PIN);

  if (switchState == HIGH) {
    digitalWrite(LED_PIN, HIGH);
    Serial.println("Button pressed -> LED ON");
  } 
  else {
    digitalWrite(LED_PIN, LOW);
    Serial.println("Button released -> LED OFF");
  }

  if (Serial.available()) {
    char cmd = Serial.read();

    if (cmd == '1') {
      digitalWrite(LED_PIN, HIGH);
      Serial.println("LED ON (Serial)");
    } 
    else if (cmd == '0') {
      digitalWrite(LED_PIN, LOW);
      Serial.println("LED OFF (Serial)");
    }
  }

  delay(1000);
}
*/


/*
//-----------------------------
// Test Section B: MQTT Potentiometer Publisher
//-----------------------------
// Purpose: Tests WiFi connection and MQTT publishing. This section reads the potentiometer on A0
// and publishes the raw value and calculated voltage to an MQTT topic every 15 seconds.
//-----------------------------

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// WiFi credentials
const char* ssid = "YOUR_WIFI_NAME";
const char* password = "YOUR_WIFI_PASSWORD";

// MQTT broker configuration
const char* mqtt_server = "broker.mqttdashboard.com";
const uint16_t mqtt_port = 1883;

// MQTT topic
const char* pub_topic = "testtopic/temp/room1";

// Pin assignments
#define POT_PIN A0

WiFiClient espClient;
PubSubClient client(espClient);

unsigned long lastPublish = 0;
const unsigned long publishIntervalMs = 15000;

void connectWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  Serial.print("Connecting WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.print("\nWiFi connected. IP: ");
  Serial.println(WiFi.localIP());
}

void reconnectMQTT() {
  while (!client.connected()) {
    Serial.print("Connecting MQTT...");

    String clientId = "ESP8266Client-";
    clientId += String(ESP.getChipId(), HEX);

    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
    } 
    else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" retry in 2s");
      delay(2000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  delay(100);

  connectWiFi();

  client.setServer(mqtt_server, mqtt_port);
  reconnectMQTT();

  Serial.println("MQTT potentiometer publisher test started.");
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    connectWiFi();
  }

  if (!client.connected()) {
    reconnectMQTT();
  }

  client.loop();

  unsigned long now = millis();

  if (now - lastPublish >= publishIntervalMs) {
    lastPublish = now;

    int raw = analogRead(POT_PIN);
    float volts = (raw / 1023.0f) * 3.3f;

    char payload[64];
    snprintf(payload, sizeof(payload), "{\"raw\":%d,\"volts\":%.2f}", raw, volts);

    bool ok = client.publish(pub_topic, payload, false);

    Serial.print("Publish to ");
    Serial.print(pub_topic);
    Serial.print(" -> ");
    Serial.print(payload);
    Serial.println(ok ? " [OK]" : " [FAIL]");
  }
}
*/


/*
//-----------------------------
// Test Section C: MQTT LED Subscriber and Switch Publisher
//-----------------------------
// Purpose: Tests two-way MQTT communication. This section subscribes to an MQTT topic for LED
// control and publishes switch events to another MQTT topic. It can be used to troubleshoot
// MQTT subscriptions, MQTT publishing, LED control, and switch input behavior.
//-----------------------------

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// WiFi credentials
const char* ssid = "YOUR_WIFI_NAME";
const char* password = "YOUR_WIFI_PASSWORD";

// MQTT broker configuration
const char* mqtt_server = "broker.mqttdashboard.com";
const int mqtt_port = 1883;

// MQTT topics
const char* sub_topic = "testtopic/temp/inTopic";
const char* pub_topic = "testtopic/temp/outTopic/room1";

// Pin assignments
#define LED_PIN D5
#define SWITCH_PIN D2

WiFiClient espClient;
PubSubClient client(espClient);

unsigned long lastSwitchPress = 0;
bool switchPressed = false;

void callback(char* topic, byte* payload, unsigned int length) {
  String msg;

  for (unsigned int i = 0; i < length; i++) {
    msg += (char)payload[i];
  }

  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("]: ");
  Serial.println(msg);

  if (String(topic) == sub_topic) {
    if (msg == "1") {
      digitalWrite(LED_PIN, HIGH);
      Serial.println("LED ON (via MQTT)");
    } 
    else if (msg == "0") {
      digitalWrite(LED_PIN, LOW);
      Serial.println("LED OFF (via MQTT)");
    }
  }
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");

    String clientId = "ESP8266Client-" + String(ESP.getChipId(), HEX);

    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      client.subscribe(sub_topic);
    } 
    else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" retry in 2s");
      delay(2000);
    }
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(LED_PIN, OUTPUT);
  pinMode(SWITCH_PIN, INPUT);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected");

  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }

  client.loop();

  int state = digitalRead(SWITCH_PIN);

  if (state == HIGH && !switchPressed) {
    switchPressed = true;
    lastSwitchPress = millis();

    client.publish(pub_topic, "1");
    Serial.println("Switch pressed -> published 1");
  }

  if (switchPressed && millis() - lastSwitchPress >= 5000) {
    client.publish(pub_topic, "0");
    Serial.println("Switch released -> published 0");

    switchPressed = false;
  }
}
*/
