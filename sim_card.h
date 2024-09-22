#ifndef SIM
#define SIM

  #include <Arduino.h>

  void setupSIM(void *pvParametrs);
  void displayInteraction(void *pvParametrs);
  void checkIncomingSMS(); // not implemented yet
  void checkIncomingCall(void *pvParametrs);

#endif