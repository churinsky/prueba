#include <Arduino.h>
#include "Buttton.h"
#include <TM1637Display.h>
#include <OneWire.h>
#include <DallasTemperature.h>
//===================================
const int pinDatosDQ = 32;
OneWire oneWireObjeto(pinDatosDQ);

DallasTemperature sensorDS18B20(&oneWireObjeto);
//===================================
#define bt1 26
#define bt2 25
#define bt3 27
//===================================
#define puerta 22
#define k1 18
#define k2 19
//===================================
#define low_1 38
#define low_2 41
#define low_3 43
#define low_4 46
#define low_5 49 ///
#define low_6 52
#define low_7 54 ///
//===================================
#define med_1 54
#define med_2 57
#define med_3 60
#define med_4 63 ///
#define med_5 66
#define med_6 68 ///
#define med_7 71
//===================================
#define hig_1 71
#define hig_2 74
#define hig_3 77
#define hig_4 79
#define hig_5 82
#define hig_6 85
#define hig_7 88
//===================================
int temperatura_h = 0;
int temperatura_l = 0;
//==================================
#define DEBUG 1 // 1 para apagarlo 0 para encenderlo
//==================================
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
//===================================
const uint8_t SEG_DONE[] = {
    SEG_B | SEG_C | SEG_D | SEG_E | SEG_G,         // d
    SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F, // O
    SEG_C | SEG_E | SEG_G,                         // n
    SEG_A | SEG_D | SEG_E | SEG_F | SEG_G          // E
};
//===================================
const uint8_t SEG_COOL[] = {
    SEG_D | SEG_E | SEG_G,         // c
    SEG_C | SEG_D | SEG_E | SEG_G, // o
    SEG_C | SEG_D | SEG_E | SEG_G, // o
    SEG_B | SEG_C                  // l
};
//===================================
int ahoras = 0;
int aminutos = 0;
int asegundos = 0;
int TIEMPO1 = 28;
int segundostotal = TIEMPO1 * 60;
int dato;
int datoAnterior;
unsigned long times = 0;
int dato2;
int datoAnterior2;
int i = 0;
bool aux = 0;
bool aux_2 = 0;
bool aux_3 = 0;
#define CLK 16
#define DIO 17
TM1637Display display(CLK, DIO);
Buttton buton(bt1);
Buttton buton2(bt2);
Buttton buton3(bt3);
void setup()
{
    pinMode(OUTPUT,pinDatosDQ);

  sensorDS18B20.begin();
  pinMode(INPUT, puerta);
  pinMode(OUTPUT, k1);
  pinMode(OUTPUT, k2);
  Serial.begin(9600);
  display.clear();
}
void loop()
{
  Serial.println(digitalRead(puerta));
  // EasyBuzzer.update();
  ahoras = ((segundostotal / 60) / 60);
  aminutos = (segundostotal / 60) % 60;
  asegundos = segundostotal % 60;
  display.setBrightness(0x0f);
  display.showNumberDec(aminutos, true, 2, 0); // Expect: __04
  display.showNumberDec(asegundos, true, 2, 2);
  // display.showNumberDec(sensorDS18B20.getTempCByIndex(0), true, 2, 2);
  if (buton.isPressed())
  {
    temperatura_h = hig_7;
    temperatura_l = hig_5;
    Serial.println(temperatura_h);
    Serial.println(temperatura_l);
    aux = 1;
  }
  /*if (buton2.isPressed())
  {
    temperatura_h = med_7;
    temperatura_l = med_5;
    Serial.println(temperatura_h);
    Serial.println(temperatura_l);
    aux = 0;
  }*/
  if (buton3.isPressed())
  {
    temperatura_h = low_7;
    temperatura_l = low_5;
    Serial.println(temperatura_h);
    Serial.println(temperatura_l);
    aux = 1;
  }
#if DEBUG
  while (digitalRead(puerta) == 1)
  {
    analogWrite(k1, 0); // pegado al capacitor
    analogWrite(k2, 0); // pegado al thermistor
    display.setBrightness(0x0f);
    display.setSegments(SEG_DOOR);
    // analogWrite(k1, 255); // pegado al capacitor
    // analogWrite(k2, 255); // pegado al thermistor}
    aux = 0;
  }
#endif

#if DEBUG
  while (segundostotal > 0 && aux == 1 && digitalRead(puerta) == 0)
#endif
#if DEBUG == 0
    while (segundostotal > 0 && aux == 1 && digitalRead(puerta) == 1)
#endif
    {
      Serial.println(sensorDS18B20.getTempCByIndex(0));
#if DEBUG
      while (sensorDS18B20.getTempCByIndex(0) < 0)
      {
        display.setBrightness(0x0f);
        display.setSegments(SEG_E1);
        analogWrite(k1, LOW); // pegado al capacitor
        analogWrite(k2, LOW); // pegado al thermistor
      }
#endif

#if DEBUG
      if (aux_2 == 0)
      {
        if (digitalRead(puerta) == 1)
        {
          analogWrite(k1, LOW); // pegado al capacitor
          analogWrite(k2, LOW); // pegado al thermistor
          display.setBrightness(0x0f);
          display.setSegments(SEG_DOOR);
          aux = 0;
        }

        else
        {
          if (sensorDS18B20.getTempCByIndex(0) >= temperatura_h && digitalRead(puerta) == 0)
          {
            analogWrite(k2, LOW); // pegado al thermistor
            analogWrite(k1, 255); // pegado al thermistor
            aux_2 = 1;
          }
        }
      }
#endif
      if (sensorDS18B20.getTempCByIndex(0) <= temperatura_l && digitalRead(puerta) == 0)
      {
        analogWrite(k1, 255); // pegado al capacitor
        analogWrite(k2, 255); // pegado al thermistor}
        aux_2 = 0;
      }
      if (buton.isPressed())
      {
        temperatura_h = hig_7;
        temperatura_l = hig_5;
        Serial.println(temperatura_h);
        Serial.println(temperatura_l);
      }
      /*if (buton2.isPressed())
      {
        temperatura_h = med_7;
        temperatura_l = med_5;
        Serial.println(temperatura_h);
        Serial.println(temperatura_l);
      }*/
      if (buton3.isPressed())
      {
        temperatura_h = low_7;
        temperatura_l = low_5;
        Serial.println(temperatura_h);
        Serial.println(temperatura_l);
      }
      //// MUSTRA TEMPORIZADOR
      times = millis() / 1000;
      dato2 = times;
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

      /// FIN del CICLO
      if (segundostotal <= 0)
      {
        while (i <= 60)
        {
          analogWrite(k2, LOW); // pegado al thermistor
          display.setBrightness(0x0f);
          display.setSegments(SEG_COOL);
          times = millis() / 1000;
          dato2 = times;
          if (dato2 != datoAnterior2)
          {
            i++;
          }
          datoAnterior2 = dato2;
        }
        display.setBrightness(0x0f);
        display.setSegments(SEG_DONE);
        aux = 0;
        times = 0;
        dato2 = 0;
        datoAnterior2 = 0;
        aux = 0;
        aux_2 = 0;
        aux_3 = 0;
        i = 0;
        segundostotal = 0;
        analogWrite(k1, LOW); // pegado al capacitor
        analogWrite(k2, LOW); // pegado al thermistor
        delay(1000);
        ESP.restart();
      }
    }
}