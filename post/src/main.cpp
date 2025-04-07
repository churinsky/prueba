#include <FS.h>
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h >
#include <WiFiManager.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <EEPROM.h>
#include <ArduinoJson.h>
char ip_svr[40];
char puerto[40];
char id[2000];
char moneda[40];
int monedas = 0;
int total = 0;

bool started = false;
bool ended = false;
char msg[20];
char incomingByte;
int dato;

//flag for saving data
bool shouldSaveConfig = false;

//callback notifying us of the need to save config
void saveConfigCallback()
{
  // Serial.println("Should save config");
  shouldSaveConfig = true;
}
void cuenta_almacenamiento_monedas()
{
  monedas += 1;
  total += 1;
}
#define DEBUG 1
#define PULSO 12 // 14 para la main ***12 para bridge
#define CONEXION 2
#define RST_RED 4 // 12 reset main **** 4 para bridge

//String serverName = "http://192.168.10.202:8080/kliclite/revisar_output.php";

unsigned long lastTime = 0;
unsigned long timerDelay = 3000;
String palabra1 = "";
int aa;
int posicion1 = 0;
volatile int contador2 = 0;
int n = contador2;

ICACHE_RAM_ATTR void con2()
{
  contador2++;
}

void setup()
{
  EEPROM.begin(512);

  Serial.begin(9600);
  while (!Serial)
  {
    ;
  }
  // Serial.println(WiFi.macAddress());
  //Serial.println("ok");

  if (SPIFFS.begin())
  {
    //Serial.println("mounted file system");
    if (SPIFFS.exists("/config.json"))
    {
      //file exists, reading and loading
      // Serial.println("reading config file");
      File configFile = SPIFFS.open("/config.json", "r");
      if (configFile)
      {
        // Serial.println("opened config file");
        size_t size = configFile.size();
        // Allocate a buffer to store contents of the file.
        std::unique_ptr<char[]> buf(new char[size]);

        configFile.readBytes(buf.get(), size);
        DynamicJsonBuffer jsonBuffer;
        JsonObject &json = jsonBuffer.parseObject(buf.get());
        json.printTo(Serial);
        if (json.success())
        {
          //  Serial.println("\nparsed json");

          strcpy(id, json["id"]);
          strcpy(puerto, json["puerto"]);
          strcpy(ip_svr, json["ip_svr"]);
          //strcpy(moneda, json["moneda"]);
        }
        else
        {
          //  Serial.println("failed to load json config");
        }
      }
    }
  }
  else
  {
    //   Serial.println("failed to mount FS");
  }

  ///////////////////////////////////////////////////////////////////////////////////////
  WiFiManagerParameter custom_puerto_server("puerto", "puerto", puerto, 40);
  //wifiManager.addParameter(&custom_puerto_server);
  WiFiManagerParameter custom_id_server("id", "id", id, 40);
  //wifiManager.addParameter(&custom_id_server);
  WiFiManagerParameter custom_ip_svr_server("ip_svr", "ip_svr", ip_svr, 40);
  // wifiManager.addParameter(&custom_ip_svr_server);
  //WiFiManagerParameter custom_moneda_server("moneda", "moneda", moneda, 40);

  ///////////////////////////////////////////////////////////////////////////
  WiFiManager wifiManager;
  wifiManager.setSaveConfigCallback(saveConfigCallback);

  wifiManager.addParameter(&custom_puerto_server);
  wifiManager.addParameter(&custom_id_server);
  wifiManager.addParameter(&custom_ip_svr_server);
  //wifiManager.addParameter(&custom_moneda_server);
  pinMode(CONEXION, OUTPUT);
  pinMode(RST_RED, INPUT);
  pinMode(PULSO, OUTPUT);
  digitalWrite(PULSO, HIGH);
  if (digitalRead(RST_RED) == 0)
  {
    wifiManager.resetSettings();
    SPIFFS.format();
  }

  wifiManager.autoConnect("050106");

  strcpy(puerto, custom_puerto_server.getValue());
  strcpy(id, custom_id_server.getValue());
  strcpy(ip_svr, custom_ip_svr_server.getValue());
  //strcpy(moneda, custom_moneda_server.getValue());

  if (shouldSaveConfig)
  {
    //  Serial.println("saving config");
    DynamicJsonBuffer jsonBuffer;
    JsonObject &json = jsonBuffer.createObject();
    json["puerto"] = puerto;
    json["id"] = id;
    json["ip_svr"] = ip_svr;
    //json["moneda"] = moneda;
    File configFile = SPIFFS.open("/config.json", "w");
    if (!configFile)
    {
      //  Serial.println("failed to open config file for writing");
    }
    json.printTo(Serial);
    json.printTo(configFile);
    configFile.close();
    //end save
  }
#if DEBUG
  Serial.print("PUERTO:");
  Serial.println(puerto);
  Serial.print("ID:");
  Serial.println(id);
  Serial.print("IP_SERVER:");
  Serial.println(ip_svr);
  // Serial.print("MONEDA:");
  // Serial.println(moneda);
#endif

  digitalWrite(CONEXION, HIGH);
  attachInterrupt(digitalPinToInterrupt(14), con2, FALLING);
}

void loop()
{
  if ((millis() - lastTime) > timerDelay)
  {
    if (WiFi.status() == WL_CONNECTED)
    {
      HTTPClient http;
      String serv1 = "http://";
      String serv2 = ":";
      String serv3 = "/kliclite/revisar_output.php";
      String resultado1 = serv1 + ip_svr;
      String resultado2 = serv2 + puerto;
      String serverName = resultado1 + resultado2 + serv3;
      String serverPath = serverName + "?token=X426ouX@OgvB3apLJ$C7li8N==&id_machine=" + id;

#if DEBUG
      Serial.println(serverPath);
#endif
      http.begin(serverPath.c_str());
      int httpResponseCode = http.GET();

      if (httpResponseCode > 0)
      {

        String payload = http.getString();
        //Serial.print(payload.length());
        //Serial.print(payload);
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

        if (started && ended)
        {
          int value = atoi(msg);
          Serial.println("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX");

          if (value > 0 && value != 20)
          {
            //Serial.print(value); //para la MAIN
            digitalWrite(CONEXION, HIGH);
            delay(500);
            digitalWrite(CONEXION, LOW);
            delay(500);
            digitalWrite(CONEXION, HIGH);
            for (int i = 1; i <= value; i++)
            {
              delay(200);
#if DEBUG
              Serial.println("L");
#endif
              digitalWrite(PULSO, LOW);
#if DEBUG
              Serial.println("H");
#endif
              delay(21);
              digitalWrite(PULSO, HIGH);
#if DEBUG
              Serial.println("L");
#endif
              delay(200);
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
      }
      http.end();
    }
    else
    {
      digitalWrite(CONEXION, HIGH);
    }
    lastTime = millis();
  }

  //
  //Aqui va lo del contador de monedas
  //
}