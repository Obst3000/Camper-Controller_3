#include <WiFi.h>
#include <PubSubClient.h>
#include "Config.h"
#include "Lighting.h"

WiFiClient wifi;
PubSubClient mqtt(wifi);

void mqttCallback(char* topic, byte* payload, unsigned int len) {
   /* String t = topic;
    String p = "";

    for (uint8_t i = 0; i < len; i++)
        p += (char)payload[i];

    if (t.startsWith("camper/zone/") && t.endsWith("/set")) {
        int zone = t.substring(12, 13).toInt() - 1;
        uint8_t val = p.toInt();
        lightingSetTarget(zone, val);
    }
    */
}

void mqttInit() {
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    while (WiFi.status() != WL_CONNECTED) delay(200);

    mqtt.setServer(MQTT_SERVER, MQTT_PORT);
    mqtt.setCallback(mqttCallback);
}

void mqttUpdate() {
    if (!mqtt.connected()) {
        mqtt.connect("camper_controller");
        mqtt.subscribe("camper/zone/+/set");
    }
    mqtt.loop();
}
