#include <SoftwareSerial.h>

SoftwareSerial slv(17,16);

char c  = ' ';
int buzzerPin = 3;
char cpast = '0';
void setup() 
{

   pinMode(buzzerPin, OUTPUT);
   Serial.begin(9600);
   Serial2.begin(9600);
  
}
 
 
void loop()
{
   if(Serial2.available())
   {
      Serial.println("Hello2");
      char c = Serial2.read();
      Serial.println(c);
      //if (c=='0') { digitalWrite(LED, LOW); }
      if (cpast==c) {tone(buzzerPin,330);delay(1000); }else{ noTone(buzzerPin);}
      
      cpast = c;
      delay(1000);
   }
   else{
      Serial.println("Hello1");
      tone(3,330);
      delay(1000);    
   }
 
}
