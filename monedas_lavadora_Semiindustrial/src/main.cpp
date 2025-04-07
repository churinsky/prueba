#include <Arduino.h>
#include <TM1637Display.h>
////////////////////////////////////
#define CLK 40
#define DIO 37
TM1637Display display(CLK, DIO);
////// PINES
#define RELE 74
#define buzz A0
///////////////////
volatile int contador2 = 0;
int n = contador2;
// 7///////////////

const uint8_t SEG_on[] = {
    0x0,                                           //
    SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F, // O
    SEG_C | SEG_E | SEG_G,                         // n
    0x0                                            //
};
////////////////////

void rele_off()
{
  digitalWrite(74, HIGH);
}

void rele_on()
{
  digitalWrite(74, LOW);
}
void con2()
{
  contador2++;
}

void setup()
{
  Serial.begin(9600);
  pinMode(RELE, OUTPUT);
  rele_on();
  display.clear();
  display.setBrightness(0x0f);
  display.showNumberDec((30 - (n * 10)), true, 3, 1);
  attachInterrupt(4, con2, FALLING);
}

void loop()
{

  if (n != contador2)
  {

    n = contador2;
    Serial.println(n);
    tone(buzz, 2000);
    delay(200);
    noTone(buzz);
    display.clear();
    display.setBrightness(0x0f);
    display.showNumberDec((30 - (n * 10)), true, 3, 1);
    if (n >= 3)
    {
      display.clear();
      display.setBrightness(0x0f);
      display.setSegments(SEG_on);
      n = 0;
      contador2 = 0;
      rele_off();
      Serial.println("rele_on");
      delay(20000);
      rele_on();
      Serial.println("rele_off");
      display.clear();
      display.setBrightness(0x0f);
      display.showNumberDec((30 - (n * 10)), true, 3, 1);
      asm volatile(" jmp 0");
    }
  }
}
