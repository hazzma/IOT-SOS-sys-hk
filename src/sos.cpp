#include "sos.h"
#include <Arduino.h>

static uint8_t ledPin;
static unsigned long unit;
static unsigned long lastChange = 0;

static const char* pattern = "...---...";
static int index = 0;
static bool ledState = false;
static bool printingDone = false;

static unsigned long getDuration(char symbol) {
    if (symbol == '.') return unit;
    if (symbol == '-') return 3 * unit;
    return unit;
}

void sos_init(uint8_t pin, unsigned long unitTime) {
    ledPin = pin;
    unit = unitTime;
    pinMode(ledPin, OUTPUT);
    digitalWrite(ledPin, LOW);
}

void sos_update() {
    unsigned long now = millis();

    if (!printingDone && index == 0) {
        Serial.println("SOS");
        printingDone = true;
    }

    if (now - lastChange >= getDuration(pattern[index])) {

        lastChange = now;

        if (ledState) {
            digitalWrite(ledPin, LOW);
            ledState = false;
            index++;

            if (pattern[index] == '\0') {
                index = 0;
                printingDone = false;
                delay(7 * unit);  // Cycle gap
            } else {
                delay(unit); // Symbol gap
            }

        } else {
            digitalWrite(ledPin, HIGH);
            ledState = true;
        }
    }
}