

void demoOne()
{
// this function will run the motors in both directions at a fixed speed
 // turn on motor A
 digitalWrite(in1, HIGH);
 digitalWrite(in2, LOW);
 // set speed to 200 out of possible range 0~255
 analogWrite(enA, 200);
 
 // turn on motor B
 digitalWrite(in3, HIGH);
 digitalWrite(in4, LOW);
 // set speed to 200 out of possible range 0~255
 analogWrite(enB, 200);
 delay(2000);
 
 // now change motor directions
 digitalWrite(in1, LOW);
 digitalWrite(in2, HIGH);
 digitalWrite(in3, LOW);
 digitalWrite(in4, HIGH);
 delay(2000);
 
 // now turn off motors
 digitalWrite(in1, LOW);
 digitalWrite(in2, LOW);
 digitalWrite(in3, LOW);
 digitalWrite(in4, LOW);
}

void demoTwo()
{
 // this function will run the motors across the range of possible speeds
 // note that maximum speed is determined by the motor itself and the operating voltage
 // the PWM values sent by analogWrite() are fractions of the maximumspeed possible
 // by your hardware
 // turn on motors
 digitalWrite(in1, LOW);
 digitalWrite(in2, HIGH);
 digitalWrite(in3, LOW);
 digitalWrite(in4, HIGH);
 // accelerate from zero to maximum speed
 for (int i = 0; i < 256; i++)
 {
 analogWrite(enA, i);
 analogWrite(enB, i);
 delay(20);
 }
 // decelerate from maximum speed to zero
 for (int i = 255; i >= 0; --i)
 {
 analogWrite(enA, i);
 analogWrite(enB, i);
 delay(20);
 }
 // now turn off motors
 digitalWrite(in1, LOW);
 digitalWrite(in2, LOW);
 digitalWrite(in3, LOW);
 digitalWrite(in4, LOW);
}

void loop()
{
 demoOne();
 delay(1000);
 demoTwo();
 delay(1000);
}



int lineS0 = A0; // Line Sensor0 (Left)
int lineS1 = A1; // Line Sensor1 (Center Left)
int lineS2 = A2; // Line Sensor2 (Center Right)
int lineS3 = A3; // Line Sensor3 (Right)
int lineSThres = 500;

// connect motor controller pins to Arduino digital pins
// motor one
int enA = 10;
int in1 = 9;
int in2 = 46; //8;
// motor two
int enB = 5;
int in3 = 47; //7;
int in4 = 6;
void setup()
{
 // set all the motor control pins to outputs
 pinMode(enA, OUTPUT);
 pinMode(enB, OUTPUT);
 pinMode(in1, OUTPUT);
 pinMode(in2, OUTPUT);
 pinMode(in3, OUTPUT);
 pinMode(in4, OUTPUT);
}


void loop() {
  // put your main code here, to run repeatedly:
  followLine();

}


// Follow Line
void followLine(){
  int val_lineS0= analogRead(lineS0);
  int val_lineS1= analogRead(lineS1);
  int val_lineS2= analogRead(lineS2);
  int val_lineS3= analogRead(lineS3);

//  Serial.println("val_lineS0");
//  Serial.println(val_lineS0);
//
//  Serial.println("val_lineS1");
//  Serial.println(val_lineS1);
//
//  Serial.println("val_lineS2");
//  Serial.println(val_lineS2);
//
//  Serial.println("val_lineS3");
//  Serial.println(val_lineS3);
  

//   if ((val_lineS1>lineSThres)&&(val_lineS2>lineSThres)){
//    servoL.write(120);
//    servoR.write(60);
//  }
//  else if((val_lineS2<lineSThres)){
//    servoL.write(90);
//    servoR.write(60);
//  }
//  else if((val_lineS1<lineSThres)){
//    servoL.write(120);
//    servoR.write(90);
//  }
//  else{
//    servoL.write(90);
//    servoR.write(90);
//  }
}


