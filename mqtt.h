#pragma once

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "config.h"
#include "stations.h"

extern WiFiClient espClient;
extern PubSubClient client;

// MQTT state
extern bool firstDataReceived;
extern bool mqttWasConnected;
extern bool readyOnce;
extern bool mqttScreenShown;
extern unsigned long lastMqttReconnectAttempt;
extern const unsigned long mqttReconnectInterval;

// Functions
void initMqtt();
void connectMqtt();
void mqttCallback(char* topic, byte* payload, unsigned int length);
