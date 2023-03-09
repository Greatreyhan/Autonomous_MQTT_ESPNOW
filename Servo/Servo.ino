#include <Servo.h>

Servo myservo;  // create servo object to control a servo

void setup() {
  myservo.attach(D1);  // attaches the servo on pin 9 to the servo object
}

void loop() {
  myservo.write(0);               
  delay(1000);
  myservo.write(180);               
  delay(1000);
 
}
