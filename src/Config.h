#pragma once
#include <Arduino.h>

// Lighting zones (physical pin assignments)
static const int LeftRearLedPin = 4;
static const int RightRearLedPin = 5;
static const int LeftFrontLedPin = 18;
static const int RightFrontLedPin = 19;
static const int KitchenLedPin = 21;
static const int LuggageLedPin = 22;
static const int SideDoorLedPin = 23;

// Number of zones — keep in sync with the number of entries in ZONE_PINS
static const int ZONE_COUNT = 7;

// Arrays for iterating zones. Use these where you need to loop over all zones.
static const int ZONE_PINS[ZONE_COUNT] = { LeftRearLedPin, RightRearLedPin, LeftFrontLedPin, RightFrontLedPin, KitchenLedPin, LuggageLedPin, SideDoorLedPin };
static const int ZONE_CHANNELS[ZONE_COUNT] = { 0, 1, 2, 3, 4, 5, 6 };

// PWM configuration used for LED output
static const int PWM_FREQ = 5000;
static const int PWM_RES = 8;


// Exported LED instances and an array of pointers (defined in Lighting.cpp).
// Other modules may iterate `leds[]` or use the named externs for direct access.
#include "Led.h"
extern Led* leds[ZONE_COUNT];
extern Led led_left_rear;
extern Led led_right_rear;
extern Led led_left_front;
extern Led led_right_front;
extern Led led_kitchen;
extern Led led_luggage;
extern Led led_side_door;

// Button pin assignments
static const int RearBedPin = 14;
static const int FrontBedPin = 27;
static const int LeftKitchenPin = 26;
static const int RightKitchenPin = 25;
static const int LuggagePin = 33;
static const int SideDoorPin = 32;

// Button configuration
static const int BUTTON_COUNT = 7;
// Ensure the number of entries in BUTTON_PINS equals BUTTON_COUNT.
// If you change BUTTON_COUNT, update this array accordingly.
static const int BUTTON_PINS[BUTTON_COUNT] = { RearBedPin, FrontBedPin, LeftKitchenPin, RightKitchenPin, LuggagePin, SideDoorPin };

// WiFi & MQTT
static const char* WIFI_SSID = "YOUR_SSID";
static const char* WIFI_PASS = "YOUR_PASS";
static const char* MQTT_SERVER = "192.168.1.100";
static const uint16_t MQTT_PORT = 1883;
