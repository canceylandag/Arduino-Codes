int R1=47;
int buzzer=3;
int R2=25;
unsigned int a=0;
void setup() 
{
    Serial.begin(9600);
//    Serial2.begin(9600);
   pinMode(R1,OUTPUT);
    pinMode(3,OUTPUT);
    pinMode(R2,OUTPUT);
    a=millis();
  digitalWrite(R1,HIGH);
  digitalWrite(R2,HIGH);
   noTone(3); 
}
 
 
void loop() 
{
  Serial2.write('1');
  delay(500);
  Serial2.write('0');
  delay(500);

unsigned int b=millis();

unsigned int tme=(b-a)/1000;
 
 
 
if(tme==5){
  
  digitalWrite(R1,LOW);
  }
int moon=digitalRead(R1);

if((tme==10)){
  
  digitalWrite(R2,LOW);
  
  }
  
  
}
