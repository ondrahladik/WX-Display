# WX-Display

The **WX-Display** project was created as part of my larger project **WX-Station**, but can also be used independently. The firmware is written in the Arduino IDE and is designed to display weather data from MQTT on an LCD display connected to a **Wemos D1 mini (ESP8266)**.

## Hardware

- Wemos D1 mini (ESP8266)
- I2C LCD (16x2, 20x4 or 40x2 with I2C converter, address `0x27`)
- Button on pin **D3** (against GND, uses internal pull-up)

## Installation (Arduino IDE)

Installing the firmware on the device is quite simple. Download and install **Arduino IDE**, add support for **ESP8266**, install the required libraries, and then upload the firmware to the device.

- Required libraries:
   - `WiFiManager`
   - `PubSubClient`
   - `LiquidCrystal_I2C`
   - `ArduinoJson`

## First Start

On the first start, the device will switch to AP mode:

- Wi‑Fi network: **WX-DisplayAP**
- connect with your phone/PC and open the configuration page `192.168.4.1`

After saving the configuration, the device will restart. 

## Configuration

The configuration is stored in EEPROM and is set via the WiFiManager portal.

### Required fields

- **Wi-Fi SSID**: Name of the Wi-Fi network to connect to.
- **Wi-Fi Password**: Password for the Wi-Fi network.
- **Display Type**: `0=16x2`, `1=20x4`, `2=40x2`
- **Display Interval (ms)**: Screen switching interval (minimum 1000 ms).
- **MQTT Server**: IP address or hostname of the MQTT broker.
- **MQTT Port**: Port number of the MQTT broker (default is 1883).
- **Root Topic**: The base topic for MQTT messages (e.g., `weather`).
- **Station 1 (Topic / Name)**: The topic and display name for the first weather station (e.g., `station1 / Living Room`).

Stations 2–4 are optional.

> Note: Do not use diacritics in text fields.

## MQTT message format

The device expects JSON, for example:

```json
{
  "temperature": 22.4,
  "humidity": 48,
  "pressure": 1013
}
```

The subscribed topic is in the format:

`<rootTopic>/<stationTopic>`

## Button Control

- **Short press**: turn the display backlight on/off
- **Long press (5 s)**: start the configuration AP portal (`WX-DisplayAP`)

## Display functions

Various states that may appear on the display are described below. These states are currently only in Czech, but translating them is not difficult. They can be translated in the Arduino IDE by simply modifying the text in the file `screens.cpp`.

- **cekam na WiFi**: At this moment, the display is attempting to connect to your WiFi network. If this is the first time powering on the display, please follow the steps in the `First Start` section.
- **cekam na data**: At this moment, the display is waiting for data from the stations configured in the settings. For stations within the WXMET network, this should take no longer than 1 minute. If any of the configured stations do not send data within 5 minutes, the data will still be displayed after 5 minutes, and --- will appear for stations with no data. Once the data becomes available, it will be displayed normally.
- **AP je zapnuto**: At this moment (after a long button press, during the first startup, or if the display fails to connect to the configured WiFi network), it will start its own AP named `WX-DisplayAP`. You can then continue according to the `First Start` and `Configuration` sections.
- **WiFi odpojena**: At this moment, the WiFi connection has been lost. If the WiFi network is restored, the station should reconnect within a maximum of 5 minutes.

If, during display operation, data from any of the configured stations is unavailable for more than 5 minutes, --- will be shown for that station. Once the data becomes available, it will be displayed normally.