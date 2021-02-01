#include <SMARTCLAMP_LED.h>

const uint8_t LED_PIN = 4;    // PWM Pin
const double LED_FREQ = 4080; // PWM Frequency
const uint8_t LED_CH = 0;     // PWM Channel
const uint8_t LED_RES = 8;    // 8-bit ADC Resolution

uint8_t dutyCycle = 255; // 8-bit Duty Cycle

bool setupLED()
{
    ledcSetup(LED_CH, LED_FREQ, LED_RES);
    ledcAttachPin(LED_PIN, LED_CH);
    turnOnLight();
    return (true);
}

void turnOnLight()
{
    setLightIntensity(255);
}

void turnOffLight()
{
    setLightIntensity(0);
}

void setLightIntensity(uint8_t intensity)
{
    dutyCycle = intensity;
    ledcWrite(LED_CH, dutyCycle);
}