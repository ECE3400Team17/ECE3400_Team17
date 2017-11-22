#include <Servo.h>
Servo servoL;
Servo servoR;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  servoL.attach(11); 
  servoR.attach(10);
}

void loop() {
  servoL.write(180);
  servoR.write(0);
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




