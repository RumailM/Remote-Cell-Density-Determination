#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_AS7341.h>

#include <Led_Control.h>
#include <Serial_Processing.h>
#include <AS7341.h>

const uint8_t SERIAL_BUFFER_LEN = 128;
char serialBuffer[SERIAL_BUFFER_LEN];

uint8_t bufferEnd = 0;
uint8_t bufferPos = 0;

unsigned int getSerialIntArgument(){
  return atoi(serialBuffer+(bufferPos+1) );
}

unsigned long getSerialLongArgument(){
  return atol(serialBuffer+(bufferPos+1) );
}

float getSerialFloatArgument(){
  return atof(serialBuffer+(bufferPos+1) );
}

void processSerialBuffer(Adafruit_AS7341 &as7341){
  if( toupper(serialBuffer[bufferPos]) == 'L'){
    if( toupper(serialBuffer[bufferPos+1]) == 'O'){
      if( toupper(serialBuffer[bufferPos+2]) == 'N'){
        // LON - Laser ON
        turnOnLight();
      }
      else if( toupper(serialBuffer[bufferPos+2]) == 'F'){
        // LOF - Laser OF
        turnOffLight();
      }
    }
  }

  if( toupper(serialBuffer[bufferPos]) == 'S'){
    if( toupper(serialBuffer[bufferPos+1]) == 'L'){
      if( toupper(serialBuffer[bufferPos+2]) == 'I'){
        // SLI - Set Light Intensity
        bufferPos += 3;
        setLightIntensity(getSerialIntArgument());
      }
    }
    if( toupper(serialBuffer[bufferPos+1]) == 'S'){
      if( toupper(serialBuffer[bufferPos+2]) == 'S'){
        // SSS - Set Sensor Sensitivity
        bufferPos += 3;
        setGain(as7341 ,getSerialFloatArgument());
      }
    }
  }
}

void read_SERIAL(Adafruit_AS7341 &as7341){
  if (Serial.available() > 0) {
    // get incoming byte:
    serialBuffer[bufferEnd] = Serial.read();
    Serial.print(serialBuffer[bufferEnd]);

    // min message length? -> process commands
    if( serialBuffer[bufferEnd] == 10 ) {
      processSerialBuffer(as7341);

      // go to message end
      bufferPos = bufferEnd+1;
    }

    if( bufferEnd < SERIAL_BUFFER_LEN - 1 )
      bufferEnd++;
    else
      bufferEnd = 0;
  }
}