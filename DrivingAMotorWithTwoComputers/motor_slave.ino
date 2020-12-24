int R1=51;
int R2=25;
char c  = ' ';
int buzzerPin = 3;
char cpast = '0';
unsigned int a=0;
void setup() 
{

  Serial.begin(9600);
    Serial2.begin(9600);
   pinMode(R1,OUTPUT);
    pinMode(R2,OUTPUT);
    a=millis();
   
  digitalWrite(R1,HIGH);
  digitalWrite(R2,HIGH);
}
 
 
void loop()
{
   if(Serial2.available())
   {
     
      char c = Serial2.read();
      delay(500);
      //if (c=='0') { digitalWrite(LED, LOW); }
      if (cpast==c) {
        
          unsigned int b=millis();

          unsigned int tme=(b-a)/1000;
          
          if((tme==5)){
            
            digitalWrite(R1,LOW);
             
            }
          
          
          if((tme==10)){
            
            digitalWrite(R2,LOW);
            
            }
      }else{
        noTone(buzzerPin);
        }
      
      cpast = c;
      delay(2);
   }else{
      unsigned int b=millis();

          unsigned int tme=(b-a)/1000;
          
          if((tme==5)){
            
            digitalWrite(R1,LOW);
             
            }
          
          
          if((tme==10)){
            
            digitalWrite(R2,LOW);
            
            }
 
}
}
