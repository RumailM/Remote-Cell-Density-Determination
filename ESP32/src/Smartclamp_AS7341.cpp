/*!
 *  @file Smartclamp_Smartclamp_AS7341::cpp
 *
 * 	This is an extension of Adafruit's I2C Driver for the Library 
 *  for the AS7341 11-Channel Spectral Sensor.
 *
 * 	This is a library for the Adafruit AS7341 breakout:
 * 	https://www.adafruit.com/product/4698
 *
 *  Developed by Cristian Garcia Zamora (cgz215@nyu.edu) 
 * 
 *  Original library Copyright 2020 to Bryan Siepert for Adafruit Industries
 *
 */


#include <Smartclamp_AS7341.h>


/**
 * @brief Construct a new Smartclamp_AS7341::Smartclamp_AS7341 object
 *
 */
Smartclamp_AS7341::Smartclamp_AS7341(void) {}

/**
 * @brief Destroy the Smartclamp_AS7341::Smartclamp_AS7341 object
 *
 */
Smartclamp_AS7341::~Smartclamp_AS7341(void) {}

bool Smartclamp_AS7341::initializeSensor(){
    if (!Smartclamp_AS7341::begin()){
        Serial.println("Could not find AS7341");
        return false;
    }
    // Set up the integration time step count
    //  Total integration time will be `(ATIME + 1) * (ASTEP + 1) * 2.78µS`
    Smartclamp_AS7341::enableSpectralMeasurement(false);
    Smartclamp_AS7341::setATIME(DEFAULT_ATIME);
    Smartclamp_AS7341::setASTEP(DEFAULT_ASTEP);
    Smartclamp_AS7341::setGain(DEFAULT_GAIN);
    Smartclamp_AS7341::enableSpectralMeasurement(true);

    return true;
}

bool Smartclamp_AS7341::printParameters(Stream &Serial){
    Serial.print("Sensor Gain: ");
    Serial.println(Smartclamp_AS7341::getGain());
    Serial.print("Sensor ATime: ");
    Serial.println(Smartclamp_AS7341::getATIME());
    Serial.print("Sensor AStep: ");
    Serial.println(Smartclamp_AS7341::getASTEP());
    Serial.print("Sensor Integration Time (ms): ");
    Serial.println( (Smartclamp_AS7341::getATIME()+1) * (Smartclamp_AS7341::getASTEP()+1) * 2.78 * 0.001);
    Serial.println();
    return true;
}