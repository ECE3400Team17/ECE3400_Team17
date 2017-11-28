#include <Servo.h>

// instantiate the 2 servos
Servo servoL;
Servo servoR;


int lineS0 = A0; // Line Sensor0 (Left)
int lineS2 = A1; // Line Sensor1 (Center Left)
int lineS3 = A2; // Line Sensor2 (Center Right)
int lineS5 = A3; // Line Sensor3 (Right)
int lineSThres = 500;

int leftWall = A6;// Left Wall Sensor
int frontWall = A4;// Front Wall Sensor
int rightWall = A5;// Right Wall Sensor

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  servoL.attach(10);
  servoR.attach(11);
  
}

void loop() {
//    bool decLeft = detectLeftWall();
//  bool decFront = detectFrontWall();
//  bool decRight = detectRightWall();
//  Serial.println("L");
//  Serial.println(decLeft);
//  Serial.println("F");
//  Serial.println(decFront);
//  Serial.println("R");
// Serial.println(decRight);
//     servoL.write(90);
//    servoR.write(90);
  
  bool cross = atCrossRoad();
  if (cross){
      servoL.write(90);
  servoR.write(90);
  delay(500);
    decisionAtCross();
  }
  else{
    followLine();
  }

//followLine();
  

}

void decisionAtCross(){
  bool decLeft = detectLeftWall();
  bool decFront = detectFrontWall();
  bool decRight = detectRightWall();
  
  if (!decLeft){
    Serial.println("here1");
   turnLeft();
   followLine();
  }
  else{
    if(!decFront){
      Serial.println("here2");
      followLine();
    }
    else{
      if (!decRight){
        Serial.println("here3");
        turnRight();
        followLine();
      }
      else{
        Serial.println("here4");
        turnU();
        followLine();
      }
    }
  }
}


bool atCrossRoad(){
  int val_lineS0= analogRead(lineS0);
  int val_lineS5= analogRead(lineS5);
  if((val_lineS0>lineSThres)&&(val_lineS5 >lineSThres)){
    return true; 
  }
  else{
    return false;
  }
}

// Follow Line
void followLine(){  
// read in values from sensors
  int val2= analogRead(lineS2);
  int val3= analogRead(lineS3);

  // if both sensors are on Black 
  if ((val2>550)&&(val3>550)){ 
    // move forward
    servoL.write(80);
    servoR.write(100);
  }
  
  // if s3 is on White
  else if ((val2<550)){ 
    // turn Right
    servoL.write(90);
    servoR.write(100);
//    servoL.write(90);
//    servoR.write(90);
  }
  
  // if s2 is on White
  else if((val3<550)){  
    // turn Left
    servoL.write(80);
    servoR.write(90);
//    servoL.write(90);
//    servoR.write(90);
  }
  
  else{ 
    // stop
    servoL.write(90);
    servoR.write(90);
  } 



  
}


// Turn Left
void turnLeft() {
  servoL.write(90);
  servoR.write(90);
  delay(500);
  servoL.write(60);
  servoR.write(120);
  delay(250);
  servoL.write(0);
  servoR.write(0);
  delay(800);
  servoL.write(90);
  servoR.write(90);
}

// Turn Right
void turnRight() {
    servoL.write(90);
  servoR.write(90);
  delay(500);
  servoL.write(60);
  servoR.write(120);
  delay(250);
  servoL.write(180);
  servoR.write(180);
  delay(800);
  servoL.write(90);
  servoR.write(90);
}

void turnU() {
  servoL.write(90);
  servoR.write(90);
  delay(180);
  servoL.write(0);
  servoR.write(0);
  delay(1600);
  servoL.write(90);
  servoR.write(90);
}




bool detectLeftWall() {
  float wallsensorValue = analogRead(leftWall)*0.0048828125;;
  int distance = 13*pow(wallsensorValue, -1);
  if(distance <= 25){ // wall detected on the left
    return true;
  }
  else{
    return false;
  }
    
}

bool detectFrontWall() {
  float wallsensorValue = analogRead(frontWall)*0.0048828125;;
  int distance = 13*pow(wallsensorValue, -1);
  if(distance <= 20){ // wall detected on the front
    return true;
  }
  else{
    return false;
  } 
}

bool detectRightWall() {
  float wallsensorValue = analogRead(rightWall)*0.0048828125;;
  int distance = 13*pow(wallsensorValue, -1);
  if(distance <= 25){ // wall detected on the right
    return true;
  }
  else{
    return false;
  }
}

