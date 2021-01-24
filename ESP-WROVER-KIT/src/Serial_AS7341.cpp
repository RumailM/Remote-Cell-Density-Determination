#include <Arduino.h>
#include <stdint.h>
#include <stdio.h>

#include <Serial_AS7341.h>

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