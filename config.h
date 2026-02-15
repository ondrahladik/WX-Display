#pragma once

#include <Arduino.h>
#include <EEPROM.h>

extern const char* programName;
extern const char* programVers;

extern unsigned long displayInterval;
extern const unsigned long dataTimeout;

#define LCD_ADDR 0x27

extern const int buttonPin;
extern const unsigned long longPress;
extern const unsigned long debouceDelay;

#define EEPROM_SIZE 512
#define EEPROM_MAGIC 0xAC

#define LEN_DISPLAY_TYPE 2
#define LEN_DISPLAY_INTERVAL 8
#define LEN_MQTT_SERVER 41
#define LEN_MQTT_PORT 6
#define LEN_ROOT_TOPIC 21
#define LEN_STATION_TOPIC 31
#define LEN_STATION_NAME 16

struct Config {
  uint8_t magic;
  char displayType[LEN_DISPLAY_TYPE];
  char displayInterval[LEN_DISPLAY_INTERVAL];
  char mqttServer[LEN_MQTT_SERVER];
  char mqttPort[LEN_MQTT_PORT];
  char rootTopic[LEN_ROOT_TOPIC];
  char stationTopic[4][LEN_STATION_TOPIC];
  char stationName[4][LEN_STATION_NAME];
};

extern Config config;

enum DisplayType {
  DISPLAY_16x2,
  DISPLAY_20x4,
  DISPLAY_40x2
};

extern DisplayType displayType;

// Functions
void loadConfig();
void saveConfig();
bool hasDiacritics(const char* str);
