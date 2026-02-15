#include "mqtt.h"
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>

WiFiClient espClient;
PubSubClient client(espClient);

// MQTT state
bool firstDataReceived = false;
bool mqttWasConnected = false;
bool readyOnce = false;
bool mqttScreenShown = false;
unsigned long lastMqttReconnectAttempt = 0;
const unsigned long mqttReconnectInterval = 5000;

void initMqtt() {
  client.setServer(config.mqttServer, atoi(config.mqttPort));
  client.setCallback(mqttCallback);
}

void connectMqtt() {
  String id = "WX-Display-" + String(random(0xffff), HEX);
  if (client.connect(id.c_str())) {
    Serial.println("[MQTT] Connected");
    for (int i = 0; i < activeCount; i++) {
      int idx = activeIdx[i];
      mqttTopics[idx] = String(config.rootTopic) + "/" + stations[idx];
      client.subscribe(mqttTopics[idx].c_str());
    }
  }
}

void mqttCallback(char* topic, byte* payload, unsigned int length) {
  String message;
  for (uint16_t i = 0; i < length; i++) message += (char)payload[i];

  StaticJsonDocument<256> doc;
  if (deserializeJson(doc, message)) return;

  for (int i = 0; i < activeCount; i++) {
    int idx = activeIdx[i];
    if (mqttTopics[idx] == topic) {
      float newTemp = doc["temperature"];
      int newHum = (int)doc["humidity"];
      int newPress = (int)doc["pressure"];
      
      // Log to serial
      Serial.print("[DATA] ");
      Serial.print(names[idx]);
      Serial.print(": T");
      Serial.print(newTemp, 1);
      Serial.print(" H");
      Serial.print(newHum);
      Serial.print(" P");
      Serial.println(newPress);
      
      bool tempChanged = abs(newTemp - data[idx].temperature) >= 0.05;
      bool humChanged = newHum != data[idx].humidity;
      bool pressChanged = newPress != data[idx].pressure;
      
      if (tempChanged || humChanged || pressChanged) {
        data[idx].changed = true;
        data[idx].temperature = newTemp;
        data[idx].humidity    = newHum;
        data[idx].pressure    = newPress;
      }
      
      if (!data[idx].received && !firstDataReceived) {
        Serial.println("[MQTT] First data received");
        firstDataReceived = true;
      }
      
      data[idx].received = true;
      data[idx].lastReceived = millis();
    }
  }
}
