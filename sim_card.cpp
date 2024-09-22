#include <Arduino.h>
#include <SoftwareSerial.h>
#include "config.h"
#include "display.h"
#include "utils.h"

extern SemaphoreHandle_t sd_status;
extern SemaphoreHandle_t sim_status;
extern SoftwareSerial Display;

SoftwareSerial GSM(sim_tx_pin, sim_rx_pin);

bool accept_call = false;

String getATResponse(const int delay_time, bool output){
  String response = "";
  unsigned long start_time = millis();
  while (millis() - start_time < delay_time) {
    while (GSM.available()) {
      char c = GSM.read();
      response += c;
    }
    vTaskDelay(10);
  }
  if (output){
    print(response);
  }
  return response;
}

void unlockSIM(const String unlocked);
String sendATCommand(const String command, const int delay_time, bool output = true) {
  GSM.println(command);
  String response = getATResponse(delay_time, output);
  if (command == "AT+CPIN?"){
    unlockSIM(response);
  }
  return response;
}

void unlockSIM(const String unlocked){
  Serial.println(unlocked);
  if ((unlocked.indexOf("READY") == -1) && (unlocked != "")){
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
    PIN.trim();
    String enter_PIN = "AT+CPIN=" + PIN;
    print(PIN + "#br");
    sendATCommand(enter_PIN, 5000);
  }
}

void setupSIM(void *pvParametrs) {
  clear();

  GSM.begin(9600);
  print("Initializing SIM800L...#br");

  sendATCommand("AT+IPR=9600", 2000);

  sendATCommand("AT+CPIN?", 2000);

  sendATCommand("AT+CREG=1", 2000);
  sendATCommand("AT+CSQ", 2000);
  //sendATCommand("AT+COPS=?", 6000);
  //sendATCommand("AT+CSCS=\"UCS2\"", 2000);
  //sendATCommand("AT+CSCA?", 2000);

  //sendATCommand("AT+CLIP=1", 2000);
  //sendATCommand("AT+COLP=1", 2000);
  sendATCommand("AT+CFUN=1,1", 5000);

  vTaskDelay(2000);
  xSemaphoreGive(sim_status);
  vTaskDelete(NULL);
}

void sendSMS(const String phone_number, const String message) {
  GSM.println("AT+CMGF=1");
  vTaskDelay(1000);
  
  GSM.print("AT+CMGS=\"");
  GSM.print(phone_number);
  GSM.println("\"");
  vTaskDelay(1000);
  
  GSM.print(message);
  GSM.write(26);
  vTaskDelay(3000);

  while (GSM.available()) {
    String response = GSM.readString();
    Serial.print(response);
  }
}

void checkIncomingSMS() { // Isn't implemented yet
  while (GSM.available()) {
    String sms = GSM.readString();
    if (sms.indexOf("+CMTI:") != -1) {
      Serial.println("New SMS received");
      sendATCommand("AT+CMGR=1", 2000);
    }
  }
}

void endCall(){
  Serial.println("Ending call...");
  execDisplay("phone_msg.pco=63488");
  sendATCommand("ATH", 1000);
  vTaskDelay(1000);
  setPage("Menu_h");
}

void inCall(String phone_number){
  setPage("Call_h");
  vTaskDelay(100 / portTICK_PERIOD_MS);
  writeStr("phone_val.txt", phone_number);
  unsigned long start_time = millis();
  while (true){
    unsigned long current_time = millis();
    unsigned long delta_time = current_time - start_time;
    unsigned long seconds = delta_time / 1000;
    unsigned long minutes = seconds / 60;
    seconds %= 60;

    char timeStr[6];
    sprintf(timeStr, "%02lu:%02lu", minutes, seconds);
    writeStr("phone_msg.txt", String(timeStr));

    String response = sendATCommand("AT+CLCC", 500, false);
    if ((response.indexOf("NO CARRIER") != -1) || (delta_time > 60 * 1000)) {
      endCall();
      break;
    }
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
  endCall();
}

void makeCall(const String phone_number) {
  sendATCommand("AT+CSQ", 2000);
  sendATCommand("AT+CREG?", 2000);
  Serial.println("Making a call...");
  setPage("Call_h");
  writeStr("phone_val.txt", phone_number);
  GSM.print("ATD");
  GSM.print(phone_number);
  GSM.println(";");
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  inCall(phone_number);
}

void checkIncomingCall(void *pvParametrs) {
  while (true) {
    String response = sendATCommand("AT+CLCC", 1000, false);

    if (response.indexOf("RING") != -1) {
      int index = response.indexOf("+CLIP:");
      if (index != -1) {
        String phone_number = response.substring(index + 8);
        index = phone_number.indexOf("\"");
        if (index != -1) {
          phone_number = phone_number.substring(0, index);
        }

        Serial.println("Incoming call detected from " + phone_number);

        setPage("Offer_h");
        writeStr("phone_val.txt", phone_number);

        unsigned long start_time = millis();
        while (true) {
          response = sendATCommand("AT+CLCC", 500, false);

          if (accept_call) {
            accept_call = false;
            sendATCommand("ATA", 1000);
            inCall(phone_number);
            break;
          }
          else if (response.indexOf("NO CARRIER") != -1 || (millis() - start_time > 10000)) {
            endCall();
            break;
          }
          vTaskDelay(50 / portTICK_PERIOD_MS);
        }
      } else {
        Serial.println("Failed to parse phone number.");
      }
    }
    vTaskDelay(200 / portTICK_PERIOD_MS);
  }
}

void displayInteraction(void *pvParametrs) {
  while (true){
    if (Display.available()) {
      String command = Display.readStringUntil('\n');
      command = command.substring(0, command.length() - 2);

      if (command == "callDecline"){
        endCall();
        continue;
      }
      else if (command == "callAccept"){
        accept_call = true;
        continue;
      }

      int index = command.indexOf('|');
      if (index != -1) {
        String cmd = command.substring(0, index);
        String rest = command.substring(index + 1);
        if (cmd == "makeCall") {
          Serial.println("Making call to: " + rest);
          makeCall(rest);
        }
        else if (cmd == "sendSMS") {
          index = rest.indexOf('|');
          if (index != -1) {
            String phoneNumber = rest.substring(0, index);
            String message = rest.substring(index + 1);
            Serial.println("Sending SMS to: " + phoneNumber);
            Serial.println("Message: " + message);
            setPage("Menu_h");
            sendSMS(phoneNumber, message);
          }
        }
      }
    }
    vTaskDelay(50);
  }
}