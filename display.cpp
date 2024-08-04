#include <Arduino.h>
#include <EasyNextionLibrary.h> // дисплей
#include "config.h"

HardwareSerial mySerial(2);
EasyNex Display(mySerial);

void setupDisplay(void *pvParametrs){
  mySerial.begin(9600, SERIAL_8N1, display_rx_pin, display_tx_pin);
  Display.begin(9600);
  Serial.println("MSG: Display - OK");
  Display.writeStr("page Terminal_h");
  Display.writeStr("t0.txt", "MSG: Serial - OK\\r");
  vTaskDelay(200);
  Display.writeStr("t0.txt+", "MSG: Display - OK\\r");
  vTaskDelay(200);
  vTaskDelete(NULL);
};

void terminalDisplay(void *pvParametrs){
  vTaskDelete(NULL);
};