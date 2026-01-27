#pragma once
#include <Arduino.h>

// Lighting zones
static const int LeftRearLedPin=4;
static const int RightRearLedPin=5;
static const int LeftFrontLedPin=18;
static const int RightFrontLedPin=19;
static const int KitchenLedPin=21;
static const int LuggageLedPin=22;
static const int SideDoorLedPin=23;

static const int ZONE_COUNT = 7;
//static const int ZONE_PINS[ZONE_COUNT] = {2,4,5,18, 19, 21,22};
//static const int ZONE_CHANNELS[ZONE_COUNT] = {0, 1, 2, 3,4,5,6};
static const int PWM_FREQ = 5000;
static const int PWM_RES = 8;


// Export Led objects array (defined in Lighting.cpp)
#include "Led.h"
extern Led leds[ZONE_COUNT];
// Named externs for direct access from other modules
extern Led LeftRearLed;
extern Led RightRearLed;
extern Led LeftFrontLed;
extern Led RightFrontLed;
extern Led KitchenLed;
extern Led LuggageLed;
extern Led SideDoorLed;

// Buttons
static const int RearBedPin=14;
static const int FrontBedPin=27;
static const int LeftKitchenPin=26;
static const int RightKitchenPin=25;
static const int LuggagePin=33;
static const int SideDoorPin=32;

static const int BUTTON_COUNT = 7;
static const int BUTTON_PINS[BUTTON_COUNT] = {RearBedPin, FrontBedPin, LeftKitchenPin, RightKitchenPin,LuggagePin,SideDoorPin  };

// WiFi & MQTT
static const char* WIFI_SSID = "YOUR_SSID";
static const char* WIFI_PASS = "YOUR_PASS";
static const char* MQTT_SERVER = "192.168.1.100";
static const uint16_t MQTT_PORT = 1883;
