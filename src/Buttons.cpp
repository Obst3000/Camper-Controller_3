#include "Buttons.h"
#include "Config.h"
#include "Lighting.h"

AceButton* buttons[ZONE_COUNT];

void handleButton(AceButton*, uint8_t, uint8_t);

void buttonsInit() {
    for (int i = 0; i < ZONE_COUNT; i++) {
        pinMode(BUTTON_PINS[i], INPUT_PULLUP);
        buttons[i] = new AceButton(BUTTON_PINS[i]);
    }

    ButtonConfig* cfg = ButtonConfig::getSystemButtonConfig();
    cfg->setEventHandler(handleButton);
    cfg->setFeature(ButtonConfig::kFeatureClick);
    cfg->setFeature(ButtonConfig::kFeatureDoubleClick);
    cfg->setFeature(ButtonConfig::kFeatureLongPress);


}

void buttonsUpdate() {
    for (int i = 0; i < ZONE_COUNT; i++)
        buttons[i]->check();
}

void handleButton(AceButton* btn, uint8_t eventType, uint8_t state) {
    int pin = btn->getPin();
    int zone = -1;

   // Serial.println("Button Event Detected!");
    for (int i = 0; i < ZONE_COUNT; i++)
        if (BUTTON_PINS[i] == pin) {
        zone = i;}
    if (zone < 0) return;

    
    switch (eventType) {
        case AceButton::kEventClicked:
            Serial.println("Button "+String(zone)+" Clicked!");
            lightingSetTarget(zone, lightingGetCurrent(zone) == 0 ? 200 : 0);
            break;

        case AceButton::kEventDoubleClicked:
                    Serial.println("Button "+String(zone)+" DoubleClicked!");
            lightingSetTarget(zone, 255);
            break;

        case AceButton::kEventLongPressed:
                    Serial.println("Button "+String(zone)+" Long Clicked!");
            lightingSetTarget(zone, 128);
            break;
    }
}
