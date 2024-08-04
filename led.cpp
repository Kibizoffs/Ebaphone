#include <Arduino.h>
#include "config.h"

void blinkEspLed(void *pvParameters)
{
  pinMode(esp_led_pin, OUTPUT);
  Serial.println("MSG: ESP LED - OK");
  while (true) {
    digitalWrite(esp_led_pin, HIGH);
    vTaskDelay(1000);
    digitalWrite(esp_led_pin, LOW);
    vTaskDelay(1000);
  }
}

void setupRgbLed(void *pvParameters)
{
  pinMode(32, OUTPUT);
  pinMode(33, OUTPUT);
  pinMode(14, OUTPUT);
  Serial.println("MSG: RGB LED - OK");
  vTaskDelete(NULL);
}

void setRgb(int r, int g, int b)
{
  analogWrite(32, r);
  analogWrite(33, g);
  analogWrite(14, b);
}
