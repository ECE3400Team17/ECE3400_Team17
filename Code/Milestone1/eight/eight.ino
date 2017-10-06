#include <Servo.h>

int senLL = A0; //connected to analog 0
int senLR = A1; //connected to analog 1
int senCL = A2; //connected to analog 2
int senCR = A3; //connected to analog 3
int senRL = A4; //connected to analog 4
int senRR = A5; //connected to analog 5

Servo servoL;
Servo servoR;

void setup(){
  Serial.begin(9600);
  servoL.attach(10); //connected to digital ~10
  servoR.attach(11); //connected to digital ~11
}

int thres = 200;
int count = 0;
int valLL;
int valLR; 
int valCL; 
int valCR; 
int valRL; 
int valRR;

void loop(){
  valLR = analogRead(senLR); 
  valCL = analogRead(senCL);  
  valCR = analogRead(senCR);  
  valRL = analogRead(senRL);  
  valRR = analogRead(senRR);
    
  if((valCL > thres) && (valCR > thres)) // CL=B, CR=B
  {
    
    if((valLL < thres) && (valLR < thres)) // LL=W, LR=W
    {
      
      if((valRL < thres) && (valRR < thres)) // RL=W, RR=W 
      {
        goStraight();
      }
      
      else if((valRL > thres) && (valRR > thres)) // RL=B, RR=B
      {
        turnRight();
      }

      else
      {
        stayPut();
      }
      
    }
    
    else if((valLL > thres) && (valLR > thres)) // LL=B, LR=B
    {
      
      if((valRL < thres) && (valRR < thres)) // RL=W, RR=W 
      {
        turnLeft();
      }

      else if((valRL > thres) && (valRR > thres)) // RL=B, RR=B
      {
        goStraight();
      }

      else
      {
        stayPut();
      }
      
    }

    else // no lines
    {
      stayPut();
    }
  }

  if((valLL > thres) && (valLR > thres) && (valCL > thres) && (valCR > thres) && (valRL > thres) && (valRR > thres)) {
    turn_eight(count);
    count++;
  }


}

void turn_eight(int count) {
  if(count % 8 >=2 && count % 8 <=5) {
    turnRight();
  }
  else {
    turnLeft();
  }
}

void turnLeft(){
  servoL.write(80);
  servoR.write(60);
}

void turnRight(){
  servoL.write(120);
  servoR.write(100);
}

void goStraight(){
  servoL.write(120);
  servoR.write(60);
}

void stayPut(){
  servoL.write(90);
  servoR.write(90);
}
