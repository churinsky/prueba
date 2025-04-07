#include <Arduino.h>
#define enable 74

bool w = 0;
volatile int contador2 = 0;
int n = contador2, contador=0;
unsigned long lastConnectionTime = 0;
unsigned long long int postingInterval = 1 * 1000; //
void con2()
{
  contador2++;
}
void setup()
{
  Serial.begin(9600);
  pinMode(enable, OUTPUT);
  digitalWrite(enable, LOW);
  attachInterrupt(4, con2, FALLING);
}

void loop()
{

  if (n != contador2)
  {
    n = contador2;
    contador++;
    w = 1;
    Serial.print(contador);
    if (contador==7)
    {
      Serial.print("ACTIVACION");
      digitalWrite(enable, HIGH);
      contador=0;
      delay(20000);
      digitalWrite(enable, LOW);
    }
  }
}