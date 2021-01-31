#ifndef _LED_CONTROL_H
#define _LED_CONTROL_H

#include <Arduino.h>

bool setupLED();

bool turnOnLight();

bool turnOffLight();

void setLightIntensity(uint8_t intensity);

bool lightIsOn();

#endif