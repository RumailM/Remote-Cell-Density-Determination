
///////////////////   LIBRARIES    ///////////////

#include <stdint.h>
#include <stdio.h>

#include <Led_Control.h>
#include <Serial_Processing.h>
#include <Smartclamp_AS7341.h>

///////////////////   CONSTANTS    ///////////////

const String SMARTCLAMP_VERSION = "0.15";
const unsigned long READING_PERIOD = 1000; 

///////////////////   GLOBAL VARIABLES    ///////////////

Smartclamp_AS7341 as7341;
unsigned long lastMsecs = millis();

///////////////////   SETUP    ///////////////

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    delay(1);
  }
  Serial.println("SmartClamp v0.1.0\n");
  as7341.initializeSensor();
  as7341.printParameters(Serial);

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

    // serialPrintBasicCounts(Serial, counts);
    serialPrintRaw(Serial, readings);
    as7341.printParameters(Serial);
    lastMsecs = millis();
  }else{read_SERIAL(as7341);}
}