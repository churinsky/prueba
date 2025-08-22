#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Preferences.h>

Preferences preferences;
// Reemplaza con tu información de red WiFi
const char *ssid = "Maquinas1"; //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
int numActivations = 0;
const char *password = "Lavan.2022"; //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
String lastPayload;
unsigned long lastReconnectAttempt = 0;
//////////////////////////////////////////////////////////////
const unsigned long tiempoParaReset = 30000;
unsigned long tiempoUltimoCheck = 0; 
///////////////////////////////////////////////////////////////
#define modelo 1  // 1 mautra 0 bridge
#define tipo 1    // 0 si es activacion doble con un bridge 1 normal
#define maquina 0 // 0 lavadora, 1 secadora
////timer reset



int push = 0;
// Reemplaza con la información de tu servidor MQTT 9 //
const char *mqtt_server = "192.168.0.200"; //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>22 23 24 25
const int mqtt_port = 1883;
const char *mqtt_topic = "use/3";       //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
const char *mqtt_publish_message = "m3";//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
String id_maquina = "3";
///  ////////////////////////////////////////
#if tipo == 0
const char *mqtt_topic_2 = "use/10";
const char *mqtt_publish_message_2 = "m10";
#endif
///  /////////////////////////////////
const char *mqtt_publish_topic = "kiosko/1"; // fijo
unsigned long lastSerialSendTime = 0;
WiFiClient espClient;
bool shouldPublish = false;
bool isSerialDataReady = false;
char serialBuffer[256];
const int LED_PIN = 2;
///////////////////////////////////////

///////////////////////////////////////
#if modelo == 0
const int PULSE_PIN = 12;
#endif

PubSubClient client(espClient);

void callback(String topic, byte *payload, unsigned int length)
{
  unsigned int i;
  for (i = 0; i < length && i < sizeof(serialBuffer) - 1; i++)
  {
    serialBuffer[i] = (char)payload[i];
  }
  serialBuffer[i] = '\0';
#if tipo == 0

  if (topic == mqtt_topic_2)
  {
    char *startPos = strchr(serialBuffer, '{');
    char *endPos = strchr(serialBuffer, '}');
    if (startPos != NULL && endPos != NULL && endPos > startPos)
    {
      char *activationsStr = startPos + 1;
      numActivations = atoi(activationsStr);
      if (numActivations > 0)
      {
        isSerialDataReady = true;
        if (topic == mqtt_topic_2)
        {
          push = 2; // Solo para mqtt_topic_2
        }
        else if (topic == mqtt_topic)
        {
          push = 1; // Solo para mqtt_topic
        }
      }
    }
  }
  else
  {
    char *startPos = strchr(serialBuffer, '{');
    char *endPos = strchr(serialBuffer, '}');
    if (startPos != NULL && endPos != NULL && endPos > startPos)
    {
      char *activationsStr = startPos + 1;
      numActivations = atoi(activationsStr);
      if (numActivations > 0)
      {
        isSerialDataReady = true;
        push = 1;
      }
    }
  }
#endif

#if tipo == 1
  char *startPos = strchr(serialBuffer, '{');
  char *endPos = strchr(serialBuffer, '}');
  if (startPos != NULL && endPos != NULL && endPos > startPos)
  {
    char *activationsStr = startPos + 1;
    numActivations = atoi(activationsStr);
    if (numActivations > 0)
    {
      isSerialDataReady = true;
      push = 1;
    }
  }
#endif
}

void reconnect()
{
  String clientId = "ESP8266Client-";
  clientId += String(WiFi.macAddress());
  while (!client.connected())
  {
    if (client.connect(clientId.c_str()))
    {
      client.subscribe(mqtt_topic, 1);
#if tipo == 0
      client.subscribe(mqtt_topic_2, 1);
#endif
      digitalWrite(LED_PIN, LOW); // Enciende el LED cuando está conectado a MQTT
    }
    else
    {
      for (int i = 0; i < 5; i++)
      {
        digitalWrite(LED_PIN, LOW); // Enciende el LED para indicar que está tratando de reconectar
        delay(100);
        digitalWrite(LED_PIN, HIGH); // Apaga el LED
        delay(100);
      }
    }
  }
}

void setup()
{
  ///////////////////////////////////////////////////////////////////////
  ///////////////////////////////////////////////////////////////////////
  preferences.begin("my-app", false);

  pinMode(LED_PIN, OUTPUT);    // Configura el pin del LED como salida
  digitalWrite(LED_PIN, HIGH); // Apaga el LED ----HIGH para apagar
#if modelo == 0
  pinMode(PULSE_PIN, OUTPUT);
  digitalWrite(PULSE_PIN, LOW); //////////////////////////////////>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
#endif
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(700);
    digitalWrite(LED_PIN, LOW); // Parpadea el LED durante la conexión
    delay(700);
    digitalWrite(LED_PIN, HIGH);
  }
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
  // Configurar MQTT
  client.setKeepAlive(60); // Keep alive de 60 segundos
}

void loop()
{
#if maquina == 0
  if (!client.connected())
  {
    digitalWrite(LED_PIN, HIGH); // Apaga el LED si se desconecta de MQTT
    reconnect();
  }

  if (Serial.available() > 0)
  {
    String receivedCommand = Serial.readStringUntil('\n');
    if (receivedCommand == "ok")
    {
      isSerialDataReady = false;
      shouldPublish = true;
      /// almacenar la activacion////////////////////////////
      int32_t numbersArray1Value_3 = 10;
      preferences.putInt("numbersArray1Value_3", numbersArray1Value_3);
      ///////////////////////////////////////////////////////
    }
    if (receivedCommand == "ok_2")
    {
      isSerialDataReady = false;
      shouldPublish = true;
      /// almacenar la activacion////////////////////////////
      int32_t numbersArray1Value_3 = 0;
      preferences.putInt("numbersArray1Value_3", numbersArray1Value_3);
      ///////////////////////////////////////////////////////
    }
    /////////////////////////////
    if (receivedCommand == "status_2") // falta modificar
    {
      int32_t numbersArray1Value_3 = preferences.getInt("numbersArray1Value_3", 0);
      // String info = "[" + String(numbersArray1Value) + ",0,0,0]\n";
      String info = "<" + String(numbersArray1Value_3) + ",0,0,0>\n";
      Serial.print(info);
    }
    /////////////////////////////

    if (receivedCommand == "status")
    {

      int32_t numbersArray1Value = preferences.getInt("numbersArray1Value", 2);
      int32_t numbersArray1Value_2 = preferences.getInt("numbersArray1Value_2", 0);
      // String info = "[" + String(numbersArray1Value) + ",0,0,0]\n";
      String info = "[" + String(numbersArray1Value) + ",0,0" + "," + String(numbersArray1Value_2) + "]\n";
      Serial.print(info);
    }
    int startPos = receivedCommand.indexOf('{');
    int endPos = receivedCommand.indexOf('}');

    if (startPos >= 0 && endPos >= 0)
    {
      String numbersString = receivedCommand.substring(startPos + 1, endPos);

      String numbersArray[3]; // 4 números
      int count = 0;
      char separator = ',';
      int prevIndex = 0;
      int nextIndex;
      do
      {
        wdt_reset();
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
      } while (nextIndex != -1 && count < 3); // Cambia 4 al número de elementos esperados

      String estado_lavado = "{c" + String(id_maquina) + "," + String(numbersArray[0]) + "," + String(numbersArray[1]) + "," + String(numbersArray[2]) + "}";
      // id_maquina, ciclo, etapa, paso;;;
      int32_t numbersArray1Value = numbersArray[1].toInt();
      int32_t numbersArray1Value_2 = numbersArray[2].toInt();
      //  if (numbersArray[2].toInt() < 50)
      preferences.putInt("numbersArray1Value", numbersArray1Value);

      preferences.putInt("numbersArray1Value_2", numbersArray1Value_2);
      // Serial.print(estado_lavado);
      const char *mqtt_publish_message = estado_lavado.c_str();
      if (numbersArray[2].toInt() < 50)
        client.publish(mqtt_publish_topic, mqtt_publish_message, 1);
    }

    int startPos_1 = receivedCommand.indexOf('[');
    int endPos_1 = receivedCommand.indexOf(']');

    if (startPos_1 >= 0 && endPos_1 >= 0)
    {
      String numbersString = receivedCommand.substring(startPos_1 + 1, endPos_1);

      String numbersArray[3]; // 4 números
      int count = 0;
      char separator = ',';
      int prevIndex = 0;
      int nextIndex;
      do
      {
        wdt_reset();
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
      } while (nextIndex != -1 && count < 3); // Cambia 4 al número de elementos esperados

      String estado_lavado = "{e" + String(id_maquina) + "," + String(numbersArray[0]) + "," + String(numbersArray[1]) + "," + String(numbersArray[2]) + "}";
      // Serial.print(estado_lavado);
      const char *mqtt_publish_message = estado_lavado.c_str();

      client.publish(mqtt_publish_topic, mqtt_publish_message, 1);
    }
  }

  if (shouldPublish)
  {
    // Serial.println("YA debe pubicar");
    if (push == 1)
    {
      client.publish(mqtt_publish_topic, mqtt_publish_message, 1);
    }
    else
    {
#if tipo == 0
      client.publish(mqtt_publish_topic, mqtt_publish_message_2, 1);
#endif
    }
    shouldPublish = false;
#if modelo == 0
    for (int i = 0; i < numActivations; i++)
    {

      digitalWrite(PULSE_PIN, LOW);
      delay(20);
      digitalWrite(PULSE_PIN, HIGH);
      delay(50);
    }

#endif
  }
  unsigned long currentTime = millis();
  if (isSerialDataReady)
  {
#if modelo == 0
    isSerialDataReady = false;
    shouldPublish = true;
#endif
    if (currentTime - lastSerialSendTime > 3000)
    {
      Serial.println(serialBuffer);
      lastSerialSendTime = currentTime;
    }
  }
  unsigned long tiempoActual = millis();
  if (tiempoActual - tiempoUltimoCheck >= tiempoParaReset)
  {
    //  ESP.reset();
  }
  client.loop();
#endif
//////////////////////////////////////////////////////////////////////
#if maquina == 1
  if (!client.connected())
  {
    digitalWrite(LED_PIN, HIGH); // Apaga el LED si se desconecta de MQTT
    reconnect();
  }

  if (Serial.available() > 0)
  {
    String receivedCommand = Serial.readStringUntil('\n');
    if (receivedCommand == "ok")
    {
      isSerialDataReady = false;
      shouldPublish = true;
      /// almacenar la activacion////////////////////////////
      ///////////////////////////////////////////////////////
    }
    /////////////////////////////
    int startPos = receivedCommand.indexOf('<');
    int endPos = receivedCommand.indexOf('>');

    if (startPos >= 0 && endPos >= 0)
    {
      String numbersString = receivedCommand.substring(startPos + 1, endPos);

      String numbersArray[4]; // 4 números
      int count = 0;
      char separator = ',';
      int prevIndex = 0;
      int nextIndex;
      do
      {
        wdt_reset();
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

      String estado_lavado = "{t" + String(id_maquina) + "," + String(numbersArray[0]) + "," + String(numbersArray[1]) + "," + String(numbersArray[2]) + "," + String(numbersArray[3]) + "}\n";
      const char *mqtt_publish_message = estado_lavado.c_str();

      client.publish(mqtt_publish_topic, mqtt_publish_message, 1);
    }

    int startPos_1 = receivedCommand.indexOf('[');
    int endPos_1 = receivedCommand.indexOf(']');

    if (startPos_1 >= 0 && endPos_1 >= 0)
    {
      String numbersString = receivedCommand.substring(startPos_1 + 1, endPos_1);

      String numbersArray[3]; // 4 números
      int count = 0;
      char separator = ',';
      int prevIndex = 0;
      int nextIndex;
      do
      {
        wdt_reset();
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
      } while (nextIndex != -1 && count < 3); // Cambia 4 al número de elementos esperados

      String estado_lavado = "{e" + String(id_maquina) + "," + String(numbersArray[0]) + "," + String(numbersArray[1]) + "," + String(numbersArray[2]) + "}";
      // Serial.print(estado_lavado);
      const char *mqtt_publish_message = estado_lavado.c_str();

      client.publish(mqtt_publish_topic, mqtt_publish_message, 1);
    }
  }

  if (shouldPublish)
  {
    // Serial.println("YA debe pubicar");
    if (push == 1)
    {
      client.publish(mqtt_publish_topic, mqtt_publish_message, 1);
    }
    else
    {
#if tipo == 0
      client.publish(mqtt_publish_topic, mqtt_publish_message_2, 1);
#endif
    }

    shouldPublish = false;
    push = 0; // Resetea el valor de push para evitar publicaciones adicionales

#if modelo == 0
    for (int i = 0; i < 4; i++)
    {
      digitalWrite(PULSE_PIN, LOW);  //////////////////////////////////>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
      delay(200);                    //////////////////////////////////>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
      digitalWrite(PULSE_PIN, HIGH); //////////////////////////////////>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
      delay(200);                    //////////////////////////////////>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
      digitalWrite(PULSE_PIN, LOW);  //////////////////////////////////>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
      delay(200);                    //////////////////////////////////>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    }

#endif
  }
  unsigned long currentTime = millis();
  if (isSerialDataReady)
  {
#if modelo == 0
    isSerialDataReady = false;
    shouldPublish = true;
#endif
    if (currentTime - lastSerialSendTime > 3000)
    {
      Serial.println(serialBuffer);
      lastSerialSendTime = currentTime;
    }
  }
  unsigned long tiempoActual = millis();
  if (tiempoActual - tiempoUltimoCheck >= tiempoParaReset)
  {
    ESP.reset();
  }
  client.loop();
#endif
}