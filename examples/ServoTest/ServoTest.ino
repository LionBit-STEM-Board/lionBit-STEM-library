/*
  This example demonstrates a simple test of a servo motor using the Lionbit board.
  The servo motor is controlled to move to different positions within a range of angles.
*/



#include <ESP32Servo.h>
#define PIN_SG90 D16 // Output pin used

Servo sg90;

void setup() {
  sg90.setPeriodHertz(50); // PWM frequency for SG90
  sg90.attach(PIN_SG90, 0, 4096); // Minimum and maximum pulse width (in µs) to go from 0° to 180
 }
void loop() {
 //rotation from 0 to 180°
  for (int pos = 0; pos <= 180; pos += 1) {
    sg90.write(pos);
    delay(10);
  }
 // Rotation from 180° to 0
  for (int pos = 180; pos >= 0; pos -= 1) {
    sg90.write(pos);
    delay(10);
  }
}