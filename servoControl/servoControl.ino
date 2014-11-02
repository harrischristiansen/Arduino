#include <Servo.h>

Servo servoX;

void setup() {
  servoX.attach(4);
  Serial.begin(57600);
}

void loop() {
  if(Serial.available() > 0) {
    int angle = Serial.parseInt();
    servoX.write(angle);
  }
}
