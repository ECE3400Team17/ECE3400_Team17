#include <StackArray.h>
#include <QueueArray.h>
#include <Servo.h>


// instantiate the 2 servos
Servo servoL;
Servo servoR;

StackArray<int> trail; 

int robot_x_pos = 1;
int robot_y_pos = 1;
int robot_orient = 0; // 0=North, 1=East, 2=South, 3=West

int robot_corr = 100*robot_y_pos + robot_x_pos;


int brain[9][11]= {
  {1,1,1,1,1,1,1,1,1,1,1},
  {1,1,1,1,1,1,1,1,1,1,1},
  {1,1,1,1,1,1,1,1,1,1,1},
  {1,1,1,1,1,1,1,1,1,1,1},
  {1,1,1,1,1,1,1,1,1,1,1},
  {1,1,1,1,1,1,1,1,1,1,1},
  {1,1,1,1,1,1,1,1,1,1,1},
  {1,1,1,1,1,1,1,1,1,1,1},
  {1,1,1,1,1,1,1,1,1,1,1},  
};


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
  
  brain[robot_y_pos][robot_x_pos] = 2;
  trail.push(robot_corr);

}

void loop() {

}


void dfs(trail,robot_orient,brain){
  
  int tLen = trail.count();
  int currPos = trail.peek(tLen);
  robot_x_pos = currPos % 100;
  robot_y_pos = (currPos - robot_x_pos)/100;
  robot_corr = 100*robot_y_pos + robot_x_pos;
  
  QueueArray <inteighbors = findNeigh();
  while (neighbors.count()>0){
    int nextN = neighbors.dequeue();
    int nextCorr = getNextCorr(nextN,robot_orient, currPos); 
    int nextX = nextCorr % 100;
    int nextY = (nextCorr - nextX)/100;
    if (brain[nextY][nextX]== 1){
      brain[nextY][nextX] = 2;
      trail.push(nextCorr);
      robot_orient = (robot_orient+nextN) % 4;

      // move

      dfs(trail,robot_orient,brain);
//      return;
    }

  }

  if neighbors.count()==0{
    if trial.count()==1{
//      return;
    }
    trial.pop();
    int old = trial.peek();

    int robot_x_posOLD = old % 100;
    int robot_y_posOLD = (old - robot_x_posOLD)/100;

    int diffX = robot_x_posOLD-robot_x_pos;
    int diffY = robot_y_posOLD-robot_y_pos;

    int nextO = backTrack(robot_orient,diffY,diffX);

    robot_orient = (robot_orient+nextO) % 4;

    // move
    dfs(trail,robot_orient,brain);
//    return;
     
    
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
    servoL.write(60);
    servoR.write(120);
  }
  
  // if s3 is on White
  else if ((val3<550)){ 
    // turn Right
    servoL.write(90);
    servoR.write(60);
  }
  
  // if s2 is on White
  else if((val2<550)){  
    // turn Left
    servoL.write(120);
    servoR.write(90);
  }
  
  else{ 
    // stop
    servoL.write(90);
    servoR.write(90);
  } 



  
}


// Turn Left
void turnLeft() {
  servoL.write(60);
  servoR.write(120);
  delay(180);
  servoL.write(0);
  servoR.write(0);
  delay(800);
  servoL.write(90);
  servoR.write(90);
}

// Turn Right
void turnRight() {
  servoL.write(60);
  servoR.write(120);
  delay(180);
  servoL.write(180);
  servoR.write(180);
  delay(800);
  servoL.write(90);
  servoR.write(90);
}

// Detect Neighbors
QueueArray<int>  findNeigh(){
  QueueArray<int> neigh;
  bool cross = atCrossRoad();
  bool decLeft = detectLeftWall();
  bool decFront = detectFrontWall();
  bool decRight = detectRightWall();
    
  if (!cross){
    decLeft = false;
    decRight = false;
  }
  
  if (decLeft){
    findNeigh.enqueue('3');
  }
  
  if (decFront){
    findNeigh.enqueue('0');
  }
  
  if (decRight){
    findNeigh.enqueue('1');
  }
  
  return neigh;
}

bool detectLeftWall() {
  float wallsensorValue = analogRead(leftWall)*0.0048828125;;
  int distance = 13*pow(wallsensorValue, -1);
  if(distance <= 25){ // wall detected on the left
    return false;
  }
  else{
    return true;
  }
    
}

bool detectFrontWall() {
  float wallsensorValue = analogRead(frontWall)*0.0048828125;;
  int distance = 13*pow(wallsensorValue, -1);
  if(distance <= 20){ // wall detected on the front
    return false;
  }
  else{
    return true;
  } 
}

bool detectRightWall() {
  float wallsensorValue = analogRead(rightWall)*0.0048828125;;
  int distance = 13*pow(wallsensorValue, -1);
  if(distance <= 25){ // wall detected on the right
    return false;
  }
  else{
    return true;
  }

int getNextCorr(int side,int orientation, int curr){
  int currX = curr % 100;
  int currY = (curr - currX)/100;
  int nextPos;
  int nY=currY;
  int nX=currX;
  
  if orientation == 0{  
    if side== '3'{
      nX = currX-1; 
    }
    else if side == '0'{
      nY = currY+1;      
    }
    else{
      nX = currX+1;
    }  
  }
  
  else if orientation == 1{
    if side== '3'{
      nY = currY+1;
    }
    else if side == '0'{
      nX = currX+1;      
    }
    else{
      nX = currX+1;      
    }  
  }
  
  else if orientation == 2{
    if side== '3'{
      nX = currX+1
    }
    else if side == '0'{
      nY = currY-1;
    }
    else{
      nX = currX-1;
    }    
  }
  
  else{
    if side== '3'{
      nY = currY-1;
    }
    else if side == '0'{
      nX = currX-1;      
    }
    else{
      nY = currY+1;
    }     
  }

  return 100*nY+nX;
}

int backTrack(int orientation,int diffY, int diffX){
  int nextO;
  if orientation == 0{  
    if diffY==0{
      if diffX==1{
        nextO = 1;
      }
      else{
        nextO = 3;
      }
    }
    else{
      if diffY==1{
        nextO=0;
      }
      else{
        nextO = 2;
      }
    }
  }
  
  else if orientation == 1{
    if diffY==0{
      if diffX==1{
        nextO = 0;
      }
      else{
        nextO = 2;
      }
    }
    else{
      if diffY==1{
        nextO = 3;
      }
      else{
        nextO = 1;
      }
    }
  }
  
  else if orientation == 2{
    if diffY==0{
      if diffX==1{
        nextO = 3;
      }
      else{
        nextO = 1;
      }  
    } 
    else{
      if diffY==1{
        nextO = 2;
      }
      else{
        nextO = 0;
      }
    }
  }
  
  else{
    if diffY=={
      if diffX==1{
        nextO = 2;
      }     
      else{
        nextO = 0;
      }
    }
    else{
      if diffY==1{
        nextO = 1;
      }
      else{
        nextO = 3;
      }
    }
  }
 return nextO;   
}


 
}
