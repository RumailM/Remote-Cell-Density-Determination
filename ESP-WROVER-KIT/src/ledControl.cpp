#include <Arduino.h>

const uint8_t LED_PIN = 4;                          // PWM Pin
const double LED_FREQ = 5000;                       // PWM Frequency
const uint8_t LED_CH = 0;                           // PWM Channel
const uint8_t LED_RES = 8;                          // 8-bit ADC Resolution

uint8_t dutyCycle = 255;                       // 8-bit Duty Cycle
bool light = true;

bool setupLED(){
    ledcSetup(LED_CH, LED_FREQ, LED_RES);
    ledcAttachPin(LED_PIN, LED_CH);
    ledcWrite(LED_CH, dutyCycle);
}

bool turnOnLight(){
    ledcWrite(LED_CH, dutyCycle);
    light = true;
    Serial.print("Light On: ");
    Serial.print(light);
    Serial.println("$");
    return light;
}

bool turnOffLight(){
    ledcWrite(LED_CH, 0);
    Serial.print("Light On: ");
    Serial.print(light);
    Serial.println("$");
    return !light;
}

void setLightIntensity(uint8_t intensity){
    dutyCycle = intensity;
}

bool lightIsOn(){
    return light;
}