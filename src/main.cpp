#include <Arduino.h>
#include "Lighting.h"
#include "Buttons.h"
#include "Mqtt.h"

void setup() {
    Serial.begin(115200);

    lightingInit();
        Serial.println("Init Lighting");
    buttonsInit();
        Serial.println("Init Buttons");
    //mqttInit();

    Serial.println("Init Finished");
}

void loop() {
    buttonsUpdate();
   // mqttUpdate();
    lightingUpdate(); // fades happen here


}
