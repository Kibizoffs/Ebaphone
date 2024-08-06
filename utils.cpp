#include <string>
#include "Display.h"
extern Display;

void replaceAll(String& str, const String& from, const String& to) {
  int pos = 0;
  while ((pos = str.indexOf(from, pos)) != -1) {
    str = str.substring(0, pos) + to + str.substring(pos + from.length());
    pos += to.length();
  }
}

void print(String msg){
  Display.writeStr("t0.txt+", msg);
  replaceAll(msg, "\\r", "\n");
  Serial.print(msg);
};
void print(char* chars){
  String msg(chars);
  print(msg);
};

void clear(){
  Display.writeStr("t0.txt", "");
}
