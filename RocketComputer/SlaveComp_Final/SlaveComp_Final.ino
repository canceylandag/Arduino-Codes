#include <SoftwareSerial.h>
#include <MPU9255.h>
#include <TinyGPS.h>
#include <stdlib.h>
#include <XBee.h>
#include <Wire.h>
#include <Arduino.h>
#include "I2Cdev.h" 
#include "SparkFunMPL3115A2.h"

MPL3115A2 myPressure;
MPU9255 mpu;
TinyGPS gps;
SoftwareSerial ss(10,11);


 SoftwareSerial aa(2,22);


XBee xbee = XBee();


  int R1=51;  
  int R2=25; 

char cc  = ' ';
int buzzerPin = 3;
char cpast = '0';
static const int RXPin = 10, TXPin = 11;
static const uint32_t GPSBaud = 9600;
float val1;
float alltitude;  
float enl;
float lon;
int sat;
int baslama=0;
int teksay=0;
int mot1=0;
int act=1;
  int app=0;
  int apps=0;
  int krt=0;
  
  float alltitude2=0;
  unsigned int sabit=0; //timerın 
  double alltitude_max=1350;  
 typedef struct XBeeStruct {
    char id='1';
    char spc2=0x20;
    char sa[2];

    char e[10];

    char b[10];
    char apc3=' ';
    char ang[4];

    char p[8];
  } XBeeDataStruct;

static XBeeDataStruct XBeeData;
int chck=0;
int chck2=0; 
// create the XBee object



// SH + SL Address of receiving XBee
XBeeAddress64 addr64 = XBeeAddress64(0x0013A200, 0x4176017B);
ZBTxRequest zbTx = ZBTxRequest(addr64, (char*)&XBeeData, sizeof(XBeeDataStruct));
ZBTxStatusResponse txStatus = ZBTxStatusResponse();

void verisend(){
  
  float flat, flon; //enlem boylam
  unsigned long age; // o age hakkında canında bir fikri yok, kütphanenin içindeydi canında fikri yok.
  gps.f_get_position(&flat, &flon, &age); //enlem boylamı alıyorsun
     
    if (flat == TinyGPS::GPS_INVALID_F_ANGLE){
      dtostrf(00.000000,10,6,XBeeData.e); 
   }
  
    else {
      dtostrf(flat,10,6,XBeeData.e);
  }
    
     
     if (flon == TinyGPS::GPS_INVALID_F_ANGLE){
       dtostrf(00.000000,10,6,XBeeData.b);    
   }
     else {
      dtostrf(flon,10,6,XBeeData.b);
   }
    
    
    if (gps.satellites() == TinyGPS::GPS_INVALID_SATELLITES){
     XBeeData.sa[0]='0';
     XBeeData.sa[1]='0';
  }
    else{
    int a=gps.satellites();
    itoa (a,XBeeData.sa,10);
  }
  
float  pressure_abs = myPressure.readAltitude();
  mpu.read_acc();
  val1=(mpu.ax/4096.0);
  itoa (val1,XBeeData.ang,10);
  dtostrf(pressure_abs,8,1,XBeeData.p); 
  
  xbee.send(zbTx);
  
  }

void setup() {
  
  Wire.begin(); 
  Serial.begin(9600);
  Serial2.begin(9600);
  ss.begin(GPSBaud);
  xbee.setSerial(Serial);

  pinMode(R1,OUTPUT);
  pinMode(R2,OUTPUT);
  
  digitalWrite(R1,HIGH);
  digitalWrite(R2,HIGH);
  
  pinMode(39,OUTPUT); //yeşil led
  pinMode(41,OUTPUT); // kırmızı led
  
  myPressure.begin();//Sensör tanımlama.
  myPressure.setModeAltimeter(); // Measure altitude above sea level in meters
  myPressure.setOversampleRate(3); // Set Oversample to the recommended 128
  myPressure.enableEventFlags(); // Enable all three pressure and temp event flags 
 
 
  xbee.setSerial(Serial);
    
  
}

    
    
    

void loop() {   
  if(Serial2.available())
  {
     char c = Serial2.read();
     delay(500);
     Serial.println(c);
        if(cpast==c) 
        {
           while(act==1)
           {
                 verisend();
              
                 mpu.read_acc();
                 alltitude = myPressure.readAltitude();
 
 
                  if((abs((mpu.ax/4096.0))>1)||(alltitude>6))
                  { //degerler düzeltilecek
                       act=0;
                       app=1;
                       sabit =millis();
                       digitalWrite(39,LOW);
                       digitalWrite(41,HIGH); 
                       
                   }
                 
  
              }
        
              while(app==1){
                 verisend();
                 
                 unsigned int degisken =millis(); // zamanlayıcı sürekli ölçüm 
                 int  tme=(degisken-sabit)/1000;

                 alltitude = myPressure.readAltitude();
                 delay(500);
                 alltitude2 = myPressure.readAltitude();
                 


                 if(alltitude>alltitude2){ //can üzerine düşünecek ben de düşüne bilirim
                    digitalWrite(R1,LOW);
                    delay(200);
                    chck=digitalRead(R1);
                         
                       if(chck==LOW){
                          app=0;
                           apps=1;
                          delay(5000);
                          digitalWrite(R1,HIGH);
                          
                          
                       }
                     
    
                }

                else if(tme>=19){
                   digitalWrite(R1,LOW);
                   delay(200);
                   chck=digitalRead(R1);
                   
                   
                   if(chck==LOW){
                      app=0;
                      apps=1;
                      delay(5000);
                      digitalWrite(R1,HIGH);
                      
                     
                  }
                   
                   
               }       
   
             chck=digitalRead(R1);
        }

        
  int  tim1=millis();
  float apogeeAltitude= myPressure.readAltitude();
  while(apps==1){
    verisend();
    unsigned int degisken =millis(); // zamanlayıcı sürekli ölçüm 
    int tme=(degisken-sabit)/1000;
    unsigned int tim2=millis();
    int timeee=(tim2-tim1)/1000;
    alltitude=myPressure.readAltitude();
   
    mpu.read_acc();
  
    if((alltitude==580)){
      digitalWrite(R2,LOW);
      delay(5000);
      digitalWrite(R2,HIGH);
      apps=0;
      krt=1; 
    }
    
     if((timeee>6)&&((mpu.ax/4096.0)>1)){  
      digitalWrite(R2,LOW);
      chck2=digitalRead(R2);
      delay(5);
      if(chck2==LOW){
      apps=0;
      krt=1; 
      delay(5000);
      digitalWrite(R2,HIGH);           
      }
      if((tme>69)&&(chck2!=LOW))
              {
               digitalWrite(R2,LOW);
               delay(5000);
               digitalWrite(R2,HIGH);
               }
      
    }
      
 
  }

    while(krt==1){
      
        verisend();
    }
     
  }
  
  else if(c=='3'){
   digitalWrite(R1,LOW);
   delay(200);
   int chck=digitalRead(R1);
    
    if(chck==LOW){
      delay(5000);
      digitalWrite(R1,HIGH);
    }
   }
   else if(c=='4'){
      digitalWrite(R2,LOW);
      delay(200);
      int  chck2=digitalRead(R2);
      
        
        if(chck2==LOW){
        delay(5000);
        digitalWrite(R2,HIGH);        
        }
     }
                 
                           
  else                     
  {
    tone(3,330);
    delay(500);
    noTone(3);
  }
  
  }
  
  else{
    while(act==1){
                 verisend();
              
                 mpu.read_acc();
                 alltitude = myPressure.readAltitude();
 
 
                  if(abs(((mpu.ax/4096.0))>1)||(alltitude>6)){ //degerler düzeltilecek
                       act=0;
                       app=1;
                       sabit =millis();
                       digitalWrite(39,LOW);
                       digitalWrite(41,HIGH); 
                       
                   }
                 
  
              }
  
              while(app==1){
                 verisend();
                 
                 unsigned int degisken =millis(); // zamanlayıcı sürekli ölçüm 
                 int  tme=(degisken-sabit)/1000;

                 alltitude = myPressure.readAltitude();
                 delay(500);
                 alltitude2 = myPressure.readAltitude();
                 


                 if(alltitude>alltitude2){ //can üzerine düşünecek ben de düşüne bilirim
                    digitalWrite(R1,LOW);
                    delay(200);
                    chck=digitalRead(R1);
                         
                       if(chck==LOW){
                          app=0;
                           apps=1;
                          delay(5000);
                          digitalWrite(R1,HIGH);
                          
                          
                       }
                     
    
                }

                else if(tme>=19){
                   digitalWrite(R1,LOW);
                   delay(200);
                    chck=digitalRead(R1);
                   
                   
                   if(chck==LOW){
                      app=0;
                      apps=1;
                      delay(5000);
                      digitalWrite(R1,HIGH);
                      
                     
                  }
                   
                   
               }       
   
             
        }

        
  int  tim1=millis();
  float apogeeAltitude= myPressure.readAltitude();
  while(apps==1){
    verisend();
    unsigned int degisken =millis(); // zamanlayıcı sürekli ölçüm 
    int tme=(degisken-sabit)/1000;
    unsigned int tim2=millis();
    int timeee=(tim2-tim1)/1000;
    alltitude=myPressure.readAltitude();
   
    mpu.read_acc();
  
    if((alltitude==580)){
      digitalWrite(R2,LOW);
      delay(5000);
      digitalWrite(R2,HIGH);
      apps=0;
      krt=1; 
    }
    
     if((timeee>6)&&((mpu.ax/4096.0)>2)){  
      digitalWrite(R2,LOW);
      chck2=digitalRead(R2);
      delay(5);
      if(chck2==LOW){
      apps=0;
      krt=1; 
      delay(5000);
      digitalWrite(R2,HIGH);           
      }
      if((tme>69)&&(chck2!=LOW))
              {
               digitalWrite(R2,LOW);
               delay(5000);
               digitalWrite(R2,HIGH);
               }
      
    }
      
 
  }

    while(krt==1){
      
        verisend();
    }
  
}}
