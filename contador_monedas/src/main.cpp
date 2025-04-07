#include <Arduino.h>
#include <TM1637Display.h>
#define CLK 4
#define DIO 2
#define MOTOR 7
#define coin 3
TM1637Display display(CLK, DIO);
volatile int contador2=0;
int n=contador2;
int i = 0;
void cont()
 {
   contador2++;
 }

void setup()
{
  display.clear();
  pinMode(MOTOR, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(MOTOR, LOW);
attachInterrupt(digitalPinToInterrupt(coin),cont,RISING);
}

void loop()
{
  if (n!=contador2)
  {
    i++;
    n=contador2;
  }

  //digitalWrite(MOTOR,LOW);
  display.setBrightness(0x0f);
  display.showNumberDec(i, true, 4, 0);
  digitalWrite(MOTOR, HIGH);
  // put your main code here, to run repeatedly:
}