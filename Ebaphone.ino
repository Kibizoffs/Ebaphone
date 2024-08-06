#include <Arduino.h>
#include <SoftwareSerial.h>
#include "config.h"
#include "led.h"
#include "display.h"
#include "sd_card.h"
#include "sim_card.h"
#include "ui.h"

SemaphoreHandle_t display_status;
SemaphoreHandle_t sd_status;
SemaphoreHandle_t sim_status;

void setup() {
  Serial.begin(9600);
  Serial.println("MSG: Serial - OK");
  xTaskCreate(blinkEspLed, "blinkEspLed", 1024, NULL, 0, NULL);
  Serial.println("MSG: Press 'x' when you are ready");
  waitForInput();
  xTaskCreate(setupRgbLed, "setupRgbLed", 1024, NULL, 1, NULL);
  display_status = xSemaphoreCreateBinary();
  xTaskCreate(setupDisplay, "setupDisplay", 1024, NULL, 1, NULL);
  sd_status = xSemaphoreCreateBinary();
  xTaskCreate(setupSD, "setupSD", 2048, NULL, 1, NULL);
  sim_status = xSemaphoreCreateBinary();
  xTaskCreate(setupSIM, "setupSIM", 4096, NULL, 1, NULL);
}

void loop() {
}

void waitForInput(){
  while (true) {
    if (Serial.available()) {
      char c = Serial.read();
      if (c == 'x' || c == 'X' || c == 'х' || c == 'Х') {
        break;
      }
    }
  }
}
