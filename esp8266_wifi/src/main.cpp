#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>

void setup()
{

  Serial.begin(115200); // Initializing if(DEBUG)Serial Monitor
}
void loop()
{

    Serial.print("{0}");
    delay(5000);
}
