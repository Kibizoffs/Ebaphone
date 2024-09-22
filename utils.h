#ifndef Utils
  #define Utils

  #include <Arduino.h>

  void waitForInput();
  String replaceAll(String str, String from, String to);
  void print(String str);
  void print(char* chars);

  void clear();

#endif