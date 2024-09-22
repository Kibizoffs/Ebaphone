#include <string>
#include <SoftwareSerial.h>
#include "Display.h"

extern SoftwareSerial Display;

void waitForInput(){
  Serial.println("MSG: Press 'x' when you are ready");
  while (true) {
    if (Serial.available()) {
      char c = Serial.read();
      if (c == 'x' || c == 'X' || c == 'х' || c == 'Х') {
        break;
      }
    }
  }
}

String replaceAll(String str, String from, String to) {
  int pos = 0;
  while ((pos = str.indexOf(from, pos)) != -1) {
    str = str.substring(0, pos) + to + str.substring(pos + from.length());
    pos += to.length();
  }
  return str;
}

void print(String msg){
  writeStr("output.txt+", replaceAll(msg, "#br", "\\r"));
  Serial.print(replaceAll(msg, "#br", "\n"));
};
void print(char* chars){
  String msg(chars);
  print(msg);
};

void clear(){
  writeStr("output.txt", "");
}
