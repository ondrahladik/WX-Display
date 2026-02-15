#include "stations.h"

String stations[MAX_STATIONS];
String names[MAX_STATIONS];
String mqttTopics[MAX_STATIONS];

int activeIdx[MAX_STATIONS];
int activeCount = 0;

StationData data[MAX_STATIONS];

void buildActiveIndex() {
  activeCount = 0;
  for (int i = 0; i < MAX_STATIONS; i++) {
    if (stations[i] != "") activeIdx[activeCount++] = i;
  }
}

bool isStationDataValid(int idx) {
  if (!data[idx].received) return false;
  if (millis() - data[idx].lastReceived > dataTimeout) return false;
  return true;
}

bool checkValidityChanged() {
  bool changed = false;
  for (int i = 0; i < activeCount; i++) {
    int idx = activeIdx[i];
    bool currentValid = isStationDataValid(idx);
    if (currentValid != data[idx].wasValid) {
      data[idx].wasValid = currentValid;
      changed = true;
    }
  }
  return changed;
}
