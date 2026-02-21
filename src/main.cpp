#include <Arduino.h>
#include <ArduinoOTA.h>
#include "sos.h"
#include "wifi_manager.h"

#define UNIT_TIME 200  // Morse base unit in ms

void setup() {
    Serial.begin(115200);
    delay(1000);
    Serial.println("System Booting...");

    sos_init(LED_BUILTIN, UNIT_TIME);
    wifi_init();

    // OTA setup
    ArduinoOTA.onStart([]() {
        Serial.println("OTA Start");
    });

    ArduinoOTA.onEnd([]() {
        Serial.println("\nOTA End");
    });

    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
        Serial.printf("OTA Progress: %u%%\r", (progress * 100) / total);
    });

    ArduinoOTA.onError([](ota_error_t error) {
        Serial.printf("OTA Error[%u]\n", error);
    });

    ArduinoOTA.begin();
    Serial.println("OTA Ready");
}

void loop() {
    sos_update();
    wifi_update();

    if (wifi_isConnected()) {
        ArduinoOTA.handle();
    }
}