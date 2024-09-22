#ifndef Display_h
  #define Display_h

  #include <Arduino.h>

  void setPage(String page);
  void writeStr(String obj, String txt);
  void execDisplay(String command);
  void setupDisplay(void *pvParametrs);
  void setBoot(void *pvParametrs);

#endif