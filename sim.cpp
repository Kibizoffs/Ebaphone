#include <Arduino.h>
#include <Sim800L.h> // модуль SIM-карты
#include <SoftwareSerial.h>

#include "Config.h"

SoftwareSerial SIM_card(sim_rx_pin, sim_tx_pin);

void setupSIM(){
  Serial.println("MSG: Initializing SIM card...");
  SIM_card.begin(baud_0);
}