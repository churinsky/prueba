#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
unsigned long lastTime = 0;
const char *ssid = "Maquinas";
const char *password = "Lavan.2023";
const char *serverIp = "192.168.0.200";
const int serverPort = 80;
const char *serverPath = "/kliclite/revisar_output.php";
const String token = "X426ouX@OgvB3apLJ$C7li8N==";
const String machineId = "7";

const int CONNECTION_PIN = 2;
const int PULSE_PIN = 14 ;// 14 para la main ***12 para bridge
const int RESET_PIN = 12;// 12 reset main **** 4 para bridge



const unsigned long TIMER_DELAY = 3000;

int numActivations = 0;

WiFiClient client;
HTTPClient http;

void activateMachine(int numActivations)
{
  for (int i = 0; i < numActivations; i++)
  {
    digitalWrite(CONNECTION_PIN, HIGH);
    delay(100);
    digitalWrite(CONNECTION_PIN, LOW);
    delay(100);
    digitalWrite(CONNECTION_PIN, HIGH);
    /*
        delay(200);
        digitalWrite(PULSE_PIN, LOW);
        delay(21);
        digitalWrite(PULSE_PIN, HIGH);
        delay(200);*/
    delay(100);
    digitalWrite(PULSE_PIN, HIGH);
    delay(100);
    digitalWrite(PULSE_PIN, LOW);
    delay(100);
 
  }
}

void setup()
{
  pinMode(CONNECTION_PIN, OUTPUT);
  pinMode(RESET_PIN, INPUT);
  pinMode(PULSE_PIN, OUTPUT);
  // digitalWrite(PULSE_PIN, HIGH);
  digitalWrite(PULSE_PIN, LOW);
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
  }

  digitalWrite(CONNECTION_PIN, HIGH);
}

void loop()
{
  if ((millis() - lastTime) > TIMER_DELAY)
  {
    if (WiFi.status() == WL_CONNECTED)
    {
      WiFiClient client;
      HTTPClient http;
      String serv1 = "http://";
      String serv2 = ":";
      String serv3 = serverPath;
      String resultado1 = serv1 + serverIp;
      String resultado2 = serv2 + serverPort;
      String serverName = resultado1 + resultado2 + serv3;
      String serverPath = serverName + "?token=X426ouX@OgvB3apLJ$C7li8N==&id_machine=" + machineId;

      // Serial.println(serverPath);

      http.begin(client, serverPath.c_str());
      int httpResponseCode = http.GET();

      if (httpResponseCode > 0)
      {
        String payload = http.getString();
         Serial.print(payload);
        int startPos = payload.indexOf('{');
        int endPos = payload.indexOf('}');
        if (startPos >= 0 && endPos >= 0)
        {
          String activationsStr = payload.substring(startPos + 1, endPos);
         // Serial.print(activationsStr);
          numActivations = activationsStr.toInt();
        }
        else
        {
        //  Serial.println("Error: Invalid payload");
        }
      }
      else
      {
       // Serial.print("Error code: ");
       // Serial.println(httpResponseCode);
        ESP.reset();
      }
      http.end();

      if (numActivations > 0)
      {
        activateMachine(numActivations);
       
      }
    }
    else
    {
      digitalWrite(CONNECTION_PIN, HIGH);
    }
    lastTime = millis();
  }
}
