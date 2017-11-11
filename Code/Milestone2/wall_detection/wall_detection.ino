#include <Servo.h>
#include "Adafruit_VL53L0X.h"

Adafruit_VL53L0X lox = Adafruit_VL53L0X();


Servo servoL;
Servo servoR;

int s2 = A2;
int s3 = A3;
int s0 = A0;
int s5 = A5;
int val0;
int val2;
int val3;
int val5;
int thres = 90;

void setup(){
  Serial.begin(115200);
  servoL.attach(10); 
  servoR.attach(11);
  servoL.write(90);
  servoR.write(90);

  // wait until serial port opens for native USB devices
  while (! Serial) {
    delay(1);
  }
  
  Serial.println("Adafruit VL53L0X test");
  if (!lox.begin()) {
    Serial.println(F("Failed to boot VL53L0X"));
    while(1);
  }
  // power 
  Serial.println(F("VL53L0X API Simple Ranging example\n\n")); 
}

void loop(){

  val0 = analogRead(s0);
  val5 = analogRead(s5);
  val2 = analogRead(s2);
  val3 = analogRead(s3);

 /* 
  Serial.println(val0);
  Serial.println(val2);
  Serial.println(val3);
  Serial.println(val5);
  Serial.println("\n");
  delay(500);
*/
  if(openwall()==1) {
    turnleft();
  }

  else {
    goStraight();
  }
  

}

bool openwall() {
  VL53L0X_RangingMeasurementData_t measure;
  lox.rangingTest(&measure, false); // pass in 'true' to get debug data printout!
  Serial.print("Distance (mm): "); Serial.println(measure.RangeMilliMeter);
  if(measure.RangeStatus != 4 && measure.RangeMilliMeter < thres && measure.RangeMilliMeter != 0)
    return 1;
  else
    return 0;
}
void goStraight() {
  if ((val2>500)&&(val3>500)){
    servoL.write(120);
    servoR.write(60);
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
    servoL.write(90);
    servoR.write(90);
  }
}

void turnleft() {
  //servoL.write(120);
  //servoR.write(60);
  
  //delay(100);
  stay();
  servoL.write(0);
  servoR.write(0);

  delay(800);

  servoL.write(90);
  servoR.write(90);
}

void stay() {
  servoL.write(90);
  servoR.write(90);
}

