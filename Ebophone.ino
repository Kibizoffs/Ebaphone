#include <Arduino.h>
#include <SoftwareSerial.h>
#include "config.h"
#include "led.h"
//#include "display.h"
//#include "sd_card.h"
//#include "player.h"
//#include "micro.h"
#include "sim_card.h"

void setup() {
  Serial.begin(115600);
  Serial.println("MSG: Serial - OK");
  xTaskCreate(blinkEspLed, "blinkEspLed", 1024, NULL, 0, NULL);
  //xTaskCreate(setupRgbLed, "setupRgbLed", 1024, NULL, 1, NULL);
  Serial.println("MSG: You have 8 seconds...");
  vTaskDelay(8000);
  //xTaskCreate(setupDisplay, "setupDisplay", 1024, NULL, 1, NULL);
  //xTaskCreate(terminalDisplay, "terminalDisplay", 1024, NULL, 1, NULL);
  //xTaskCreate(setupSD, "setupSD", 1024, NULL, 1, NULL);
  xTaskCreate(setupSIM, "setupSIM", 1024, NULL, 1, NULL);
}

void loop() {
}
