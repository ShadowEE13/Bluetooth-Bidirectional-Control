#include <SoftwareSerial.h>

// ===== 腳位 =====
const int BTN = 2;
const int EN  = 9;    // L293D 1,2EN（PWM 控制轉速）
const int IN1 = 7;    // L293D 1A
const int IN2 = 8;    // L293D 2A
SoftwareSerial BT(10, 11);   // (RX, TX)

// ===== 參數 =====
const int MIN_PWM      = 120;    // 馬達起動門檻
const int POT_DEADBAND = 10;
const unsigned long DEBOUNCE_MS = 50;
const unsigned long TIMEOUT_MS  = 1500;   // 超過這段時間沒收到 POT 就停馬達

// ===== 按鈕狀態 =====
int lastReading = HIGH;
int stableState = HIGH;
unsigned long lastChangeTime = 0;

// ===== 藍牙接收 =====
String rxBuf = "";
unsigned long lastPotTime = 0;
bool motorRunning = false;

// ----- 馬達 -----
void setMotor(int pot) {
  int pwm;
  if (pot < POT_DEADBAND) pwm = 0;
  else                    pwm = map(pot, POT_DEADBAND, 1023, MIN_PWM, 255);
  pwm = constrain(pwm, 0, 255);

  analogWrite(EN, pwm);
  motorRunning = (pwm > 0);

  Serial.print("RX POT=");
  Serial.print(pot);
  Serial.print("  -> PWM=");
  Serial.println(pwm);
}

// ----- 按鈕 -----
void checkButton() {
  int reading = digitalRead(BTN);

  if (reading != lastReading) {
    lastChangeTime = millis();
  }

  if (millis() - lastChangeTime > DEBOUNCE_MS) {
    if (reading != stableState) {
      stableState = reading;
      if (stableState == LOW) {
        BT.println("LED:ON");
        Serial.println("TX LED:ON");
      } else {
        BT.println("LED:OFF");
        Serial.println("TX LED:OFF");
      }
    }
  }

  lastReading = reading;
}

// ----- 藍牙接收 -----
void handleBT() {
  while (BT.available()) {
    char c = BT.read();

    if (c == '\n') {
      rxBuf.trim();
      if (rxBuf.startsWith("POT:") && rxBuf.length() > 4) {
        int pot = rxBuf.substring(4).toInt();
        if (pot >= 0 && pot <= 1023) {                // 範圍檢查
          setMotor(pot);
          lastPotTime = millis();
        }
      }
      rxBuf = "";
    } 
    else {
      rxBuf += c;
      if (rxBuf.length() > 20) rxBuf = "";            // 防止無限累積
    }
  }
}

// ---- 斷線保護 ----
void checkTimeout() {
  if (motorRunning && millis() - lastPotTime > TIMEOUT_MS) {
    analogWrite(EN, 0);
    motorRunning = false;
    Serial.println("Timeout: no POT data, motor stopped");
  }
}

void setup() {
  Serial.begin(9600);
  BT.begin(9600);
  rxBuf.reserve(24);           // 預先配置rxbuf的記憶體

  pinMode(BTN, INPUT_PULLUP);
  pinMode(EN,  OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(EN, 0);

  Serial.println("Master ready.");
}

void loop() {
  checkButton();
  handleBT();
  checkTimeout();
}