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
int aux2 = 0;
int ahoras = 0;
int aminutos = 0;
int asegundos = 0;
int segundostotal = 30* 60;
unsigned long time = 0;
int dato2;
int datoAnterior2;
// 7///////////////
unsigned long lastTime = 0;
// Timer set to 10 minutes (600000)
// unsigned long timerDelay = 600000;
// Set timer to 5 seconds (5000)
unsigned long timerDelay = 10000;
///////////////////
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
  display.showNumberDec((35 - (n * 5)), true, 3, 1);
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
    display.showNumberDec((35 - (n * 5)), true, 3, 1);
    if (n >= 7)
    {
      aux2 = 1;
      display.clear();
      display.setBrightness(0x0f);
      display.setSegments(SEG_on);
      n = 0;
      contador2 = 0;
      rele_off();
      delay(500);
      Serial.println("rele_on");
      ///
      while (segundostotal > 0)
      {
        // delay (1000);
        time = millis() / 1000;
        dato2 = time;
        if (dato2 != datoAnterior2)
        {
          segundostotal--;
        }
        datoAnterior2 = dato2;

        ahoras = ((segundostotal / 60) / 60);
        aminutos = (segundostotal / 60) % 60;
        asegundos = segundostotal % 60;
        // display.clear();
        display.setBrightness(0x0f);
        display.showNumberDecEx(aminutos, 0b01000000, true, 2, 2);
       // display.showNumberDec(asegundos, true, 2, 2);
      }
      //
      rele_on();
      Serial.println("rele_off");
      display.clear();
      display.setBrightness(0x0f);
      display.showNumberDec((35 - (n * 5)), true, 3, 1);
      asm volatile(" jmp 0");
    }
  }
}
