#include <Arduino.h>
#define RELE 74
bool w = 0;
volatile int contador2 = 0;
int n = contador2;
unsigned long lastConnectionTime = 0;
unsigned long long int postingInterval = 200; //
void con2()
{
  contador2++;
  delay(100);
}
void setup()
{
  Serial.begin(9600);
  pinMode(RELE, OUTPUT);
  digitalWrite(RELE, LOW);
  attachInterrupt(4, con2, FALLING);
}

void loop()
{
  while (Serial.available())
  {
    String coin = Serial.readString();
                 
    if (coin == "who")
    {
      Serial.println("coin");
      //return;
    }
    if (coin == "reset")
    {
      Serial.println("resetok");
      asm volatile(" jmp 0");
    }
    if (coin == "on")
    {
      digitalWrite(RELE, HIGH);
      Serial.println("onok");
    }
    if (coin == "off")
    {
      digitalWrite(RELE, LOW);
      Serial.println("offok");
    }
  }

  if (millis() - lastConnectionTime > postingInterval && w == 1)
  {
    if (contador2 == 1)
    {
      Serial.println(1);
    }
    if (contador2 == 2)
    {
      Serial.println(2);
    }
    if (contador2 == 3)
    {
      Serial.println(5);
    }
    if (contador2 == 4)
    {
      Serial.println(10);
    }
    if (contador2 == 5)
    {
      Serial.println(5);
    }
    if (contador2 == 6)
    {
      Serial.println(10);
    }
    if (contador2 == 10)
    {
      Serial.println(10);
    }    w = 0;
    contador2 = 0;
    n = 0;
    //digitalWrite(RELE, LOW);
  }

  if (n != contador2)
  {
    n = contador2;
    w = 1;
    lastConnectionTime = millis();
    digitalWrite(RELE, HIGH);
  }
}