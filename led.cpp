#include <Arduino.h>

#include "Config.h"

void setupLed()
{
  pinMode(led_red_pin, OUTPUT);
  pinMode(led_green_pin, OUTPUT);
  pinMode(led_blue_pin, OUTPUT);
  Serial.println("MSG: led - OK\n");
};

void setRGB(int r, int g, int b)
{
  analogWrite(led_red_pin, r);
  analogWrite(led_green_pin, g);
  analogWrite(led_blue_pin, b);
};
