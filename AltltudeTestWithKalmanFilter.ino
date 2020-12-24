#include <Wire.h>
#include "SparkFunMPL3115A2.h"
#include <LiquidCrystal.h>
LiquidCrystal LCD(24,25,26,27,28,29);
MPL3115A2 myPressure;
float kalman_old=0;
float cov_old=1;
int ledd=0;
int ledd2=0;


//Pressure sensor altitude test with Kalman filter.

float kalman_filter (float input)
{
   
  float kalman_new = kalman_old; // eski değer alınır
  float cov_new = cov_old + 0.0035; //yeni kovaryans değeri belirlenir. Q=0.50 alınmıştır
  
  float kalman_gain = cov_new / (cov_new + 0.451960806); //kalman kazancı hesaplanır. R=0.9 alınmıştır
  float kalman_calculated = kalman_new + (kalman_gain * (input - kalman_new)); //kalman değeri hesaplanır
  
  cov_new = (1 - kalman_gain) * cov_old; //yeni kovaryans değeri hesaplanır
  cov_old = cov_new; //yeni değerler bir sonraki döngüde kullanılmak üzere kaydedilir
  
  kalman_old = kalman_calculated;
 
  return kalman_calculated; //hesaplanan kalman değeri çıktı olarak verilir
}
void setup()
{
  Wire.begin();        // Join i2c bus
  Serial.begin(9600);  // Start serial for output
  LCD.begin(16,2);
  LCD.clear();
  pinMode(6,OUTPUT);
  pinMode(7,OUTPUT);
  myPressure.begin();
  myPressure.setModeAltimeter(); // Measure altitude above sea level in meters
  //myPressure.setModeBarometer(); // Measure pressure in Pascals from 20 to 110 kPa

  myPressure.setOversampleRate(2); // Set Oversample to the recommended 128
  myPressure.enableEventFlags(); // Enable all three pressure and temp event flags 
unsigned long tme1=millis();
  unsigned long tme2=millis();
  while(tme2-tme1<3000){
    float cal = myPressure.readAltitude();
    float cal2=kalman_filter(cal);
    LCD.setCursor(0,0); 
    LCD.print("Calibrating wait.");    
    tme2=millis();
  }
  LCD.clear();
  delay(10);
  LCD.print("Calibrating done."); 
  digitalWrite(6,HIGH);
  digitalWrite(7,HIGH);
}

void loop()
{

ledd=digitalRead(7);
 ledd2=digitalRead(6);
while(ledd!=LOW){
float altitude = myPressure.readAltitude();
  float pressure2=kalman_filter(altitude);
  LCD.setCursor(0,1); 
  LCD.print("Ele");
  LCD.setCursor(4,1);
  LCD.print(pressure2,2);

  if(pressure2>=15000){
      digitalWrite(7,LOW);
    
    }
 ledd=digitalRead(7);

}
ledd=digitalRead(7);
ledd2=digitalRead(6);
while((ledd==LOW)&&(ledd2!=LOW)){
  
 float altitude = myPressure.readAltitude();
  float pressure22=kalman_filter(altitude);
 
  LCD.setCursor(0,1); 
  LCD.print("Ele:");
  LCD.setCursor(4,1);
 LCD.print(pressure22,2);
 
 if(pressure22<=500){
      digitalWrite(6,LOW);
    
    }

 ledd2=digitalRead(6);

}
  
 



}
