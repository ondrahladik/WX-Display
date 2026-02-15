#pragma once

#include <Arduino.h>
#include "config.h"

#define MAX_STATIONS 4

// Station data structure
struct StationData {
  float temperature = NAN;
  int humidity = -1;
  int pressure = -1;
  bool received = false;
  bool changed = false;
  unsigned long lastReceived = 0;
  bool wasValid = false;
};

extern String stations[MAX_STATIONS];
extern String names[MAX_STATIONS];
extern String mqttTopics[MAX_STATIONS];

extern int activeIdx[MAX_STATIONS];
extern int activeCount;

extern StationData data[MAX_STATIONS];

// Functions
void buildActiveIndex();
bool isStationDataValid(int idx);
bool checkValidityChanged();
