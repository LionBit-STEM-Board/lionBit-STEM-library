#include "lionbitsupport.h"

// L298N Motor Driver Pins
#define ENA D5
#define IN1 D6
#define IN2 D7

#define ENB D8
#define IN3 D9
#define IN4 D10

void setup() {
  lionStart();
  Serial.begin(115200);

  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  stopMotors();
}

void loop() {

  // Move Forward
  moveForward(150);
  TextDisplay("Moving Forward");
  Serial.println("Moving Forward");
  delay(3000);

  // Stop
  stopMotors();
  TextDisplay("Stopped");
  Serial.println("Stopped");
  delay(2000);

  // Move Reverse
  moveReverse(150);
  Serial.println("Moving Reverse");
  TextDisplay("Moving Reverse");
  delay(3000);

  // Stop
  stopMotors();
  Serial.println("Stopped");
  delay(3000);
}

// -------- Motor Functions --------

void moveForward(int speedVal) {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

  analogWrite(ENA, speedVal);
  analogWrite(ENB, speedVal);
}

void moveReverse(int speedVal) {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);

  analogWrite(ENA, speedVal);
  analogWrite(ENB, speedVal);
}

void stopMotors() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);

  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}
