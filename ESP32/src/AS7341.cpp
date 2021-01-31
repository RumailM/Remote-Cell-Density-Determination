#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <FS.h>
#include <Adafruit_AS7341.h>

#include <AS7341.h>


/*
typedef enum {
  AS7341_GAIN_0_5X,
  AS7341_GAIN_1X,
  AS7341_GAIN_2X,
  AS7341_GAIN_4X,
  AS7341_GAIN_8X,
  AS7341_GAIN_16X,
  AS7341_GAIN_32X,
  AS7341_GAIN_64X,
  AS7341_GAIN_128X,
  AS7341_GAIN_256X,
  AS7341_GAIN_512X,
} as7341_gain_t;
*/

float possibleGains[10] = {1, 2, 4, 8, 16, 32, 64, 128, 256, 512};
as7341_gain_t AS7341_gain[10] = {AS7341_GAIN_1X, AS7341_GAIN_2X, 
                            AS7341_GAIN_4X, AS7341_GAIN_8X, AS7341_GAIN_16X,
                            AS7341_GAIN_32X, AS7341_GAIN_64X, AS7341_GAIN_128X,
                            AS7341_GAIN_256X, AS7341_GAIN_512X};

bool initializeSensor(Adafruit_AS7341 &as7341){
    if (!as7341.begin()){
        Serial.println("Could not find AS7341");
        return false;
    }
        // Set up the integration time step count
        //  Total integration time will be `(ATIME + 1) * (ASTEP + 1) * 2.78µS`
        as7341.setATIME(100);
        as7341.setASTEP(999);

        // Set up the ADC gain multiplier
        as7341.setGain(AS7341_GAIN_0_5X);
    return true;
}

int getGain(Adafruit_AS7341 &as7341){
    int gain_val = 0;
    as7341_gain_t gain = as7341.getGain();
    switch (gain) {
    case AS7341_GAIN_0_5X:
        gain_val = 0.5;
        break;
    case AS7341_GAIN_1X:
        gain_val = 1;
        break;
    case AS7341_GAIN_2X:
        gain_val = 2;
        break;
    case AS7341_GAIN_4X:
        gain_val = 4;
        break;
    case AS7341_GAIN_8X:
        gain_val = 8;
        break;
    case AS7341_GAIN_16X:
        gain_val = 16;
        break;
    case AS7341_GAIN_32X:
        gain_val = 32;
        break;
    case AS7341_GAIN_64X:
        gain_val = 64;
        break;
    case AS7341_GAIN_128X:
        gain_val = 128;
        break;
    case AS7341_GAIN_256X:
        gain_val = 256;
        break;
    case AS7341_GAIN_512X:
        gain_val = 512;
        break;
  }
  return(gain_val);
}