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

float possibleGains[11] = {0.5, 1, 2, 4, 8, 16, 32, 64, 128, 256, 512};
as7341_gain_t AS7341_gain[11] = {AS7341_GAIN_0_5X, AS7341_GAIN_1X, AS7341_GAIN_2X, 
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
        as7341.setGain(AS7341_GAIN_32X);
    return true;
}

bool setGain(Adafruit_AS7341 &as7341, float gain){
    for (int i = 0; i < 11; i++){
        if (gain == possibleGains[i]){
            as7341.setGain(AS7341_gain[i]);
            return true;
        }
    }
    return false;
}