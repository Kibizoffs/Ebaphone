#include <Arduino.h>
#include <EasyNextionLibrary.h> // дисплей
#include <FS.h>
#include <SD.h> // модуль SD-карты
#include <SPI.h> // периферийные устройства
#include "Config.h"
#include "Display.h"
extern EasyNex Display;

File root;

void setupSD(void *pvParametrs){
  SD.begin(sd_cs_pin);
  File root = SD.open("/");

  uint8_t cardType = SD.cardType();
  if (cardType == CARD_NONE) {
    Serial.println("ERR: No SD card attached");
    Display.writeStr("t0.txt+", "ERR: No SD card attached");
  }

  Serial.print("MSG: SD card type - ");
  Display.writeStr("t0.txt+", "MSG: SD card type - ");
  if (cardType == CARD_MMC) {
    Serial.println("MMC");
    Display.writeStr("t0.txt+", "MMC\\r");
  }
  else if (cardType == CARD_SD) {
    Serial.println("SDSC");
    Display.writeStr("t0.txt+", "SDSC\\r");
  }
  else if (cardType == CARD_SDHC) {
    Serial.println("SDHC");
    Display.writeStr("t0.txt+", "SDHC\\r");
  }
  else {
    Serial.println("Unknown");
    Display.writeStr("t0.txt+", "Unknown\\r");
  }

  uint32_t cardSize32 = SD.cardSize() / (1024 * 1024);
  char cardSizeStr[64];
  sprintf(cardSizeStr, "%lu", cardSize32);
  Serial.printf("MSG: SD Card Size - %sMB\n", cardSizeStr);
  sprintf(cardSizeStr, "MSG: SD Card Size - %sMB\\r", cardSizeStr);
  Display.writeStr("t0.txt+", cardSizeStr);

  vTaskDelete(NULL);
}