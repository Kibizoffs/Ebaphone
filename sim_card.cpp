#include <Arduino.h>
#include <SoftwareSerial.h>
#include "Config.h"
#include "Utils.h"

SoftwareSerial SIM800(9, 10);

void setupSIM(void *pvParametrs){
  SIM800.begin(9600);
  SIM800.println("AT");
  SIM800.println("AT+CSQ");
  SIM800.println("AT+CCID");
  SIM800.println("AT+CREG?");
  Serial.println("YOO");
  terminalPrint("MSG: SMS card - ON");

  vTaskDelete(NULL);
}