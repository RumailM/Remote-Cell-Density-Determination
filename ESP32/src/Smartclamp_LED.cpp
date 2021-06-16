#include <SMARTCLAMP_LED.h>

const double LED_FREQ = 5000; // PWM Frequency
const uint8_t LED_RES = 8;    // 8-bit ADC Resolution
const uint8_t LED_PIN_RED = 4;    // PWM Pin Red
const uint8_t LED_PIN_GREEN = 25;    // PWM Pin Green
const uint8_t LED_CH_RED = 0;     // PWM Channel Red
const uint8_t LED_CH_GREEN = 1;     // PWM Channel Green

uint8_t dutyCycle = 255; // 8-bit Duty Cycle

bool setupLED()
{
    ledcSetup(LED_CH_RED, LED_FREQ, LED_RES);
    ledcAttachPin(LED_PIN_RED, LED_CH_RED);
    ledcSetup(LED_CH_GREEN, LED_FREQ, LED_RES);
    ledcAttachPin(LED_PIN_GREEN, LED_CH_GREEN);

    return (true);
}

void turnOnLight(uint8_t channel)
{
    setLightIntensity(channel, 255);
}

void turnOffLight(uint8_t channel)
{
    setLightIntensity(channel, 0);
}

void setLightIntensity(uint8_t channel, uint8_t intensity)
{
    dutyCycle = intensity;
    ledcWrite(channel, dutyCycle);
}