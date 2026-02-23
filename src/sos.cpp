#include "sos.h"
#include <Arduino.h>

static uint8_t ledPin;
static unsigned long unit;
static unsigned long lastChange = 0;

static const char* pattern = "...---...";
static int patternIndex = 0;
static bool ledState = false;
static bool printingDone = false;
static bool waitingGap = false;

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

    if (!printingDone && patternIndex == 0) {
        Serial.println("SOS");
        printingDone = true;
    }

    if (now - lastChange >= getDuration(pattern[patternIndex])) {

        lastChange = now;

        if (ledState) {
            digitalWrite(ledPin, LOW);
            ledState = false;

            waitingGap = true;
        } else {
            if (waitingGap) {
                waitingGap = false;
                patternIndex++;

                if (pattern[patternIndex] == '\0') {
                    patternIndex = 0;
                    printingDone = false;
                }
            }

            digitalWrite(ledPin, HIGH);
            ledState = true;
        }
    }
}
