#include <Arduino.h>
#include <TM1637Display.h>
#include "Boton.h"
#include "EEPROM.h"
#include "Ciclo.h"
#include "Parametros.h"
#include <avr/wdt.h>
#define buzzer 9
#define ignitor 28
#define motor 29
#define moneda_ 12
#define puerta 85 // primer vesion 38,nueva 85

#define eeprom 1// actualizar parametros  1 para que queden bloqueados 0
#define opl 0    // 0 opl, para quitar el opl 1
#define ciclo 0  // ciclo largo-corto, 1 tiempo fijo
//////////////////////////////////////
int _datoAnterior22 = 0;
//////////////////////////////////////
const int MAX_LENGTH = 50; // Define el tamaño máximo de tu cadena
char received[MAX_LENGTH]; // Crea un arreglo de caracteres
int index = 0;             // Índice para controlar la posición en el arreglo

int aux10;
int aux11;
int aux12 = 0;
int aux112 = 0;
//////////////////////////////////////////////////////
int temp_boton;
static bool instruccionEnviada = false;
static bool instruccionEnviada2 = false;
static bool instruccionEnviada3 = false;
static bool instruccionEnviada4 = false;

int ciclo_str = 0;
int etapa_str = 0;
int paso_str = 0;
int paso_sec = 0;
/// almacenamiento_de info //////////////////////////
int aminutosAnterior = -1;
////////////////////////////////////////////////////
// Definición del tamaño del arreglo
#define NUM_PARES 5
int cambiosAminutos = 0;
// Estructura para almacenar un par de datos
struct Datos
{
  int aminutos;
  float temperatura;
};

Datos datos[NUM_PARES];
//////////////////////////////////////
String recived, palabra1 = "";
int posicion1 = 0, aa;
//////////////////////////////////////////// 78
int tiempo_secado = _tiempo_secado;       // 1
int tiempo_cd = _tiempo_cooldown;         // 2
int precio_temp1 = _precio_temp1;         // 4
int temperatura_baja = _temperatura_baja; // 7
int temperatura_alta = _temperatura_alta; // 8
int valor_moneda = _valor_moneda;         // 4
int tiempo_adicional = _tiempo_adicional; // 13
//////////////////////////////////////////// 78
unsigned long lastTime = 0;
unsigned long timerDelay = 3000;

unsigned long lastTime2 = 0;
unsigned long timerDelay2 = 30000;

#if (opl == 0)
int var = 1;
#endif

#if (opl == 1)
int var = 10000000;
#endif

int price;
int cont = 1;
int w;
int aux2 = 1;
int aux3;
int aux4 = 0;
int aux5 = 0;
int datoAnterior_moneda = HIGH, dato_moneda = HIGH;
bool door = LOW;
#define CLK 73
#define DIO 74
TM1637Display ddisplay(CLK, DIO);
int i, j;
#define CLK2 4
#define DIO2 2
TM1637Display display(CLK2, DIO2);
Ciclo tiem_ciclo;
Boton b1(23, 27);
Boton b2(24, 25);
Boton b3(26, 22);
int Vo;

///////////////TIMEER
int TIEMPO1 = tiempo_secado + tiempo_cd;
int segundostotal = TIEMPO1 * 60;
int ahoras = ((segundostotal / 60) / 60);
int aminutos = (segundostotal / 60) % 60;
int asegundos = segundostotal % 60;
int dato2;
int datoAnterior2;
/////////////////////////
const uint8_t SEG_on[] = {SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F, SEG_C | SEG_E | SEG_G};
const uint8_t SEG_dr[] = {SEG_B | SEG_C | SEG_D | SEG_E | SEG_G, SEG_E | SEG_G};
const uint8_t SEG[] = {SEG_G, SEG_G};
const uint8_t SEG_P[] = {SEG_D | SEG_E | SEG_F | SEG_G}; // esta es una T no P
const uint8_t SEG_PROG[] = {SEG_A | SEG_B | SEG_E | SEG_F | SEG_G, SEG_E | SEG_G};
//////////////////
float R1 = 47000; // resistencia fija del divisor de tensión
float logR2, R2, TEMPERATURA;
float A = 1.976148856e-03, B = 0.3985279933e-04, C = 7.623337487e-07;

void par()
{
  wdt_reset();
  tiempo_secado = _tiempo_secado;       // 1
  tiempo_cd = _tiempo_cooldown;         // 2
  precio_temp1 = _precio_temp1;         // 4
  temperatura_baja = _temperatura_baja; // 7
  temperatura_alta = _temperatura_alta; // 8
  valor_moneda = _valor_moneda;
  TIEMPO1 = tiempo_secado + tiempo_cd;
  segundostotal = TIEMPO1 * 60;
  tiempo_adicional = _tiempo_adicional;
  if (precio_temp1 > 0)
  {
    cont = 2;
  }
}
void tiempo()
{
  wdt_reset();
  if ((millis() / 1000) != _datoAnterior22)
  {
    segundostotal--;
  }
  _datoAnterior22 = (millis() / 1000);
  ahoras = ((segundostotal / 60) / 60);
  aminutos = (segundostotal / 60) % 60;
  asegundos = segundostotal % 60;
  // ddisplay.setSegments(SEG_P);
  //  ddisplay.showNumberDec(var, false, 1, 1); // Expect: __04

  if (aminutos != aminutosAnterior)
  {
    // La modificacion para enviar  tiempo, tempertura&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
    datos[cambiosAminutos].aminutos = aminutos;
    datos[cambiosAminutos].temperatura = TEMPERATURA;

    cambiosAminutos++;

    if (cambiosAminutos == NUM_PARES)
    {
      // Enviar el arreglo completo por serial
      String cadena = "<" + String(temp_boton) + "," + String(paso_sec) + "," + String(datos[NUM_PARES - 1].aminutos) + "," + String(datos[NUM_PARES - 1].temperatura) + ">\n";
      Serial3.println(cadena);
      cambiosAminutos = 0; // Reiniciar contador
    }
    //&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
    aminutosAnterior = aminutos;
  }

  if (b1.isPressed())
  {
    temp_boton = 1;
    ddisplay.showNumberDec(1, false, 1, 1); // Expect: __04
    temperatura_1(1);
    temperatura_baja = _temperatura_baja; // 7
    temperatura_alta = _temperatura_alta; // 8
  }
  if (b2.isPressed())
  {
    temp_boton = 2;
    ddisplay.showNumberDec(2, false, 1, 1); // Expect: __04
    temperatura_1(2);
    temperatura_baja = _temperatura_baja; // 7
    temperatura_alta = _temperatura_alta; // 8
  }
  if (b3.isPressed())
  {
    temp_boton = 3;
    ddisplay.showNumberDec(3, false, 1, 1); // Expect: __04
    temperatura_1(3);
    temperatura_baja = _temperatura_baja; // 7
    temperatura_alta = _temperatura_alta; // 8
  }
  if (aminutos > 0)
  {
    display.showNumberDec(aminutos, true, 2, 0); // Expect: __04
  }
  else
  {
    display.showNumberDec(asegundos, true, 2, 0); // Expect: __04
  }
  if (segundostotal <= 0)
  {
    while (1)
    {
      if (!instruccionEnviada2)
      {
        ciclo_str = 7;
        etapa_str = 0;
        paso_str = 0;
        String estado_lavadora = "[" + String(ciclo_str) + "," + String(etapa_str) + "," + String(paso_str) + "]\n";
        Serial3.print(estado_lavadora);
        instruccionEnviada2 = true;
      }
    }
  }
}
void cases_1()
{
  wdt_reset();
  if (b1.isPressed())
  {
    door = LOW;
    var = 1;
    ddisplay.showNumberDec(1, false, 1, 1); // Expect: __04
    temperatura_1(1);
    aux10 = 1;
    digitalWrite(ignitor, HIGH);
    digitalWrite(motor, HIGH);
    // b1.led_on();
  }
  if (b2.isPressed())
  {
    door = LOW;
    var = 2;
    ddisplay.showNumberDec(2, false, 1, 1); // Expect: __04
    temperatura_1(2);
    digitalWrite(ignitor, HIGH);
    digitalWrite(motor, HIGH);
    // b2.led_on();
    aux10 = 2;
  }
  if (b3.isPressed())
  {
    door = LOW;
    var = 3;
    ddisplay.showNumberDec(3, false, 1, 1); // Expect: __04
    temperatura_1(3);
    digitalWrite(ignitor, HIGH);
    digitalWrite(motor, HIGH);
    //  b3.led_on();
    aux10 = 3;
  }
}
void cases()
{
  wdt_reset();
  if (Serial3.available() > 0)
  {
    recived = Serial3.readString();
     //Serial.print(recived);
    int startPos = recived.indexOf('{');
    int endPos = recived.indexOf('}');
    if (startPos >= 0 && endPos >= 0)
    {
      String numbersString = recived.substring(startPos + 1, endPos);

      String numbersArray[4]; // 4 números
      int count = 0;
      char separator = ',';
      int prevIndex = 0;
      int nextIndex;
      do
      {
        nextIndex = numbersString.indexOf(separator, prevIndex);
        if (nextIndex != -1)
        {
          numbersArray[count] = numbersString.substring(prevIndex, nextIndex);
          prevIndex = nextIndex + 1;
        }
        else
        {
          numbersArray[count] = numbersString.substring(prevIndex);
        }

        count++;
      } while (nextIndex != -1 && count < 4); // Cambia 4 al número de elementos esperados

      // Ahora tienes cada número en el array numbersArray
      for (int i = 0; i < count; i++)
      {
        //  Serial.print("Número ");
        //  Serial.print(i + 1);
        //   Serial.print(": ");
        //   Serial.println(numbersArray[i].toInt());
      }
      if (numbersArray[0].toInt() > 0)
      {
        door = LOW;
        // cont++;
        price = precio_temp1;
        aux11 = 1;
        // Serial3.println("OK");
        // Serial3.println("02");
        temperatura_1(aa);
        par();
        tiempo();
        var = 4;
        digitalWrite(motor, HIGH);
        digitalWrite(ignitor, HIGH);
      }
    }
  }
  if ((millis() - lastTime2) > timerDelay2) // reset de la tarjeta cada 5mins
  {
    while (1)
    {
    }
    // asm volatile(" jmp 0");
    lastTime2 = millis();
  }

  if (digitalRead(puerta) == 0) // 0 puerta cerrada 1 puerta abierta
  {
    if ((millis() - lastTime) > 1000)
    {
      lastTime = millis();
    }

    // ddisplay.showNumberDec(var, false, 1, 1);
    if (b1.isPressed())
    {
      //   b1.led_on();
      door = LOW;
#if (opl == 0)
      cont++;
#endif
      var = 1;
      ddisplay.showNumberDec(1, false, 1, 1);
      temperatura_1(1);
      par();
      price = precio_temp1;
      aux11 = 1;
    }
    if (b2.isPressed())
    {
      //  b2.led_on();
      door = LOW;
#if (opl == 0)
      cont++;
#endif
      var = 2;
      ddisplay.showNumberDec(2, false, 1, 1);
      temperatura_1(2);
      par();
      price = precio_temp1;
      aux11 = 2;
    }
    if (b3.isPressed())
    {
      //   b3.led_on();
      door = LOW;

#if (opl == 0)
      cont++;
#endif
      var = 3;
      ddisplay.showNumberDec(3, false, 1, 1);
      temperatura_1(3);
      par();
      price = precio_temp1;
      aux11 = 3;
    }
    if (cont % 2 == 1)
    {
      display.setSegments(SEG);
    }
    else
    {

      dato_moneda = digitalRead(moneda_);
      if (dato_moneda == HIGH && datoAnterior_moneda == LOW || price == 0)
      {
        price = price - valor_moneda;
        b1.ton3();
        if (price <= 0)
        {
          price = 0;
          var = 4;
          digitalWrite(motor, HIGH);
          digitalWrite(ignitor, HIGH);
        }
      }
      datoAnterior_moneda = dato_moneda;
      display.showNumberDec(price, true, 2, 0);
    }
  }
  else
  {
    door = HIGH;
    ddisplay.showNumberDec(var, false, 1, 1);

    ////////////////////////////////////////////////////////

    if (cont % 2 == 1)
    {
      display.setSegments(SEG_dr);
    }
    else
    {

      dato_moneda = digitalRead(moneda_);
      if (dato_moneda == HIGH && datoAnterior_moneda == LOW || price == 0)
      {
        price = price - valor_moneda;
        b1.ton3();
        if (price <= 0)
        {
          price = 0;
          var = 4;
          digitalWrite(motor, HIGH);
          digitalWrite(ignitor, HIGH);
        }
      }
      datoAnterior_moneda = dato_moneda;
      display.showNumberDec(price, true, 2, 0);
    }

    /////////////////////////////////////////////////////
  }
}
void setup()
{
  wdt_disable();
#if eeprom == 1
  paramreos_eeprom();
#endif
  Serial.begin(9600);
  Serial3.begin(115200);
  // Serial.print("aa");
  pinMode(moneda_, INPUT_PULLUP);
  pinMode(puerta, INPUT_PULLUP);
  pinMode(ignitor, OUTPUT);
  pinMode(motor, OUTPUT);
  digitalWrite(ignitor, LOW);
  digitalWrite(motor, LOW);
  ddisplay.clear();
  ddisplay.setBrightness(0x0f);
  display.clear();
  display.setBrightness(0x0f);
  ddisplay.setSegments(SEG_P); // muestra la T de la temperatura

  door = LOW;

#if (opl == 0)
  var = 3;
#endif
  temp_boton = 3;
  temperatura_1(3);
  par();
  price = precio_temp1;
  TIEMPO1 = tiempo_secado + tiempo_cd;
  aux11 = 1;
  if (b2.isPressed())
  {
    var = 5;
    ddisplay.setSegments(SEG);
    display.setSegments(SEG);
    delay(1000);
  }
  // EEPROM.update(14, 65); // temp baja
  //  EEPROM.update(15, 85); // temp alta
  // Serial.println("INICIO5");
  wdt_enable(WDTO_8S);
}
void loop()
{
  switch (var)
  {
  case 1:
    cases();
    break;
  case 2:
    cases();
    break;
  case 3:
    cases();
    break;
  case 4:
    tiempo();
    instruccionEnviada4 = false;
    instruccionEnviada3 = false;
    /////////////inicio del ciclo
    if (!instruccionEnviada)
    {

      ciclo_str = 0;
      etapa_str = 0;
      paso_str = 0;
      String estado_lavadora = "[" + String(ciclo_str) + "," + String(etapa_str) + "," + String(paso_str) + "]\n";
      Serial3.print(estado_lavadora);
      instruccionEnviada = true;
    }

    if (segundostotal > 0)
    {

      if (segundostotal - tiempo_cd * 60 >= 0)
      {
        paso_sec = 1;
        // Serial.println(segundostotal - tiempo_cd * 60);
        if (TEMPERATURA > temperatura_alta)
        {
          digitalWrite(ignitor, LOW);
        }
        if (TEMPERATURA < temperatura_baja)
        {
          digitalWrite(ignitor, HIGH);
        }
      }
      else
      {
        paso_sec = 2;
        digitalWrite(ignitor, LOW);
      }

      unsigned long interval = 1000; // Intervalo de 1 segundo entre cada envío
      unsigned long previousMillis = millis();
      if (aux112 == 0)
      {
        for (int i = 0; i < 4; i++)
        {
          Serial3.print("ok\n"); // Envía el comando "01"
                                 //  Serial.print("ok\n");  // Envía el comando "01"
          // Espera hasta que haya pasado 1 segundo
          while (millis() - previousMillis < interval)
          {
            // No hacer nada aquí, solo esperar
          }
          if (i == 3)
          {
            aux112 = 1;
          }
        }
        previousMillis = millis(); // Actualiza el tiempo para la próxima espera
      }
    }
    else // final de ciclo
    {
      // Serial3.println("OK");
      // Serial3.println("02");
      digitalWrite(ignitor, LOW);
      digitalWrite(motor, LOW);
      b1.ton2();
      ddisplay.setSegments(SEG_P);
      cont = 1;
      b3.led_off();
      b2.led_off();
      b1.led_off();
      aux11 = 1;
      door = LOW;
      var = 1;
      temperatura_1(1);
      par();
      price = precio_temp1;
      aux12 = 0;
      aux112 = 0;

      delay(1000);

      while (1)
      {
      }
    }
    if ((millis() - lastTime) > timerDelay)
    {
      Vo = analogRead(A0);
      R2 = R1 * (1023.0 / (float)Vo - 1.0);
      logR2 = log(R2);
      TEMPERATURA = (1.0 / (A + B * logR2 + C * logR2 * logR2 * logR2));
      TEMPERATURA = round(TEMPERATURA - 273.15);
      //  Serial.println(TEMPERATURA);
      //  Serial.print("temperatura_alta:");
      //  Serial.println(temperatura_alta);
      //  Serial.print("temperatura_baja:");
      //   Serial.println(temperatura_baja);
      lastTime = millis();
    }

    if (digitalRead(puerta) == 1) // 0 puerta cerrada 1 puerta abierta
    {
      var = 6;
      door = HIGH;
    }
    break;
  case 5: // programacion
    wdt_reset();
    if (b1.isPressed())
    {
      if (aux4 == 0)
      {
        aux2++;
        if (aux2 >= 22)
        {
          aux2 = 1;
          var = 1;
          delay(1000);
          while (1)
          {
          }
        }
        display.showNumberDec(aux2, true, 2, 0);
        ddisplay.setSegments(SEG_PROG);
      }
      else
      {
        aux3++;
        if (aux3 >= 90)
        {
          aux3 = 90;
        }
        display.showNumberDec(aux3, true, 2, 0);
        ddisplay.setSegments(SEG);
      }
    }
    if (b2.isPressed())
    {
      if (aux4 == 0)
      {
        aux2--;
        if (aux2 <= 1)
        {
          aux2 = 1;
        }
        display.showNumberDec(aux2, true, 2, 0);
        ddisplay.setSegments(SEG_PROG);
      }
      else
      {
        aux3--;
        if (aux3 <= 0)
        {
          aux3 = 0;
        }
        ddisplay.setSegments(SEG);
        display.showNumberDec(aux3, true, 2, 0);
      }
    }
    if (b3.isPressed())
    {
      if (aux5 == 0)
      {
        aux3 = EEPROM.read(aux2);
        display.showNumberDec(aux3, true, 2, 0);
        ddisplay.setSegments(SEG);
        aux4 = 1;
        aux5 = 1;
      }
      else
      {
        EEPROM.update(aux2, aux3);
        delay(500);
        display.showNumberDec(aux2, true, 2, 0);
        aux5 = 0;
        aux4 = 0;
        ddisplay.setSegments(SEG_PROG);
      }
    }
    //////////////////////////////////
    break;
  case 6:
    cases_1();
    if (digitalRead(puerta) == 0 && door == LOW) // 0 puerta cerrada 1 puerta abierta
    {
      digitalWrite(ignitor, HIGH);
      digitalWrite(motor, HIGH);
      var = 4; // faj
    }
    else
    {
      var = 6;
      digitalWrite(ignitor, LOW);
      digitalWrite(motor, LOW);
    }
    if (digitalRead(puerta) == 0)
    {
      tiempo();
      ////
      if (Serial3.available() > 0)
      {
        recived = Serial3.readString();
       // Serial.print(recived);
        int startPos = recived.indexOf('{');
        int endPos = recived.indexOf('}');
        if (startPos >= 0 && endPos >= 0)
        {
          String numbersString = recived.substring(startPos + 1, endPos);

          String numbersArray[4]; // 4 números
          int count = 0;
          char separator = ',';
          int prevIndex = 0;
          int nextIndex;
          do
          {
            wdt_reset();
            nextIndex = numbersString.indexOf(separator, prevIndex);
            if (nextIndex != -1)
            {
              numbersArray[count] = numbersString.substring(prevIndex, nextIndex);
              prevIndex = nextIndex + 1;
            }
            else
            {
              numbersArray[count] = numbersString.substring(prevIndex);
            }

            count++;
          } while (nextIndex != -1 && count < 4); // Cambia 4 al número de elementos esperados

          // Ahora tienes cada número en el array numbersArray
          for (int i = 0; i < count; i++)
          {
            wdt_reset();
            //   Serial.print("Número ");
            //   Serial.print(i + 1);
            //   Serial.print(": ");
            //   Serial.println(numbersArray[i].toInt());
          }
          if (numbersArray[0].toInt() > 0)
          {
            // Serial3.println("OK");
            // Serial3.println("02");
            // segundostotal = segundostotal + (tiempo_adicional * 60);
          }
        }
      }
      dato_moneda = digitalRead(moneda_);
      if (dato_moneda == HIGH && datoAnterior_moneda == LOW)
      {
        segundostotal = segundostotal + (tiempo_adicional * 60);
        b1.ton3();
      }
      datoAnterior_moneda = dato_moneda;
      //////////////////
      if (segundostotal <= 0)
      {
        digitalWrite(ignitor, LOW);
        digitalWrite(motor, LOW);
        b1.ton2();
        ddisplay.setSegments(SEG_P);
        cont = 1;
        b3.led_off();
        b2.led_off();
        b1.led_off();
        door = LOW;
        aux11 = 1;
        door = LOW;
        var = 1;
        temperatura_1(1);
        par();
        price = precio_temp1;
        aux12 = 0;
        delay(1000);
        while (1)
        {
          /* code */
        }
      }
    }
    else
    {
      display.setSegments(SEG_dr);

      b3.led_off();
      b2.led_off();
      b1.led_off();
    }
    break;
  default:
    wdt_reset();

    recived = "";
    while (var != 4)
    {
      // Serial.println((millis() - lastTime2));
      /////////////////////////////////////
      if ((millis() - lastTime2) >= timerDelay2) // reset de la tarjeta cada 5mins
      {
        // Serial.print("aa");
        while (1)
        {
        }
        // asm volatile(" jmp 0");
        lastTime2 = millis();
      }
      //////////////////////////////////////
      wdt_reset();
      if (Serial3.available() > 0)
      {
        
        char c = Serial3.read();
        recived += c;
       // Serial.print(c);
        if (c == '}')
        {
          break;
        }
      }
    }
    int startPos = recived.indexOf('{');
    int endPos = recived.indexOf('}');
    if (startPos >= 0 && endPos >= 0)
    {
      String numbersString = recived.substring(startPos + 1, endPos);

      String numbersArray[4]; // 4 números
      int count = 0;
      char separator = ',';
      int prevIndex = 0;
      int nextIndex;
      do
      {
        wdt_reset();
        nextIndex = numbersString.indexOf(separator, prevIndex);
        if (nextIndex != -1)
        {
          numbersArray[count] = numbersString.substring(prevIndex, nextIndex);
          prevIndex = nextIndex + 1;
        }
        else
        {
          numbersArray[count] = numbersString.substring(prevIndex);
        }

        count++;
      } while (nextIndex != -1 && count < 4); // Cambia 4 al número de elementos esperados

      if (numbersArray[0].toInt() > 0)
      {
        door = LOW;
        // cont++;
        price = precio_temp1;
        aux11 = 1;
        temp_boton = 3;
        temperatura_1(3); // temperatura alta por defecto le puse 3, ya esta modificado para lo que venga del mqtt
        par();

#if (ciclo == 1)
        segundostotal = (TIEMPO1 * 60);
#endif

#if (ciclo == 0)
        segundostotal = (TIEMPO1 * 60) * numbersArray[0].toInt();
#endif
        tiempo();
        var = 4;
        digitalWrite(motor, HIGH);
        digitalWrite(ignitor, HIGH);
      }
    }

    break;
  }
}