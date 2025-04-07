#include <Arduino.h>
#include <SPI.h>
#include <UIPEthernet.h>
#include <avr/wdt.h>
#include <EEPROM.h>

#define DEBUG 1
String codigo = "";
String palabra1 = "";
int aa;
int posicion1 = 0;
char cc;
int dato, datoAnterior = 0;
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};

IPAddress ip(192, 168, 10, 177);
IPAddress myDns(192, 168, 10, 1);

EthernetClient client;

IPAddress server(192, 168, 10, 202);
unsigned long lastConnectionTime = 0;
const unsigned long postingInterval = 10 * 1000;

void setup()
{
  wdt_disable();
  Ethernet.init(10); // ss
  Serial.begin(9600);
  while (!Serial)
  {
    ;
  }
#if DEBUG
  Serial.println("OK");
  Serial.print("rele_off");
#endif
  wdt_enable(WDTO_4S);

  Ethernet.begin(mac, ip, myDns);
#if DEBUG
  Serial.println("OK");
  Serial.print("rele_on");
#endif
}
void cut()
{
  char lc;
  while (client.connected())
  {
    if (client.available())
    {
      char nc = client.read();
      if ((lc == 10) && (nc == 13))
      {
        while (client.available() == 0)
          ;
        client.read();
        break;
      }
      else
        lc = nc;
    }
  }
}
void resultado()
{
  posicion1 = codigo.indexOf("Resultado: ");
  palabra1 = codigo.substring(posicion1 + 11, posicion1 + 13);
  aa = String(palabra1).toInt();
}

void httpRequest_cut()
{

  client.stop();

  if (client.connect(server, 8080))
  {
#if DEBUG
    Serial.println("haciendo corte...");
#endif

    client.print("GET /kliclite/add_mov.php?token=X426ouX@OgvB3apLJ$C7li8N==&number=7469692D3031&date_reg=19-07-24&id_branch=1&id_machine=1&card=1&action=2&data=1&amount=200");
    client.println(" HTTP/1.0");
    client.print("Host: ");
    client.println(server);
    client.println("Connection: close");
    client.println();
    lastConnectionTime = millis();

  }
  else
  {
#if DEBUG
    Serial.println("reset");
#endif
    while (1)
    {
      ;
    }
  }
}
void httpRequest()
{
  client.stop();
  if (client.connect(server, 8080))
  {
#if DEBUG
    Serial.println("revisando outputs...");
#endif
    client.println("GET /kliclite/revisar_output.php?token=X426ouX@OgvB3apLJ$C7li8N==&id_machine=1 HTTP/1.0");
    client.print("Host: ");
    client.println(server);
    client.println("Connection: close");
    client.println();

    lastConnectionTime = millis();
  }
  else
  {
#if DEBUG
    Serial.println("reset");
#endif
    while (1)
    {
      ;
    }
  }
}
void loop()
{
  //brinco las lineas de los encabezados del html
  cut();
  while (client.available())
  {
    cc = client.read();
    codigo = codigo + cc;
  }
  if (codigo != "")
  {
    // saco el valor que hay despues de la palabra resultado
    resultado();
#if DEBUG
    Serial.print("Resultado:");
    Serial.println(aa);
#endif
    if (aa == 200)
    {
      httpRequest_cut();
    }
    else
    {
      if (aa > 0 && aa != 200)
      {
#if DEBUG
        Serial.print("PULSOS:");
        Serial.println(aa);
#endif
      }
      else
      {
#if DEBUG
        Serial.println("rele_on");
#endif
        codigo = "";
      }
    }

    //codigo = "";
  }
  if (millis() - lastConnectionTime > postingInterval)
  {
#if DEBUG
    Serial.print("rele_off");
#endif
    httpRequest();
  }
#if DEBUG
  if (dato != datoAnterior)
  {
    Serial.print("Tiempo de espera de monedas:");
    Serial.println((millis() - lastConnectionTime) / 1000);
  }
  datoAnterior = dato;

  dato = (millis() - lastConnectionTime) / 1000;

#endif
  wdt_reset();
}
