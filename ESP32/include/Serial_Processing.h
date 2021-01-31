#include <Adafruit_AS7341.h>

unsigned int getSerialIntArgument();

unsigned long getSerialLongArgument();

float getSerialFloatArgument();

void processSerialBuffer(Adafruit_AS7341 &as7341);

void read_SERIAL(Adafruit_AS7341 &as7341);

bool serialPrintBasicCounts(Stream &serialport, float_t counts[12]);