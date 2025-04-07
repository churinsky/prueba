#include <Arduino.h>
#include "Buttton.h"
#include <TM1637Display.h>
#include <OneWire.h>
#include <DallasTemperature.h>
//===================================
const int pinDatosDQ = 49;
OneWire oneWireObjeto(pinDatosDQ);
DallasTemperature sensorDS18B20(&oneWireObjeto);
//===================================
#define bt1 7
#define bt2 8
#define bt3 9
//===================================
#define puerta 32
#define k1 40
#define k2 41
//===================================
#define led1 3
#define led2 81
#define led3 82
#define led4 80
#define led5 77
#define led6 78
#define led7 12
//===================================
const uint8_t SEG_E1[] = {
    SEG_G,
    SEG_A | SEG_D | SEG_E | SEG_F | SEG_G, // E
    SEG_B | SEG_C,                         // 1
    SEG_G};
//===================================
const uint8_t SEG_DOOR[] = {
    SEG_B | SEG_C | SEG_D | SEG_E | SEG_G,         // d
    SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F, // O
    SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F, // O
    SEG_E | SEG_G                                  // R
};
const uint8_t SEG_DONE[] = {
    SEG_B | SEG_C | SEG_D | SEG_E | SEG_G,         // d
    SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F, // O
    SEG_C | SEG_E | SEG_G,                         // n
    SEG_A | SEG_D | SEG_E | SEG_F | SEG_G          // E
};
const uint8_t SEG_COLD[] = {
    SEG_D | SEG_E | SEG_G,         // c
    SEG_C | SEG_D | SEG_E | SEG_G, // o
    SEG_C | SEG_D | SEG_E | SEG_G, // o
    SEG_B | SEG_C                  // l

};
//===================================
int ahoras = 0;
int aminutos = 0;
int asegundos = 0;
int TIEMPO1 = 4;
int segundostotal = TIEMPO1 * 60;
int dato;
int datoAnterior;
unsigned long time = 0;
int dato2;
int datoAnterior2;
int i = 0;
bool aux = 0;
#define CLK 4
#define DIO 2
TM1637Display display(CLK, DIO);
Buttton buton(bt1);
Buttton buton2(bt2);
Buttton buton3(bt3);

void setup()
{
  sensorDS18B20.begin();
  pinMode(INPUT_PULLUP, puerta);
  pinMode(OUTPUT, k1);
  pinMode(OUTPUT, k2);
  pinMode(OUTPUT, led1);
  pinMode(OUTPUT, led2);
  pinMode(OUTPUT, led3);
  pinMode(OUTPUT, led4);
  pinMode(OUTPUT, led5);
  pinMode(OUTPUT, led6);
  pinMode(OUTPUT, led7);
  Serial.begin(9600);
  display.clear();
}

void loop()
{

  ahoras = ((segundostotal / 60) / 60);
  aminutos = (segundostotal / 60) % 60;
  asegundos = segundostotal % 60;
  display.setBrightness(0x0f);
  display.showNumberDec(aminutos, true, 2, 0); // Expect: __04
  display.showNumberDec(asegundos, true, 2, 2);
  // display.showNumberDec(sensorDS18B20.getTempCByIndex(0), true, 2, 2);
  if (buton.isPressed())
  {
    analogWrite(led4, 255); // led boton 1
    aux = 1;
  }
  if (buton2.isPressed())
  {
    analogWrite(led1, 255); // led boton 2
    aux = 1;
  }
  if (buton3.isPressed())
  {
    analogWrite(led2, 255); // led boton 3
    aux = 1;
  }
  while (digitalRead(puerta) == 1)
  {
    aux = 0;
    display.setBrightness(0x0f);
    display.setSegments(SEG_DOOR);
  }

  while (segundostotal > 0 && aux == 1 && digitalRead(puerta) == 0)
  {

    Serial.println(sensorDS18B20.getTempCByIndex(0));

    while (sensorDS18B20.getTempCByIndex(0) < 0)
    {
      display.setBrightness(0x0f);
      display.setSegments(SEG_E1);
      digitalWrite(led4, LOW); // led boton 1
      digitalWrite(led1, LOW); // led boton 2
      digitalWrite(led2, LOW); // led boton 3
      digitalWrite(led3, LOW); // led 4
      digitalWrite(led7, LOW); // led 5
      digitalWrite(led5, LOW); // led 6
      digitalWrite(led6, LOW); // led 7
      digitalWrite(k1, LOW);   // pegado al capacitor
      digitalWrite(k2, LOW);   // pegado al thermistor
    }
    if (sensorDS18B20.getTempCByIndex(0) >= 25 && sensorDS18B20.getTempCByIndex(0) <= 26)
    {
      analogWrite(led3, 255); // led  4
      analogWrite(led7, 255); // led 5
      analogWrite(led6, 255); // led 6
      analogWrite(led5, 255); // led  7

      analogWrite(k1, 255); // pegado al capacitor
      analogWrite(k2, 255); // pegado al thermistor
    }
    else
    {
      digitalWrite(led3, LOW); // led 4
      digitalWrite(led7, LOW); // led 5
      digitalWrite(led5, LOW); // led 6
      digitalWrite(led6, LOW); // led 7

      digitalWrite(k1, LOW); // pegado al capacitor
      digitalWrite(k2, LOW); // pegado al thermistor
    }
    if (buton.isPressed())
    {
      analogWrite(led4, 255);  // led boton 1
      digitalWrite(led1, LOW); // led boton 2
      digitalWrite(led2, LOW); // led boton 3
      aux = 1;
    }
    if (buton2.isPressed())
    {
      analogWrite(led1, 255);  // led boton 2
      digitalWrite(led4, LOW); // led boton 1
      digitalWrite(led2, LOW); // led boton 3
      aux = 1;
    }
    if (buton3.isPressed())
    {
      analogWrite(led2, 255);  // led boton 3
      digitalWrite(led4, LOW); // led boton 1
      digitalWrite(led1, LOW); // led boton 2
      aux = 1;
    }

    while (digitalRead(puerta) == 1)
    {
      display.setBrightness(0x0f);
      display.setSegments(SEG_DOOR);
      digitalWrite(led4, LOW); // led boton 1
      digitalWrite(led1, LOW); // led boton 2
      digitalWrite(led2, LOW); // led boton 3
      digitalWrite(led3, LOW); // led 4
      digitalWrite(led7, LOW); // led 5
      digitalWrite(led5, LOW); // led 6
      digitalWrite(led6, LOW); // led 7
      digitalWrite(k1, LOW);   // pegado al capacitor
      digitalWrite(k2, LOW);   // pegado al thermistor
    }

    time = millis() / 1000;
    dato2 = time;
    if (dato2 != datoAnterior2)
    {
      sensorDS18B20.requestTemperatures();
      segundostotal--;
    }
    datoAnterior2 = dato2;

    ahoras = ((segundostotal / 60) / 60);
    aminutos = (segundostotal / 60) % 60;
    asegundos = segundostotal % 60;

    display.setBrightness(0x0f);
    display.showNumberDec(aminutos, true, 2, 0); // Expect: __04
    display.showNumberDec(asegundos, true, 2, 2);

    if (segundostotal <= 0)
    {
      display.setBrightness(0x0f);
      display.setSegments(SEG_DONE);
      aux = 0;
      
      digitalWrite(led4, LOW); // led boton 1
      digitalWrite(led1, LOW); // led boton 2
      digitalWrite(led2, LOW); // led boton 3
      digitalWrite(led3, LOW); // led 4
      digitalWrite(led7, LOW); // led 5
      digitalWrite(led5, LOW); // led 6
      digitalWrite(led6, LOW); // led 7
      digitalWrite(k1, LOW);   // pegado al capacitor
      digitalWrite(k2, LOW);   // pegado al thermistor
      delay(1000);
      asm volatile(" jmp 0");
    }
  }
}
