#include <SoftwareSerial.h>
const int LED_PIN = 5;
const int POT_PIN = A0;
// SoftwareSerial(RX, TX)
SoftwareSerial BT(10, 11);

String cmd = "";
bool ledState = false;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(POT_PIN, INPUT);

  Serial.begin(9600);   // USB Serial：給電腦看除錯訊息
  BT.begin(9600);       // HC-05：先假設資料模式為9600

  Serial.println("System ready");
}

void loop() {

  //send data to master
  int potValue = analogRead(POT_PIN);

  //debug
  Serial.print("POT = ");
  Serial.println(potValue);

  //real data 
  BT.print("POT:");
  BT.println(potValue);


delay(200);

  if (BT.available()) {
    cmd = BT.readStringUntil('\n');
    cmd.trim();

    Serial.print("Bluetooth received: ");
    Serial.println(cmd);

    if (cmd == "LED:ON") {
      ledState = true;
      digitalWrite(LED_PIN, HIGH);
      //ACK signal (not deal yet)
      //BT.println("OK: LED ON");
    }
    else if (cmd == "LED:OFF") {
      ledState = false;
      digitalWrite(LED_PIN, LOW);
      //ACK signal (not deal yet)
      //BT.println("OK: LED OFF");
    }
  }
}