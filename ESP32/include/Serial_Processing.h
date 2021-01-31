#ifndef _SERIAL_PROCESSING_H
#define _SERIAL_PROCESSING_H

#include <Smartclamp_AS7341.h>
#include <Led_Control.h>

unsigned int getSerialIntArgument();

unsigned long getSerialLongArgument();

float getSerialFloatArgument();

void processSerialBuffer(Smartclamp_AS7341 &as7341);

void read_SERIAL(Smartclamp_AS7341 &as7341);

bool serialPrintBasicCounts(Stream &serialport, float_t counts[12]);

#endif