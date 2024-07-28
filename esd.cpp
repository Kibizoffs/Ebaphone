#include <Arduino.h>
#include <SD.h> // модуль SD-карты
#include <SPI.h> // периферийные устройства

#include "Config.h"
#include "eSD.h"

void setupSD(){
  SD.begin(sd_cs_pin);
  Serial.println("MSG: SD card - OK\n");
  root = SD.open("/");
}

void printDirectory(File dir, int numTabs) {
  while (true) {
    File entry =  dir.openNextFile();
    if (!entry) {
      break;
    }
    for (uint8_t i = 0; i < numTabs; i++) {
      Serial.print('\t');
    }
    Serial.print(entry.name());
    if (entry.isDirectory()) {
      Serial.println("/");
      printDirectory(entry, numTabs + 1);
    }
    entry.close();
  }
}
