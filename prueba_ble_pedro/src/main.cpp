#include <Arduino.h>
#include <avr/wdt.h>

#define enable_bill 9
//enable normal 8 pcb 9
#define pulsos_bill 11
//pulses normal 10 pcb 8
#define cinco A2
#define diez A5
#define h5 A8
#define h10 A11
#define motor5a A0
//31 pcb normal4
#define motor5b A1
//32 pcb normal3
#define motor10a A3
//33 pcb normal2
#define motor10b A4
//34 pcb normal 1
#define buzz 46
//46 pcb  no hay en normal

#define ble_gnd 36

int resetqr = 0;
int moneda20 = 0;
int moneda50 = 0;
int moneda100 = 0;
int moneda200 = 0;
int moneda500 = 0;
String input = "";

int tim = 0, datotim, datoAnteriortim, contadortim, contadorreset_bill = 0;
int dato = 0, datoreset_bill, datoAnterior = 0, contador = 0;
int dato3 = 0, datoAnterior3 = 0, contador3 = 0, datoAnteriorreset_bill = 0;
int contador4 = 0, contadore = 0;
unsigned long time, dato2 = 0, datoAnterior2 = 0, contador2 = 0;
int datoe_m, datoAnteriore_m = 0;
int datoqr, datoAnteriorqr = 0, contadorqr = 0;
char recived;
String recived2;
int monedas = 0, k = 20;
unsigned int tiempo = 0;
bool w = 0, x = 0, m = 0, n = 0, nn = 0;
int frecuencia = 2000, contadort = 0;
;
bool connectionAccepted = false, runTime = false, stopp = 1, z = 0, reset_bills = 0;

void killSession()
{
  tiempo = 0;
  connectionAccepted = false;
  runTime = false;
  //asm volatile("  jmp 0");
  while (1)
  {
  };
}
void killConnection()
{
  killSession();
}
void acceptTime()
{
  tiempo = 0;
  tiempo = millis() / 1000;
  runTime = true;
}
void compareToken()
{
  if ((input.substring(0, 80)).equals("2228d9836739691c36f1cc149ae15564asfd616335664e9191ee7b9f348d3a8a782fa8412671e273"))
  {
    Serial.print("token_mio");
    Serial.print(input);
    connectionAccepted = 1;
    k = 30;
    reset_bills = 1;
    digitalWrite(enable_bill, LOW);
    Serial3.write("0200");
  }
  else
  {
    Serial3.write("401");
    killConnection();
  }
}
void setup()
{
  wdt_disable();
  Serial3.begin(9600); //nextion
  Serial2.begin(9600); // blutubi
  Serial1.begin(9600); //esclavo cambiador
  Serial.begin(9600);
  Serial.println("inicio");

  pinMode(ble_gnd, OUTPUT);
  digitalWrite(ble_gnd, LOW);
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  wdt_enable(WDTO_8S);
  tone(buzz, frecuencia);
  delay(250);
  noTone(buzz);
}
void reversa_monedas5()
{
  digitalWrite(motor5a, HIGH);
  digitalWrite(motor5b, LOW);
  digitalWrite(cinco, HIGH);
}

void reversa_monedas10()
{
  digitalWrite(motor10a, HIGH);
  digitalWrite(motor10b, LOW);
  digitalWrite(diez, HIGH);
}

void apagado5()
{
  digitalWrite(motor5a, LOW);
  digitalWrite(motor5b, LOW);
  digitalWrite(cinco, LOW);
}

void apagado10()
{
  digitalWrite(motor10a, LOW);
  digitalWrite(motor10b, LOW);
  digitalWrite(diez, LOW);
}

void encendido5()
{
  digitalWrite(motor5a, LOW);
  digitalWrite(motor5b, HIGH);
  digitalWrite(cinco, HIGH);
}
void encendido10()
{
  digitalWrite(motor10a, LOW);
  digitalWrite(motor10b, HIGH);
  digitalWrite(diez, HIGH);
}
void loop()
{
  if (k == 0)
  {

    if (Serial.available() > 0)
    {
      recived = Serial.read();
      if (recived == '1')
      {
        Serial.println("ON_BLE");
        tone(buzz, frecuencia);
        digitalWrite(ble_gnd, HIGH);
        k = 20;
        recived = ' ';
      }
    }
  }
  if (k == 20)
  {
    if (Serial3.available())
    {
      Serial.println("entro en blutubi");
      input = Serial3.readString();
      // delay(100);
      Serial.print("INPUT_valor:");
      Serial.println((input.substring(0, 7)));
      Serial.print("input_longitud:");
      Serial.print(input.length());
      if (!Serial3.available())
      {
        Serial.print(input);
        Serial.print(input.length());
        if ((input.substring(0, 7)).equals("OK+CONN"))
        {
          Serial.print("aceptime");
          acceptTime();
        }
        else if (input.equals("OK+LOST") && connectionAccepted)
        {
          Serial.println("kill_sesion");
          killSession();
        }
        else
        {
          Serial.println("remplace_input");
          input.replace("OK+CONN", "");
          input.replace("OK+LOST", "");
          input.replace("OK", "");
          if (!input.equals("") && !connectionAccepted)
          {
            Serial.print("comparacion");
            compareToken();
            Serial.print(input);
          }
          else if (connectionAccepted)
          {
            Serial.println(input);
            Serial.println(input.length());
            input == "100" ? digitalWrite(13, HIGH) : digitalWrite(13, LOW);
            if (input == "100")
            {
              resetqr = 0;
              //            while (1)
              //            {
              //              Serial.println("si jalo");
              //            };
            }
            else
            {
              //            Serial.println("no jalo");
              while (1)
              {
              }
              //            while (1)
              //            {
              //            };
            }
            // killConnection();
          }
        }
      }
    }
  }

  if (k == 30)
  {
    delay(1000);
    Serial3.print(150);
    delay(2000);
    Serial3.write("END");
    while (1)
    {
    }
    /* while (Serial.available() > 0)
    {
      recived2 = Serial.readString();
      if (recived2 == "0")
      {
        Serial3.write("END");
        while (1)
        {
        }
      }
      else
      {
        Serial3.print(recived2);
        delay(1000);
      }
    }*/
  }
  if (!connectionAccepted && runTime)
  {
    if ((millis() / 1000) > tiempo + 5)
    {
      //asm volatile("  jmp 0");
      killConnection();
    }
  }
  wdt_reset();
}