
///////////////////   LIBRARIES    ///////////////

#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <FS.h>
#include <Adafruit_GFX.h>
#include <Adafruit_AS7341.h>
#include <string>

#include <Led_Control.h>
#include <Serial_Processing.h>

///////////////////   CONSTANTS    ///////////////

const String SMARTCLAMP_VERSION = "0.15";

///////////////////   GLOBAL VARIABLES    ///////////////

Adafruit_AS7341 as7341;

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

  Serial.print("F1 415nm : ");
  Serial.println(counts[0]);
  Serial.print("F2 445nm : ");
  Serial.println(counts[1]);
  Serial.print("F3 480nm : ");
  Serial.println(counts[2]);
  Serial.print("F4 515nm : ");
  Serial.println(counts[3]);
  Serial.print("F5 555nm : ");
  // again, we skip the duplicates  
  Serial.println(counts[6]);
  Serial.print("F6 590nm : ");
  Serial.println(counts[7]);
  Serial.print("F7 630nm : ");
  Serial.println(counts[8]);
  Serial.print("F8 680nm : ");
  Serial.println(counts[9]);
  Serial.print("Clear    : ");
  Serial.println(counts[10]);
  Serial.print("NIR      : ");
  Serial.println(counts[11]);

  Serial.println();
  
  delay(500);
}