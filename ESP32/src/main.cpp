
///////////////////   LIBRARIES    ///////////////

#include <stdint.h>
#include <stdio.h>
#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <FS.h>
#include <Adafruit_GFX.h>
#include <Adafruit_AS7341.h>

#include <Led_Control.h>
#include <Serial_Processing.h>
#include <Serial_AS7341.h>
#include <AS7341.h>

///////////////////   CONSTANTS    ///////////////

const String SMARTCLAMP_VERSION = "0.15";
const unsigned long READING_PERIOD = 1000; 

///////////////////   GLOBAL VARIABLES    ///////////////

Adafruit_AS7341 as7341;
unsigned long lastMsecs = millis();

///////////////////   SETUP    ///////////////

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    delay(1);
  }
  Serial.println("SmartClamp v0.1.0\n");
  
  if (!as7341.begin()){
    Serial.println("Could not find AS7341");
    while (1) { delay(10); }
  }
  

  // Set up the integration time step count
  //  Total integration time will be `(ATIME + 1) * (ASTEP + 1) * 2.78µS`

  as7341.setATIME(100);
  as7341.setASTEP(999);

  // Set up the ADC gain multiplier
  as7341.setGain(AS7341_GAIN_256X);

  // Setup LED PWM Signal.
  setupLED();
 

  Serial.println("Done with setup");
}

///////////////////   LOOP    ///////////////


void loop(void) {

  uint16_t readings[12];
  float counts[12];
  if (millis() - lastMsecs > READING_PERIOD){
    if (!as7341.readAllChannels(readings)){
      Serial.println("Error reading all channels!");
      return;
    }

    for(uint8_t i = 0; i < 12; i++) {
      if(i == 4 || i == 5) continue;
      // we skip the first set of duplicate clear/NIR readings
      // (indices 4 and 5)
      counts[i] = as7341.toBasicCounts(readings[i]);
    }

    serialPrintBasicCounts(Serial, counts);
    lastMsecs = millis();
  }else{read_SERIAL(as7341);}
}