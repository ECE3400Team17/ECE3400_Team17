//Code for the QRE1113 Analog board
//Outputs via the serial terminal – Lower numbers mean more reflected
int QRE1113_Pin = A5; //connected to analog 0

void setup(){
Serial.begin(9600);
}

void loop(){

int QRE_Value = analogRead(QRE1113_Pin);
Serial.println(QRE_Value);
delay(500);

}





//int analogPin = 5;
//int val = 0; 
//
//void setup() {
//  // put your setup code here, to run once:
//  Serial.begin(9600);
//
//}
//
//void loop() {
//  // put your main code here, to run repeatedly:
//
//}



