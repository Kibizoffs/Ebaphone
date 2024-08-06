#include <Arduino.h>
#include <SoftwareSerial.h>
#include "config.h"
#include "utils.h"

extern SemaphoreHandle_t sd_status;
extern SemaphoreHandle_t sim_status;

SoftwareSerial GSM(sim_tx_pin, sim_rx_pin);

String getATResponse(const int delay_time){
  String response = "";
  unsigned long start_time = millis();
  while (millis() - start_time < delay_time) {
    while (GSM.available()) {
      char c = GSM.read();
      response += c;
    }
    vTaskDelay(10);
  }
  replaceAll(response, "\n", " ");
  response += "\\r";
  print(response);
  return response;
}

void unlockSIM(const String unlocked);
void sendATCommand(const String command, const int delay_time) {
  GSM.println(command);
  String response = getATResponse(delay_time);
  if (command == "AT+CPIN?"){
    unlockSIM(response);
  }
}

void unlockSIM(const String unlocked){
  if ((unlocked.indexOf("READY") == -1) && (unlocked != "AT+CPIN?")){
    print("Enter PIN: ");
    String PIN = "";
    while (true) {
      if (Serial.available()) {
        char c = Serial.read();
        if (c == '\\') {
          break;
        }
        PIN += c;
      }
    }
    PIN += "\n";
    print(PIN);
    String enter_PIN = "AT+CPIN=" + PIN;
    sendATCommand(enter_PIN, 5000);
  }
}

void setupSIM(void *pvParametrs) {
  xSemaphoreTake(sd_status, portMAX_DELAY);
  clear();

  GSM.begin(9600);
  print("Initializing SIM800L...\\r");

  sendATCommand("AT+IPR=9600", 2000);

  sendATCommand("AT+CPIN?", 2000);

  sendATCommand("AT+CREG=1", 2000);
  sendATCommand("AT+CSQ", 2000);
  sendATCommand("AT+COPS=?", 4000);
  //sendATCommand("AT+CSCS=\"UCS2\"", 2000);
  //sendATCommand("AT+CMGF=1", 2000);
  //sendATCommand("AT+CSCA?", 2000);

  //psendATCommand("AT+CLIP=1", 2000);
  //sendATCommand("AT+COLP=1", 2000);
  sendATCommand("AT+CFUN=1,1", 5000);
  pinMode(sim_ring_pin, INPUT_PULLUP);

  vTaskDelay(2000);
  xSemaphoreGive(sim_status);
  vTaskDelete(NULL);
}

void sendSMS(const String phone_number, const String message) {
  GSM.print("AT+CMGS=\"");
  GSM.print(phone_number);
  GSM.println("\"");
  GSM.print(message);
  GSM.write(26);
  vTaskDelay(2000);

  long start_time = millis();
  /*
  while (millis() - start_time < 3000) {
    while (GSM.available()) {
      char c = GSM.read();
      Serial.print(c);
    }
  }
  */
}

void checkIncomingSMS() {
  while (GSM.available()) {
    String sms = GSM.readString();
    if (sms.indexOf("+CMTI:") != -1) {
      Serial.println("New SMS received");
      sendATCommand("AT+CMGR=1", 2000);
    }
  }
}

void inCall(){
  String response = "";
  long start_time = millis();
  while (true){
    if ((millis() - start_time) < 59000) {
      while (GSM.available()) {
        char c = GSM.read();
        response += c;
        if (response.indexOf("NO CARRIER") != -1){
          Serial.println("Call ended: NO CARRIER");
          return;
        }
      }
    }
    else {
      Serial.println("Ending call...");
      sendATCommand("ATH", 1000);
    }
  }
}

void makeCall(const String phone_number) {
  sendATCommand("AT+CSQ", 2000);
  sendATCommand("AT+CREG?", 2000);
  Serial.println("Making a call...");
  GSM.print("ATD");
  GSM.print(phone_number);
  GSM.println(";");
  vTaskDelay(2000);
  inCall();
}

void checkIncomingCall(){
  if (digitalRead(sim_ring_pin) == LOW) {
    Serial.println("Incoming call detected");
    Serial.println("Answering call...");
    sendATCommand("ATA", 1000);
    vTaskDelay(2000);
    inCall();
  }
}
