#pragma once
#include <Arduino.h>
#include "Config.h"

void lightingInit();
void lightingUpdate();
void lightingSetTarget(int zone, uint8_t value);
uint8_t lightingGetCurrent(int zone);
