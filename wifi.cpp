#include "wifi.h"
#include "display.h"
#include "screens.h"
#include "input.h"
#include "mqtt.h"

// WiFi state
bool wifiDisconnected = false;
unsigned long lastWifiReconnectAttempt = 0;
const unsigned long wifiReconnectInterval = 300000;
bool shouldSaveConfig = false;

// WiFiManager parameters
bool wmParamsAllocated = false;
WiFiManagerParameter* p_displayType = nullptr;
WiFiManagerParameter* p_displayInterval = nullptr;
WiFiManagerParameter* p_mqttServer = nullptr;
WiFiManagerParameter* p_mqttPort = nullptr;
WiFiManagerParameter* p_rootTopic = nullptr;
WiFiManagerParameter* p_station1Topic = nullptr;
WiFiManagerParameter* p_station1Name = nullptr;
WiFiManagerParameter* p_station2Topic = nullptr;
WiFiManagerParameter* p_station2Name = nullptr;
WiFiManagerParameter* p_station3Topic = nullptr;
WiFiManagerParameter* p_station3Name = nullptr;
WiFiManagerParameter* p_station4Topic = nullptr;
WiFiManagerParameter* p_station4Name = nullptr;

void allocateWmParams() {
  if (wmParamsAllocated) return;
  p_displayType = new WiFiManagerParameter("displayType", "Display Type (0=16x2, 1=20x4, 2=40x2) *", config.displayType, LEN_DISPLAY_TYPE - 1);
  p_displayInterval = new WiFiManagerParameter("displayInterval", "Display Interval (ms) *", config.displayInterval, LEN_DISPLAY_INTERVAL - 1);
  p_mqttServer = new WiFiManagerParameter("mqttServer", "MQTT Server *", config.mqttServer, LEN_MQTT_SERVER - 1);
  p_mqttPort = new WiFiManagerParameter("mqttPort", "MQTT Port *", config.mqttPort, LEN_MQTT_PORT - 1);
  p_rootTopic = new WiFiManagerParameter("rootTopic", "Root Topic *", config.rootTopic, LEN_ROOT_TOPIC - 1);
  p_station1Topic = new WiFiManagerParameter("station1Topic", "", config.stationTopic[0], LEN_STATION_TOPIC - 1, "style='width:60%' placeholder='Topic'");
  p_station1Name = new WiFiManagerParameter("station1Name", "", config.stationName[0], LEN_STATION_NAME - 1, "style='width:40%' placeholder='Name'");
  p_station2Topic = new WiFiManagerParameter("station2Topic", "", config.stationTopic[1], LEN_STATION_TOPIC - 1, "style='width:60%' placeholder='Topic'");
  p_station2Name = new WiFiManagerParameter("station2Name", "", config.stationName[1], LEN_STATION_NAME - 1, "style='width:40%' placeholder='Name'");
  p_station3Topic = new WiFiManagerParameter("station3Topic", "", config.stationTopic[2], LEN_STATION_TOPIC - 1, "style='width:60%' placeholder='Topic'");
  p_station3Name = new WiFiManagerParameter("station3Name", "", config.stationName[2], LEN_STATION_NAME - 1, "style='width:40%' placeholder='Name'");
  p_station4Topic = new WiFiManagerParameter("station4Topic", "", config.stationTopic[3], LEN_STATION_TOPIC - 1, "style='width:60%' placeholder='Topic'");
  p_station4Name = new WiFiManagerParameter("station4Name", "", config.stationName[3], LEN_STATION_NAME - 1, "style='width:40%' placeholder='Name'");
  wmParamsAllocated = true;
}

void saveConfigCallback() {
  if (strlen(p_displayType->getValue()) == 0 ||
      strlen(p_displayInterval->getValue()) == 0 ||
      strlen(p_mqttServer->getValue()) == 0 ||
      strlen(p_mqttPort->getValue()) == 0 ||
      strlen(p_rootTopic->getValue()) == 0 ||
      strlen(p_station1Topic->getValue()) == 0 ||
      strlen(p_station1Name->getValue()) == 0) {
    Serial.println("[CFG] Validation failed - empty required fields");
    return;
  }
  
  if (hasDiacritics(p_mqttServer->getValue()) ||
      hasDiacritics(p_rootTopic->getValue()) ||
      hasDiacritics(p_station1Topic->getValue()) ||
      hasDiacritics(p_station1Name->getValue()) ||
      hasDiacritics(p_station2Topic->getValue()) ||
      hasDiacritics(p_station2Name->getValue()) ||
      hasDiacritics(p_station3Topic->getValue()) ||
      hasDiacritics(p_station3Name->getValue()) ||
      hasDiacritics(p_station4Topic->getValue()) ||
      hasDiacritics(p_station4Name->getValue())) {
    Serial.println("[CFG] Validation failed - diacritics detected");
    return;
  }
  
  strncpy(config.displayType, p_displayType->getValue(), LEN_DISPLAY_TYPE - 1);
  config.displayType[LEN_DISPLAY_TYPE - 1] = '\0';
  
  strncpy(config.displayInterval, p_displayInterval->getValue(), LEN_DISPLAY_INTERVAL - 1);
  config.displayInterval[LEN_DISPLAY_INTERVAL - 1] = '\0';
  
  strncpy(config.mqttServer, p_mqttServer->getValue(), LEN_MQTT_SERVER - 1);
  config.mqttServer[LEN_MQTT_SERVER - 1] = '\0';
  
  strncpy(config.mqttPort, p_mqttPort->getValue(), LEN_MQTT_PORT - 1);
  config.mqttPort[LEN_MQTT_PORT - 1] = '\0';
  
  strncpy(config.rootTopic, p_rootTopic->getValue(), LEN_ROOT_TOPIC - 1);
  config.rootTopic[LEN_ROOT_TOPIC - 1] = '\0';
  
  strncpy(config.stationTopic[0], p_station1Topic->getValue(), LEN_STATION_TOPIC - 1);
  config.stationTopic[0][LEN_STATION_TOPIC - 1] = '\0';
  strncpy(config.stationName[0], p_station1Name->getValue(), LEN_STATION_NAME - 1);
  config.stationName[0][LEN_STATION_NAME - 1] = '\0';
  
  strncpy(config.stationTopic[1], p_station2Topic->getValue(), LEN_STATION_TOPIC - 1);
  config.stationTopic[1][LEN_STATION_TOPIC - 1] = '\0';
  strncpy(config.stationName[1], p_station2Name->getValue(), LEN_STATION_NAME - 1);
  config.stationName[1][LEN_STATION_NAME - 1] = '\0';
  
  strncpy(config.stationTopic[2], p_station3Topic->getValue(), LEN_STATION_TOPIC - 1);
  config.stationTopic[2][LEN_STATION_TOPIC - 1] = '\0';
  strncpy(config.stationName[2], p_station3Name->getValue(), LEN_STATION_NAME - 1);
  config.stationName[2][LEN_STATION_NAME - 1] = '\0';
  
  strncpy(config.stationTopic[3], p_station4Topic->getValue(), LEN_STATION_TOPIC - 1);
  config.stationTopic[3][LEN_STATION_TOPIC - 1] = '\0';
  strncpy(config.stationName[3], p_station4Name->getValue(), LEN_STATION_NAME - 1);
  config.stationName[3][LEN_STATION_NAME - 1] = '\0';
  
  saveConfig();
  Serial.println("[CFG] Config saved to EEPROM");
  shouldSaveConfig = true;
}

void setupWifi() {
  Serial.println("[WIFI] Starting WiFi setup");
  screenWaitingWifi();
  
  WiFiManager wm;
  wm.setHostname(programName);
  wm.setTitle(programName);
  wm.setClass("invert");
  wm.setConnectRetries(3);

  allocateWmParams();

  WiFiManagerParameter custom_html_display("<h3>LCD Display</h3>");
  WiFiManagerParameter custom_html_mqtt("<h3>MQTT</h3>");
  WiFiManagerParameter custom_html_stations("<h3>Stations</h3>");
  WiFiManagerParameter custom_html_station1("<label>Station 1 (Topic / Name) *</label><br><div style='display:flex;gap:2px;margin-top:0;padding:0;'>");
  WiFiManagerParameter custom_html_station1_end("</div>");
  WiFiManagerParameter custom_html_station2("<label>Station 2 (Topic / Name)</label><br><div style='display:flex;gap:2px;margin-top:0;padding:0;'>");
  WiFiManagerParameter custom_html_station2_end("</div>");
  WiFiManagerParameter custom_html_station3("<label>Station 3 (Topic / Name)</label><br><div style='display:flex;gap:2px;margin-top:0;padding:0;'>");
  WiFiManagerParameter custom_html_station3_end("</div>");
  WiFiManagerParameter custom_html_station4("<label>Station 4 (Topic / Name)</label><br><div style='display:flex;gap:2px;margin-top:0;padding:0;'>");
  WiFiManagerParameter custom_html_station4_end("</div>");
  
  wm.addParameter(&custom_html_display);
  wm.addParameter(p_displayType);
  wm.addParameter(p_displayInterval);
  wm.addParameter(&custom_html_mqtt);
  wm.addParameter(p_mqttServer);
  wm.addParameter(p_mqttPort);
  wm.addParameter(p_rootTopic);
  wm.addParameter(&custom_html_stations);
  wm.addParameter(&custom_html_station1);
  wm.addParameter(p_station1Topic);
  wm.addParameter(p_station1Name);
  wm.addParameter(&custom_html_station1_end);
  wm.addParameter(&custom_html_station2);
  wm.addParameter(p_station2Topic);
  wm.addParameter(p_station2Name);
  wm.addParameter(&custom_html_station2_end);
  wm.addParameter(&custom_html_station3);
  wm.addParameter(p_station3Topic);
  wm.addParameter(p_station3Name);
  wm.addParameter(&custom_html_station3_end);
  wm.addParameter(&custom_html_station4);
  wm.addParameter(p_station4Topic);
  wm.addParameter(p_station4Name);
  wm.addParameter(&custom_html_station4_end);
  
  wm.setSaveConfigCallback(saveConfigCallback);
  wm.setSaveParamsCallback(saveConfigCallback);
  wm.setBreakAfterConfig(true);
  
  // Show AP screen before autoConnect (will try 3 retries, then start AP if failed)
  wm.setAPCallback([](WiFiManager* wm) {
    Serial.println("[AP] Starting AP (boot) - no WiFi connection");
    loadStartupIcons();
    screenApEnabled();
  });
  
  if (!wm.autoConnect("WX-DisplayAP")) {
    Serial.println("[SYS] AP failed, restarting");
    ESP.restart();
  }
  
  Serial.println("[WIFI] Connected: " + WiFi.localIP().toString());
  
  if (shouldSaveConfig) {
    Serial.println("[SYS] Restarting after config save");
    ESP.restart();
  }
}

void startWifiPortal() {
  Serial.println("[AP] Starting AP (runtime)");
  loadStartupIcons();
  screenApEnabled();

  WiFiManager wm;
  wm.setHostname(programName);
  wm.setTitle(programName);
  wm.setClass("invert");
  shouldSaveConfig = false;
  
  allocateWmParams();
  
  WiFiManagerParameter custom_html_display("<h3>LCD Display</h3>");
  WiFiManagerParameter custom_html_mqtt("<h3>MQTT</h3>");
  WiFiManagerParameter custom_html_stations("<h3>Stations</h3>");
  WiFiManagerParameter custom_html_station1("<label>Station 1 (Topic / Name) *</label><br><div style='display:flex;gap:2px;margin-top:0;padding:0;'>");
  WiFiManagerParameter custom_html_station1_end("</div>");
  WiFiManagerParameter custom_html_station2("<label>Station 2 (Topic / Name)</label><br><div style='display:flex;gap:2px;margin-top:0;padding:0;'>");
  WiFiManagerParameter custom_html_station2_end("</div>");
  WiFiManagerParameter custom_html_station3("<label>Station 3 (Topic / Name)</label><br><div style='display:flex;gap:2px;margin-top:0;padding:0;'>");
  WiFiManagerParameter custom_html_station3_end("</div>");
  WiFiManagerParameter custom_html_station4("<label>Station 4 (Topic / Name)</label><br><div style='display:flex;gap:2px;margin-top:0;padding:0;'>");
  WiFiManagerParameter custom_html_station4_end("</div>");
  
  wm.addParameter(&custom_html_display);
  wm.addParameter(p_displayType);
  wm.addParameter(p_displayInterval);
  wm.addParameter(&custom_html_mqtt);
  wm.addParameter(p_mqttServer);
  wm.addParameter(p_mqttPort);
  wm.addParameter(p_rootTopic);
  wm.addParameter(&custom_html_stations);
  wm.addParameter(&custom_html_station1);
  wm.addParameter(p_station1Topic);
  wm.addParameter(p_station1Name);
  wm.addParameter(&custom_html_station1_end);
  wm.addParameter(&custom_html_station2);
  wm.addParameter(p_station2Topic);
  wm.addParameter(p_station2Name);
  wm.addParameter(&custom_html_station2_end);
  wm.addParameter(&custom_html_station3);
  wm.addParameter(p_station3Topic);
  wm.addParameter(p_station3Name);
  wm.addParameter(&custom_html_station3_end);
  wm.addParameter(&custom_html_station4);
  wm.addParameter(p_station4Topic);
  wm.addParameter(p_station4Name);
  wm.addParameter(&custom_html_station4_end);
  
  wm.setSaveConfigCallback(saveConfigCallback);
  wm.setSaveParamsCallback(saveConfigCallback);
  wm.setBreakAfterConfig(true);
  wm.setConfigPortalBlocking(true);
  
  wm.startConfigPortal("WX-DisplayAP");

  Serial.println("[SYS] Restarting after AP portal");
  ESP.restart();
}

void handleWiFiDisconnect() {
  if (!wifiDisconnected) {
    Serial.println("[WIFI] Connection lost");
    wifiDisconnected = true;
    lastWifiReconnectAttempt = millis();
    mqttScreenShown = false;
    
    client.disconnect();
    
    loadStartupIcons();
    screenWifiDisconnected();
  }
}

void handleWiFiReconnect() {
  Serial.println("[WIFI] Reconnected: " + WiFi.localIP().toString());
  wifiDisconnected = false;
  mqttWasConnected = false;
  mqttScreenShown = false;
  readyOnce = false;
  firstRenderDone = false;
}

bool tryWiFiReconnect() {
  Serial.println("[WIFI] Attempting reconnect...");
  WiFi.reconnect();
  
  unsigned long reconnectStart = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - reconnectStart < 10000) {
    if (checkLongPress()) {
      startWifiPortal();
      return false;
    }
    yield();
  }
  
  return WiFi.status() == WL_CONNECTED;
}
