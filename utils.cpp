#include <EasyNextionLibrary.h> // дисплей
#include "Display.h"
extern EasyNex Display;

void terminalPrint(const String& str){
  Serial.println(str);
  Display.writeStr(str);
}