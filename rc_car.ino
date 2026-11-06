/*
  Bluetooth RC Car (reconstructed firmware)
  Hardware: Arduino Nano + HC-05 + L298N + 4x N20 motors
  Course:   Basics of Robotics, BRAC University Residential Semester
  Author:   Md. Farhan Shahriar Shihab (github.com/iamfarhan99)
*/

#include <SoftwareSerial.h>

// HC-05 on software serial (keeps hardware Serial free for USB upload/debug)
const int BT_RX = 2;   // Nano RX <- HC-05 TX
const int BT_TX = 3;   // Nano TX -> HC-05 RX (use a voltage divider)
SoftwareSerial bt(BT_RX, BT_TX);

// L298N pins
const int ENA = 5;     // PWM, left motors
const int IN1 = 6;
const int IN2 = 7;
const int IN3 = 8;
const int IN4 = 9;
const int ENB = 10;    // PWM, right motors

int speedVal = 200;    // 0-255

void setup() {
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  Serial.begin(9600);
  bt.begin(9600);      // HC-05 default baud rate
  stopCar();
}

void loop() {
  if (bt.available()) {
    char cmd = bt.read();
    Serial.println(cmd);   // debug

    switch (cmd) {
      case 'F': forward();  break;
      case 'B': backward(); break;
      case 'L': left();     break;
      case 'R': right();    break;
      case 'S': stopCar();  break;
      default:
        // '0'..'9' sets speed
        if (cmd >= '0' && cmd <= '9') {
          speedVal = map(cmd - '0', 0, 9, 90, 255);
        }
        break;
    }
  }
}

void setSpeed() {
  analogWrite(ENA, speedVal);
  analogWrite(ENB, speedVal);
}

void forward() {
  setSpeed();
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
}

void backward() {
  setSpeed();
  digitalWrite(IN1, LOW);  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);  digitalWrite(IN4, HIGH);
}

void left() {
  setSpeed();
  // left side backward, right side forward (spin turn)
  digitalWrite(IN1, LOW);  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
}

void right() {
  setSpeed();
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);  digitalWrite(IN4, HIGH);
}

void stopCar() {
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
  digitalWrite(IN1, LOW); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, LOW);
}
