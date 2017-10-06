#include <Servo.h>
Servo servoL;
Servo servoR;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  servoL.attach(10); 
  servoR.attach(11);
}

void loop() {
  servoL.write(0);
  servoR.write(180);
  delay(1000);
}







//#include <Servo.h>
//
//Servo servo1;
//
//int servoPin = 6;
//
//void setup() {
//  // put your setup code here, to run once:
//  servo1.attach(servoPin);
//  servo1.write(90);
//}
//
//
//
//void loop() {
//  // put your main code here, to run repeatedly:
//  val = analogRead(analogPin);   // read the input pin
//  Serial.println(val);
//  
//<<<<<<< HEAD
//}
//=======
//}
//>>>>>>> 16c2bea53a483d545b6428cc473c436f8f9a300d




