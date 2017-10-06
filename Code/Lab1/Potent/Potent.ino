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
