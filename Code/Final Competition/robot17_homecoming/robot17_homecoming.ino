#define LOG_OUT 1 // use the log output function
#define FFT_N 256 // set to 256 point fft
#include <FFT.h> // include the library

#include <Servo.h>
#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"

int again_count = 0;

int binNum = 20; // 660Hz fft bin number
int soundThres = 80; // 660Hz threshold
int audio_sig = 0;

#define TREASURE_PIN 45 // Pin that the treasure circuit is connected too

int D47 = 47; // input from start button
int button_sig = 0;
int button_count = 0;

// Define data.
// pack bits as follows:
// packet 20
// indicator(DONE) = 0 | Orientation (N/S/E/W) | Position (0-19, start is always at 19)
// 1 bit | 2 bits | 5 bits
// packet 19-0 
// treasure(7Hz,12Hz,17Hz)| color | wall(W/E/S/N)
// 2 bit | 2 bits | 4 bits

int homecoming = 0;
int num_visited = 0;
int visited[20]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

unsigned char packet20;
unsigned char packet19; 
unsigned char packet18 = 0; // 00001111
unsigned char packet17 = 0;
unsigned char packet16 = 0;
unsigned char packet15 = 0;
unsigned char packet14 = 0;
unsigned char packet13 = 0;
unsigned char packet12 = 0;
unsigned char packet11 = 0;
unsigned char packet10 = 0;
unsigned char packet9 = 0;
unsigned char packet8 = 0;
unsigned char packet7 = 0;
unsigned char packet6 = 0;
unsigned char packet5 = 0;
unsigned char packet4 = 0;
unsigned char packet3 = 0;
unsigned char packet2 = 0;
unsigned char packet1 = 0;
unsigned char packet0 = 0;

// packet 20
unsigned char ind_done = 0;
unsigned char rob_orien = 0; // always start facing North (N=0/E=1/S=2/W=3)
unsigned char rob_pos = 19; // always start at 19th cell (0-19)

// packet 19
unsigned char treas = 0;
unsigned char tile_clr = 1;
unsigned char wall = 6; // (W_E_S_N)

unsigned char mapPacks[21]= {packet0,packet1,packet2,packet3,packet4,packet5,packet6,packet7,packet8,packet9,packet10,
                            packet11,packet12,packet13,packet14,packet15,packet16,packet17,packet18,packet19,packet20}; 

// instantiate the 2 servos
Servo servoL;
Servo servoR;
RF24 radio(7,8);

// Radio pipe addresses for the 2 nodes to communicate.
const uint64_t pipes[2] = { 0x000000003ALL, 0x000000003BLL };

// The various roles supported by this sketch
typedef enum { role_ping_out = 1, role_pong_back } role_e;

// The debug-friendly names of those roles
const char* role_friendly_name[] = { "invalid", "Ping out", "Pong back"};

// The role of the current running sketch
role_e role = role_ping_out;

int lineS0 = A8; // Line Sensor0 (Left)
int lineS2 = A1; // Line Sensor1 (Center Left)
int lineS3 = A2; // Line Sensor2 (Center Right)
int lineS5 = A3; // Line Sensor3 (Right)
int lineSThres = 500;

int leftWall = A6;// Left Wall Sensor
int frontWall = A4;// Front Wall Sensor
int rightWall = A5;// Right Wall Sensor
int backWall = A7; // Back Wall Sensor

bool decLeft;
bool decFront;
bool decRight;
bool decBack;

bool wallN;
bool wallS;
bool wallE;
bool wallW;

// **************************************************************************
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  pinMode(TREASURE_PIN, INPUT); // make the treasure pin an input
  
  pinMode(D47,INPUT); // input from button
  
//  servoL.attach(10);
//  servoR.attach(11);
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

// ******************** MAIN ******************** 
void loop() {
  if((button_sig==0)&&(audio_sig == 0 )){
    button_sig = digitalRead(D47);
    if (button_sig==1){
      audio_sig = 1;
      button_count = button_count +1; 
    }
    if (audio_sig == 0 && button_sig==0 ){
      startTone();
    }
  }
  
  if((button_count>0)||audio_sig>0){
//    Serial.println("Start");
    servoL.attach(10);
    servoR.attach(11);
    bool cross = atCrossRoad();
    if (ind_done){
      servoL.write(90);
      servoR.write(90);
      delay(500);
    }
    else if (cross){
      servoL.write(90);
      servoR.write(90);
      delay(500);
      transmitSingle();
      decisionAtCross();
    }
    else{
      followLine();
    }
  }
  else{
    servoL.detach();
    servoR.detach();
//    Serial.println("stay");
//    stay();
  }

}
// ********************************************** 



// -------------
// transmit()
// -------------
void transmit() {
  radio.stopListening();
  updatePack();
  int i;
  for (i = 0; i < 21; i = i + 1) {
    Serial.println("Counter:");
    Serial.println(i);
    Serial.println("Packet:");
    Serial.println(mapPacks[i]);
    Serial.println("Robot Pos:");
    Serial.println(rob_pos);
    
    bool tx_OK = radio.write(&mapPacks[i], sizeof(unsigned char) );
    while (!tx_OK){
      Serial.println("Counter:");
      Serial.println(i);
      Serial.println("Packet:");
      Serial.println(mapPacks[i]);
    
      tx_OK = radio.write(&mapPacks[i], sizeof(unsigned char) );
    }
  }
  delay(1000); 
  // Now, continue listening
  radio.startListening();
}


// ------------------
// transmitSingle()
// ------------------
void transmitSingle() {
  radio.stopListening();
  updatePack();

  Serial.println("Robot Pos:");
  Serial.println(rob_pos);

  Serial.println("Robot Orien:");
  Serial.println(rob_orien);

  bool tx_OK = radio.write(&mapPacks[20], sizeof(unsigned char) );
  Serial.println("Packet:");
  Serial.println("20");
  while (!tx_OK && again_count<5){
    again_count = again_count +1;
    Serial.println("Packet Again:");
    Serial.println("20");
    tx_OK = radio.write(&mapPacks[20], sizeof(unsigned char) );
  }
  again_count = 0;  
  tx_OK = radio.write(&mapPacks[rob_pos], sizeof(unsigned char) );
  while (!tx_OK && again_count<5){
    again_count = again_count +1;
    Serial.println("Packet Again:");
    Serial.println(rob_pos);
    tx_OK = radio.write(&mapPacks[rob_pos], sizeof(unsigned char) );
  }
  delay(1000); 
  // Now, continue listening
  radio.startListening();
}


// ------------------------------------------------
// updatePack()
// Update the 168bit packet with new information
// ------------------------------------------------
void updatePack(){
  getEnvir(); // update wall information
  getTreas(); //  update treasure information
  rob_pos = rob_pos%21;
  if (rob_pos = 19){
    homecoming = homecoming +1;
  }
  visited[rob_pos]=visited[rob_pos]+1; // updated visited with new position
  checkDone(); // check for done signal
  mapPacks[20]= ind_done<<7 | rob_orien<<5 | rob_pos;
  mapPacks[rob_pos]= treas<<6 | tile_clr<<4 | wall;
  treas=0;
  wall =0;
}

// -------------------------
// checkDone()
// update for done signal
// -------------------------
void checkDone(){
  ind_done = visited[0];
  int indexV = 1; 
  while ((ind_done > 0 && indexV < 20)){
    ind_done = ind_done * visited[indexV]; 
    indexV = indexV + 1;
  }
  if (homecoming >=2){
    ind_done = 1;
  }
}

// -------------------------
// decisionAtCross()
// -------------------------
void decisionAtCross(){
  decLeft = detectLeftWall();
  decFront = detectFrontWall();
  decRight = detectRightWall();
  if (!decLeft){
   turnLeft();
   followLine();
  }
  else{
    if(!decFront){
      moveFront();
      followLine();
    }
    else{
      if (!decRight){
        turnRight();
        followLine();
      }
      else{
        turnBack();
        followLine();
      }
    }
  }

}

// -------------------------
// atCrossRoad()
// -------------------------
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

// -------------------------
// followLine()
// -------------------------
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
  }  
  // if s2 is on White
  else if((val3<550)){  
    // turn Left
    servoL.write(80);
    servoR.write(90);
  }
  else{ 
    // stop
    servoL.write(90);
    servoR.write(90);
  }   
}


// ---------------------
// stay()
// robot does not move
// ---------------------
void stay(){
    servoL.write(90);
    servoR.write(90);
//    delay(500);
}



// -----------------------------------------------------------------
// moveFront()
// update to new position, update to new orientation, Move Forward
// -----------------------------------------------------------------
void moveFront(){
  // update robot position
  if (rob_orien == 0){
    rob_pos = rob_pos - 4;
  }
  else if (rob_orien == 1){
    rob_pos = rob_pos + 1;
  }
  else if (rob_orien == 2){
    rob_pos = rob_pos + 4;
  }
  else{
    rob_pos = rob_pos - 1;
  }
  rob_orien = (rob_orien+0)%4;  // update orientation 
  servoL.write(80);
  servoR.write(100);
  delay(200);
}

// -----------------------------------------------------------------
// turnLeft()
// update to new position, update to new orientation, Turn Left
// -----------------------------------------------------------------
void turnLeft() {
  // update robot position
  if (rob_orien == 0){
    rob_pos = rob_pos - 1;
  }
  else if (rob_orien == 1){
    rob_pos = rob_pos - 4;
  }
  else if (rob_orien == 2){
    rob_pos = rob_pos + 1;
  }
  else{
    rob_pos = rob_pos + 4;
  }
  rob_orien = (rob_orien+3)%4; // update orientation 
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

// -----------------------------------------------------------------
// turnRight()
// update to new position, update to new orientation, Turn Right
// -----------------------------------------------------------------
void turnRight() {
  // update robot position
  if (rob_orien == 0){
    rob_pos = rob_pos + 1;
  }
  else if (rob_orien == 1){
    rob_pos = rob_pos + 4;
  }
  else if (rob_orien == 2){
    rob_pos = rob_pos - 1;
  }
  else{
    rob_pos = rob_pos - 4;
  }
  rob_orien = (rob_orien+1)%4; // update orientation
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


// ---------------------------------------------------------------------
// turnBack
// update to new position, update to new orientation, Turn Back (U-turn)
// ---------------------------------------------------------------------
void turnBack(){
  // update robot position
  if (rob_orien == 0){
    rob_pos = rob_pos + 4;
  }
  else if (rob_orien == 1){
    rob_pos = rob_pos - 1;
  }
  else if (rob_orien == 2){
    rob_pos = rob_pos - 4;
  }
  else{
    rob_pos = rob_pos + 1;
  }
  rob_orien = (rob_orien+2)%4;  // update orientation
  servoL.write(90);
  servoR.write(90);
  delay(180);
  servoL.write(0);
  servoR.write(0);
  delay(1600);
  servoL.write(90);
  servoR.write(90);
}

// -------------------
// detectLeftWall()
// -------------------
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

// -------------------
// detectFrontWall()
// -------------------
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

// -------------------
// detectRightWall()
// -------------------
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

// -------------------
// detectBackWall()
// -------------------
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

// ----------------------------
// getEnvir()
// updates surrounding walls 
// ----------------------------
void getEnvir(){
  decLeft = detectLeftWall();
  decFront = detectFrontWall();
  decRight = detectRightWall();
  decBack = detectBackWall();

  if (rob_orien == 0){
    wallN = decFront;
    wallS = decBack;
    wallE = decRight;
    wallW = decLeft;
  }
  else if (rob_orien == 1){
    wallN = decLeft;
    wallS = decRight;
    wallE = decFront;
    wallW = decBack;
  }
  else if (rob_orien == 2){
    wallN = decBack;
    wallS = decFront;
    wallE = decLeft;
    wallW = decRight;
  }
  else{
    wallN = decRight;
    wallS = decLeft;
    wallE = decBack;
    wallW = decFront;
  }  
  wall = (8*wallW)+(4*wallE)+(2*wallS)+(wallN);
}

// ----------------------------
// testWalls()
// for debugging
// ----------------------------
void testWalls(){
  decLeft = detectLeftWall();
  decFront = detectFrontWall();
  decRight = detectRightWall();
  decBack = detectBackWall();
  Serial.println("Left");
  Serial.println(decLeft);
  Serial.println("Front");
  Serial.println(decFront);
  Serial.println("Right");
  Serial.println(decRight);
  Serial.println("Back");
  Serial.println(decBack);
  delay(1000); 
}


// ----------------------------
// treasure_frequency()
// 
// ----------------------------
float treasure_frequency(){
  // measure both high and low pulses to get the actual pulse width
  unsigned long ph = pulseIn(TREASURE_PIN,HIGH);
  unsigned long pl = pulseIn(TREASURE_PIN,LOW);

  // collect a total of four periods
  ph += pulseIn(TREASURE_PIN,HIGH);
  pl += pulseIn(TREASURE_PIN,LOW);
  ph += pulseIn(TREASURE_PIN,HIGH);
  pl += pulseIn(TREASURE_PIN,LOW);
  ph += pulseIn(TREASURE_PIN,HIGH);
  pl += pulseIn(TREASURE_PIN,LOW);

  // divide the count by four, the number of samples collected
  ph = ph>>2;
  pl = pl>>2;

  // the sum is the period and each integer value is 1us thus the 1e-6
  float period = (ph+pl)*1.0e-6;

  // frequency is one over period
  float frequency = 1.0/period;

  return frequency;
}

// ----------------------------
// getTreas()
// Identifies the treasure
// ----------------------------
void getTreas(){
  float ave_treas = treasure_frequency();
  if ((7000 <= ave_treas)&&(ave_treas<=9500)){
    // Red Treasure
    Serial.println("Red Treasure");
    treas = 1 ; 
  }
  else if ((12000 <= ave_treas)&&(ave_treas<=14500)){
    // Green Treasure
    Serial.println("Green Treasure");
    treas = 2 ; 
  }
  else if ((17000 <= ave_treas)&&(ave_treas<=19500)){
    // Blue Treasure
    Serial.println("Blue Treasure");
    treas = 3 ;
  }
  else {
    // No Treasure
    treas = 0; 
  }
}

// ----------------------------
// startTone()
// Waits for start tone
// ----------------------------
void startTone(){
  while(audio_sig==0) {
    cli();
    for (int i = 0 ; i < 512 ; i += 2) {
      fft_input[i] = analogRead(A9); // <-- NOTE THIS LINE
      fft_input[i+1] = 0;
    }
    fft_window();
    fft_reorder();
    fft_run();
    fft_mag_log();
    sei();
//  Serial.println("start");
    for (byte i = 0 ; (i < FFT_N/2)&&(audio_sig == 0) ; i++) {
//    Serial.println(fft_log_out[i]);
      if (fft_log_out[binNum]> soundThres){
        Serial.println("here");
        audio_sig = 1;
        button_sig=1;
      }
      else{
        button_sig = digitalRead(D47);
        if (button_sig==1){
          audio_sig = 1;
          button_count = button_count +1; 
        }
      }
    }
  }
}





