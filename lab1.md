[To Home Page](./index.md)

# Lab 1: Microcontroller
### Objective: 
The objective of this lab was to familiarize ourselves with the Arduino Uno board and IDE. We utilized digital and analog outputs, digital outputs, and writing analog PWM outputs.

### Equipment:
* 1 Arduino Uno
* 1 solderless breadboard
* 1 USB A/B cable
* 1 LED 
* 2-3 resistors (kΩ range)
* 1 potentiometer
* 1 continuous rotation servo
* 1 pushbutton

## Part 1: Communicating between the Uno and IDE
To learn about the input/output functionalities of the Uno board, we modified sample code from the blink program provided by Arduino to make an LED blink on and off every second.

```
void setup() {
    pinMode(LED_BUILTIN, OUTPUT);  // set signal to built-in LED
}

void loop() {
  digitalWrite(LED_BUILTIN, HIGH);  //toggle on
  delay(1000);                       
  digitalWrite(LED_BUILTIN, LOW);   //toggle off  
  delay(1000);                       
}
```
The code compiled and was sent to the Arduino board, which began to blink an LED embedded on the board as desired. As shown below:

<iframe width="560" height="315" src="https://www.youtube.com/embed/ttuvQ9fFO5o" frameborder="0" allowfullscreen></iframe>

## Part 2: Modify the Blink Sketch
Utilizing the board’s digital writing capabilities, we set up an LED to digital output pin 12 on the Arduino board. Then we modified the code to send a signal to the LED. 

```
void setup() {
    pinMode(12, OUTPUT);  // set LED connection to pin 12
}

void loop() {
  digitalWrite(12, HIGH);	// toggle on
  delay(1000);                       
  digitalWrite(12, LOW);    // toggle off
  delay(1000);                       
}
```
The blue LED repeatedly toggled on and off with a time interval of one second in between. Here is a video of the LED turning on and off:

<iframe width="560" height="315" src="https://www.youtube.com/embed/WlTisecNfO4" frameborder="0" allowfullscreen></iframe>

## Part 3: The Serial Monitor and the Analog Pins
![](./images/Lab1/pot.jpg)

## Part 4: Analog Output
![](./images/Lab1/pot_LED.jpg)

```
int ledPin = 6;      // LED connected to digital pin 6
int analogPin = 5;   // potentiometer connected to analog pin 5

int val = 0;         // variable to store the read value

void setup()
{
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);   // sets the pin as output
}

void loop()
{
  val = analogRead(analogPin);   // read the input pin
  Serial.println(val);
  analogWrite(ledPin, val / 4);  // analogRead values go from 0 to 1023, analogWrite values from 0 to 255
  delay(1000);
}
```

## Part 5: Parallax Servos







## Part 6: Assemble and Run Your Robot
Our assembled robot:

![](./images/Lab1/assembled_bot.jpg)

We wrote code to have out robot move back and forth autonomously: 

```
#include <Servo.h>
Servo servoL;
Servo servoR;

void setup() {
  Serial.begin(9600);
  servoL.attach(10); 
  servoR.attach(11);
}

void loop() {
  // move forward for 2 seconds
  servoL.write(60); 
  servoR.write(120);
  delay(2000);
  
  // pause for 1 second
  servoL.detach(); 
  servoR.detach();
  delay(1000);
  
  // move backward for 2 seconds
  servoL.attach(10); 
  servoR.attach(11);
  servoL.write(120);
  servoR.write(60);
  delay(2000);
  
  // pause for 1 second
  servoL.detach(); 
  servoR.detach();
  delay(1000);
  servoL.attach(10); 
  servoR.attach(11);
}
```

Watch our robot move back and forth:

<iframe width="560" height="315" src="https://www.youtube.com/embed/Do-kx_vWezc" frameborder="0" allowfullscreen></iframe>

[To Home Page](./index.md)



