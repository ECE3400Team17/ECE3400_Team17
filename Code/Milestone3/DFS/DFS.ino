int lineS0 = A0; // Line Sensor0 (Left)

int lineS1 = A1; // Line Sensor1 (Center Left)

int lineS2 = A2; // Line Sensor2 (Center Right)

int lineS3 = A3; // Line Sensor3 (Right)

int lineSThres = 500;

// Left Wall Sensor

// Front Wall Sensor

// Right Wall Sensor







void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:

}


// Follow Line
void followLine(){
  val_lineS0= analogRead(s0);
  val_lineS1= analogRead(s1);
  val_lineS2= analogRead(s2);
  val_lineS3= analogRead(s3);

   if ((val_lineS1>lineSThres)&&(val_lineS2>lineSThres)){
    servoL.write(120);
    servoR.write(60);
  }
  else if((val_lineS2<lineSThres)){
    servoL.write(90);
    servoR.write(60);
  }
  else if((val_lineS1<lineSThres)){
    servoL.write(120);
    servoR.write(90);
  }
  else{
    servoL.write(90);
    servoR.write(90);
  }
}


// Turn Left

// Turn Right
