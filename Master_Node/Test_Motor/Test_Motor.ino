const int EN  = 9;
const int IN1 = 7;
const int IN2 = 8;

// speed: -255 ~ 255，正負號決定轉向，0 = 停
void setMotor(int speed) {
  speed = constrain(speed, -255, 255);

  if (speed > 0) {          // 正轉
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
  } else if (speed < 0) {   // 反轉
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
  } else {                  // 煞車
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
  }
  analogWrite(EN, abs(speed));
}

void setup() {
  Serial.begin(9600);
  pinMode(EN,  OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  setMotor(0);
  Serial.println("Enter speed -255 ~ 255 (0 = stop):");
}

void loop() {
  if (Serial.available()) {
    String s = Serial.readStringUntil('\n');
    s.trim();
    if (s.length() == 0) return;   // 忽略空行

    int v = s.toInt();
    setMotor(v);
    Serial.print("Speed = ");
    Serial.println(v);
  }
}