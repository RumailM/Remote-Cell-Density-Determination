#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <FS.h>
#include <Adafruit_GFX.h>
#include <WROVER_KIT_LCD.h>
#include <Adafruit_AS7341.h>
#include <string>

WROVER_KIT_LCD tft;
Adafruit_AS7341 as7341;

#define min(X, Y) (((X) < (Y)) ? (X) : (Y))

void displayValues(float counts[]) {
  tft.fillScreen(WROVER_BLACK);
  unsigned long start = micros();
  tft.setCursor(0, 0);
  tft.setTextColor(WROVER_WHITE);  tft.setTextSize(1);
  tft.println("Hello World!");

  tft.print("F1 415nm : ");
  tft.println(counts[0]);
  tft.print("F2 445nm : ");
  tft.println(counts[1]);
  tft.print("F3 480nm : ");
  tft.println(counts[2]);
  tft.print("F4 515nm : ");
  tft.println(counts[3]);
  tft.print("F5 555nm : ");
  // again, we skip the duplicates  
  tft.println(counts[6]);
  tft.print("F6 590nm : ");
  tft.println(counts[7]);
  tft.print("F7 630nm : ");
  tft.println(counts[8]);
  tft.print("F8 680nm : ");
  tft.println(counts[9]);
  tft.print("Clear    : ");
  tft.println(counts[10]);
  tft.print("NIR      : ");
  tft.println(counts[11]);
}

///////////////////   SETUP AND LOOP ///////////////


void setup() {
  Serial.begin(115200);

  tft.begin();
  tft.setRotation(1);

  tft.fillScreen(WROVER_BLACK);
  unsigned long start = micros();
  tft.setCursor(0, 0);
  tft.setTextColor(WROVER_WHITE);  tft.setTextSize(1);
  tft.println("Waiting for connection!");

  while (!Serial) {
    delay(1);
  }

  tft.println("Serial is Connected");
  
  if (!as7341.begin()){
    Serial.println("Could not find AS7341");
    while (1) { delay(10); }
  }
  
  as7341.setATIME(100);
  as7341.setASTEP(999);
  as7341.setGain(AS7341_GAIN_256X);

  Serial.println("Done with setup");
  tft.println("Done with setup");
}


void loop(void) {

  uint16_t readings[12];
  float counts[12];

  tft.fillScreen(WROVER_BLACK);
  unsigned long start = micros();
  tft.setCursor(0, 0);
  tft.setTextColor(WROVER_WHITE);  tft.setTextSize(1);
  tft.println("Hello Death!");

  if (!as7341.readAllChannels(readings)){
    Serial.println("Error reading all channels!");
    return;
  }

  for(uint8_t i = 0; i < 12; i++) {
    if(i == 4 || i == 5) continue;
    // we skip the first set of duplicate clear/NIR readings
    // (indices 4 and 5)
    counts[i] = as7341.toBasicCounts(readings[i]);
  }

  Serial.print("F1 415nm : ");
  Serial.println(counts[0]);
  Serial.print("F2 445nm : ");
  Serial.println(counts[1]);
  Serial.print("F3 480nm : ");
  Serial.println(counts[2]);
  Serial.print("F4 515nm : ");
  Serial.println(counts[3]);
  Serial.print("F5 555nm : ");
  // again, we skip the duplicates  
  Serial.println(counts[6]);
  Serial.print("F6 590nm : ");
  Serial.println(counts[7]);
  Serial.print("F7 630nm : ");
  Serial.println(counts[8]);
  Serial.print("F8 680nm : ");
  Serial.println(counts[9]);
  Serial.print("Clear    : ");
  Serial.println(counts[10]);
  Serial.print("NIR      : ");
  Serial.println(counts[11]);

  Serial.println();
  displayValues(counts);
  
  delay(500);
}