#include <Arduino.h>
#include <SoftwareSerial.h>
#include "config.h"

extern SemaphoreHandle_t display_status;

SoftwareSerial Display(display_tx_pin, display_rx_pin);

void writeNull(){
  Display.write(0xFF);
  Display.write(0xFF);
  Display.write(0xFF);
}

void setPage(String page){
  Display.print("page " + page);
  writeNull();
}

void writeStr(String obj, String txt){
  Display.print(obj + "=\"" + txt + "\"");
  writeNull();
}

String getDisplayResponse(const int delay_time){
  int start_time = millis();
  String response = "";
  while (millis() - start_time < delay_time){
    while (Display.available()){
      char c = Display.read();
      response += c;
    }
    vTaskDelay(10);
  }
  Serial.println(response);
  return response;
}

void setupRTC(){
  Display.println("rtc0=2024");
  getDisplayResponse(1000);
  Display.println("rtc1=8");
  getDisplayResponse(1000);
  Display.println("rtc2=6");
  getDisplayResponse(1000);
  Display.println("rtc3=20");
  getDisplayResponse(1000);
  Display.println("rtc4=32");
  getDisplayResponse(1000);
  Display.println("rtc5=0");
  getDisplayResponse(10000);
}

void setupDisplay(void *pvParametrs){
  Display.begin(9600, SERIAL_8N1, display_tx_pin, display_rx_pin);
  Serial.println("MSG: Display - OK");
  setPage("Terminal_h");
  vTaskDelay(100);
  writeStr("t0.txt", "MSG: Serial - OK\\r");
  vTaskDelay(100);
  writeStr("t0.txt+", "MSG: Display - OK\\r");
  setupRTC();
  vTaskDelay(2000);
  xSemaphoreGive(display_status);
  vTaskDelete(NULL);
};

void updateTime(){
  Display.println("rtc0?");
  String year = getDisplayResponse(1000);
  Display.println("rtc1?");
  String month = getDisplayResponse(1000);
  Display.println("rtc2?");
  String day = getDisplayResponse(1000);
  Display.println("rtc3?");
  String hour = getDisplayResponse(1000);
  Display.println("rtc4?");
  String minute = getDisplayResponse(1000);
  Display.println("rtc5?");
  String second = getDisplayResponse(1000);
  String timeStr = year + "-" + month + "-" + day + " " + hour + ":" + minute + ":" + second;
  writeStr("watch_time", timeStr);
}
