#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#define RST_RED 12 // 12 reset main **** 4 para bridge

char ip_svr[40];
char puerto[40];
char id[40];
String recived;
//flag for saving datap
bool shouldSaveConfig = false;
const char *ssid = "Maquinas1";
const char *password = "Lavan.2022";
//const char* ssid = "Maquinas";
//const char* password = "Lavadoritos.2021";
//callback notifying us of the need to save config
void saveConfigCallback()
{
  // Serial.println("Should save config");
  shouldSaveConfig = true;
}

//Your Domain name with URL path or IP address with path
//String serverName = "http://192.168.10.202/klicdashs/sanroque/Api/frmActualiza_Precios.aspx"; pa resetear la main
//String serverName = "http://192.168.10.202/klicdashs/sanroque/Api/frmalta_log.aspx"; pa revisar saldo

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastTime = 0;
// Timer set to 10 minutes (600000)
//unsigned long timerDelay = 600000;
// Set timer to 5 seconds (5000)
unsigned long timerDelay = 600000;

void setup()
{
  Serial.begin(115200);
   WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    //Serial.print(".");
  }

    if (WiFi.status() == WL_CONNECTED)
    {
      //String serverName = "http://192.168.10.202/klicdashs/sanroque/Api/frmalta_log.aspx";
      //String serverName = "http://192.168.10.202/klicdashs/sanroque/Api/frmactualiza_precios.aspx?token=X426ouX@OgvB3apLJ$C7li8N==&machine=1";
      WiFiClient client;
      HTTPClient http;
      String serv1 = "http://";
      String serv2 = ":";
      String serv3 = "/klicdash/Api/frmactualiza_precios.aspx";
      String resultado1 = serv1 + "192.168.0.200";
      String resultado2 = serv2 + "80";
      String serverName = resultado1 + resultado2 + serv3;
      String serverPath = serverName + "?token=X426ouX@OgvB3apLJ$C7li8N==&machine=6";
      // String serverPath = serverName + "?token=X426ouX@OgvB3apLJ$C7li8N==&card=;88010003000120490188?&machine=5410ECF577B5&ctn=1,1,1";
      http.begin(client,serverPath.c_str());
      int httpResponseCode = http.GET();

      if (httpResponseCode > 0)
      {
        String payload = http.getString();
        Serial.println(payload);
      }
      else
      {
        ESP.reset();
      }
      // Free resources
      http.end();
    }
    else
    {
      ESP.reset();
    }
 
}

void loop()
{
  
 if ((millis() - lastTime) > timerDelay)
  {
    if (WiFi.status() == WL_CONNECTED)
    {
      //String serverName = "http://192.168.10.202/klicdashs/sanroque/Api/frmalta_log.aspx";
      //String serverName = "http://192.168.10.202/klicdashs/sanroque/Api/frmactualiza_precios.aspx?token=X426ouX@OgvB3apLJ$C7li8N==&machine=1";
      WiFiClient client;
      HTTPClient http;
      String serv1 = "http://";
      String serv2 = ":";
      String serv3 = "/klicdash/Api/frmactualiza_precios.aspx";
      String resultado1 = serv1 + "192.168.0.200";
      String resultado2 = serv2 + "80";
      String serverName = resultado1 + resultado2 + serv3;
      String serverPath = serverName + "?token=X426ouX@OgvB3apLJ$C7li8N==&machine=6";
      // String serverPath = serverName + "?token=X426ouX@OgvB3apLJ$C7li8N==&card=;88010003000120490188?&machine=5410ECF577B5&ctn=1,1,1";
      http.begin(client,serverPath.c_str());
      int httpResponseCode = http.GET();

      if (httpResponseCode > 0)
      {
        String payload = http.getString();
        Serial.println(payload);
      }
      else
      {
        ESP.reset();
      }
      // Free resources
      http.end();
    }
    else
    {
      ESP.reset();
    }
    lastTime = millis();
  }
//////////////////////////////////////////////////////////////////////////////////////////////////////////  
    if (Serial.available() > 0)
  {
    recived = Serial.readString();
    //Serial.println(recived);
    //Serial.println(recived.substring(0, 20));
    //Serial.println(recived.substring(20, 25));

    if (WiFi.status() == WL_CONNECTED)
    {
      //String serverName = "http://192.168.10.202/klicdashs/sanroque/Api/frmalta_log.aspx";
      WiFiClient client;
      HTTPClient http;
      String serv1 = "http://";
      String serv2 = ":";
      String serv3 = "/klicdash/Api/frmalta_log.aspx";
      String resultado1 = serv1 + "192.168.0.200";
      String resultado2 = serv2 + "80";
      String serverName = resultado1 + resultado2 + serv3;
      String serverPath = serverName + "?token=X426ouX@OgvB3apLJ$C7li8N==&card=;" + recived.substring(0, 20) + "?&machine=6&ctn=" + recived.substring(20, 25);
       //String serverPath = serverName + "?token=X426ouX@OgvB3apLJ$C7li8N==&card=;88010003000120490188?&machine=11&ctn=1,1,1";
      http.begin(client,serverPath.c_str());
      int httpResponseCode = http.GET();

      if (httpResponseCode > 0)
      {
        String payload = http.getString();
        Serial.println(payload);
      }
      else
      {
        ESP.reset();
      }
      http.end();
    }
    else
    {
      ESP.reset();
    }
  }
}