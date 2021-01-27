#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_AS7341.h>

bool serialPrintBasicCounts(Stream &serialport, float_t counts[12]);
bool printParameters(Stream &Serial, Adafruit_AS7341 &as7341);