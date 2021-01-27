#include <Arduino.h>
#include <stdint.h>
#include <stdio.h>

#include <Serial_Processing.h>
#include <AS7341.h>

bool serialPrintBasicCounts(Stream &serialport, float_t counts[12]){
    
    serialport.print("F1 415nm : ");
    serialport.println(counts[0]);
    serialport.print("F2 445nm : ");
    serialport.println(counts[1]);
    serialport.print("F3 480nm : ");
    serialport.println(counts[2]);
    serialport.print("F4 515nm : ");
    serialport.println(counts[3]);
    serialport.print("F5 555nm : ");
    // We skip the duplicates  
    serialport.println(counts[6]);
    serialport.print("F6 590nm : ");
    serialport.println(counts[7]);
    serialport.print("F7 630nm : ");
    serialport.println(counts[8]);
    serialport.print("F8 680nm : ");
    serialport.println(counts[9]);
    serialport.print("Clear    : ");
    serialport.println(counts[10]);
    serialport.print("NIR      : ");
    serialport.println(counts[11]);

    serialport.println();
    return true;
}

bool printParameters(Stream &Serial, Adafruit_AS7341 &as7341){
    Serial.print("Sensor Gain: ");
    Serial.println(getGain(as7341));
    Serial.print("Sensor ATime: ");
    Serial.println(as7341.getATIME());
    Serial.print("Sensor AStep: ");
    Serial.println(as7341.getASTEP());
    Serial.print("Sensor Integration Time (ms): ");
    Serial.println( (as7341.getATIME()+1) * (as7341.getASTEP()+1) * 2.78 * 0.001);
    Serial.println();
    return true;
}