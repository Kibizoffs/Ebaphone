#include "Config.h"
#include "Led.h"
#include "Display.h"
#include "eSD.h"
#include "Player.h"
#include "Micro.h"
#include "SIM.h"

void setup() {
  Serial.begin(baud_0);
  Serial.println("MSG: Serial - OK\n");

  setupLed();
  setupDisplay();
  setupSD();
  setupPlayer();
  setupMicro();
  setupSIM();
}

void loop() {
  
}
