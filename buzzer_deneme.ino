int buzzerPin=3;

void setup()
{
  pinMode(buzzerpin,OUTPUT);  
}

void loop()
{
  tone(buzzerPin, 330);
    delay(1000);
    noTone(buzzerPin);
    delay(1000);
 
  noTone(buzzerPin);
}
