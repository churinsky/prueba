#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Reemplaza con tu información de red WiFi
const char *ssid = "GTM1"; //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
int numActivations = 0;
const char *password = "Itos#2020"; //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
String lastPayload;
unsigned long lastReconnectAttempt = 0;
//////////////////////////////////////////////////////////////
const unsigned long tiempoParaReset = 30000;
unsigned long tiempoUltimoCheck = 0;
///////////////////////////////////////////////////////////////
#define modelo 0 // 1 mautra 0 bridge
#define tipo 1   // 0 si es activacion doble con un bridge 1 normal

////timer reset

int push = 0;
// Reemplaza con la información de tu servidor MQTT 9 //
const char *mqtt_server = "192.168.10.64"; //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>22 23 24 25
const int mqtt_port = 1883;
const char *mqtt_topic = "use/7";        //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
const char *mqtt_publish_message = "m7"; //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
///  ////////////////////////////////////////
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
const int PULSE_PIN_1 = 12;
const int PULSE_PIN_2 = 14;
const int SW_PIN = 4;
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
  pinMode(LED_PIN, OUTPUT);    // Configura el pin del LED como salida
  digitalWrite(LED_PIN, HIGH); // Apaga el LED ----HIGH para apagar
#if modelo == 0
  pinMode(PULSE_PIN_1, OUTPUT);
  pinMode(SW_PIN, INPUT_PULLUP);
  digitalWrite(PULSE_PIN_1, LOW);
  pinMode(PULSE_PIN_2, OUTPUT);
  digitalWrite(PULSE_PIN_2, LOW);
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
  if (!client.connected())
  {
    digitalWrite(LED_PIN, HIGH); // Apaga el LED si se desconecta de MQTT
    reconnect();
  }
  if (shouldPublish)
  {
    // Serial.println("YA debe pubicar");
    if (push == 1)
    {
      client.publish(mqtt_publish_topic, mqtt_publish_message, 1);
    }

    shouldPublish = false;
#if modelo == 0
    //////////////////////////////////////////////////////
    
    unsigned long seconds = millis() / 1000;
    static unsigned long previousSeconds = seconds - 1;
    unsigned long tiempo_auxiliar = 0;

    if (seconds != previousSeconds)
    {
      previousSeconds = seconds;
      tiempo_auxiliar++;
    }
    
    //////////////////////////////////////////////////////
    for (int i = 0; i < numActivations; i++)
    {
      digitalWrite(PULSE_PIN_1, LOW);
      delay(20);
      digitalWrite(PULSE_PIN_1, HIGH);
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
      //   Serial.println(serialBuffer);
      lastSerialSendTime = currentTime;
    }
  }
  unsigned long tiempoActual = millis();
  if (tiempoActual - tiempoUltimoCheck >= tiempoParaReset)
  {
    ESP.reset();
  }
  client.loop();
}