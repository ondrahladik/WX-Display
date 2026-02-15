#pragma once

#include <Arduino.h>
#include "config.h"

// Button state
extern bool lastButtonReading;
extern bool debouncedState;
extern unsigned long lastDebounceTime;
extern bool buttonPressed;
extern unsigned long buttonPressStart;
extern bool longPressDetected;

// Functions
bool checkLongPress();
void handleButton();
