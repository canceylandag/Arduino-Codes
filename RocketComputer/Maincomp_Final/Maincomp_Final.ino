#include <SoftwareSerial.h>
#include <Wire.h>
#include <SparkFun_MS5803_I2C.h>
#include <TinyGPS.h>
#include <stdlib.h>
#include <XBee.h>
#include <Arduino.h>
#include "I2Cdev.h" 
#include <MPU6050_tockn.h>

TinyGPS gps;  /*Gps nesnesi ve bağlanacağı seri port.                                                         */
SoftwareSerial ss(10,11); //gps i tanımladığın serialpin
  
   MPU6050 mpu6050(Wire);
   MS5803 sensor(ADDRESS_HIGH); //basınç sensörüyle ilgili



XBee xbee = XBee();


  int tme=0;
  static const int RXPin = 10, TXPin = 11; //serial pinler gps
  static const uint32_t GPSBaud = 9600;
  double pressure_baseline,pressure_abs; 
  double alltitude_max=1350;
  float enl; //enlem
  float lon; //boylam
  
  int act=1;
  int app=0;
  int apps=0;
  int krt=0;
  float alltitude=0;
  float alltitude2=0;

unsigned int tim1=0;
  int16_t ax, ay, az;  //ivme değerleri x,y,z ekseninde
  int16_t gx, gy, gz;  // jiroskop değerleri x,y,z ekseninde
  int val1, val0;  // val1 anlık değer, val0 ilk değer.
  int sat;
  int teksay=0;
  int baslama=0;
  int mot1=0;
  
  //RÖLE 
  int R1=47;  
  int R2=25;  
  
unsigned int sabit=0; //timerın 
  
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
  else{
    dtostrf(flat,10,6,XBeeData.e);
  }
    
     if (flon == TinyGPS::GPS_INVALID_F_ANGLE){
       dtostrf(00.000000,10,6,XBeeData.b);
     }
     else{
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
 
  pressure_abs = sensor.getPressure(ADC_1024);
  alltitude=altitude(pressure_abs, pressure_baseline);
  val1=mpu6050.getAccY();
  dtostrf(val1,4,2,XBeeData.ang);
  dtostrf(alltitude,8,2,XBeeData.p); 
  
  xbee.send(zbTx);
  
  }




void start(){  //iletisim fonksiyonu
  Serial2.write('1'); //Yedek bilgisayar ayakta sinyali.
  delay(1000);
  Serial2.write('0');
  delay(1000);
}

void stopp(){  //iletisimi durdurma fonksiyonu
  while(1){
    Serial2.write('0');
    delay(500);
    Serial2.write('0');
    delay(500);
  }
}
  
 double altitude(double P, double P0)
// Given a pressure measurement P (mbar) and the pressure at a baseline P0 (mbar),
// return altitude (meters) above baseline.
{
  return (44330.0 * (1 - pow(P / P0, 1 / 5.255)));
}


 
void setup() {
 
Wire.begin();
Serial.begin(9600);
Serial2.begin(9600); 

  ss.begin(GPSBaud);

pinMode(R1,OUTPUT);
pinMode(R2,OUTPUT);
digitalWrite(R1,HIGH);
digitalWrite(R2,HIGH);
  
  pinMode(39,OUTPUT); //yeşil led
  pinMode(41,OUTPUT); // kırmızı led
  digitalWrite(39,HIGH);
  digitalWrite(41,LOW);
  sensor.reset();
  sensor.begin();
  pressure_baseline = sensor.getPressure(ADC_1024);
  
    mpu6050.begin();
  mpu6050.calcGyroOffsets(true);

 
  xbee.setSerial(Serial);
 
}

 
    










void loop() {


while(act==1)
{
  verisend();
  mpu6050.update();
  pressure_abs = sensor.getPressure(ADC_1024);
  alltitude=altitude(pressure_abs, pressure_baseline);
   
    if((abs(mpu6050.getAccY())>1.1)||(alltitude>6))
    { //degerler düzeltilecek
       //uçuyoruz.
       act=0; 
       app=1;
       sabit =millis();
       digitalWrite(39,LOW);
       digitalWrite(41,HIGH);
       
    }
     
       start();//Yedeğe hsysttsyız sinyali;

}

  
  while(app==1)
  {
                verisend();
                
                unsigned int degisken =millis(); // zamanlayıcı sürekli ölçüm 
                tme=(degisken-sabit)/1000;
            
                pressure_abs = sensor.getPressure(ADC_1024);
                alltitude=altitude(pressure_abs, pressure_baseline);
                delay(500);//Sensöre zaman tanı.
                pressure_abs = sensor.getPressure(ADC_1024);
                alltitude2=altitude(pressure_abs, pressure_baseline);
                
                
            
                if(alltitude>alltitude2)
                { //Düşüşe geçtik.
                    digitalWrite(R1,LOW);//Paraşüt rölesi.
                    delay(200);
                   int chck=digitalRead(R1);
               
                     if(chck==LOW)
                         {
                            app=0;
                            apps=1;
                            delay(5000);
                            digitalWrite(R1,HIGH);//Röle koruma
                           
                          }
                      else
                          {
                           Serial2.write('3');//Ben açamıyorum yedek röle aç.
                           delay(2000);
                           stopp();
                          }
                }
                //yükselis
                
                      if(tme>=19)
                    
                      {
                        
                              //İniş prosedürü başlat.
                              digitalWrite(R1,LOW);
                              delay(200);
                             int chck=digitalRead(R1);
                              if(chck==LOW)
                                    {
                                      delay(5000);
                                      digitalWrite(R1,HIGH);
                                      app=0;
                                      apps=1;
                                    }
                       
                              else
                                    {
                                      Serial2.write('3');
                                      delay(500);
                                      stopp();
                                    }
                      }
                    
   start();//Backup hayattayız
  }//while devam
 

  tim1=millis();
  pressure_abs = sensor.getPressure(ADC_1024);
  float apogeeAltitude=altitude(pressure_abs, pressure_baseline);
  //Düşüşe geçtiğimiz an
  while(apps==1)
  
  {
    verisend();
    unsigned int degisken =millis(); // zamanlayıcı sürekli ölçüm 
    tme=(degisken-sabit)/1000;
    unsigned int tim2=millis();
    int timeee=(tim2-tim1)/1000;
    
    pressure_abs = sensor.getPressure(ADC_1024);
    alltitude=altitude(pressure_abs, pressure_baseline);
      
      

    float fark=apogeeAltitude-alltitude; 
    
    if(alltitude<=580) //yeterli yakınlik var ana paraşüt
          {
              digitalWrite(R2,LOW);
             int chck2=digitalRead(R2);
              delay(200);
              chck2=digitalRead(R2);
             
              if(chck2==LOW)
                  {
                    delay(5000);
                    digitalWrite(R2,HIGH);
                    apps=0;
                    krt=1;           
                  }
                else
                    {
                      Serial2.write('4');
                      stopp();
                      delay(500);
                    }
              
          }
          else 
          
          {
            if((timeee>=6)&&(fark>160))
              {  
                 digitalWrite(R2,LOW);
               int chck2=digitalRead(R2);
                 delay(5);
                  if(chck2==LOW)
                        {
                        delay(5000);
                        digitalWrite(R2,HIGH);
                         apps=0;
                         krt=1;           
                        }
                      else
                        {
                          Serial2.write('4');
                         delay(2000);
                          stopp();
                        }
              }
             int chck2=digitalRead(R2);
             if((tme>69)&&(chck2!=LOW))
              {
                Serial2.write('4');
                  delay(2000);
                 stopp();
            
               }
            }
  }//Loop kapandı
  
  while(krt==1){
    
  verisend();
  }
}
