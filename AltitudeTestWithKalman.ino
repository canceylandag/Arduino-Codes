
#include <Wire.h>
#include "SparkFunMPL3115A2.h"

//Pressure sensor altitude test with Kalman filter.

//Create an instance of the object
MPL3115A2 myPressure;
float kalman_old=0;
float cov_old=1;
float ccc;

float kalman_filter (float input)
{
   
  float kalman_new = kalman_old; // eski değer alınır
  float cov_new = cov_old + 0.005; //yeni kovaryans değeri belirlenir. Q=0.50 alınmıştır
  
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

  myPressure.begin(); // Get sensor online

  //Configure the sensor
  myPressure.setModeAltimeter(); // Measure altitude above sea level in meters
  //myPressure.setModeBarometer(); // Measure pressure in Pascals from 20 to 110 kPa

  myPressure.setOversampleRate(2); // Set Oversample to the recommended 128
  myPressure.enableEventFlags(); // Enable all three pressure and temp event flags 
}

void loop()
{
  float altitude = myPressure.readAltitude();
    Serial.print(altitude, 2);
    Serial.print(",");
    ccc=kalman_filter(altitude);
    Serial.println(ccc, 2);

  Serial.println();
}
