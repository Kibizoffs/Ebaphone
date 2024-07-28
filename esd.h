#ifndef eSD
#define eSD

#include <SD.h> // модуль SD-карты

File root;

void setupSD();

void printDirectory(File dir, int numTabs);

#endif