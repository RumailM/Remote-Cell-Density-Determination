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

/**
 * @brief Initializes sensor with default values
 * 
 * @return true: success false: failure
 */
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

/**
 * @brief Prints out sensor Gain, ATime, AStep and Integration time to stream
 *
 * @param stream Pointer to stream object (ie. Serial object)
 * 
 * @return true: success false: failure
 */
bool Smartclamp_AS7341::printParameters(Stream &stream){
    stream.print("Sensor Gain: ");
    stream.println(Smartclamp_AS7341::getGain());
    stream.print("Sensor ATime: ");
    stream.println(Smartclamp_AS7341::getATIME());
    stream.print("Sensor AStep: ");
    stream.println(Smartclamp_AS7341::getASTEP());
    stream.print("Sensor Integration Time (ms): ");
    stream.println( (Smartclamp_AS7341::getATIME()+1) * (Smartclamp_AS7341::getASTEP()+1) * 2.78 * 0.001);
    stream.println();
    return true;
}