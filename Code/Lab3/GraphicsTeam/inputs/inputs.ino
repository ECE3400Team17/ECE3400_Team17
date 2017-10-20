int squX=12;
int squY=13;


void setup()
{
  pinMode(squX, OUTPUT);
  pinMode(squY, OUTPUT);
}

void loop()
{
  digitalWrite(squX, HIGH);  // analogRead values go from 0 to 1023, analogWrite values from 0 to 255
  digitalWrite(squY, HIGH);

}



//void setup() {
//  pinMode(squX, OUTPUT);
//  pinMode(squY, OUTPUT);
//
//}
//
//void loop() {
//  digitalWrite(squX, LOW);   
//  digitalWrite(squY, LOW); 
//  delay(1000);  
//  digitalWrite(squX, HIGH);   
//  digitalWrite(squY, LOW); 
//  delay(1000); 
//  digitalWrite(squX, LOW);   
//  digitalWrite(squY, HIGH); 
//  delay(1000); 
//  digitalWrite(squX, HIGH);   
//  digitalWrite(squY, HIGH); 
//  delay(1000);  
//}



