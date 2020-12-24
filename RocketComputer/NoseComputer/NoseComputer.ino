#include <SoftwareSerial.h>

#include <TinyGPS.h>
#include <stdlib.h>
#include <XBee.h>
 typedef struct XBeeStruct {
char id='3';
char spc2=0x20;
char sa[2];
char e[10];
char b[10];

} XBeeDataStruct;


TinyGPS gps;
SoftwareSerial aa(1,0);


SoftwareSerial ss(11,10);
XBee xbee = XBee();

static XBeeDataStruct XBeeData;

// SH + SL Address of receiving XBee
XBeeAddress64 addr64 = XBeeAddress64(0x0013A200, 0x4176017B);
ZBTxRequest zbTx = ZBTxRequest(addr64, (char*)&XBeeData, sizeof(XBeeDataStruct));
ZBTxStatusResponse txStatus = ZBTxStatusResponse();
void setup()
{
  Serial.begin(9600);
  aa.begin(9600);
  
  xbee.setSerial(aa);
  
  
  ss.begin(9600);
 
}

void loop()
{
  
  bool newData = false;
  unsigned long chars;
  unsigned short sentences, failed;

  // For one second we parse GPS data and report some key values
  for (unsigned long start = millis(); millis() - start < 1000;)
  {
    while (ss.available())
    {
      char c = ss.read();
      // Serial.write(c); // uncomment this line if you want to see the GPS data flowing
      if (gps.encode(c)) // Did a new valid sentence come in?
        newData = true;
    }
  }

  
    float flat, flon;
    unsigned long age;
    gps.f_get_position(&flat, &flon, &age);
    if (flat == TinyGPS::GPS_INVALID_F_ANGLE)
  {
    dtostrf(00.000000,10,6,XBeeData.e);
    
  }
  else
  {
    dtostrf(flat,10,6,XBeeData.e);
  }
     if (flon == TinyGPS::GPS_INVALID_F_ANGLE)
  {
    dtostrf(00.000000,10,6,XBeeData.e);
     
  }
  else
  {
    dtostrf(flon,10,6,XBeeData.b);
  }
    if (gps.satellites() == TinyGPS::GPS_INVALID_SATELLITES)
  {
     XBeeData.sa[0]='0';
     XBeeData.sa[1]='0';
  }
  else
  {
    int a=gps.satellites();
    itoa (a,XBeeData.sa,10);
  }
    
    
    
   
    
  
  xbee.send(zbTx);
  
  gps.stats(&chars, &sentences, &failed);
  Serial.print(" CHARS=");
  Serial.print(chars);
  Serial.print(" SENTENCES=");
  Serial.print(sentences);
  Serial.print(" CSUM ERR=");
  Serial.println(failed);
  if (chars == 0)
    Serial.println("** No characters received from GPS: check wiring **");
}
