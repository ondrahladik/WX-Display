#include "input.h"
#include "display.h"
#include "wifi.h"

// Button state
bool lastButtonReading = HIGH;
bool debouncedState = HIGH;
unsigned long lastDebounceTime = 0;
bool buttonPressed = false;
unsigned long buttonPressStart = 0;
bool longPressDetected = false;

bool checkLongPress() {
  static bool btnPressed = false;
  static unsigned long btnStart = 0;
  
  bool reading = digitalRead(buttonPin);
  
  if (reading == LOW && !btnPressed) {
    btnPressed = true;
    btnStart = millis();
  }
  
  if (reading == LOW && btnPressed) {
    if (millis() - btnStart >= longPress) {
      Serial.println("[BTN] Long press detected");
      return true;
    }
  }
  
  if (reading == HIGH) {
    btnPressed = false;
  }
  
  return false;
}

void handleButton() {
  bool reading = digitalRead(buttonPin);

  if (reading != lastButtonReading) lastDebounceTime = millis();
  if ((millis() - lastDebounceTime) > debouceDelay) debouncedState = reading;
  lastButtonReading = reading;

  bool pressed = (debouncedState == LOW);

  if (pressed && !buttonPressed) {
    buttonPressed = true;
    buttonPressStart = millis();
    longPressDetected = false;
  }

  if (pressed && buttonPressed && !longPressDetected) {
    if (millis() - buttonPressStart >= longPress) {
      longPressDetected = true;
      Serial.println("[BTN] Long press in loop");
      startWifiPortal();
    }
  }

  if (!pressed && buttonPressed) {
    if (!longPressDetected) {
      backlightOn = !backlightOn;
      backlightOn ? lcd.backlight() : lcd.noBacklight();
    }
    buttonPressed = false;
  }
}
