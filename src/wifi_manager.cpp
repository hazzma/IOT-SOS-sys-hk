#include "wifi_manager.h"
#include <Arduino.h>
#include <WiFi.h>

#define WIFI_TIMEOUT 15000
#define RETRY_INTERVAL 10000

const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";

static unsigned long connectStart = 0;
static unsigned long lastRetry = 0;
static bool connected = false;

void wifi_init() {
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    connectStart = millis();
    Serial.println("WiFi Connecting...");
}

void wifi_update() {
    if (WiFi.status() == WL_CONNECTED) {
        if (!connected) {
            connected = true;
            Serial.println("WiFi Connected!");
            Serial.print("IP: ");
            Serial.println(WiFi.localIP());
        }
        return;
    }

    connected = false;

    if (millis() - connectStart > WIFI_TIMEOUT) {
        if (millis() - lastRetry > RETRY_INTERVAL) {
            Serial.println("WiFi Retry...");
            WiFi.disconnect();
            WiFi.begin(ssid, password);
            lastRetry = millis();
        }
    }
}

bool wifi_isConnected() {
    return WiFi.status() == WL_CONNECTED;
}