#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
//#define RST_RED 12 // 12 reset main **** 4 para bridge
// http://192.168.10.202:8080/kliclite/registra_error.php?token=X426ouX@OgvB3apLJ$C7li8N==&id_machine=1&error=18
int value = 0;
String recived;
String payload;
String comando;
bool aux_1 = 1;
//const char *ssid = "Klinko_sucursal";
//const char *password = "Klinko.2021";
// flag for saving datap
 //const char *ssid = "GTM1";
 const char *ssid = "Maquinas";
// const char *ssid = "Laundry";
// const char *password = "Lava.2022";
// const char *password = "Itos#2020";
 const char *password = "Lavan.2022";
//  callback notifying us of the need to save config
String confirmacion;
int confirmacion2 = 0;
bool started = false;
bool ended = false;
char msg[20];
char incomingByte;
int dato;

#define DEBUG 1
#define PULSO 14 // 14 para la main ***12 para bridge
#define CONEXION 2
#define RST_RED 12 // 12 reset main **** 4 para bridge

// String serverName = "http://192.168.10.202:8080/kliclite/revisar_output.php";

unsigned long lastTime = 0;
unsigned long timerDelay = 3000;

void setup()
{
  pinMode(CONEXION, OUTPUT);
  pinMode(RST_RED, INPUT);
  pinMode(PULSO, OUTPUT);
  digitalWrite(PULSO, HIGH);
  Serial.begin(115200);
  WiFi.begin(ssid, password);
#if DEBUG
  // Serial.println("Connecting");
#endif

  while (WiFi.status() != WL_CONNECTED)
  {
#if DEBUG
    delay(500);
#endif
  }
  digitalWrite(CONEXION, HIGH);
}

void loop()
{
  if (Serial.available() > 0)
  {
    recived = Serial.readString();
    if (recived.substring(0, 2) == "OK")
    {
      confirmacion2 = 0;
     // aux_1 = 0;
    }
    if (recived.substring(0, 2) == "03")
    {
      if (WiFi.status() == WL_CONNECTED)
      {
        // http://192.168.10.202:8080/kliclite/registra_error.php?token=X426ouX@OgvB3apLJ$C7li8N==&id_machine=1&error=18
        WiFiClient client;
        HTTPClient http;
        String serv1 = "http://";
        String serv2 = ":";
        String serv3 = "/kliclite/registra_error.php";
        String resultado1 = serv1 + "192.168.0.200";
        String resultado2 = serv2 + "80";
        String serverName = resultado1 + resultado2 + serv3;
        String serverPath = serverName + "?token=X426ouX@OgvB3apLJ$C7li8N==&id_machine=" + "28&error=1";
        http.begin(client, serverPath.c_str());
        http.GET();
        http.end();
        recived = "";
      }
    }

    if (recived.substring(0, 2) == "04")
    {
      if (WiFi.status() == WL_CONNECTED)
      {
        // http://192.168.10.202:8080/kliclite/registra_error.php?token=X426ouX@OgvB3apLJ$C7li8N==&id_machine=1&error=18
        WiFiClient client;
        HTTPClient http;
        String serv1 = "http://";
        String serv2 = ":";
        String serv3 = "/kliclite/registra_error.php";
        String resultado1 = serv1 + "192.168.0.200";
        String resultado2 = serv2 + "80";
        String serverName = resultado1 + resultado2 + serv3;
        String serverPath = serverName + "?token=X426ouX@OgvB3apLJ$C7li8N==&id_machine=" + "28&error=2";
        http.begin(client, serverPath.c_str());
        http.GET();
        http.end();
        recived = "";
      }
    }

    if (recived.substring(0, 2) == "05")
    {
      if (WiFi.status() == WL_CONNECTED)
      {
        // http://192.168.10.202:8080/kliclite/registra_error.php?token=X426ouX@OgvB3apLJ$C7li8N==&id_machine=1&error=18
        WiFiClient client;
        HTTPClient http;
        String serv1 = "http://";
        String serv2 = ":";
        String serv3 = "/kliclite/registra_error.php";
        String resultado1 = serv1 + "192.168.0.200";
        String resultado2 = serv2 + "80";
        String serverName = resultado1 + resultado2 + serv3;
        String serverPath = serverName + "?token=X426ouX@OgvB3apLJ$C7li8N==&id_machine=" + "28&error=3";
        http.begin(client, serverPath.c_str());
        http.GET();
        http.end();
        recived = "";
      }
    }

    if (recived.substring(0, 2) == "02")
    {
      aux_1 = 1;
    }
    if (recived.substring(0, 2) == "01")
    {
      comando = "1";
      if (WiFi.status() == WL_CONNECTED)
      {
        // http://192.168.10.202:8080/kliclite/registra_error.php?token=X426ouX@OgvB3apLJ$C7li8N==&id_machine=1&error=18
        WiFiClient client;
        HTTPClient http;
        String serv1 = "http://";
        String serv2 = ":";
        String serv3 = "/kliclite/outputs_del.php";
        String resultado1 = serv1 + "192.168.0.200";
        String resultado2 = serv2 + "80";
        String serverName = resultado1 + resultado2 + serv3;
        String serverPath = serverName + "?token=X426ouX@OgvB3apLJ$C7li8N==&id_machine=" + "28";
        http.begin(client, serverPath.c_str());
        http.GET();
        http.end();
        recived = "";
      }
    }
  }
  if (((millis() - lastTime) > timerDelay) && aux_1 == 1)
  {
    if (WiFi.status() == WL_CONNECTED)
    {
      WiFiClient client;
      HTTPClient http;
      String serv1 = "http://";
      String serv2 = ":";
      String serv3 = "/kliclite/revisar_output.php";
      String resultado1 = serv1 + "192.168.0.200";
      String resultado2 = serv2 + "80";
      String serverName = resultado1 + resultado2 + serv3;
      String serverPath = serverName + "?token=X426ouX@OgvB3apLJ$C7li8N==&id_machine=" + "28";

#if DEBUG
      // Serial.println(serverPath);
#endif
      http.begin(client, serverPath.c_str());
      int httpResponseCode = http.GET();

      if (httpResponseCode > 0)
      {
        payload = http.getString();
        for (int i = 0; i <= payload.length() - 1; i++)
        {
          incomingByte = payload[i];
          // Serial.print(incomingByte);
          if (incomingByte == '{')
          {
            started = true;
            dato = 0;
            msg[dato] = '\0';
          }
          else if (incomingByte == '}')
          {
            ended = true;
          }
          else
          {
            if (dato < 4)
            {
              msg[dato] = incomingByte;
              dato++;
              msg[dato] = '\0';
            }
          }
        }

        if (started && ended)
        {
          value = atoi(msg);
          if (value > 0 && payload.length() > 5)
          {
            confirmacion = payload;
            confirmacion2 = value;
            recived = "";
          }
        }
      }
      /////////////////////////////////////////////////////////////////////////////////////
      else
      {
        ESP.reset();
      }
      http.end();
    }
    else
    {
      ESP.reset();
      digitalWrite(CONEXION, HIGH);
    }
    if (confirmacion2 > 0)
    {
      Serial.print(confirmacion);
    }
    lastTime = millis();
  }
}