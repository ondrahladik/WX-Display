#include "config.h"
#include "stations.h"
#include "icons.h"

const char* programName = "WX-Display";
const char* programVers = "v1.0.0";

unsigned long displayInterval = 10000;
const unsigned long dataTimeout = 300000;

const int buttonPin = D3;
const unsigned long longPress = 5000;
const unsigned long debouceDelay = 50;

Config config;
DisplayType displayType = DISPLAY_16x2;

// Startup icons (logo)
byte tempchar1[8] = { B00000, B00001, B00010, B00100, B00100, B00100, B00100, B00111 };
byte tempchar2[8] = { B00111, B00111, B00111, B01111, B11111, B11111, B01111, B00011 };
byte tempchar3[8] = { B00000, B10000, B01011, B00100, B00111, B00100, B00111, B11100 };
byte tempchar4[8] = { B11111, B11100, B11100, B11110, B11111, B11111, B11110, B11000 };

// Main icons (measurement symbols)
byte symbolTemp[8]     = { B00100, B01010, B01010, B01110, B01110, B11111, B11111, B01110 };
byte symbolHumi[8]     = { B00100, B00100, B01010, B01010, B10001, B10001, B10001, B01110 };
byte symbolHumiFull[8] = { B00100, B00100, B01110, B01110, B11111, B11111, B11111, B01110 };
byte symbolPress[8]    = { B00000, B01110, B10001, B10101, B10101, B10001, B01110, B00000 };

void loadConfig() {
  Serial.println("[CFG] Loading config from EEPROM");
  EEPROM.begin(EEPROM_SIZE);
  EEPROM.get(0, config);
  
  if (config.magic != EEPROM_MAGIC) {
    Serial.println("[CFG] Invalid config - using defaults");
    config.magic = EEPROM_MAGIC;
    strcpy(config.displayType, "0");
    strcpy(config.displayInterval, "10000");
    strcpy(config.mqttServer, "example.com");
    strcpy(config.mqttPort, "1883");
    strcpy(config.rootTopic, "meteo");
    strcpy(config.stationTopic[0], "station1");
    strcpy(config.stationName[0], "Station 1");
    for (int i = 1; i < 4; i++) {
      config.stationTopic[i][0] = '\0';
      config.stationName[i][0] = '\0';
    }
    saveConfig();
  } else {
    Serial.println("[CFG] Config loaded OK");
  }
  
  // Apply configuration
  int dt = atoi(config.displayType);
  if (dt == 1) displayType = DISPLAY_20x4;
  else if (dt == 2) displayType = DISPLAY_40x2;
  else displayType = DISPLAY_16x2;
  
  displayInterval = atol(config.displayInterval);
  if (displayInterval < 1000) displayInterval = 10000;
  
  for (int i = 0; i < MAX_STATIONS; i++) {
    stations[i] = String(config.stationTopic[i]);
    names[i] = String(config.stationName[i]);
  }
}

void saveConfig() {
  EEPROM.put(0, config);
  EEPROM.commit();
}

// Validation - no diacritics allowed
bool hasDiacritics(const char* str) {
  while (*str) {
    unsigned char c = (unsigned char)*str;
    if (c > 127) return true;
    str++;
  }
  return false;
}
