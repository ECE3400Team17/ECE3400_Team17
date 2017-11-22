#include <Servo.h>

Servo servoL;
Servo servoR;

int thresLine = 550;

int s2 = A1;
int s3 = A2;

int val2;
int val3;

void setup(){
  Serial.begin(9600);
  servoL.attach(11); 
  servoR.attach(10);
}

void loop(){

  val2 = analogRead(s2);
  val3 = analogRead(s3);

//  Serial.println("val2");
//  Serial.println(val2);
//  Serial.println("val3");
//  Serial.println(val3);

//  Serial.println(val2);
//  Serial.println(val3);

  
  if ((val2>thresLine)&&(val3>thresLine)){
    servoL.write(120);
    servoR.write(60);
  }
  else if((val3<thresLine)){
    servoL.write(90);
    servoR.write(60);
  }
  else if((val2<thresLine)){
    servoL.write(120);
    servoR.write(90);
  }
  else{
    servoL.write(90);
    servoR.write(90);
  }

}




