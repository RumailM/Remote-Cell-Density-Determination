/*!
 *  @file Smartclamp_LED.cpp
 *
 *  This file contains all functions related to LED Control
 *
 *  Developed by Ahmad Nasralla (an2485@nyu.edu) 
 * 
 */

#include <Smartclamp_LED.h>

/**
 * @brief Construct a new Smartclamp_LED::Smartclamp_LED object
 * 
 */
Smartclamp_LED::Smartclamp_LED(){};

/**
 * @brief Destroy the Smartclamp_LED::Smartclamp_LED object
 * 
 */
Smartclamp_LED::~Smartclamp_LED(){};

/**
 * @brief Setup the ESP32 LEDC PWM Outputs and Channels
 * 
 */
bool Smartclamp_LED::setupLED()
{
    ledcSetup(LED_CH_RED, LED_FREQ, LED_RES);
    ledcAttachPin(LED_PIN_RED, LED_CH_RED);
    ledcSetup(LED_CH_GREEN, LED_FREQ, LED_RES);
    ledcAttachPin(LED_PIN_GREEN, LED_CH_GREEN);
    isAwake = false;

    return true;
}

/**
 * @brief Sets the PWM Output of channel to its maximum value
 *
 * @param channel PWM Channel to be controlled
 * 
 */
void Smartclamp_LED::turnOnLight(uint8_t channel)
{
    setLightIntensity(channel, 255);
}

/**
 * @brief Sets the PWM Output of channel to zero
 *
 * @param channel PWM Channel to be controlled
 * 
 */
void Smartclamp_LED::turnOffLight(uint8_t channel)
{
    setLightIntensity(channel, 0);
}

/**
 * @brief Sets the PWM Output of channel to intensity
 *
 * @param channel PWM Channel to be controlled
 * @param intensity 8-bit intensity scale (duty cycle) of PWM Output
 * 
 */
void Smartclamp_LED::setLightIntensity(uint8_t channel, uint8_t intensity)
{
    dutyCycle = intensity;
    ledcWrite(channel, dutyCycle);
}

// Setters
void Smartclamp_LED::setColor(lz7_color color)
{
    this->color = color;
}

void Smartclamp_LED::setWakeTime(uint16_t wakeTime)
{
    this->wakeTime = wakeTime;
}

void Smartclamp_LED::setSleepTime(uint16_t sleepTime)
{
    this->sleepTime = sleepTime;
}

// Getters
lz7_color Smartclamp_LED::getColor(){return color;}
uint16_t Smartclamp_LED::getWakeTime(){return wakeTime;}
uint16_t Smartclamp_LED::getSleepTime(){return sleepTime;}