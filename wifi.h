#pragma once

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiManager.h>
#include "config.h"

// WiFi state
extern bool wifiDisconnected;
extern unsigned long lastWifiReconnectAttempt;
extern const unsigned long wifiReconnectInterval;
extern bool shouldSaveConfig;

// WiFiManager parameters
extern bool wmParamsAllocated;
extern WiFiManagerParameter* p_displayType;
extern WiFiManagerParameter* p_displayInterval;
extern WiFiManagerParameter* p_mqttServer;
extern WiFiManagerParameter* p_mqttPort;
extern WiFiManagerParameter* p_rootTopic;
extern WiFiManagerParameter* p_station1Topic;
extern WiFiManagerParameter* p_station1Name;
extern WiFiManagerParameter* p_station2Topic;
extern WiFiManagerParameter* p_station2Name;
extern WiFiManagerParameter* p_station3Topic;
extern WiFiManagerParameter* p_station3Name;
extern WiFiManagerParameter* p_station4Topic;
extern WiFiManagerParameter* p_station4Name;

// Functions
void allocateWmParams();
void saveConfigCallback();
void setupWifi();
void startWifiPortal();
void handleWiFiDisconnect();
void handleWiFiReconnect();
bool tryWiFiReconnect();
