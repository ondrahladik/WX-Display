#include <ESP8266WiFi.h>
#include <WiFiManager.h>
#include <PubSubClient.h>
#include <LiquidCrystal_I2C.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <EEPROM.h>

#include "config.h"
#include "stations.h"
#include "display.h"
#include "screens.h"
#include "input.h"
#include "wifi.h"
#include "mqtt.h"

void setup() {
  Serial.begin(115200);
  Serial.println("[BOOT] WX-Display starting");
  pinMode(buttonPin, INPUT_PULLUP);

  loadConfig();
  buildActiveIndex();
  initDisplay();
  loadStartupIcons();
  setupWifi();
  initMqtt();

  // Show waiting for data screen after WiFi connected
  screenWaitingMqtt();
  loadMainIcons();
  startTime = millis();
  Serial.println("[BOOT] Setup complete");
}

void loop() {
  // WiFi disconnect handling
  if (WiFi.status() != WL_CONNECTED) {
    handleWiFiDisconnect();
    
    // Reconnect attempt every 5 minutes
    if (millis() - lastWifiReconnectAttempt >= wifiReconnectInterval) {
      lastWifiReconnectAttempt = millis();
      if (tryWiFiReconnect()) {
        handleWiFiReconnect();
      }
    }
    
    // Process only button during disconnect
    handleButton();
    return;
  }
  
  // WiFi was disconnected and is now connected again
  if (wifiDisconnected && WiFi.status() == WL_CONNECTED) {
    handleWiFiReconnect();
  }
  
  // MQTT handling
  if (!client.connected()) {
    if (mqttWasConnected) {
      Serial.println("[MQTT] Connection lost");
      mqttWasConnected = false;
    }
    
    // Show waiting screen only once
    if (!mqttScreenShown) {
      loadStartupIcons();
      screenWaitingMqtt();
      mqttScreenShown = true;
    }
    
    // Non-blocking MQTT reconnect attempt
    if (millis() - lastMqttReconnectAttempt >= mqttReconnectInterval) {
      lastMqttReconnectAttempt = millis();
      connectMqtt();
      if (client.connected()) {
        mqttWasConnected = true;
        mqttScreenShown = false;
        loadMainIcons();
        firstRenderDone = false;
        readyOnce = false;
      }
    }
    
    handleButton();
    return;
  }

  // MQTT connected - always process messages
  client.loop();

  if (!readyOnce) {
    bool ok = true;
    for (int i = 0; i < activeCount; i++) {
      if (!data[activeIdx[i]].received) ok = false;
    }
    // Continue when all data received or after 5 minutes from start
    if (!ok && (millis() - startTime < dataTimeout)) {
      handleButton();
      return;
    }
    readyOnce = true;
  }

  if (!firstRenderDone) {
    currentPage = 0;
    topPage = 0;
    bottomPage = 1;
    if (displayType == DISPLAY_16x2)
      renderStation16x2(0);
    else if (displayType == DISPLAY_20x4)
      renderStations20x4(topPage, (activeCount > 1) ? bottomPage : -1);
    else
      renderStations40x2(topPage, (activeCount > 1) ? bottomPage : -1);
    lastPageSwitch = millis();
    firstRenderDone = true;
  }

  if (millis() - lastPageSwitch >= displayInterval) {
    lastPageSwitch = millis();
    
    // Check validity change (for timeout detection)
    bool validityChanged = checkValidityChanged();
    
    if (displayType == DISPLAY_16x2) {
      if (activeCount == 1) {
        if (data[activeIdx[0]].changed || validityChanged) {
          renderStation16x2(0);
          data[activeIdx[0]].changed = false;
        }
      } else {
        int maxPages = activeCount;
        currentPage = (currentPage + 1) % maxPages;
        renderStation16x2(currentPage);
      }
    } else if (displayType == DISPLAY_20x4) {
      if (activeCount == 1) {
        if (data[activeIdx[0]].changed || validityChanged) {
          renderStations20x4(0, -1);
          data[activeIdx[0]].changed = false;
        }
      } else if (activeCount == 2) {
        if (data[activeIdx[0]].changed || data[activeIdx[1]].changed || validityChanged) {
          renderStations20x4(0, 1);
          data[activeIdx[0]].changed = false;
          data[activeIdx[1]].changed = false;
        }
      } else if (activeCount == 3) {
        bottomPage++;
        if (bottomPage > 2) bottomPage = 1;
        renderStations20x4(0, bottomPage);
      } else {
        if (topPage == 0) {
          topPage = 2;
          bottomPage = 3;
        } else {
          topPage = 0;
          bottomPage = 1;
        }
        renderStations20x4(topPage, bottomPage);
      }
    } else {
      if (activeCount == 1) {
        if (data[activeIdx[0]].changed || validityChanged) {
          renderStations40x2(0, -1);
          data[activeIdx[0]].changed = false;
        }
      } else if (activeCount == 2) {
        if (data[activeIdx[0]].changed || data[activeIdx[1]].changed || validityChanged) {
          renderStations40x2(0, 1);
          data[activeIdx[0]].changed = false;
          data[activeIdx[1]].changed = false;
        }
      } else if (activeCount == 3) {
        bottomPage++;
        if (bottomPage > 2) bottomPage = 1;
        renderStations40x2(0, bottomPage);
      } else {
        if (topPage == 0) {
          topPage = 2;
          bottomPage = 3;
        } else {
          topPage = 0;
          bottomPage = 1;
        }
        renderStations40x2(topPage, bottomPage);
      }
    }
  }

  // Button handling
  handleButton();
}