#include "Lighting.h"

static uint8_t currentBrightness[ZONE_COUNT] = {0};
static uint8_t targetBrightness[ZONE_COUNT]  = {0};

const uint16_t FADE_STEP_MS = 15;
const uint8_t FADE_STEP = 4;
uint32_t lastFadeUpdate = 0;

void lightingInit() {
    for (int i = 0; i < ZONE_COUNT; i++) {
        ledcSetup(ZONE_CHANNELS[i], PWM_FREQ, PWM_RES);
        ledcAttachPin(ZONE_PINS[i], ZONE_CHANNELS[i]);
        ledcWrite(ZONE_CHANNELS[i], 0);
    }
}

void lightingSetTarget(int zone, uint8_t value) {
    if (zone < 0 || zone >= ZONE_COUNT) return;
    targetBrightness[zone] = value;
    Serial.println("Set Light "+String(zone)+" to "+String(value));
}

uint8_t lightingGetCurrent(int zone) {
    return currentBrightness[zone];
}

void lightingUpdate() {
    uint32_t now = millis();
    if (now - lastFadeUpdate < FADE_STEP_MS) return;
    lastFadeUpdate = now;

    for (int i = 0; i < ZONE_COUNT; i++) {
        uint8_t cur = currentBrightness[i];
        uint8_t tgt = targetBrightness[i];

        if (cur == tgt) continue;

        if (cur < tgt) {
            cur = min<uint8_t>(tgt, cur + FADE_STEP);
        } else {
            cur = max<uint8_t>(tgt, cur - FADE_STEP);
        }

        currentBrightness[i] = cur;
        ledcWrite(ZONE_CHANNELS[i], cur);
    }
}
