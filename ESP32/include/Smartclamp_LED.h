#ifndef _SMARTCLAMP_LED_H
#define _SMARTCLAMP_LED_H

#include <Arduino.h>

const uint8_t LED_PIN = 4;    // PWM Pin
const double LED_FREQ = 4080; // PWM Frequency
const uint8_t LED_CH = 0;     // PWM Channel
const uint8_t LED_RES = 8;    // 8-bit ADC Resolution

uint8_t dutyCycle = 255; // 8-bit Duty Cycle

bool setupLED();

void turnOnLight();

void turnOffLight();

void setLightIntensity(uint8_t intensity);

#endif