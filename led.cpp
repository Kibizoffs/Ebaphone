#include <Arduino.h>
#include "config.h"

extern SemaphoreHandle_t esp_led_status;
extern SemaphoreHandle_t rgb_led_status;

void blinkEspLed(void *pvParameters)
{
  pinMode(esp_led_pin, OUTPUT);
  Serial.println("MSG: ESP LED - OK");
  xSemaphoreGive(esp_led_status);
  digitalWrite(esp_led_pin, HIGH);
  vTaskDelay(1000);
  digitalWrite(esp_led_pin, LOW);
  vTaskDelay(1000);
  vTaskDelete(NULL);
}

void setRgb(int r, int g, int b)
{
  analogWrite(led_red_pin, r);
  analogWrite(led_green_pin, g);
  analogWrite(led_blue_pin, b);
}

void setupRgbLed(void *pvParameters)
{
  pinMode(led_red_pin, OUTPUT);
  pinMode(led_green_pin, OUTPUT);
  pinMode(led_blue_pin, OUTPUT);
  Serial.println("MSG: RGB LED - OK");
  setRgb(200, 0, 0);
  vTaskDelay(500);
  setRgb(0, 200, 0);
  vTaskDelay(500);
  setRgb(0, 0, 200);
  vTaskDelay(500);
  setRgb(0, 0, 0);
  xSemaphoreGive(rgb_led_status);
  vTaskDelete(NULL);
}
