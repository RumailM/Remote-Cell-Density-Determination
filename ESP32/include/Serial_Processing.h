#ifndef _SERIAL_PROCESSING_H
#define _SERIAL_PROCESSING_H

#include <Smartclamp_AS7341.h>
#include <Smartclamp_LED.h>

unsigned int getSerialIntArgument();

unsigned long getSerialLongArgument();

float getSerialFloatArgument();

void processSerialBuffer(Smartclamp_AS7341 &as7341);

void read_SERIAL(Smartclamp_AS7341 &as7341);

bool serialPrintBasicCounts(Stream &serialport, float_t counts[12]);

bool serialPrintRaw(Stream &serialport, uint16_t rawCounts[12]);

#endif