#include <Arduino.h>
#include <SoftwareSerial.h>
#include "config.h"
#include "utils.h"

extern SemaphoreHandle_t display_status;
extern SemaphoreHandle_t boot_status;

SoftwareSerial Display(display_tx_pin, display_rx_pin);

void writeNull(){
  Display.write(0xFF);
  Display.write(0xFF);
  Display.write(0xFF);
}

void setPage(String page){
  vTaskDelay(100 / portTICK_PERIOD_MS);
  Display.print("page " + page);
  writeNull();
  vTaskDelay(100 / portTICK_PERIOD_MS);
}

void writeStr(String obj, String txt){
  vTaskDelay(100 / portTICK_PERIOD_MS);
  Display.print(obj + "=\"" + txt + "\"");
  writeNull();
  vTaskDelay(100 / portTICK_PERIOD_MS);
}

void execDisplay(String command){
  vTaskDelay(100 / portTICK_PERIOD_MS);
  Display.print(command);
  writeNull();
  vTaskDelay(100 / portTICK_PERIOD_MS);
}


void setupRTC(String hour, String minute, String second, String day, String month, String year){
  Display.print("rtc0=" + year);
  writeNull();
  Display.print("rtc1=" + month);
  writeNull();
  Display.print("rtc2=" + day);
  writeNull();
  Display.print("rtc3=" + hour);
  writeNull();
  Display.print("rtc4=" + minute);
  writeNull();
  Display.print("rtc5=" + second);
  writeNull();
  print("MSG: Time set#br");
}

void setupDisplay(void *pvParametrs){
  Display.begin(9600);
  writeStr("t0.txt", "");
  setPage("Terminal_h");
  print("MSG: Display - OK#br");
  vTaskDelay(2000 / portTICK_PERIOD_MS);
  xSemaphoreGive(display_status);
  vTaskDelete(NULL);
};

void setBoot(void *pvParametrs){
  clear();
  setPage("Boot_h");
  execDisplay("slider.val=33");
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  execDisplay("slider.val=66");
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  execDisplay("slider.val=100");
  xSemaphoreGive(boot_status);
  setPage("Menu_h");
  vTaskDelete(NULL);
}