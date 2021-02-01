#include <SMARTCLAMP_LED.h>

const uint8_t LED_PIN = 4;                          // PWM Pin
const double LED_FREQ = 5000;                       // PWM Frequency
const uint8_t LED_CH = 0;                           // PWM Channel
const uint8_t LED_RES = 8;                          // 8-bit ADC Resolution

uint8_t dutyCycle = 163;                            // 8-bit Duty Cycle
bool light;

bool setupLED(){
    ledcSetup(LED_CH, LED_FREQ, LED_RES);
    ledcAttachPin(LED_PIN, LED_CH);
    ledcWrite(LED_CH, dutyCycle);
    turnOnLight();
    return(true);
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
    light = false;
    return !light;
}

void setLightIntensity(uint8_t intensity){
    dutyCycle = intensity;
    ledcWrite(LED_CH, dutyCycle);
    Serial.print("Light On: ");
    Serial.print(light);
    Serial.println("$");
}

bool lightIsOn(){
    ledcWrite(LED_CH, dutyCycle);
    Serial.print("Light On: ");
    Serial.print(light);
    Serial.println("$");
    return light;
}