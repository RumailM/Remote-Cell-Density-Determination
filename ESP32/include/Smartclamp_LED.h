/*!
 *  @file Smartclamp_LED.h
 *
 *  This file contains all functions related to LED Control
 *
 *  Developed by Ahmad Nasralla (an2485@nyu.edu) 
 * 
 */

#ifndef _SMARTCLAMP_LED_H
#define _SMARTCLAMP_LED_H

#include <Arduino.h>

// Constants
const double LED_FREQ = 5000; // PWM Frequency
const uint8_t LED_RES = 8;    // 8-bit ADC Resolution
const uint8_t LED_PIN_RED = 26;    // PWM Pin Red
const uint8_t LED_PIN_GREEN = 4;    // PWM Pin Green
const uint8_t LED_CH_RED = 0;     // PWM Channel Red
const uint8_t LED_CH_GREEN = 1;     // PWM Channel Green

// LED Color
typedef enum
{
    LZ7_COLOR_RED,
    LZ7_COLOR_GREEN,
} lz7_color;

const lz7_color DEFAULT_LZ7_COLOR = LZ7_COLOR_RED;
const uint16_t DEFAULT_WAKE_TIME = 10;
const uint16_t DEFAULT_SLEEP_TIME = 50;

class Smartclamp_LED
{
public:

    // Constants
    lz7_color color;
    uint16_t wakeTime, sleepTime;
    uint8_t dutyCycle;
    bool isAwake;
    unsigned long slp_millis;

    Smartclamp_LED();
    ~Smartclamp_LED();

    ///////////////////   LED CONTROL    ///////////////
    bool setupLED();
    void turnOnLight(uint8_t channel);
    void turnOffLight(uint8_t channel);
    void setLightIntensity(uint8_t channel, uint8_t intensity);

    ///////////////////   SETTERS AND GETTERS    ///////////////
    void setColor(lz7_color color);
    void setWakeTime(uint16_t wakeTime);
    void setSleepTime(uint16_t sleepTime);
    uint8_t getChannelFromColor(lz7_color clr);
    lz7_color getColor();
    uint16_t getWakeTime();
    uint16_t getSleepTime();

};

#endif