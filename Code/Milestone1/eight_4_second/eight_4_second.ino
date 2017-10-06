#include <Servo.h>

// motors
Servo servoL;
Servo servoR;
 
// inputs for sensors
int s0 = A0;
int s2 = A1;
int s3 = A2;
int s5 = A3;
 
int val0 ;
int val2 ;
int val3 ;
int val5 ;
 
int thres = 500;
int count = 0;
 
void setup(){
  count = 0;
  Serial.begin(9600);
  servoL.attach(10);
  servoR.attach(11);
  servoL.write(120);
  servoR.write(60);
  delay(800);
}

// move forward
void goForward(){
  servoL.write(120);
  servoR.write(60);
}
 
// left turn 90 degrees
void turnLeft(){
  servoL.write(120);
  servoR.write(60);
 
  delay(100);
  stay();
  servoL.write(0);
  servoR.write(0);
 
  delay(650);
 
  servoL.write(90);
  servoR.write(90);
}
 
// right turn 90 degrees 
void turnRight(){
  servoL.write(120);
  servoR.write(60);
  delay(100);
  stay();
  servoL.write(180);
  servoR.write(180);
 
  delay(650);
 
  servoL.write(90);
  servoR.write(90);
}

// stop
void stay(){
  servoL.write(90);
  servoR.write(90);
 
 
}
 
// follow the black line 
void followLine(){
  // take the sensor reading
  val0 = analogRead(s0);
  val2 = analogRead(s2);
  val3 = analogRead(s3);
  val5 = analogRead(s5);
  
  // if both sensors are on Black 
  if ((val2>500)&&(val3>500)){ 
    // move forward
    goForward();
  }
  
  // if s3 is on White
  else if ((val3<500 && val2>500)){ 
    // turn Right
    servoL.write(90);
    servoR.write(60);
  }
  
  // if s2 is on White
  else if((val2<500 && val3>500)){  
    // turn Left
    servoL.write(120);
    servoR.write(90);
  }
  
  else{ 
    // stop
    stay();
  } 
  
}
 
// make the number 8 
void turn_eight(int count) {
  val0 = analogRead(s0);
  val2 = analogRead(s2);
  val3 = analogRead(s3);
  val5 = analogRead(s5);
  if((count%8 >=2) && (count%8 <=5)) {
    turnRight();
    val0 = analogRead(s0);
    val2 = analogRead(s2);
    val3 = analogRead(s3);
    val5 = analogRead(s5);
  
  }
  else {
    turnLeft();
    val0 = analogRead(s0);
    val2 = analogRead(s2);
    val3 = analogRead(s3);
    val5 = analogRead(s5);
  }
}
 
 
void loop(){
  val0 = analogRead(s0);
  val2 = analogRead(s2);
  val3 = analogRead(s3);
  val5 = analogRead(s5);
 
  Serial.println(val0);
  
  // at a crosssection
  if ((val0>thres)&&(val2>thres)&&(val3>thres)&&(val5>thres)){ 
  stay();
  turn_eight(count);
  count = count +1;
  }
  
  // otherwise
  else{
  followLine();
  }
}  
