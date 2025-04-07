#include <Arduino.h>
#include <SoftwareSerial.h> 

SoftwareSerial BT(10,11);   
 
void setup()
{
  BT.begin(9600);   
  Serial.begin(9600);   
}
 
void loop()
{
  if(BT.available()) 
  {
    Serial.write(BT.read());
  }
 
  if(Serial.available()) 
  {
     BT.write(Serial.read());
  }
}