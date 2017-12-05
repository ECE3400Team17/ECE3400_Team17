[To Home Page](./index.md)

# Milestone2

## Objective:
The two main goals of this Milestone was to identify the 3 treasures and all the ability to detect walls.


## Treasure detection:

Based on the FFT library of Arduino, we developed a code that could generate “frequency spectrums” of IR signal inputs of different frequencies.

Using the Excel, we plotted the spectrum, which is essentially a distribution over 128 FFT bins. (There are 256 bins in total, but only 128 of them stores the real value) From the distribution graph we could see that IR signal with different frequencies has different peak value locations:

![](./images/Milestone2/treasureTable.PNG)

![](./images/Milestone2/treasureFFT.PNG)

Based on the distribution, we could set thresholds in between to identify 7, 12, and 17 kHz IR signals. Considering the accuracy of the categorization, we set the threshold in the middle of the two biggest peak bin #:

* The threshold between no-detection and 7kHz is 20.
* The threshold between 7kHz and 12kHz is 64.
* The threshold between 12kHz and 17kHz is 97.

### The code for treasure-detection:
```
#define LOG_OUT 1 // use the log output function
#define FFT_N 256 // set to 256 point fft

#include <FFT.h> // include the library

void setup() {
  Serial.begin(115200); // use the serial port
  TIMSK0 = 0; // turn off timer0 for lower jitter
  ADCSRA = 0xe5; // set the adc to free running mode
  ADMUX = 0x40; // use adc0
  DIDR0 = 0x01; // turn off the digital input for adc0
}

void loop() {
  while(1) { // reduces jitter
    cli();  // UDRE interrupt slows this way down on arduino1.0
    for (int i = 0 ; i < 512 ; i += 2) { // save 256 samples
      while(!(ADCSRA & 0x10)); // wait for adc to be ready
      ADCSRA = 0xf5; // restart adc
      byte m = ADCL; // fetch adc data
      byte j = ADCH;
      int k = (j << 8) | m; // form into an int
      k -= 0x0200; // form into a signed int
      k <<= 6; // form into a 16b signed int
      fft_input[i] = k; // put real data into even bins
      fft_input[i+1] = 0; // set odd bins to 0
    }
    fft_window(); // window the data for better frequency response
    fft_reorder(); // reorder the data before doing the fft
    fft_run(); // process the data in the fft
    fft_mag_log(); // take the output of the fft
    sei();
    
    int maximum = fft_log_out[0];
    int index = 0;
    int N = FFT_N/2;
    int average = maximum/N;
    for (byte i = 1 ; i < N ; i++) { 
      int val = fft_log_out[i]; 
      if(val>= maximum){
        index = i; 
        maximum = val;
      }
      average += val/N; 
    }
    int prom = maximum/average; 
    Serial.println(index);
    if(index <= 20){
      Serial.println("Not detected"); 
    }
    else if(index <= 64){
      Serial.println("7k");
    }
    else if(index <= 97){
      Serial.println("12k");
    }
    else {
      Serial.println("17k");
    }
    delay(100);
  }}
```
### 7kHz Treasure:
![](./images/Milestone2/7k.jpg)

<iframe width="560" height="315" src="https://www.youtube.com/embed/A4SQmpU6chg" frameborder="0" allowfullscreen></iframe>

### 12kHz Treasure:
![](./images/Milestone2/12k.jpg)

<iframe width="560" height="315" src="https://www.youtube.com/embed/9KYcdi5lAQ0" frameborder="0" allowfullscreen></iframe>

### 17kHz Treasure:
![](./images/Milestone2/17k.jpg)

<iframe width="560" height="315" src="https://www.youtube.com/embed/DN0Qly8Bqns" frameborder="0" allowfullscreen></iframe>


## Wall detection:

For the distance sensor, we’re using the VL53L0X Time-of-Flight Distance Sensor. 

### Pinout:
![](./images/Milestone2/pinout.png)


### Wiring:

* SDA - I2C data SDA pin (also A4 for arduino Uno)
* SCL - I2C data SCL pin (also A5 for arduino Uno)
* Vdd - 5V    GND - GND 

![](./images/Milestone2/arduinoLayout.png)

The sensor uses Vertical Cavity Surface-Emitting Laser (VCSEL) to detect the absolute distance between the objects and the sensor. According to the VL53L0X library on adafruit, we could measure and store the distance data through arduino. The following picture shows how we detect the distance between a card and the sensor and print it out:

![](./images/Milestone2/1.jpg)

![](./images/Milestone2/2.jpg)

Since the sensor could generate an absolute distance data, we could just use this data to set a threshold for the robot to detect a wall in front of it.

For our preliminary code, we would attach the sensor in the front of the robot, and would call that the robot detects a wall and turn left when the distance <= 90 mm. 

### Our implementation: 

![](./images/Milestone2/finalPic.jpg)

### The code for wall-detection:
```
#include <Servo.h>
#include "Adafruit_VL53L0X.h"

Adafruit_VL53L0X lox = Adafruit_VL53L0X();


Servo servoL;
Servo servoR;

int s2 = A1;
int s3 = A2;
int s0 = A0;
int s5 = A3;
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
  
  servoL.write(120);
  servoR.write(60);
  
  delay(100);
  stay();
  servoL.write(0);
  servoR.write(0);

  delay(650);

  //servoL.write(90);
  servoR.write(90);
}
/*
void turnRight() {
  stay();
  servoL.write(180);
  servoR.write(180);

  delay(650);

  servoL.write(90);
  servoR.write(90);
}
*/
void stay() {
  servoL.write(90);
  servoR.write(90);
}
```
### Video of the wall detection:
<iframe width="560" height="315" src="https://www.youtube.com/embed/Xuz5DzYh7hI" frameborder="0" allowfullscreen></iframe>



[To Home Page](./index.md)
