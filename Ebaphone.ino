#include <Arduino.h>
#include <SoftwareSerial.h>
#include "config.h"
#include "led.h"
#include "display.h"
//#include "sd_card.h"
#include "sim_card.h"
#include "ui.h"
#include "utils.h"

SemaphoreHandle_t esp_led_status;
SemaphoreHandle_t rgb_led_status;
SemaphoreHandle_t display_status;
//SemaphoreHandle_t sd_status;
SemaphoreHandle_t sim_status;
SemaphoreHandle_t boot_status;

void setup() {
  Serial.begin(9600);
  Serial.println("MSG: Serial - OK");

  esp_led_status = xSemaphoreCreateBinary();
  xTaskCreate(blinkEspLed, "blinkEspLed", 1024, NULL, 1, NULL);
  rgb_led_status = xSemaphoreCreateBinary();
  xTaskCreate(setupRgbLed, "setupRgbLed", 1024, NULL, 1, NULL);
  xSemaphoreTake(esp_led_status, portMAX_DELAY);
  xSemaphoreTake(rgb_led_status, portMAX_DELAY);

  waitForInput();

  display_status = xSemaphoreCreateBinary();
  xTaskCreate(setupDisplay, "setupDisplay", 2048, NULL, 1, NULL);
  xSemaphoreTake(display_status, portMAX_DELAY);

  /*
  sd_status = xSemaphoreCreateBinary();
  xTaskCreate(setupSD, "setupSD", 2048, NULL, 1, NULL);
  xSemaphoreTake(sd_status, portMAX_DELAY);
  */

  sim_status = xSemaphoreCreateBinary();
  xTaskCreate(setupSIM, "setupSIM", 2048, NULL, 1, NULL);
  xSemaphoreTake(sim_status, portMAX_DELAY);

  boot_status = xSemaphoreCreateBinary();
  xTaskCreate(setBoot, "setBoot", 2048, NULL, 1, NULL);
  xSemaphoreTake(boot_status, portMAX_DELAY);

  xTaskCreate(displayInteraction, "displayInteraction", 2048, NULL, 1, NULL);
  xTaskCreate(checkIncomingCall, "checkIncomingCall", 2048, NULL, 1, NULL);
}

void loop() {
}
