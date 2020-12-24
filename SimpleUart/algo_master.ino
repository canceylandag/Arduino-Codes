#include <SoftwareSerial.h>

SoftwareSerial mst(16,17);
void setup() 
{
    Serial.begin(9600);
    
    Serial2.begin(9600);
   
    // wait for the serial port to connect. Required for Leonardo/Micro native USB port only
    while (!mst) {  ;  }
}
 
 
void loop() 
{
  Serial2.write('1');
  Serial.println("1");
  delay(1000);
  Serial2.write('0');
  Serial.println("0");
  delay(1000);
  
}
