#include <Arduino.h>
#include <SoftwareSerial.h>

const int sim_rx_pin = 12;
const int sim_tx_pin = 13;
const int sim_ring_pin = 27;

SoftwareSerial GSM(sim_rx_pin, sim_tx_pin);

void setup() {
  Serial.begin(9600);
  GSM.begin(9600);

  waitForInput();

  Serial.println("Initializing SIM800L...");
  sendATCommand("AT", 2000);
  sendATCommand("AT+IPR=9600", 2000);

  sendATCommand("AT+CPIN?", 2000);

  sendATCommand("AT+CREG=1", 2000);
  sendATCommand("AT+CSQ", 2000);
  sendATCommand("AT+COPS=?", 4000);
  sendATCommand("AT+CSCS=\"UCS2\"", 2000);
  sendATCommand("AT+CMGF=1", 2000);
  sendATCommand("AT+CSCA?", 2000);

  sendATCommand("AT+CLIP=1", 2000);
  sendATCommand("AT+COLP=1", 2000);
  sendATCommand("AT+CFUN=1,1", 5000);
  pinMode(sim_ring_pin, INPUT_PULLUP);

  makeCall("+79100092107");
  Serial.println("YOOO");
}

void loop() {
  checkIncomingSMS();
  checkIncomingCall();
}

void waitForInput(){
  while (true) {
    if (Serial.available()) {
      char c = Serial.read();
      if (c == ' ') {
        break;
      }
    }
  }
}

String getResponse(const String command, const int delay_time){
  String response = command + ": ";
  unsigned long start_time = millis();
  while (millis() - start_time < delay_time) {
    while (GSM.available()) {
      char c = GSM.read();
      response += c;
    }
  }
  Serial.println(response);
  return response;
}

void sendATCommand(const String command, const int delay_time) {
  GSM.println(command);
  String response = getResponse(command, delay_time);
  if (command == "AT+CPIN?"){
    unlockSIM(response);
  }
}

void unlockSIM(const String unlocked){
  Serial.println(unlocked);
  if ((unlocked.indexOf("READY") == -1) && (unlocked != "AT+CPIN?")){
    Serial.println("Enter PIN: ");
    String PIN = "";
    while (true) {
      if (Serial.available()) {
        char c = Serial.read();
        if ((c == '\n') || (c == '\r')) {
          break;
        }
        PIN += c;
      }
    }
    Serial.print("Entered PIN: ");
    Serial.println(PIN);

    const String enterPIN = "AT+CPIN=" + PIN;

    sendATCommand(enterPIN, 5000);
  }
}

void checkIncomingSMS() {
  while (GSM.available()) {
    String sms = GSM.readString();
    if (sms.indexOf("+CMTI:") != -1) {
      Serial.println("New SMS received.");
      sendATCommand("AT+CMGR=1", 2000);
    }
  }
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

void sendSMS(const String phone_number, const String message) {
  GSM.print("AT+CMGS=\"");
  GSM.print(phone_number);
  GSM.println("\"");
  GSM.print(message);
  GSM.write(26);
  vTaskDelay(2000);

  long start_time = millis();
  while (millis() - start_time < 3000) {
    while (GSM.available()) {
      char c = GSM.read();
      Serial.print(c);
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
