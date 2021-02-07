#ifndef _SERIAL_PROCESSING_H
#define _SERIAL_PROCESSING_H

#include <Smartclamp_AS7341.h>
#include <Smartclamp_LED.h>

#define SERIAL_DECIMALS 5

unsigned int getSerialIntArgument();
unsigned long getSerialLongArgument();
float getSerialFloatArgument();
void processSerialBuffer(Smartclamp_AS7341 &as7341);
void read_SERIAL(Smartclamp_AS7341 &as7341);
bool serialAllCounts(Stream &serialport, float_t counts[12]);
bool serialAllRaw(Stream &serialport, uint16_t rawCounts[12]);
bool serialLowReadings(Stream &serialport, float_t counts[12]);
bool serialHighReadings(Stream &serialport, float_t counts[12]);
bool serialHighRaw(Stream &serialport, uint16_t rawCounts[12]);
bool serialLowRaw(Stream &serialport, uint16_t rawCounts[12]);

#endif