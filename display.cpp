#include "display.h"
#include "icons.h"

LiquidCrystal_I2C lcd(LCD_ADDR, 16, 2);

uint8_t LCD_COLS;
uint8_t LCD_ROWS;
uint8_t stationsPerScreen;

// Display state
bool backlightOn = true;
unsigned long lastPageSwitch = 0;
int currentPage = 0;
int topPage = 0;
int bottomPage = 0;
bool firstRenderDone = false;
unsigned long startTime = 0;

void initDisplay() {
  switch (displayType) {
    case DISPLAY_16x2:
      LCD_COLS = 16;
      LCD_ROWS = 2;
      stationsPerScreen = 1;
      break;
    case DISPLAY_20x4:
      LCD_COLS = 20;
      LCD_ROWS = 4;
      stationsPerScreen = 2;
      break;
    case DISPLAY_40x2:
      LCD_COLS = 40;
      LCD_ROWS = 2;
      stationsPerScreen = 2;
      break;
  }

  lcd = LiquidCrystal_I2C(LCD_ADDR, LCD_COLS, LCD_ROWS);
  lcd.init();
  lcd.backlight();
}

void loadStartupIcons() {
  lcd.createChar(0, tempchar1);
  lcd.createChar(1, tempchar2);
  lcd.createChar(2, tempchar3);
  lcd.createChar(3, tempchar4);
}

void loadMainIcons() {
  lcd.createChar(4, symbolTemp);
  lcd.createChar(5, symbolHumi);
  lcd.createChar(6, symbolPress);
  lcd.createChar(7, symbolHumiFull);
}

void clearLine(uint8_t row) {
  lcd.setCursor(0, row);
  for (int i = 0; i < LCD_COLS; i++) lcd.print(" ");
}

int centerPosition(const String& text) {
  return (LCD_COLS - text.length()) / 2;
}

int centerPositionHalf(const String& text) {
  return (20 - text.length()) / 2;
}

void renderStation16x2(int logical) {
  int s = activeIdx[logical];
  clearLine(0);
  clearLine(1);

  lcd.setCursor(centerPosition(names[s]), 0);
  lcd.print(names[s]);

  bool valid = isStationDataValid(s);
  
  lcd.setCursor(0,1);
  lcd.write(byte(4));
  if (valid) lcd.print(data[s].temperature,1);
  else lcd.print("---");

  lcd.setCursor(7,1);
  lcd.write(data[s].humidity >= 100 ? byte(7) : byte(5));
  if (valid) lcd.print(min(data[s].humidity, 99));
  else lcd.print("--");

  lcd.setCursor(11,1);
  lcd.write(byte(6));
  if (valid) lcd.print(data[s].pressure,0);
  else lcd.print("----");
}

void renderStations20x4(int topIdx, int bottomIdx) {
  lcd.clear();

  // Top station (rows 0-1)
  int first = activeIdx[topIdx];
  bool validFirst = isStationDataValid(first);
  
  lcd.setCursor(centerPosition(names[first]), 0);
  lcd.print(names[first]);

  lcd.setCursor(0,1);
  lcd.write(byte(4));
  if (validFirst) lcd.print(data[first].temperature,1);
  else lcd.print("---");
  
  lcd.setCursor(8,1);
  lcd.write(data[first].humidity >= 100 ? byte(7) : byte(5));
  if (validFirst) lcd.print(data[first].humidity);
  else lcd.print("--");
  
  lcd.setCursor(15,1);
  lcd.write(byte(6));
  if (validFirst) lcd.print(data[first].pressure,0);
  else lcd.print("----");

  // Bottom station (rows 2-3) - only if exists
  if (bottomIdx < 0 || bottomIdx >= activeCount) return;

  int second = activeIdx[bottomIdx];
  bool validSecond = isStationDataValid(second);
  
  lcd.setCursor(centerPosition(names[second]), 2);
  lcd.print(names[second]);

  lcd.setCursor(0,3);
  lcd.write(byte(4));
  if (validSecond) lcd.print(data[second].temperature,1);
  else lcd.print("---");
  
  lcd.setCursor(8,3);
  lcd.write(data[second].humidity >= 100 ? byte(7) : byte(5));
  if (validSecond) lcd.print(data[second].humidity);
  else lcd.print("--");
  
  lcd.setCursor(15,3);
  lcd.write(byte(6));
  if (validSecond) lcd.print(data[second].pressure,0);
  else lcd.print("----");
}

void renderStations40x2(int topIdx, int bottomIdx) {
  lcd.clear();

  // Top station (row 0)
  int first = activeIdx[topIdx];
  bool validFirst = isStationDataValid(first);
  
  lcd.setCursor(centerPositionHalf(names[first]), 0);
  lcd.print(names[first]);

  lcd.setCursor(20, 0);
  lcd.write(byte(4));
  if (validFirst) lcd.print(data[first].temperature, 1);
  else lcd.print("---");
  
  lcd.setCursor(28, 0);
  lcd.write(data[first].humidity >= 100 ? byte(7) : byte(5));
  if (validFirst) lcd.print(data[first].humidity);
  else lcd.print("--");
  
  lcd.setCursor(35, 0);
  lcd.write(byte(6));
  if (validFirst) lcd.print(data[first].pressure, 0);
  else lcd.print("----");

  // Bottom station (row 1) - only if exists
  if (bottomIdx < 0 || bottomIdx >= activeCount) return;

  int second = activeIdx[bottomIdx];
  bool validSecond = isStationDataValid(second);
  
  lcd.setCursor(centerPositionHalf(names[second]), 1);
  lcd.print(names[second]);

  lcd.setCursor(20, 1);
  lcd.write(byte(4));
  if (validSecond) lcd.print(data[second].temperature, 1);
  else lcd.print("---");
  
  lcd.setCursor(28, 1);
  lcd.write(data[second].humidity >= 100 ? byte(7) : byte(5));
  if (validSecond) lcd.print(data[second].humidity);
  else lcd.print("--");
  
  lcd.setCursor(35, 1);
  lcd.write(byte(6));
  if (validSecond) lcd.print(data[second].pressure, 0);
  else lcd.print("----");
}
