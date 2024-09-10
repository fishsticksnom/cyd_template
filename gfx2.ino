#include <SPI.h>
#include <SD.h>
#include <FS.h>

#include "ESP32_SPI_9341.h"

#define SD_SCK 18
#define SD_MISO 19
#define SD_MOSI 23
#define SD_CS 5
#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 320

SPIClass SD_SPI;

LGFX lcd;

int xTouch, yTouch;

void checkTouch(){
  int pos[2] = {0, 0};
  while (1){
    if (lcd.getTouch(&pos[0], &pos[1])){
      xTouch = map(pos[0], 200, 3700, 1, SCREEN_WIDTH);
      yTouch = map(pos[1], 240, 3800, SCREEN_HEIGHT, 1);
      delay(100);
      Serial.printf("X: %d \n", xTouch);
      Serial.printf("Y: %d \n", yTouch);
   }
  }
}

void setup(){
    Serial.begin(115200);
    lcd.init();
    lcd.setBrightness(100);
    lcd.setTextSize(2);
    lcd.setRotation(0);
    lcd.fillScreen(TFT_BLACK);
    lcd.print("Working");
}

void loop(){
  checkTouch();
}
