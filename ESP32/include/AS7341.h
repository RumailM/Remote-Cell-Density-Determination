#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <FS.h>
#include <Adafruit_AS7341.h>

bool initializeSensor(Adafruit_AS7341 &as7341);
bool setGain(Adafruit_AS7341 &as7341, float gain);
int getGain(Adafruit_AS7341 &as7341);