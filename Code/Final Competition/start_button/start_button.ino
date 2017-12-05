int d=2;   // to store on or off value
int button_count = 0; 

void setup(){
  Serial.begin(115200);
  pinMode(2,INPUT);
pinMode(13,OUTPUT);

}
void loop()
{
d=digitalRead(2);

if (d==1){
  button_count = button_count +1; 
}

if(button_count>0)
{digitalWrite(13,HIGH);
Serial.println("here1");}
else
{digitalWrite(13,LOW);
Serial.println("here2");}
delay(500);
}

