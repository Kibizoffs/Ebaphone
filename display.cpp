#include <Arduino.h>
#include <EasyNextionLibrary.h> // дисплей

#include "Config.h"

EasyNex Display(Serial1);

void setupDisplay(){
  Display.begin(baud_0);
  Serial.println("MSG: Display - OK\n");
};