#include "screens.h"

void screenWaitingWifi() {
  lcd.clear();
  int offsetX, offsetY;
  if (displayType == DISPLAY_40x2) {
    offsetX = 12;
    offsetY = 0;
  } else if (displayType == DISPLAY_20x4) {
    offsetX = 2;
    offsetY = 1;
  } else {
    offsetX = 0;
    offsetY = 0;
  }
  lcd.setCursor(0 + offsetX, 0 + offsetY); lcd.write(byte(0));
  lcd.setCursor(0 + offsetX, 1 + offsetY); lcd.write(byte(1));
  lcd.setCursor(1 + offsetX, 0 + offsetY); lcd.write(byte(2));
  lcd.setCursor(1 + offsetX, 1 + offsetY); lcd.write(byte(3));
  lcd.setCursor(5 + offsetX, 0 + offsetY); lcd.print("wxmet.cz");
  lcd.setCursor(3 + offsetX, 1 + offsetY); lcd.print("cekam na WiFi");
}

void screenWaitingMqtt() {
  lcd.clear();
  int offsetX, offsetY;
  if (displayType == DISPLAY_40x2) {
    offsetX = 12;
    offsetY = 0;
  } else if (displayType == DISPLAY_20x4) {
    offsetX = 2;
    offsetY = 1;
  } else {
    offsetX = 0;
    offsetY = 0;
  }
  lcd.setCursor(0 + offsetX, 0 + offsetY); lcd.write(byte(0));
  lcd.setCursor(0 + offsetX, 1 + offsetY); lcd.write(byte(1));
  lcd.setCursor(1 + offsetX, 0 + offsetY); lcd.write(byte(2));
  lcd.setCursor(1 + offsetX, 1 + offsetY); lcd.write(byte(3));
  lcd.setCursor(5 + offsetX, 0 + offsetY); lcd.print("wxmet.cz");
  lcd.setCursor(3 + offsetX, 1 + offsetY); lcd.print("cekam na data");
}

void screenWifiDisconnected() {
  lcd.clear();
  int offsetX, offsetY;
  if (displayType == DISPLAY_40x2) {
    offsetX = 12;
    offsetY = 0;
  } else if (displayType == DISPLAY_20x4) {
    offsetX = 2;
    offsetY = 1;
  } else {
    offsetX = 0;
    offsetY = 0;
  }
  lcd.setCursor(0 + offsetX, 0 + offsetY); lcd.write(byte(0));
  lcd.setCursor(0 + offsetX, 1 + offsetY); lcd.write(byte(1));
  lcd.setCursor(1 + offsetX, 0 + offsetY); lcd.write(byte(2));
  lcd.setCursor(1 + offsetX, 1 + offsetY); lcd.write(byte(3));
  lcd.setCursor(5 + offsetX, 0 + offsetY); lcd.print("wxmet.cz");
  lcd.setCursor(3 + offsetX, 1 + offsetY); lcd.print("WiFi odpojena");
}

void screenApEnabled() {
  lcd.clear();
  int offsetX, offsetY;
  if (displayType == DISPLAY_40x2) {
    offsetX = 12;
    offsetY = 0;
  } else if (displayType == DISPLAY_20x4) {
    offsetX = 2;
    offsetY = 1;
  } else {
    offsetX = 0;
    offsetY = 0;
  }
  lcd.setCursor(0 + offsetX, 0 + offsetY); lcd.write(byte(0));
  lcd.setCursor(0 + offsetX, 1 + offsetY); lcd.write(byte(1));
  lcd.setCursor(1 + offsetX, 0 + offsetY); lcd.write(byte(2));
  lcd.setCursor(1 + offsetX, 1 + offsetY); lcd.write(byte(3));
  lcd.setCursor(5 + offsetX, 0 + offsetY); lcd.print("wxmet.cz");
  lcd.setCursor(3 + offsetX, 1 + offsetY); lcd.print("AP je zapnuto");
}
