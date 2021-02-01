#ifndef _SMARTCLAMP_LED_H
#define _SMARTCLAMP_LED_H

#include <Arduino.h>

bool setupLED();

void turnOnLight();

void turnOffLight();

void setLightIntensity(uint8_t intensity);

#endif