#include <Arduino.h>
#define enable_bill 25
bool w = 0;
volatile int contador2 = 0;
int n = contador2;
unsigned long lastConnectionTime = 0;
unsigned long long int postingInterval = 600; //
void con2()
{
  contador2++;
}
void setup()
{
  Serial.begin(9600);
  pinMode(enable_bill, OUTPUT);
  digitalWrite(enable_bill, HIGH);
  attachInterrupt(4, con2, FALLING);
}

void loop()
{
  while (Serial.available())
  {
    String billetero = Serial.readString();

    if (billetero == "who")
    {
      Serial.println("bill");
      //return;
    }
    if (billetero == "reset")
    {
      Serial.println("resetok");
      asm volatile(" jmp 0");      
    }
    if (billetero == "on")
    {
      digitalWrite(enable_bill, LOW);
      Serial.println("onok");
    }
    if (billetero == "off")
    {
      digitalWrite(enable_bill, HIGH);
      Serial.println("offok");
    }
  }

  if (millis() - lastConnectionTime > postingInterval && w == 1)
  {
    Serial.println(contador2*10);
    w = 0;
    contador2 = 0;
    n = 0;
    digitalWrite(enable_bill, LOW);
  }

  if (n != contador2)
  {
    n = contador2;
    w = 1;
    lastConnectionTime = millis();
    digitalWrite(enable_bill, HIGH);
  }
}