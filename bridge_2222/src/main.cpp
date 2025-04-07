#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <Preferences.h>

Preferences preferences; // Instancia de Preferences para guardar el número en la flash

unsigned long lastTime = 0;
const char *baseSsid = "Maquinas";      // OK
const char *basePass = "Lavan.20";      // ok
const char *serverIp = "192.168.0.200"; //_____ no se modifica
const int serverPort = 8;               // ok
const char *serverPath = "/kliclite/revisar_output.php";
const char *serverPath_1 = "/kliclite/outputs_del.php";
const String token = "X426ouX@OgvB3apLJ$C7li8N==";
const String machineId = "17"; // OK
const unsigned long SERIAL_SEND_INTERVAL = 100000;
const int CONNECTION_PIN = 2;
const int PULSE_PIN = 12; // 14 para la main ***12 para bridge
const int RESET_PIN = 4;  // 12 reset main **** 4 para bridge
String payload;
int var = 20; //////////////////////////////////////////////////////////////2 para lavadora mautra 20 original
const unsigned long TIMER_DELAY = 3000;
String fullMachineId;
String fullPort;
int numActivations = 0;
WiFiClient client;
HTTPClient http;

void activateMachine(int numActivations)
{
  for (int i = 0; i < numActivations; i++)
  {
    delay(200);
    digitalWrite(CONNECTION_PIN, LOW);
    delay(21);
    digitalWrite(CONNECTION_PIN, HIGH);

    delay(200);
    digitalWrite(PULSE_PIN, LOW);
    delay(21);
    digitalWrite(PULSE_PIN, HIGH);
    delay(200);
    /*
    delay(200);
    digitalWrite(PULSE_PIN, HIGH);
    delay(21);
    digitalWrite(PULSE_PIN, LOW);
    delay(200);
    */
  }
}

void setup()
{
  // Inicializar EEPROM
  pinMode(CONNECTION_PIN, OUTPUT);
  pinMode(RESET_PIN, INPUT);
  pinMode(PULSE_PIN, OUTPUT);
  digitalWrite(PULSE_PIN, HIGH);
  // digitalWrite(PULSE_PIN, LOW);
  Serial.begin(115200);
  // digitalWrite(CONNECTION_PIN, HIGH);
  //  Inicializar la instancia Preferences
  preferences.begin("my-app", false);
  // Leer el número guardado en la flash
  int savedNumber = preferences.getInt("savedNumber", 1);      // ssid----------------
  int savedNumber_1 = preferences.getInt("savedNumber_1", 8);  // id maquima-------------
  int savedNumber_2 = preferences.getInt("savedNumber_2", 0);  //  puerto
  int savedNumber_3 = preferences.getInt("savedNumber_3", 22); //  pass

  // Configurar WiFi con el SSID completo
  String fullSsid = String(baseSsid) ;
  String fullPass = String(basePass) + String(savedNumber_3);
  fullPort = String(serverPort) + String(savedNumber_2);
  fullMachineId = String(savedNumber_1);
  WiFi.begin(fullSsid.c_str(), fullPass.c_str());
  unsigned long startTime = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - startTime < 30000) // Intentar conectarse durante 30 segundos
  {
    delay(500);
    Serial.print(".");
  }
}

void loop()
{
  switch (var)
  {
  case 1:
    digitalWrite(CONNECTION_PIN, HIGH);
    if (Serial.available() > 0)
    {
      String recived = Serial.readString();
     // Serial.println(recived);
      int startPos = recived.indexOf('{');
      int endPos = recived.indexOf('}');
      if (startPos >= 0 && endPos >= 0)
      {
        String numbersString = recived.substring(startPos + 1, endPos);
        String numbersArray[4]; // 4 números
        int count = 0;
        char separator = ',';
        int prevIndex = 0;
        int nextIndex;
        do
        {
          nextIndex = numbersString.indexOf(separator, prevIndex);
          if (nextIndex != -1)
          {
            numbersArray[count] = numbersString.substring(prevIndex, nextIndex);
            prevIndex = nextIndex + 1;
          }
          else
          {
            numbersArray[count] = numbersString.substring(prevIndex);
          }
          count++;
        } while (nextIndex != -1 && count < 4); // Cambia 4 al número de elementos esperados
        // Ahora tienes cada número en el array numbersArray
        for (int i = 0; i < count; i++)
        {
          // Guardar el número en la EEPROM
          preferences.putInt("savedNumber", numbersArray[0].toInt());
          preferences.putInt("savedNumber_1", numbersArray[1].toInt());
          preferences.putInt("savedNumber_3", numbersArray[2].toInt());
          preferences.putInt("savedNumber_2", numbersArray[3].toInt());
          digitalWrite(CONNECTION_PIN, LOW);
          delay(3000);
          // Reiniciar el ESP8266 para aplicar el nuevo SSID
          ESP.restart();
        }
      }
    }
    break;
  case 2:
    if (Serial.available() > 0) // esto lo uso para el trafico
    {
      String recived = Serial.readString();
     // Serial.print(recived);
      int startPos = recived.indexOf('[');
      int endPos = recived.indexOf(']');
      if (startPos >= 0 && endPos >= 0)
      {
        String activationsStr = recived.substring(startPos + 1, endPos);
        var = activationsStr.toInt();
      }
    }
    break;
  default:
    if ((millis() - lastTime) > TIMER_DELAY)
    {
      if (WiFi.status() == WL_CONNECTED)
      {
        String serverName = "http://" + String(serverIp) + ":" + fullPort + serverPath + "?token=" + token + "&id_machine=" + fullMachineId;

        //   Serial.println(serverName);

        http.begin(client, serverName.c_str());
        int httpResponseCode = http.GET();

        if (httpResponseCode > 0)
        {
          payload = http.getString();
          Serial.print(payload);
          int startPos = payload.indexOf('{');
          int endPos = payload.indexOf('}');
          if (startPos >= 0 && endPos >= 0)
          {
            String activationsStr = payload.substring(startPos + 1, endPos);
            //  Serial.print(activationsStr);
            numActivations = activationsStr.toInt();
            // Serial.print(numActivations);
          }
        }
        else
        {
          Serial.println("Error en la solicitud HTTP");
          delay(500);
        }
        http.end();

        //   Serial.println(serverName);

        http.begin(client, serverName.c_str());
        httpResponseCode = http.GET();

        // mandar llamar ouputs_del siempre y cuando tenga algo valido el payload

        //////////////////////////////////////////////////////////////////////////
        if (numActivations > 0)
        {
          serverName = "http://" + String(serverIp) + ":" + fullPort + serverPath_1 + "?token=" + token + "&id_machine=" + fullMachineId;
          http.begin(client, serverName.c_str());
          int httpResponseCode = http.GET();
          if (httpResponseCode > 0)
          {
            //  payload = http.getString();
            //  Serial.print(payload);
            int startPos = payload.indexOf('{');
            int endPos = payload.indexOf('}');
            if (startPos >= 0 && endPos >= 0)
            {
              String activationsStr = payload.substring(startPos + 1, endPos);
              //  Serial.print(activationsStr);
              // numActivations = activationsStr.toInt();
              // Serial.print(numActivations);
            }
          }
          else
          {
            Serial.println("Error en la solicitud HTTP");
            delay(500);
          }
          http.end();
          activateMachine(numActivations);
        }
      }
      unsigned long startTime = millis();

      while (millis() - startTime < 60000 && numActivations > 0)
      { // Ejecutar durante 1 minuto (60,000 ms)
        Serial.print(payload);//////////////////////////////////////////////////////////////////
        delay(5000);
      }
      lastTime = millis();
    }
    // desde la tarjeta
    if (Serial.available() > 0) // esto lo uso para hacer dar nombre a ssid, pass, puerto, id de maquina
    {
      String recived = Serial.readString();
      Serial.print(recived);
      int startPos = recived.indexOf('[');
      int endPos = recived.indexOf(']');
      if (startPos >= 0 && endPos >= 0)
      {
        String activationsStr = recived.substring(startPos + 1, endPos);
        var = activationsStr.toInt();
        // Serial.print(var);
      }
    }

    break;
  }
}
