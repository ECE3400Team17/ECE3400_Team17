#include <Servo.h>
#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"

// instantiate the 2 servos
Servo servoL;
Servo servoR;
RF24 radio(7,8);

// Radio pipe addresses for the 2 nodes to communicate.
const uint64_t pipes[2] = { 0x000000003ALL, 0x000000003BLL };
//
// Role management
//
// Set up role.  This sketch uses the same software for all the nodes
// in this system.  Doing so greatly simplifies testing.
//

// The various roles supported by this sketch
typedef enum { role_ping_out = 1, role_pong_back } role_e;

// The debug-friendly names of those roles
const char* role_friendly_name[] = { "invalid", "Ping out", "Pong back"};

// The role of the current running sketch
role_e role = role_ping_out;


int lineS0 = A0; // Line Sensor0 (Left)
int lineS2 = A1; // Line Sensor1 (Center Left)
int lineS3 = A2; // Line Sensor2 (Center Right)
int lineS5 = A3; // Line Sensor3 (Right)
int lineSThres = 500;

int leftWall = A6;// Left Wall Sensor
int frontWall = A4;// Front Wall Sensor
int rightWall = A5;// Right Wall Sensor
int backWall = A7; // Back Wall Sensor

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  servoL.attach(10);
  servoR.attach(11);
  radio.begin();

  // optionally, increase the delay between retries & # of retries
  radio.setRetries(15,15);
  radio.setAutoAck(true);
  // set the channel
  radio.setChannel(0x50);
  // set the power
  // RF24_PA_MIN=-18dBm, RF24_PA_LOW=-12dBm, RF24_PA_MED=-6dBM, and RF24_PA_HIGH=0dBm.
  radio.setPALevel(RF24_PA_MIN);
  //RF24_250KBPS for 250kbs, RF24_1MBPS for 1Mbps, or RF24_2MBPS for 2Mbps
  radio.setDataRate(RF24_250KBPS);

  radio.openWritingPipe(pipes[0]);
  radio.openReadingPipe(1,pipes[1]);

  radio.startListening();
}

void loop() {

  bool decLeft = detectBackWall();
  Serial.println(decLeft);
  

//  turnRight();
//  servoL.write(80);
//  servoR.write(100);
//  delay(500);
  
//  bool cross = atCrossRoad();
//  if (cross){
//    servoL.write(90);
//    servoR.write(90);
//    delay(500);
//    transmit();
//    decisionAtCross();
//  }
//  else{
//    followLine();
//  }

//followLine();
  

}

void transmit() {
  radio.stopListening();
      // Define data.
      unsigned char packet1;
      unsigned char packet2;
      // pack bits as follows:
      
      // packet 1
      // indicator = 0 | direction (N/S/E/W) | long side of maze (y-coord)
      // 1 bit | 2 bits | 5 bits

      // packet 2 
      // indicator = 1| presence of wall (L/F/W) | short side of maze (x-coord)
      // 1 bit | 3 bits | 4 bits

      // assigning chars??

      // example packet 1: ind = 0, 
      unsigned char ind_1 = 0;
      unsigned char dir = 0;
      unsigned char pos_y = 0;

      unsigned char ind_2 = 1;
      unsigned char wall = 0;
      unsigned char pos_x = 0;
    
      // shift bits in order to pack bits, then or them together
      packet1 = ind_1<<7 | dir<<5 | pos_y;
      packet2 = ind_2<<7 | wall<<4 | pos_x;
    
      // Send maze in single payload
      printf("Now sending packet 1!\n");
      bool ok1 = radio.write(&packet1, sizeof(unsigned char) );
      
    
      if (ok1)
        printf("ok\n");
      else
        printf("failed.\n\r");

    // delay(1000); //wait before sending packet 2 (we can shorten this)

      printf("Now sending packet 2!\n");
      bool ok2 = radio.write(&packet2, sizeof(unsigned char) );

      if (ok2)
        printf("ok\n");
      else
        printf("failed.\n\r");

         delay(1000); 
    
      // Now, continue listening
      radio.startListening();
}

//unsigned char parsing()

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
      servoL.write(80);
      servoR.write(100);
      delay(200);
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

bool detectBackWall() {
  float wallsensorValue = analogRead(backWall)*0.0048828125;;
  int distance = 13*pow(wallsensorValue, -1);
  if(distance <= 20){ // wall detected on the front
    return true;
  }
  else{
    return false;
  } 
}
