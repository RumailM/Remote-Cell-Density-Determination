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

#ifndef _SMARTCLAMP_AS7341_H
#define _SMARTCLAMP_AS7341_H

#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <FS.h>
#include <Adafruit_AS7341.h>

const float possibleGains[11] = {0.5, 1, 2, 4, 8, 16, 32, 64, 128, 256, 512};
const as7341_gain_t AS7341_gain[11] = {AS7341_GAIN_0_5X, AS7341_GAIN_1X, AS7341_GAIN_2X,
                                       AS7341_GAIN_4X, AS7341_GAIN_8X, AS7341_GAIN_16X,
                                       AS7341_GAIN_32X, AS7341_GAIN_64X, AS7341_GAIN_128X,
                                       AS7341_GAIN_256X, AS7341_GAIN_512X};

typedef enum
{
    AS7341_AGC_LOW_12_5,
    AS7341_AGC_LOW_25,
    AS7341_AGC_LOW_37_5,
    AS7341_AGC_LOW_50,
} as7341_agc_low_t;

typedef enum
{
    AS7341_AGC_HIGH_50,
    AS7341_AGC_HIGH_62_5,
    AS7341_AGC_HIGH_75,
    AS7341_AGC_HIGH_87_5,
} as7341_agc_high_t;

typedef enum
{
    AS7341_READ_ALL_CHANNELS,
    AS7341_READ_LOW_CHANNELS,
    AS7341_READ_HIGH_CHANNELS,
} as7341_read_band_mode;

struct as7431_info_t
{
    bool sp_meas_en;
    as7341_gain_t gain;
    uint8_t atime;
    uint16_t astep;
    unsigned int agc_freq;
    uint32_t intTime;
    bool sp_int_en;
    bool sp_agc_en;
    as7341_agc_high_t agc_high_th;
    as7341_agc_low_t agc_low_th;
    as7341_read_band_mode read_band_mode;
};

const uint8_t DEFAULT_ATIME = 59;
const uint16_t DEFAULT_ASTEP = 599;
const unsigned int DEFAULT_AGC_FREQUENCY = 50;
const as7341_gain_t DEFAULT_GAIN = AS7341_GAIN_16X;
const as7341_agc_low_t DEFAULT_SP_AGC_LOW = AS7341_AGC_LOW_25;
const as7341_agc_high_t DEFAULT_SP_AGC_HIGH = AS7341_AGC_HIGH_75;
const as7341_read_band_mode DEFAULT_READ_BAND_MODE = AS7341_READ_ALL_CHANNELS;

class Smartclamp_AS7341 : public Adafruit_AS7341
{

private:
    bool setSMUXCommand(as7341_smux_cmd_t command);
    void setSMUXLowChannels(bool f1_f4);
    bool enableSMUX(void);

public:
    as7431_info_t as7341Info;

    Smartclamp_AS7341();
    ~Smartclamp_AS7341();

    bool initializeSensor();
    void updateSensorInfo();
    bool readLowChannels(uint16_t *readings_buffer);
    bool readHighChannels(uint16_t *readings_buffer);
    bool printParameters(Stream &Serial);
    bool enableSpAutoGainCtrl(bool enable_sp_agc);
    bool getSpAutoGainCtrl();
    bool enableSaturationInterrupt(bool enable_asien);
    bool getSaturationInterrupt();
    bool automaticGainContol();
    bool setAgcFrequency(unsigned int agc_freq_value);
    unsigned int getAgcFrequency();
    bool setLowAgcThreshold(as7341_agc_low_t low_threshold);
    as7341_agc_low_t getLowAgcThreshold();
    bool setHighAgcThreshold(as7341_agc_high_t high_threshold);
    as7341_agc_high_t getHighAgcThreshold();
    bool setReadBandMode(as7341_read_band_mode read_mode);
    as7341_read_band_mode getReadBandMode();
};

#endif
