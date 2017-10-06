#include <Servo.h>

Servo servoL;
Servo servoR;

int s0 = A0; 
int s2 = A2;
int s3 = A3;
int s5 = A5;

int val0 ;
int val2 ;
int val3 ;
int val5 ;

int thres = 500;
int count = 0;

void setup(){
  Serial.begin(9600);
  servoL.attach(10); 
  servoR.attach(11);
}
  
void goForward(){
  servoL.write(120);
  servoR.write(60);
}

void turnLeft(){
  servoL.write(80);
  servoR.write(80);
}

void turnRight(){
  servoL.write(100);
  servoR.write(100);
}

void stay(){
  servoL.write(90);
  servoR.write(90);
}


void followLine(){
  val0 = analogRead(s0);
  val2 = analogRead(s2);
  val3 = analogRead(s3);
  val5 = analogRead(s5);

  if ((val0<500)&&(val5<500)){
    if ((val2>500)&&(val3>500)){
      goForward();
    }
    else if((val3<500)){
      servoL.write(90);
      servoR.write(60);
    }
    else if((val2<500)){
      servoL.write(120);
      servoR.write(90);
    }
    else{
      stay();
    }
  }
}


void turn_eight(int count) {
  val0 = analogRead(s0);
  val2 = analogRead(s2);
  val3 = analogRead(s3);
  val5 = analogRead(s5);
  if((count%8 >=2) && (count%8 <=5)) {
    while(!((val0<thres)&&(val2>thres)&&(val3>thres)&&(val5<thres))){
      turnRight();
      val0 = analogRead(s0);
      val2 = analogRead(s2);
      val3 = analogRead(s3);
      val5 = analogRead(s5);
    }
    
  }
  else {
    while(!((val0<thres)&&(val2>thres)&&(val3>thres)&&(val5<thres))){
      turnLeft();
      val0 = analogRead(s0);
      val2 = analogRead(s2);
      val3 = analogRead(s3);
      val5 = analogRead(s5);
    }
  }
}
  
void loop(){

  val0 = analogRead(s0);
  val2 = analogRead(s2);
  val3 = analogRead(s3);
  val5 = analogRead(s5);

  if ((val0>thres)&&(val2>thres)&&(val3>thres)&&(val5>thres)){ // at a crosssection
    stay();
    turn_eight(count);
    count = count +1;
  }
  else{
    followLine();
  }
}  



