#pragma once

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include "config.h"
#include "stations.h"

extern LiquidCrystal_I2C lcd;

extern uint8_t LCD_COLS;
extern uint8_t LCD_ROWS;
extern uint8_t stationsPerScreen;

// Display state
extern bool backlightOn;
extern unsigned long lastPageSwitch;
extern int currentPage;
extern int topPage;
extern int bottomPage;
extern bool firstRenderDone;
extern unsigned long startTime;

// Functions
void initDisplay();
void loadStartupIcons();
void loadMainIcons();
void clearLine(uint8_t row);
int centerPosition(const String& text);
int centerPositionHalf(const String& text);
void renderStation16x2(int logical);
void renderStations20x4(int topIdx, int bottomIdx);
void renderStations40x2(int topIdx, int bottomIdx);
