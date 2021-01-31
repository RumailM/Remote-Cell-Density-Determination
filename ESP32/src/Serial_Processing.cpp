#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_AS7341.h>

#include <Led_Control.h>
#include <Serial_Processing.h>
#include <AS7341.h>
#include <Serial_AS7341.h>

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
    if( toupper(serialBuffer[bufferPos+1]) == 'S'){
      if( toupper(serialBuffer[bufferPos+2]) == 'G'){
        // SSG - Set Sensor Gain
        int gain = getSerialIntArgument();
        if (!setGain(as7341 ,gain)){
          Serial.print("Failed when attempting to set: ");
          Serial.println(gain);
        }
        Serial.print("Sensor Gain: ");
        Serial.println(getGain(as7341));
      }
    }
    if( toupper(serialBuffer[bufferPos+1]) == 'L'){
      if( toupper(serialBuffer[bufferPos+2]) == 'I'){
        // SLI - Set Light Intensity
        bufferPos += 3;
        setLightIntensity(getSerialIntArgument());
      }
    }
    if( toupper(serialBuffer[bufferPos+1]) == 'A'){
      if( toupper(serialBuffer[bufferPos+2]) == 'S'){
        // SAS - Set AStep
        bufferPos += 3;
        as7341.setASTEP(getSerialIntArgument());
        Serial.print("Sensor AStep: ");
        Serial.println(as7341.getASTEP());
      }
      if( toupper(serialBuffer[bufferPos+2]) == 'T'){
        // SAT - Set ATime
        bufferPos += 3;
        as7341.setATIME(getSerialIntArgument());
        Serial.print("Sensor ATime: ");
        Serial.println(as7341.getATIME());
      }
    }
  }

  if(toupper(serialBuffer[bufferPos]) == 'G'){
    if( toupper(serialBuffer[bufferPos+1]) == 'S'){
      if( toupper(serialBuffer[bufferPos+2]) == 'P'){
        // GSP - Get Sensor Parameters
        Serial.print("Sensor Gain: ");
        Serial.println(getGain(as7341));
        Serial.print("Sensor ATime: ");
        Serial.println(as7341.getATIME());
        Serial.print("Sensor AStep: ");
        Serial.println(as7341.getASTEP());
        Serial.print("Sensor Integration Time (ms): ");
        Serial.println( (as7341.getATIME()+1) * (as7341.getASTEP()+1) * 2.78 * 0.001);
        Serial.println();
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