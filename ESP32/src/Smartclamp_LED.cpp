#include <SMARTCLAMP_LED.h>

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