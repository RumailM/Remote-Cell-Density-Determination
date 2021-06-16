#ifndef _SMARTCLAMP_LED_H
#define _SMARTCLAMP_LED_H

#include <Arduino.h>

bool setupLED();

void turnOnLight(uint8_t channel);

void turnOffLight(uint8_t channel);

void setLightIntensity(uint8_t channel, uint8_t intensity);

#endif