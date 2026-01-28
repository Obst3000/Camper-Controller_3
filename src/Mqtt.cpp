#include <WiFi.h>
#include <PubSubClient.h>
#include "Config.h"
#include "Lighting.h"

// Network and MQTT clients (global instances)
WiFiClient wifi_client;
PubSubClient mqtt_client(wifi_client);

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

    mqtt_client.setServer(MQTT_SERVER, MQTT_PORT);
    mqtt_client.setCallback(mqttCallback);
}

void mqttUpdate() {
    if (!mqtt_client.connected()) {
        mqtt_client.connect("camper_controller");
        mqtt_client.subscribe("camper/zone/+/set");
    }
    mqtt_client.loop();
}
