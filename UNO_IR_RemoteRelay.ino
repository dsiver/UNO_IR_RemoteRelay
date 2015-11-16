#include <IRremote.h>
#include <IRremoteInt.h>

/*
 * Project 2 Receiver
 * Board: Arduino UNO or similar with ATmega328P
 * Relay: Parallax, Inc. Single Relay Board (#27115)
 *
 * David Siver
 * CS 4985
 * Fall 2015
 */

#define RELAY_PIN 7
#define RELAY_NO_CLOSED HIGH
#define RELAY_NO_OPEN LOW
#define LED_PIN 8
#define IR_PIN 10
#define OFF "off"
#define ON "on"
#define NONE "none"
#define POWER 0xFFA25D
#define CYCLE_3X 0xFF7A85

IRrecv irReceiver(IR_PIN);
decode_results signals;
boolean relayON;

void setup() {
  relayON = false;
  Serial.begin(9600);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  irReceiver.enableIRIn();
}

void loop() {
  String remoteResult = checkRemote();
  if (remoteResult.equals(ON)) {
    toggleRelay(ON);
  }
  else if (remoteResult.equals(OFF)) {
    toggleRelay(OFF);
  }
  else if (remoteResult.equals(String(CYCLE_3X))) {
    cycleRelay(3);
  }
  setLED();
}

void cycleRelay(int times) {
  for (int i = 0; i < times; i++) {
    if (relayON) {
      toggleRelay(OFF);
      delay(500);
      toggleRelay(ON);
      delay(500);
    }
  }
}

void setLED() {
  if (relayON) {
    toggleLED(ON);
  }
  else {
    toggleLED(OFF);
  }
}

String checkRemote() {
  String result = NONE;
  if (irReceiver.decode(&signals)) {
    if (signals.value == POWER) {
      if (relayON) {
        result = OFF;
      }
      else {
        result = ON;
      }
    }
    else if (signals.value == CYCLE_3X) {
      result = String(CYCLE_3X);
    }
    //Serial.println(signals.value, HEX);
    irReceiver.resume();
  }
  return result;
}

void toggleLED(String mode) {
  if (mode == ON) {
    digitalWrite(LED_PIN, HIGH);
  }
  else if (mode == OFF) {
    digitalWrite(LED_PIN, LOW);
  }
}

void toggleRelay(String mode) {
  if (mode == ON) {
    digitalWrite(RELAY_PIN, RELAY_NO_CLOSED);
    relayON = true;
  }
  else if (mode == OFF) {
    digitalWrite(RELAY_PIN, RELAY_NO_OPEN);
    relayON = false;
  }
}
