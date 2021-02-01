#include <Smartclamp_Serial.h>

const uint8_t SERIAL_BUFFER_LEN = 128;
char serialBuffer[SERIAL_BUFFER_LEN];

uint8_t bufferEnd = 0;
uint8_t bufferPos = 0;

unsigned int getSerialIntArgument()
{
  return atoi(serialBuffer + (bufferPos + 1));
}

unsigned long getSerialLongArgument()
{
  return atol(serialBuffer + (bufferPos + 1));
}

float getSerialFloatArgument()
{
  return atof(serialBuffer + (bufferPos + 1));
}

void processSerialBuffer(Smartclamp_AS7341 &as7341)
{
  if (toupper(serialBuffer[bufferPos]) == 'L')
  {
    if (toupper(serialBuffer[bufferPos + 1]) == 'O')
    {
      if (toupper(serialBuffer[bufferPos + 2]) == 'N')
      {
        // LON - Laser ON
        turnOnLight();
      }
      else if (toupper(serialBuffer[bufferPos + 2]) == 'F')
      {
        // LOF - Laser OF
        turnOffLight();
      }
    }
  }

  if (toupper(serialBuffer[bufferPos]) == 'S')
  {
    if (toupper(serialBuffer[bufferPos + 1]) == 'L')
    {
      if (toupper(serialBuffer[bufferPos + 2]) == 'I')
      {
        // SLI - Set Light Intensity
        bufferPos += 3;
        setLightIntensity(getSerialIntArgument());
      }
    }
    if (toupper(serialBuffer[bufferPos + 1]) == 'A')
    {
      if (toupper(serialBuffer[bufferPos + 2]) == 'S')
      {
        // SAS - Set AStep
        bufferPos += 3;
        as7341.setASTEP(getSerialIntArgument());
        Serial.print("Sensor AStep: ");
        Serial.println(as7341.getASTEP());
      }
      if (toupper(serialBuffer[bufferPos + 2]) == 'T')
      {
        // SAT - Set ATime
        bufferPos += 3;
        as7341.setATIME(getSerialIntArgument());
        Serial.print("Sensor ATime: ");
        Serial.println(as7341.getATIME());
      }
    }
  }

  if (toupper(serialBuffer[bufferPos]) == 'G')
  {
    if (toupper(serialBuffer[bufferPos + 1]) == 'S')
    {
      if (toupper(serialBuffer[bufferPos + 2]) == 'P')
      {
        // GSP - Get Sensor Parameters
        as7341.printParameters(Serial);
      }
    }
  }
}

void read_SERIAL(Smartclamp_AS7341 &as7341)
{
  if (Serial.available() > 0)
  {
    // get incoming byte:
    serialBuffer[bufferEnd] = Serial.read();
    Serial.print(serialBuffer[bufferEnd]);

    // min message length? -> process commands
    if (serialBuffer[bufferEnd] == 10)
    {
      processSerialBuffer(as7341);

      // go to message end
      bufferPos = bufferEnd + 1;
    }

    if (bufferEnd < SERIAL_BUFFER_LEN - 1)
      bufferEnd++;
    else
      bufferEnd = 0;
  }
}

bool serialPrintBasicCounts(Stream &serialport, float_t counts[12])
{

  serialport.print("415nm: ");
  serialport.println(counts[0], SERIAL_DECIMALS);
  serialport.print("445nm: ");
  serialport.println(counts[1], SERIAL_DECIMALS);
  serialport.print("480nm: ");
  serialport.println(counts[2], SERIAL_DECIMALS);
  serialport.print("515nm: ");
  serialport.println(counts[3], SERIAL_DECIMALS);
  serialport.print("555nm: ");
  // We skip the duplicates
  serialport.println(counts[6], SERIAL_DECIMALS);
  serialport.print("590nm: ");
  serialport.println(counts[7], SERIAL_DECIMALS);
  serialport.print("630nm: ");
  serialport.println(counts[8], SERIAL_DECIMALS);
  serialport.print("680nm: ");
  serialport.println(counts[9], SERIAL_DECIMALS);
  serialport.print("Clear: ");
  serialport.println(counts[10], SERIAL_DECIMALS);
  serialport.print("NIR: ");
  serialport.println(counts[11], SERIAL_DECIMALS);
  return true;
}

bool serialPrintRaw(Stream &serialport, uint16_t rawCounts[12])
{

  serialport.print("415nm: ");
  serialport.println(rawCounts[0]);
  serialport.print("445nm: ");
  serialport.println(rawCounts[1]);
  serialport.print("480nm: ");
  serialport.println(rawCounts[2]);
  serialport.print("515nm: ");
  serialport.println(rawCounts[3]);
  serialport.print("555nm: ");
  // We skip the duplicates
  serialport.println(rawCounts[6]);
  serialport.print("590nm: ");
  serialport.println(rawCounts[7]);
  serialport.print("630nm: ");
  serialport.println(rawCounts[8]);
  serialport.print("680nm: ");
  serialport.println(rawCounts[9]);
  serialport.print("Clear: ");
  serialport.println(rawCounts[10]);
  serialport.print("NIR: ");
  serialport.println(rawCounts[11]);
  return true;
}