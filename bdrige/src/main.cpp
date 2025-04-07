#include <Arduino.h>
#include <SPI.h>
#include <UIPEthernet.h>
#include <avr/wdt.h>
#include <EEPROM.h>
////// PINES
#define PULSO 23
#define RELE 74
#define LED 13
#define data 20 //EL QUE CUENTA LOS PULSOS DE ENTRADA
///////////////////

#define DEBUG 0
#define CONFIG 0
#define MAQUINA 0 //LAV_SQ Y SEC_WASC=0, SEC_SP=1, SEC_ADC Y LAV_WASC_EMERAL=2;
String codigo = "";
String palabra1 = "";
int i = 0;
int aa;
int posicion1 = 0;
char cc;
int monedas = EEPROM.read(101);
volatile int contador2 = 0;
int n = contador2;
unsigned long dato, datoAnterior = 0;

byte mac[] = {0x74, 0x69, 0x69, EEPROM.read(9), EEPROM.read(10), EEPROM.read(11)};
IPAddress server(EEPROM.read(54), EEPROM.read(55), EEPROM.read(56), EEPROM.read(57));
IPAddress ip(EEPROM.read(0), EEPROM.read(1), EEPROM.read(2), EEPROM.read(3));
IPAddress myDns(EEPROM.read(48), EEPROM.read(49), EEPROM.read(50), EEPROM.read(51));
EthernetClient client;

byte H1 = EEPROM.read(102);
byte L1 = EEPROM.read(100);
int w1 = H1 << 8;
int total = w1 + L1;

int puerto = EEPROM.read(12) * 1000 + EEPROM.read(13) * 100 + EEPROM.read(14) * 10 + EEPROM.read(15);

unsigned long lastConnectionTime = 0;
unsigned long long int postingInterval = 3 * 1000; //

void rele_off()
{
  // led_off();
  //digitalWrite(74, HIGH);
  // delay(15);
}

void rele_on()
{

  //led_on();
 // digitalWrite(74, LOW);
  //delay(15);
}

void cuenta_almacenamiento_monedas()
{
  monedas = EEPROM.read(101) + 1;
  EEPROM.update(101, monedas);
  //total = EEPROM.read(100) + 1;
  //EEPROM.update(100, total);
  H1 = EEPROM.read(102);
  L1 = EEPROM.read(100);
  w1 = H1 << 8;
  total = w1 + L1;
  total += 1;
  H1 = highByte(total);
  EEPROM.update(102, H1);
  L1 = lowByte(total);
  EEPROM.update(100, L1);
  w1 = H1 << 8;
  total = w1 + L1;
}

void con2()
{
  contador2++;
}
void calculo_total()
{
  total = 0;
  H1 = highByte(total);
  EEPROM.update(100, H1);
  L1 = lowByte(total);
  EEPROM.update(102, L1);
  w1 = H1 << 8;
  total = w1 + L1;
}

void setup()
{
  wdt_disable();
  Ethernet.init(53); // ss
#if DEBUG
  Serial.begin(9600);
  while (!Serial)
  {
    ;
  }
#endif
  pinMode(RELE, OUTPUT);  //control de rel
  pinMode(PULSO, OUTPUT); //control de monedas
#if MAQUINA == 0
  digitalWrite(PULSO, LOW); //low normal
#endif
#if MAQUINA == 1
  digitalWrite(PULSO, LOW); //low normal
#endif
#if MAQUINA == 2
  digitalWrite(PULSO, HIGH); //low normal
#endif
  pinMode(LED, OUTPUT);
  rele_off();

#if DEBUG
  Serial.println("BRIDGE ON");
  Serial.println("rele_off");
#endif
  
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // Check for Ethernet hardware present
    if (Ethernet.hardwareStatus() == EthernetNoHardware) {
      Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
      while (true) {
        delay(1); // do nothing, no point running without Ethernet hardware
      }
    }
    if (Ethernet.linkStatus() == LinkOFF) {
      Serial.println("Ethernet cable is not connected.");
    }
    // try to congifure using IP address instead of DHCP:
    Ethernet.begin(mac, ip, myDns);
    Serial.print("My IP address: ");
    Serial.println(Ethernet.localIP());
  } else {
    Serial.print("  DHCP assigned IP ");
    Serial.println(Ethernet.localIP());
  }

  //Ethernet.begin(mac, ip, myDns);
#if DEBUG
  Serial.println("OK conexion_INICIO");
  Serial.println("rele_on");
#endif
  rele_on();
  attachInterrupt(4, con2, FALLING);
  wdt_enable(WDTO_4S);
}


void resultado()
{
  posicion1 = codigo.indexOf("Resultado: ");
  palabra1 = codigo.substring(posicion1 + 12, posicion1 + 13);
  aa = String(palabra1).toInt();
}

void httpRequest_cut()
{
  client.stop();

  if (client.connect(server, puerto))
  {
#if DEBUG
    Serial.println("haciendo corte...");
#endif

    client.print("GET /kliclite/add_mov.php?token=X426ouX@OgvB3apLJ$C7li8N==&number=7469692D3031&date_reg=19-07-24&id_branch=1");
    client.print("&id_machine=");
    client.print(EEPROM.read(18));
    client.print("&card=1&action=2&data=1&amount=");
    client.print((total * EEPROM.read(30)) / 2);
    client.println(" HTTP/1.0");
    client.print("Host: ");
    client.println(server);
    client.println("Connection: close");
    client.println();
    EEPROM.update(100, 0);
    EEPROM.update(102, 0);
    calculo_total();
    lastConnectionTime = millis();
#if DEBUG
    Serial.println("SI SE REGISTRO CORTE...");
#endif
  }
  else
  {
#if DEBUG
    Serial.println("reset");
#endif
    asm volatile(" jmp 0");
  }
}

void httpRequest_coin()
{
  client.stop();

  if (client.connect(server, puerto))
  {
#if DEBUG
    Serial.println("haciendo venta...");
#endif
    client.print("GET /kliclite/add_mov.php?token=X426ouX@OgvB3apLJ$C7li8N==&number=7469692D3031&date_reg=19-07-24&id_branch=1");
    client.print("&id_machine=");
    client.print(EEPROM.read(18));
    client.print("&card=1&action=1&data=1&amount=");
    client.print((monedas * EEPROM.read(30)) / 2);
    client.println(" HTTP/1.0");
    client.print("Host: ");
    client.println(server);
    client.println("Connection: close");
    client.println();
    EEPROM.update(101, 0);
    lastConnectionTime = millis();
    n = 0;
    contador2 = 0;
#if DEBUG
    Serial.println("SI SE REGISTRO venta...");
#endif
  }
  else
  {
#if DEBUG
    Serial.println("reset");
#endif
    asm volatile(" jmp 0");
  }
}

void httpRequest()
{
  client.stop();
  if (client.connect(server, puerto))
  {
#if DEBUG
    Serial.println("revisando outputs...");
#endif
    client.print("GET /kliclite/revisar_output.php?token=X426ouX@OgvB3apLJ$C7li8N==&id_machine=");
    client.print(EEPROM.read(18));
    client.println(" HTTP/1.1");
    client.print("Host: ");
    client.println(server);
    client.println("Connection: close");
    client.println();

    lastConnectionTime = millis();
#if DEBUG
    Serial.println("SI SE REVISO OUTPUTS...");
#endif
  }
  else
  {
#if DEBUG
    Serial.println("reset");
#endif
    asm volatile(" jmp 0");
  }
}

void loop()
{
#if CONFIG
  //ip
  EEPROM.update(0, 192);
  EEPROM.update(1, 168);
  EEPROM.update(2, 0);
  EEPROM.update(3, 188);

  //mac
  EEPROM.update(9, 170);
  EEPROM.update(10, 250);
  EEPROM.update(11, 18);
  //id
  EEPROM.update(18, 18);

 
  //////////////////////////////////////////////////////////////////////////
   //pulse
  EEPROM.update(42, 7);
  //puerto
  EEPROM.update(12, 0);
  EEPROM.update(13, 0);
  EEPROM.update(14, 8);
  EEPROM.update(15, 0);

  //coin
  EEPROM.update(30, 5);
  //price
  EEPROM.update(36, 35);

  //dsn
  EEPROM.update(48, 192);
  EEPROM.update(49, 168);
  EEPROM.update(50, 0);
  EEPROM.update(51, 1);
  //server
  EEPROM.update(54, 192);
  EEPROM.update(55, 168);
  EEPROM.update(56, 0);
  EEPROM.update(57, 200);
  // EEPROM.update(2000, 0);
  //EEPROM.update(2001, 0);
#endif
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
    Serial.print("--------Resultado:");
    Serial.print(aa);
    Serial.println("--------");
#endif
    if (aa == 20)
    {
      httpRequest_cut();
    }
    else
    {
      if (aa > 0 && aa != 20)
      {
        for (int i = 1; i <= aa; i++)
        {

#if MAQUINA == 0
          delay(30); // (1 0 1 maquina x)
          digitalWrite(PULSO, HIGH);
          delay(30);
          digitalWrite(PULSO, LOW);
          delay(60);
#endif
#if MAQUINA == 1
          delay(200);
          digitalWrite(PULSO, HIGH);
          delay(21);
          digitalWrite(PULSO, LOW);
          delay(200);
#endif
#if MAQUINA == 2
          delay(30);
          digitalWrite(PULSO, LOW);
          delay(30);
          digitalWrite(PULSO, HIGH);
          delay(60);
#endif

#if DEBUG
          Serial.print("PULSOS:");
          Serial.println(aa);
#endif
        }
      }
      else
      {
#if DEBUG
        Serial.println("rele_on_outputs");
#endif
        i++;
        if (i == 60)
        {
          rele_off();
          while (1)
          {
          }
#if DEBUG
          Serial.println("RESET");
#endif
        }
        rele_on();
        codigo = "";
      }
    }
#if DEBUG
    Serial.println("rele_on_outputs");
#endif
    rele_on();
    codigo = "";
  }
  else
  {
    rele_on();
  }
  if (millis() - lastConnectionTime > postingInterval)
  {
    rele_off();
#if DEBUG
    Serial.print("tiempo de espera___:");
    Serial.println(millis() - lastConnectionTime);
    Serial.println("rele_off");
    Serial.print("valor de contador2:");
    Serial.println(contador2);
#endif
    if (contador2 == 0)

      httpRequest();
    else
      httpRequest_coin();
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
  //LEER MONEDERO #####################################
  if (n != contador2)
  {
#if DEBUG
    Serial.print("Contador de monedas:");
    Serial.println(contador2);
#endif
    lastConnectionTime = millis();
    cuenta_almacenamiento_monedas();
    n = contador2;
  }
  //###################################################
  wdt_reset();
}
