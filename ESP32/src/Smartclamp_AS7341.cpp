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

/**
 * @brief Sets the spectral automatic gain control state of the sensor
 *
 * @param enable_sp_agc true: on false: off
 */
bool Smartclamp_AS7341::spAgcEnable(bool enable_sp_agc){
  Adafruit_BusIO_Register enable_sp_agc_reg =
      Adafruit_BusIO_Register(i2c_dev, AS7341_CFG8);
  Adafruit_BusIO_RegisterBits sp_agc_en =
      Adafruit_BusIO_RegisterBits(&enable_sp_agc_reg, 1, 2);
  return sp_agc_en.write(enable_sp_agc);
}

/**
 * @brief sets Automatic Gain Change (AGC) low thresholds
 *
 * @param low_threshold as7341_agc_low_t [0-3] to specify if gain should be increased at
 * 12.5%, 25%, 37.5%, or 50% respectively of the ADC's full-scale range.
 * 
 * @return true: success false: failure
 */
bool Smartclamp_AS7341::setLowAgcThreshold(as7341_agc_low_t low_threshold){
    Adafruit_BusIO_Register agc_threshold_reg = 
        Adafruit_BusIO_Register(i2c_dev, AS7341_CFG10, 1);
  Adafruit_BusIO_RegisterBits agc_low_threshold =
      Adafruit_BusIO_RegisterBits(&agc_threshold_reg, 2, 4);
    return agc_low_threshold.write(low_threshold);
}

/**
 * @brief gets Automatic Gain Change (AGC) low threshold
 * 
 * @return as7341_agc_low_t object
 */
as7341_agc_low_t Smartclamp_AS7341::getLowAgcThreshold(){
    Adafruit_BusIO_Register agc_threshold_reg = 
        Adafruit_BusIO_Register(i2c_dev, AS7341_CFG10, 1);
  Adafruit_BusIO_RegisterBits agc_low_threshold =
      Adafruit_BusIO_RegisterBits(&agc_threshold_reg, 2, 4);
    return (as7341_agc_low_t)agc_low_threshold.read();
}

/**
 * @brief sets Automatic Gain Change (AGC) high threshold
 * 
 * @param high_threshold as7341_agc_high_t [0-3] to specify if gain should be reduced at 
 * 50%, 62.5%, 75%, or 87.5% respectively of the ADC's full-scale range. 
 * 
 * @return true: success false: failure
 */
bool Smartclamp_AS7341::setHighAgcThreshold(as7341_agc_high_t high_threshold){
    Adafruit_BusIO_Register agc_threshold_reg = 
        Adafruit_BusIO_Register(i2c_dev, AS7341_CFG10, 1);
  Adafruit_BusIO_RegisterBits agc_high_threshold =
      Adafruit_BusIO_RegisterBits(&agc_threshold_reg, 2, 6);
    return agc_high_threshold.write(high_threshold);
}

/**
 * @brief gets Automatic Gain Change (AGC) high threshold
 * 
 * @return as7341_agc_high_t object
 */
as7341_agc_high_t Smartclamp_AS7341::getHighAgcThreshold(){
    Adafruit_BusIO_Register agc_threshold_reg = 
        Adafruit_BusIO_Register(i2c_dev, AS7341_CFG10, 1);
  Adafruit_BusIO_RegisterBits agc_high_threshold =
      Adafruit_BusIO_RegisterBits(&agc_threshold_reg, 2, 6);
    return (as7341_agc_high_t)agc_high_threshold.read();
}