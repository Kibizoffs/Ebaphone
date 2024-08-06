#include <Arduino.h>
#include <FS.h>
#include <SD.h>
#include <SPI.h>
#include "Config.h"
#include "Utils.h"

extern SemaphoreHandle_t display_status;
extern SemaphoreHandle_t sd_status;

File root;

void setupSD(void *pvParametrs) {
  xSemaphoreTake(display_status, portMAX_DELAY);
  clear();

  SD.begin(sd_cs_pin);
  File root = SD.open("/");

  uint8_t cardType = SD.cardType();
  if (cardType == CARD_NONE) {
    print("ERR: No SD card attached\\r");
    vTaskDelete(NULL);
  }

  print("MSG: SD card type - ");
  if (cardType == CARD_MMC) print("MMC\\r");
  else if (cardType == CARD_SD) print("SDSC\\r");
  else if (cardType == CARD_SDHC) print("SDHC\\r");
  else print("Unknown\\r");

  uint32_t cardSize32 = SD.cardSize() / (1024 * 1024);
  char cardSizeStr[64];
  sprintf(cardSizeStr, "MSG: SD Card Size - %luMB\\r", cardSize32);
  print(cardSizeStr);

  vTaskDelay(2000);
  xSemaphoreGive(sd_status);
  vTaskDelete(NULL);
}
