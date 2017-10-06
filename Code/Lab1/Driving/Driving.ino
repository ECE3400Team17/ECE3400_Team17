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
 
  servoL.write(90);
  servoR.write(90);
//  delay(2000);
//  servoL.detach(); 
//  servoR.detach();
//  delay(1000);
//  servoL.attach(10); 
//  servoR.attach(11);
//  servoL.write(120);
//  servoR.write(60);
//  delay(2000);
//  servoL.detach(); 
//  servoR.detach();
//  delay(1000);
//  servoL.attach(10); 
//  servoR.attach(11);

//  servoL.write(0);
//  servoR.write(0);
//  delay(1000);
//  


}



