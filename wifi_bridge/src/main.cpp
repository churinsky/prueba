#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
// #define RST_RED 12 // 12 reset main **** 4 para bridge
int auxx = 1;
String recived;
// flag for saving datap
// const char *ssid = "GTM1";
// const char *password = "Itos#2020";
// callback notifying us of the need to save config
// const char *ssid = "Klinko_sucursal";
const char *ssid = "Maquinas";
// const char *ssid = "Laundry";
const char *password = "Lavan.2022";
/*const char *ssid = "KLINKO";
const char *password = "klinko.2021";*/
// pablo livas

// const char *ssid = "Maquinas2";
// const char *password = "Mautra.2022";
// const char *ssid = "Klinko_sucursal";
// const char *password = "Klinko.2021";
// const char *ssid = "Maquinas";
// const char *password = "Lava.2022";

bool started = false;
bool ended = false;
char msg[20];
char incomingByte;
int dato;

#define DEBUG 1
#define PULSO 12 // 14 para la main ***12 para bridge
#define CONEXION 2
#define RST_RED 4 // 12 reset main **** 4 para bridge

// String serverName = "http://192.168.10.202:8080/kliclite/revisar_output.php";

unsigned long lastTime = 0;
unsigned long timerDelay = 3000;

void setup()
{
  pinMode(CONEXION, OUTPUT);
  pinMode(RST_RED, INPUT);
  pinMode(PULSO, OUTPUT);
  digitalWrite(PULSO, LOW);
  Serial.begin(115200);
  WiFi.begin(ssid, password);
#if DEBUG
  Serial.println("Connecting");
#endif

  while (WiFi.status() != WL_CONNECTED)
  {
#if DEBUG
    delay(500);
    Serial.print(".");
#endif
  }
  digitalWrite(CONEXION, HIGH);
}

void loop()
{

  if (Serial.available() > 0)
  {
    recived = Serial.readString();
    if (recived.substring(0, 2) == "01")
    {
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
        String serverPath = serverName + "?token=X426ouX@OgvB3apLJ$C7li8N==&id_machine=" + "20";
        http.begin(client, serverPath.c_str());
        http.GET();
        http.end();
        recived = "";
      }
    }
  }

  if ((millis() - lastTime) > timerDelay)
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
      String serverPath = serverName + "?token=X426ouX@OgvB3apLJ$C7li8N==&id_machine=" + "8";

#if DEBUG
      // Serial.println(serverPath);
#endif
      http.begin(client, serverPath.c_str());
      int httpResponseCode = http.GET();

      if (httpResponseCode > 0)
      {

        String payload = http.getString();
        // Serial.print(payload.length());
        // Serial.print(payload);
        /////////////////
        for (int i = 0; i <= payload.length() - 1; i++)
        {
          incomingByte = payload[i];
          Serial.print(incomingByte);
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

        if (started && ended) // if (started && ended)para que leas con llaves original con llaves  asi para omitir las llaves (!started && !ended)
        {
          int value = atoi(msg);
          /// sse comenta todo si lleva llaves//////////////////////////////////////
          /*
          if (payload.substring(2, 11) == "Resultado")
          {
            int posicion1 = payload.indexOf("Resultado: ");
            String palabra1 = payload.substring(posicion1 + 10, posicion1 + 12);
            Serial.println(palabra1);
            int aa = String(palabra1).toInt();

            Serial.println("xxxxxxxxxxxxxxxxxxxxxxxxxxxx");
            Serial.println(aa);
            value = aa;
            Serial.println("xxxxxxxxxxxxxxxxxxxxxxxxxxxx");
          }
          */
          ///////////////////////////////////////////////////////////////
          if (value > 0)
          {
            for (int i = 0; i < value; i++)
            {
              Serial.print(value); //para la MAIN

              //digitalWrite(PULSO, LOW);
              // delay(200);
            }
          }
        }
        ////////////////////

        ///////////////////////////////////////////////////////////////////////////////////////////////
      }
      /////////////////////////////////////////////////////////////////////////////////////
      else
      {
#if DEBUG
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);

#endif
        ESP.reset();
      }
      http.end();
    }
    else
    {
      digitalWrite(CONEXION, HIGH);
    }
    lastTime = millis();
  }
}