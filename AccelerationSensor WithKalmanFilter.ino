#include "Wire.h"
#include "I2Cdev.h" 
#include "MPU6050.h" 
int v0=0; unsigned int t0=0;
  MPU6050 mpu; 
    int16_t ax, ay, az;  
    int16_t gx, gy, gz;  
    int val; 
    int led=7;
    float kalman_old=0;
float cov_old=1;

float kalman_filter (float input)
{
   
  float kalman_new = kalman_old; // eski değer alınır
  float cov_new = cov_old + 75; //yeni kovaryans değeri belirlenir. Q=0.50 alınmıştır
  
  float kalman_gain = cov_new / (cov_new + 3734801,903); //kalman kazancı hesaplanır. R=0.9 alınmıştır
  float kalman_calculated = kalman_new + (kalman_gain * (input - kalman_new)); //kalman değeri hesaplanır
  
  cov_new = (1 - kalman_gain) * cov_old; //yeni kovaryans değeri hesaplanır
  cov_old = cov_new; //yeni değerler bir sonraki döngüde kullanılmak üzere kaydedilir
  
  kalman_old = kalman_calculated;
 
  return kalman_calculated; //hesaplanan kalman değeri çıktı olarak verilir
}

void setup() 
{ 
Wire.begin(); 
Serial.begin(9600);  
Serial.println("Initialize MPU"); 
mpu.initialize(); 
Serial.println(mpu.testConnection() ? "Connected" : "Connection failed"); 
pinMode(7, OUTPUT);

}
  
void loop() 
{ 

   
mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz); 
Serial.print(ax);
Serial.print(" ");
float a=kalman_filter(ax);
Serial.println(a);

// int vel=v0+(val*(millis()-t0));
//  delay(100); 
//  v0=vel;
//  t0=millis();
//  Serial.println(vel);
  
}
