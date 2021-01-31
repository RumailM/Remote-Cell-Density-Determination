/*!
 *  @file Smartclamp_AS7341.h

 *  @mainpage Smartclamp AS7341 11-Channel Spectral Sensor
 *
 *  @section intro_sec Introduction
 *
 * 	This is an extension of Adafruit's I2C Driver for the Library 
 *  for the AS7341 11-Channel Spectral Sensor.
 *
 * 	This is a library for the Adafruit AS7341 breakout:
 * 	https://www.adafruit.com/product/4698
 *
 *  @section dependencies Dependencies
 *  This library depends on:
 *      - Arduino library
 *      - Wire library
 *      - SPI library
 *      - FS library
 *      - Adafruit_AS7341 library
 *      - Adafruit BusIO library
 *
 *  @section author Author
 *
 *  Cristian Garcia (cgz215@nyu.edu)
 *
 * 	@section  HISTORY
 *
 *     v1.0 - First release
 */

#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <FS.h>
#include <Adafruit_AS7341.h>

const uint8_t DEFAULT_ATIME = 29;
const uint16_t DEFAULT_ASTEP = 599;
const as7341_gain_t DEFAULT_GAIN = AS7341_GAIN_16X;

const float possibleGains[11] = {0.5, 1, 2, 4, 8, 16, 32, 64, 128, 256, 512};
const as7341_gain_t AS7341_gain[11] = {AS7341_GAIN_0_5X, AS7341_GAIN_1X, AS7341_GAIN_2X, 
                            AS7341_GAIN_4X, AS7341_GAIN_8X, AS7341_GAIN_16X,
                            AS7341_GAIN_32X, AS7341_GAIN_64X, AS7341_GAIN_128X,
                            AS7341_GAIN_256X, AS7341_GAIN_512X};

class Smartclamp_AS7341 : public Adafruit_AS7341 {
    public:
    Smartclamp_AS7341();
    ~Smartclamp_AS7341();

    bool setAgcThreshold(uint8_t low_threshold, uint8_t high_threshold);

    bool initializeSensor();

    bool printParameters(Stream &Serial);

};