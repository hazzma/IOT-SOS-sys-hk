#ifndef SOS_H
#define SOS_H

#include <Arduino.h>

void sos_init(uint8_t pin, unsigned long unitTime);
void sos_update();

#endif
