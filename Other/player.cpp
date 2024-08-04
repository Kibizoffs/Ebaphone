#include <Arduino.h>
#include <DFRobotDFPlayerMini.h> // плеер
#include <SoftwareSerial.h>

#include "Config.h"

SoftwareSerial player_serial(player_rx_pin, player_tx_pin);

DFRobotDFPlayerMini Player;

void setupPlayer(void *pvParametrs){
  Player.begin(player_serial);
  Serial.println("MSG: Player - OK\n");
}