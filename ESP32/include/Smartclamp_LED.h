#ifndef _SMARTCLAMP_LED_H
#define _SMARTCLAMP_LED_H

#include <Arduino.h>

bool setupLED();

bool turnOnLight();

bool turnOffLight();

void setLightIntensity(uint8_t intensity);

bool lightIsOn();

#endif