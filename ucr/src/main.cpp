#include <Arduino.h>

#include <SPI.h>
#include <MFRC522.h>
#define SS_PIN 10
#define RST_PIN 6

byte arr[17];
byte tipo[80] = {"parameters"};
byte tipo2[80] = {"expiration"};
String parametros = "";
int par = 4000;
int datoAnterior = 0, dato = 0, contador2 = 0;
byte blockcontent60[16] = {5, 7, 1, 19, 10, 30, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0};
String c = "";
char b;
MFRC522 mfrc522(SS_PIN, RST_PIN);
MFRC522::MIFARE_Key key;
byte readbackblock[18];
#define azul 4
#define rojo 5
#define buzz 3
void on()
{
  tone(buzz, 2000);
  digitalWrite(rojo, HIGH);
  delay(100);
  noTone(buzz);
  digitalWrite(azul, LOW);
  delay(100);
  digitalWrite(azul, HIGH);
  digitalWrite(rojo, LOW);
}
int writeBlock(int blockNumber, byte arrayAddress[])
{
  int largestModulo4Number = blockNumber / 4 * 4;
  int trailerBlock = largestModulo4Number + 3;
  if (blockNumber > 2 && (blockNumber + 1) % 4 == 0)
  {
    Serial.print(blockNumber);
    Serial.println(" is a trailer block:");
    return 2;
  }
  //Serial.print(blockNumber);

  byte status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, trailerBlock, &key, &(mfrc522.uid));
  if (status != MFRC522::STATUS_OK)
  {
    Serial.println("ERROR DE ESCRITURA (autentificacion):");
    for (int i = 0; i < 2; i++)
    {
      on();
    }
    asm volatile(" jmp 0");
    return 3; //return "3" as error message
  }

  status = mfrc522.MIFARE_Write(blockNumber, arrayAddress, 16);
  if (status != MFRC522::STATUS_OK)
  {
    Serial.println("EEROR DE ESCRTITURA DEL TAG: ");
    for (int i = 0; i < 2; i++)
    {
      on();
    }
    asm volatile(" jmp 0");
    return 4; //return "4" as error message
  }
  //Serial.println("LLAVE ACTUALIZADA");
}

int readBlock(int blockNumber, byte arrayAddress[])
{
  int largestModulo4Number = blockNumber / 4 * 4;
  int trailerBlock = largestModulo4Number + 3;
  byte status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, trailerBlock, &key, &(mfrc522.uid));

  if (status != MFRC522::STATUS_OK)
  {
    Serial.println("EEROR DE LECTURA (autentificacion): ");
    for (int i = 0; i < 2; i++)
    {
      on();
    }
    asm volatile(" jmp 0");
    return 3; //return "3" as error message
  }

  byte buffersize = 18;
  status = mfrc522.MIFARE_Read(blockNumber, arrayAddress, &buffersize);
  if (status != MFRC522::STATUS_OK)
  {
    Serial.println("ERROR DE LECTURA DEL TAG: ");
    for (int i = 0; i < 2; i++)
    {
      on();
    }
    asm volatile(" jmp 0");
    return 4; //return "4" as error message
  }
}
void setup()
{
  Serial.begin(9600);
  pinMode(rojo, OUTPUT);
  digitalWrite(rojo, LOW);
  pinMode(azul, OUTPUT);
  digitalWrite(azul, HIGH);

  SPI.begin();
  mfrc522.PCD_Init();
  key.keyByte[0] = 0xA3;
  key.keyByte[1] = 0xB2;
  key.keyByte[2] = 0xC1;
  key.keyByte[3] = 0xC3;
  key.keyByte[4] = 0xB2;
  key.keyByte[5] = 0xAF;

  //Serial.println("Coloca RFID");

  for (int i = 0; i < 2; i++)
  {
    on();
  }
}
void loop()
{
  while (Serial.available())
  {
    String ucr = Serial.readString();
    if (ucr == "port")
    {
      Serial.print("pon");
      for (int i = 0; i < 3; i++)
      {
        on();
      }
      return;
    }
    if (ucr == "reset")
    {
      asm volatile(" jmp 0");
    }
  }

  digitalWrite(rojo, LOW);
  digitalWrite(azul, HIGH);
  // Look for new cards
  if (!mfrc522.PICC_IsNewCardPresent())
    return;

  // Select one of the cards
  if (!mfrc522.PICC_ReadCardSerial())
    return;

  //Serial.println("No retire RFID");
  digitalWrite(rojo, HIGH);
  digitalWrite(azul, LOW);
  tone(buzz, 2000);
  delay(100);
  noTone(buzz);
  String ucr = "";
  while (Serial.available() == 0)
  {
    ucr = Serial.readString();
    // Serial.println("no disponible: " + ucr);
    if (ucr == "parameters")
    {
      writeBlock(62, tipo);
      Serial.print("okpar");
      ucr = "";
    }
    if (ucr == "expiration")
    {
      writeBlock(62, tipo2);
      Serial.print("okexp");
      ucr = "";
    }
    if (ucr == "opl")
    {
      writeBlock(62, tipo2);
      Serial.print("okopl");
      ucr = "";
    }
    if (ucr == "start")
    {
      Serial.print("son");
      for (int i = 0; i < 3; i++)
      {
        on();
      }
      break;
    }
    ucr = "";
  }
  /*if (ucr == "start") 
    return;*/
  while (Serial.available() == 0)
  {
  }
  parametros = Serial.readString();
  //parametros = ucr;
  Serial.println("LLAVE leida:" + parametros);
  int index = 0;
  int i = 0;
  int a = 0;
  int m = 1;
  while (index <= (parametros.length()) - 1)
  {

    char c = parametros[index++];
    par = 0;
    while (c >= '0' && c <= '9')
    {
      par = 10 * par + (c - '0');
      c = parametros[index++];
    }
    if (m > 2 && (m + 1) % 4 == 0)
    {
      m++;
    }
    Serial.print("bloque.");
    Serial.print(m);
    Serial.print("Parámetro.");
    Serial.print(i);
    Serial.print("= ");
    Serial.println(par);
    if (i % 2 == 0)
    {
      digitalWrite(rojo, HIGH);
      digitalWrite(azul, LOW);
      // tone(buzz, 2000);
    }
    else
    {
      digitalWrite(rojo, LOW);
      digitalWrite(azul, HIGH);
      //  noTone(buzz);
    }
    arr[a] = par;

    if ((i + 1) % 16 == 0)
    {
      writeBlock(m, arr);
      readBlock(m, readbackblock);
      for (int h = 0; h < 16; h++)
      {

        Serial.print(readbackblock[h]);
      }
      m++;
    }
    i++;
    a++;
    if (a == 16)
      a = 0;
    if (m == 63)
      break;
    if (i == 1000)
    {
      i = 0;
      for (int i = 0; i < 2; i++)
      {
        on();
      }
    }
  }

  Serial.print("lon");
  ////////////////////////////
  for (int i = 0; i < 6; i++)
  {
    on();
  }

  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
  delay(10000);
  on();
}
