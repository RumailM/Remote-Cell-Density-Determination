
///////////////////   LIBRARIES    ///////////////

#include <stdint.h>
#include <stdio.h>

#include <Smartclamp_LED.h>
#include <Smartclamp_Serial.h>
#include <Smartclamp_AS7341.h>

///////////////////   CONSTANTS    ///////////////

const String SMARTCLAMP_VERSION = "0.15";
const unsigned long SERIAL_DELAY = 227;
const unsigned long TARGET_PERIOD = 500;
const unsigned long READING_PERIOD = TARGET_PERIOD - SERIAL_DELAY;

///////////////////   GLOBAL VARIABLES    ///////////////

Smartclamp_AS7341 as7341;
unsigned long lastMsecs = millis();
bool rawCountsMode = false;

///////////////////   SETUP    ///////////////

void setup()
{
  Serial.begin(115200);
  while (!Serial)
  {
    delay(1);
  }
  Serial.println("START");
  as7341.initializeSensor();

  // Setup LED PWM Signal.
  setupLED();
}

///////////////////   LOOP    ///////////////

void loop(void)
{

  uint16_t readings[12];
  float counts[12];
  if (millis() - lastMsecs > READING_PERIOD)
  {
    as7341.automaticGainContol();
    if (!as7341.readAllChannels(readings))
    {
      Serial.println("ERROR: Couldn't read all channels!");
      return;
    }

    if (!rawCountsMode){
    for (uint8_t i = 0; i < 12; i++)
    {
      if (i == 4 || i == 5)
        continue;
      // we skip the first set of duplicate clear/NIR readings
      // (indices 4 and 5)
      counts[i] = as7341.toBasicCounts(readings[i]);
    }
    }

    if (!rawCountsMode)
      serialAllReadings(Serial, counts);
    else
      serialPrintRaw(Serial, readings);

    as7341.printParameters(Serial);
    lastMsecs = millis();
  }
  else
  {
    read_SERIAL(as7341);
  }
}