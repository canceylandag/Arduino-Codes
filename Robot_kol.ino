#include <Servo.h>

Servo motor1;
Servo motor2;

//For now it is for driving two servo motor with a joystick it will be updated in the future.  

// Arduino pin numbers
const int SW_pin = 4; // digital pin connected to switch output
const int X_pin = A0; // analog pin connected to X output
const int Y_pin = A1; // analog pin connected to Y output
int but_durum=0;
int counter=0;

void setup() {
  pinMode(SW_pin, INPUT);
  digitalWrite(SW_pin, HIGH);
  Serial.begin(9600);
  motor1.attach(2);
  motor2.attach(3);
  motor1.write(90);
  motor2.write(90);
}

void loop() {
  
   but_durum=digitalRead(SW_pin);
  if(but_durum==0){
    
     counter++;
    
    }

  if((counter%2==0)){
     int pot1=analogRead(X_pin);
    int derece=map(pot1,0,1023,0,180);
     motor1.write(derece);
       
    }else if((counter%2==1)){
     int pot2= analogRead(X_pin);
    int derece=map(pot2,0,1023,0,180);
     motor2.write(derece);
    
    }
  
}
