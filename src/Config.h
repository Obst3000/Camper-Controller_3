#pragma once
#include <Arduino.h>

// Lighting zones
static const int ZONE_COUNT = 4;
static const int ZONE_PINS[ZONE_COUNT] = {27, 18, 19, 21};
static const int ZONE_CHANNELS[ZONE_COUNT] = {0, 1, 2, 3};
static const int PWM_FREQ = 5000;
static const int PWM_RES = 8;

// Buttons
static const int BUTTON_PINS[ZONE_COUNT] = {32, 33, 25, 26};

// WiFi & MQTT
static const char* WIFI_SSID = "YOUR_SSID";
static const char* WIFI_PASS = "YOUR_PASS";
static const char* MQTT_SERVER = "192.168.1.100";
static const uint16_t MQTT_PORT = 1883;
