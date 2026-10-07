#include <SoftwareSerial.h>

SoftwareSerial BT(10, 11);   // (RX, TX)

void setup() {
  Serial.begin(9600);        // 電腦 ⇄ Arduino
  BT.begin(38400);           // Arduino ⇄ HC-05（AT 模式固定 38400）
  Serial.println("Ready. Enter AT commands:");
}

void loop() {
  if (Serial.available()) BT.write(Serial.read());
  if (BT.available())     Serial.write(BT.read());
}