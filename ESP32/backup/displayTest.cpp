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

unsigned long testText() {
  tft.fillScreen(WROVER_BLACK);
  unsigned long start = micros();
  tft.setCursor(0, 0);
  tft.setTextColor(WROVER_WHITE);  tft.setTextSize(1);
  tft.println("Hello World!");
  tft.setTextColor(WROVER_YELLOW); tft.setTextSize(2);
  tft.println(1234.56);
  tft.setTextColor(WROVER_RED);    tft.setTextSize(3);
  tft.println(0xDEADBEEF, HEX);
  tft.println();
  tft.setTextColor(WROVER_GREEN);
  tft.setTextSize(5);
  tft.println("Groop");
  tft.setTextSize(2);
  tft.println("I implore thee,");
  tft.setTextSize(1);
  tft.println("my foonting turlingdromes.");
  tft.println("And hooptiously drangle me");
  tft.println("with crinkly bindlewurdles,");
  tft.println("Or I will rend thee");
  tft.println("in the gobberwarts");
  tft.println("with my blurglecruncheon,");
  tft.println("see if I don't!");
  return micros() - start;
}

///////////////////   SETUP AND LOOP ///////////////


void setup() {
  Serial.begin(115200);
 
  tft.begin();
  tft.setRotation(1);

  uint8_t x = 0;
  uint32_t id = tft.readId();
  if(id){
      Serial.println("======= WROVER ST7789V Display Test ========");
  } else {
      Serial.println("======= WROVER ILI9341 Display Test ========");
  }
  Serial.println("============================================");
  Serial.printf("Display ID:      0x%06X\n", id);

  x = tft.readcommand8(WROVER_RDDST);
  Serial.print("Status:          0x"); Serial.println(x, HEX);
  x = tft.readcommand8(WROVER_RDDPM);
  Serial.print("Power Mode:      0x"); Serial.println(x, HEX);
  x = tft.readcommand8(WROVER_RDDMADCTL);
  Serial.print("MADCTL Mode:     0x"); Serial.println(x, HEX);
  x = tft.readcommand8(WROVER_RDDCOLMOD);
  Serial.print("Pixel Format:    0x"); Serial.println(x, HEX);
  x = tft.readcommand8(WROVER_RDDIM);
  Serial.print("Image Format:    0x"); Serial.println(x, HEX);
  x = tft.readcommand8(WROVER_RDDSDR);
  Serial.print("Self Diagnostic: 0x"); Serial.println(x, HEX);

  Serial.print(F("Text                     "));
  Serial.println(testText());
  // delay(3000);

  Serial.println(F("Done!"));

}


void loop(void) {
  // for(uint8_t rotation=0; rotation<4; rotation++) {
  //   tft.setRotation(rotation);
  //   testText();
  //   delay(1000);
  // }
}