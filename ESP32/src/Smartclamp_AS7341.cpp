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
    enableSpectralMeasurement(false);
    setATIME(DEFAULT_ATIME);
    setASTEP(DEFAULT_ASTEP);
    setGain(DEFAULT_GAIN);
    enableSpAutoGainCtrl(true);        // true after debug
    enableSaturationInterrupt(false);
    setLowAgcThreshold(DEFAULT_SP_AGS_LOW);
    setHighAgcThreshold(DEFAULT_SP_AGS_HIGH);
    enableSpectralMeasurement(true);

    return true;
}

/**
 * @brief Performs a cycle of automatic gain control
 * 
 * @return true: gain was changes false: gain wasnt changed
 */
bool Smartclamp_AS7341::automaticGainContol(){
    enableSpAutoGainCtrl(true);
    enableSpectralMeasurement(true); // Start integration
    delayForData(0); 
    enableSpAutoGainCtrl(false);
    return true;
}

void Smartclamp_AS7341::updateSensorInfo(){
    as7341Info.gain = getGain();
    as7341Info.atime = getATIME();
    as7341Info.astep = getASTEP();
    as7341Info.sp_agc_en = getSpAutoGainCtrl();
    as7341Info.agc_low_th = getLowAgcThreshold();
    as7341Info.agc_high_th = getHighAgcThreshold();
    as7341Info.sp_int_en = getSaturationInterrupt();
}

/**
 * @brief Prints out sensor Gain, ATime, AStep and Integration time to stream
 *
 * @param stream Pointer to stream object (ie. Serial object)
 * 
 * @return true: success false: failure
 */
bool Smartclamp_AS7341::printParameters(Stream &stream){
    updateSensorInfo();
    stream.print("Sensor Gain: ");
    stream.println(as7341Info.gain);
    stream.print("Sensor ATime: ");
    stream.println(as7341Info.atime);
    stream.print("Sensor AStep: ");
    stream.println(as7341Info.astep);
    stream.print("Sensor Sp Int Enable: ");
    stream.println(as7341Info.sp_int_en);
    stream.print("Sensor Sp AGC Enable: ");
    stream.println(as7341Info.sp_agc_en);
    stream.print("Sensor Sp AGC Low: ");
    stream.println(as7341Info.agc_low_th);
    stream.print("Sensor Sp AGC High: ");
    stream.println(as7341Info.agc_high_th);
    stream.print("Sensor Integration Time (ms): ");
    stream.println( (as7341Info.atime+1) * (as7341Info.astep+1) * 2.78 * 0.001);
    stream.println();
    return true;
}

/**
 * @brief Sets the spectral automatic gain control state of the sensor
 *
 * @param enable_sp_agc true: on false: off
 */
bool Smartclamp_AS7341::enableSpAutoGainCtrl(bool enable_sp_agc){
  Adafruit_BusIO_Register enable_sp_agc_reg =
      Adafruit_BusIO_Register(i2c_dev, AS7341_CFG8);
  Adafruit_BusIO_RegisterBits sp_agc_en =
      Adafruit_BusIO_RegisterBits(&enable_sp_agc_reg, 1, 2);
    sp_agc_en.write(enable_sp_agc);
    as7341Info.sp_agc_en = getSpAutoGainCtrl();
  return as7341Info.sp_agc_en == enable_sp_agc;
}

/**
 * @brief get the spectral automatic gain control state of the sensor
 *
 * @return true: on false: off
 */
bool Smartclamp_AS7341::getSpAutoGainCtrl(){
  Adafruit_BusIO_Register enable_sp_agc_reg =
      Adafruit_BusIO_Register(i2c_dev, AS7341_CFG8);
  Adafruit_BusIO_RegisterBits sp_agc_en =
      Adafruit_BusIO_RegisterBits(&enable_sp_agc_reg, 1, 2);
  return sp_agc_en.read();
}

/**
 * @brief Sets the Spectral and Flicker Detect Saturation Interrupt Enable register.
 *
 * @param enable_asien true: on false: off
 */
bool Smartclamp_AS7341::enableSaturationInterrupt(bool enable_asien){
  Adafruit_BusIO_Register enable_sp_agc_reg =
      Adafruit_BusIO_Register(i2c_dev, AS7341_INTENAB);
  Adafruit_BusIO_RegisterBits sp_agc_en =
      Adafruit_BusIO_RegisterBits(&enable_sp_agc_reg, 1, 7);
    sp_agc_en.write(enable_asien);
    as7341Info.sp_int_en = getSpAutoGainCtrl();
  return as7341Info.sp_int_en == enable_asien;
}

/**
 * @brief get the Spectral and Flicker Detect Saturation Interrupt Enable register.
 *
 * @return true: on false: off
 */
bool Smartclamp_AS7341::getSaturationInterrupt(){
  Adafruit_BusIO_Register enable_sp_agc_reg =
      Adafruit_BusIO_Register(i2c_dev, AS7341_INTENAB);
  Adafruit_BusIO_RegisterBits sp_agc_en =
      Adafruit_BusIO_RegisterBits(&enable_sp_agc_reg, 1, 7);
  return sp_agc_en.read();
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
      agc_low_threshold.write(low_threshold);
    as7341Info.agc_low_th = getLowAgcThreshold();
    return as7341Info.agc_low_th == low_threshold;
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
    agc_high_threshold.write(high_threshold);
    as7341Info.agc_high_th = getHighAgcThreshold();
    return as7341Info.agc_high_th == high_threshold;
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