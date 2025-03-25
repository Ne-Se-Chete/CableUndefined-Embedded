#include "Arduino.h"

#define STB_PIN 12
#define DAT_PIN 13

#define AY0_PIN 4
#define AY1_PIN 2
#define AY2_PIN 15

#define AX0_PIN 18
#define AX1_PIN 5
#define AX2_PIN 17
#define AX3_PIN 16

#define ADDR0_PIN 19
#define ADDR1_PIN 21
#define ADDR2_PIN 22
#define ADDR3_PIN 23

#define MAX_ADDRESS 15
#define MAX_AX 15
#define MAX_AY 7

void setupPins() {
  pinMode(STB_PIN, OUTPUT);
  pinMode(DAT_PIN, OUTPUT);

  pinMode(AY0_PIN, OUTPUT);
  pinMode(AY1_PIN, OUTPUT);
  pinMode(AY2_PIN, OUTPUT);

  pinMode(AX0_PIN, OUTPUT);
  pinMode(AX1_PIN, OUTPUT);
  pinMode(AX2_PIN, OUTPUT);
  pinMode(AX3_PIN, OUTPUT);

  pinMode(ADDR0_PIN, OUTPUT);
  pinMode(ADDR1_PIN, OUTPUT);
  pinMode(ADDR2_PIN, OUTPUT);
  pinMode(ADDR3_PIN, OUTPUT);
}

int setConnection(uint8_t addr, uint8_t AX, uint8_t AY, bool mode) {
  if (addr > MAX_ADDRESS || AX > MAX_AX || AY > MAX_AY) {
    return -1;
  }

  // Set ADDR_PORT bits using digitalWrite
  digitalWrite(ADDR0_PIN, 1);
  digitalWrite(ADDR1_PIN, 1);
  digitalWrite(ADDR2_PIN, 1);
  digitalWrite(ADDR3_PIN, 1);

  // Set AX_PORT bits using digitalWrite
  digitalWrite(AX0_PIN, 1);
  digitalWrite(AX1_PIN, 1);
  digitalWrite(AX2_PIN, 0); 
  digitalWrite(AX3_PIN, 0);

  // Set AY_PORT bits using digitalWrite
  digitalWrite(AY0_PIN, 0);
  digitalWrite(AY1_PIN, 0);
  digitalWrite(AY2_PIN, 0);

  digitalWrite(DAT_PIN, mode);

  digitalWrite(STB_PIN, HIGH);  // Set STB high

  delayMicroseconds(2);

  digitalWrite(STB_PIN, LOW);  // Set STB low

  return 1;
}

void setup() {
  Serial.begin(9600);
  setupPins();
  setConnection(0b1111, 1, 1, true);
}

void loop() {
  digitalWrite(DAT_PIN, 0);
  Serial.println("Here1");

  delay(2000);

  digitalWrite(DAT_PIN, 1);
  Serial.println("Here2");


  delay(2000);

}
