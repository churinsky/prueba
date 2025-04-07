#include <Arduino.h>
#include <avr/wdt.h>

#define enable_bill 9
//enable normal 8 pcb 9
#define pulsos_bill 8
//pulses normal 10 pcb 8
#define cinco A2
#define diez A5
#define h5 A8
#define h10 A11
#define motor5a 31
//31 pcb normal4
#define motor5b 32
//32 pcb normal3
#define motor10a 33
//33 pcb normal2
#define motor10b 34
//34 pcb normal 1
#define buzz 46
//46 pcb  no hay en normal
#define ble_vcc 7
#define ble_gnd 6

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
String recived;
int monedas = 0, k = 0;
unsigned int tiempo = 0;
bool w = 0, x = 0, m = 0, n = 0, nn = 0;
int frecuencia = 2000, contadort = 0;
;
bool connectionAccepted = false, runTime = false, stopp = 1, z = 0, reset_bills = 0;
void valores()
{
  Serial3.print("n0.val=");
  Serial3.print(contadort * 10);
  Serial3.write(0xff);
  Serial3.write(0xff);
  Serial3.write(0xff);

  Serial3.print("n10.val=");
  Serial3.print(moneda20);
  Serial3.write(0xff);
  Serial3.write(0xff);
  Serial3.write(0xff);

  Serial3.print("n5.val=");
  Serial3.print(moneda20 * 20);
  Serial3.write(0xff);
  Serial3.write(0xff);
  Serial3.write(0xff);

  Serial3.print("n9.val=");
  Serial3.print(moneda50);
  Serial3.write(0xff);
  Serial3.write(0xff);
  Serial3.write(0xff);

  Serial3.print("n4.val=");
  Serial3.print(moneda50 * 50);
  Serial3.write(0xff);
  Serial3.write(0xff);
  Serial3.write(0xff);

  Serial3.print("n8.val=");
  Serial3.print(moneda100);
  Serial3.write(0xff);
  Serial3.write(0xff);
  Serial3.write(0xff);

  Serial3.print("n3.val=");
  Serial3.print(moneda100 * 100);
  Serial3.write(0xff);
  Serial3.write(0xff);
  Serial3.write(0xff);

  Serial3.print("n7.val=");
  Serial3.print(moneda200);
  Serial3.write(0xff);
  Serial3.write(0xff);
  Serial3.write(0xff);

  Serial3.print("n2.val=");
  Serial3.print(moneda200 * 200);
  Serial3.write(0xff);
  Serial3.write(0xff);
  Serial3.write(0xff);

  Serial3.print("tsw b0,1");
  Serial3.write(0xff);
  Serial3.write(0xff);
  Serial3.write(0xff);
}

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
  if ((input.substring(0, 80)).equals("1118d9836739691c36f1cc149ae15564asfd616335664e9191ee7b9f348d3a8a782fa8412671e273"))
  {
    Serial.print("token_mio");
    Serial.print(input);
    connectionAccepted = 1;
    k = 30;
    reset_bills = 1;
    digitalWrite(enable_bill, LOW);
    Serial3.print("page 2");
    Serial3.write(0xff);
    Serial3.write(0xff);
    Serial3.write(0xff);
    Serial2.write("0200");
  }
  else
  {
    Serial2.write("401");
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
  pinMode(enable_bill, OUTPUT);
  digitalWrite(enable_bill, HIGH);
  //cuenta los pulsos que vienen del billetro
  pinMode(pulsos_bill, INPUT);
  //cuentan las monedas
  pinMode(h5, INPUT);
  pinMode(h10, INPUT);
  //activa el hooper de 10p
  pinMode(diez, OUTPUT);
  digitalWrite(diez, LOW);
  //activa el hooper de 5p
  pinMode(cinco, OUTPUT);
  digitalWrite(cinco, LOW);
  pinMode(motor10a, OUTPUT);
  pinMode(motor10b, OUTPUT);
  //activa el hooper de 5p
  pinMode(motor5a, OUTPUT);
  pinMode(motor5b, OUTPUT);

  pinMode(ble_vcc, OUTPUT);
  pinMode(ble_gnd, OUTPUT);
  digitalWrite(ble_gnd, HIGH);
  digitalWrite(ble_vcc, HIGH);

  Serial3.print("page 0");
  Serial3.write(0xff);
  Serial3.write(0xff);
  Serial3.write(0xff);
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
    if (Serial3.available() > 0)
    {
      recived = Serial3.readString();
      if (int(recived[0]) == 0)
      {
        digitalWrite(enable_bill, LOW);
        apagado5();
        apagado10();
        Serial3.print("page 1");
        Serial3.write(0xff);
        Serial3.write(0xff);
        Serial3.write(0xff);
        tone(buzz, frecuencia);
        delay(250);
        noTone(buzz);
        k = 10;
      }
      if (int(recived[0]) == 1)
      {
        Serial3.print("page 1");
        Serial3.write(0xff);
        Serial3.write(0xff);
        Serial3.write(0xff);
        tone(buzz, frecuencia);
        delay(250);
        noTone(buzz);
        k = 20;
        resetqr = 1;
        digitalWrite(ble_gnd, LOW);
      }
    }
  }
  if (k == 10)
  {
    apagado5();
    apagado10();
    if (Serial3.available() > 0)
    {
      recived = Serial3.readString();
      if (int(recived[0]) == 4)
      {
        while (1)
        {
          digitalWrite(enable_bill, HIGH);
          ;
        }
      }
    }

    time = millis() / 1000;
    dato2 = time;
    if (dato2 != datoAnterior2 && w == 1) //W EMPIEZA EN 0 Y PASA A LA ETAPA DE MOSTRAR LA LECTURA DEL BILLETE EN NEXTION
    {
      tim = 0;
      contador2++;
      if (contador2 == 4)
      {
        contador2 = 0;
        x = 1;
        w = 0;
      }
    }
    datoAnterior2 = dato2;
    datotim = time;
    if (datotim != datoAnteriortim && tim == 1) //rreset pantalla
    {
      contadortim++;
      if (contadortim >= 1800)
      {
        while (1)
        {
          digitalWrite(enable_bill, HIGH);
          tone(buzz, frecuencia);
          delay(1000);
          noTone(buzz);
          delay(1000);
        }
      }
    }
    datoAnteriortim = datotim;
    dato = digitalRead(pulsos_bill);

    if (dato == 0 && datoAnterior == 1) //CUENTA LOS PULSOS 1 PULSO CADA $10
    {
      contador++;
      digitalWrite(enable_bill, HIGH);
      w = 1;
    }

    datoAnterior = dato;

    if (x == 1) //X EMPIEZA EN 0
    {
      if (contador < 1)
      {
        apagado5();
        apagado10();
      }
      else if (contador > 10)
      {
        apagado5();
        apagado10();
        contador = 20;
        delay(200);
      }
      else if (contador > 1 && contador <= 2)
      {
        apagado5();
        apagado10();
        contador = 2;
        delay(200);
      }
      else if (contador > 2 && contador <= 5)
      {
        apagado5();
        apagado10();
        contador = 5;
        delay(200);
      }
      else if (contador > 5 && contador <= 10)
      {
        apagado5();
        apagado10();
        contador = 10;
        delay(200);
      }

      Serial3.print("n0.val=");
      Serial3.print(contador * 10);
      Serial3.write(0xff);
      Serial3.write(0xff);
      Serial3.write(0xff);

      tone(buzz, frecuencia);
      delay(250);
      Serial1.write(contador * 10);
      Serial1.write(5);
      Serial1.write(1);
      noTone(buzz);
      x = 0;
      m = 1;
      w = 0;
      nn = 0;
      k = 1;
    }

    if (m == 1 && nn == 0) // m empieza en 0 y nn en 1
    {
      nn = 1;
      w = 0;
    }

    if (int(recived[0]) == 0)
    {

      if (contador == 0)
      {
        tim = 1;
      }
    }
  }

  if (k == 1)
  {
    if (contador == 0)
    {
      apagado10();
      apagado5();
      for (;;)
      {
        tone(buzz, frecuencia);
        delay(250);
        noTone(buzz);
        Serial3.print("page 2");
        Serial3.write(0xff);
        Serial3.write(0xff);
        Serial3.write(0xff);
        delay(10000);
      }
      dato = 0;
      datoAnterior = 0;
      contador = 0;
      dato3 = 0;
      datoAnterior3 = 0;
      contador3 = 0;
      contador4 = 0;
      contadore = 0;
      time = 0;
      dato2 = 0;
      datoAnterior2 = 0;
      contador2 = 0;
      datoe_m = 0;
      datoAnteriore_m = 0;
      monedas = 0;
      w = 0;
      x = 0;
      m = 0;
      n = 0;
      nn = 0;
    }
    if (contador > 0 && contador < 20)
    {
      dato3 = digitalRead(h5);
      time = millis() / 1000;
      datoe_m = time;
      if (datoe_m != datoAnteriore_m)
      {
        contadore++;
        if (contadore <= 20)
        {
          encendido5();
        }
        if (contadore > 20 && contadore <= 30)
        {
          reversa_monedas5();
        }
        if (contadore > 30 && contadore <= 40)
        {
          encendido5();
        }
        if (contadore > 40)
        {
          apagado10();
          apagado5();
          for (;;)
          {
            tone(buzz, frecuencia);
            delay(250);
            noTone(buzz);
            Serial3.print("page 2");
            Serial3.write(0xff);
            Serial3.write(0xff);
            Serial3.write(0xff);
            delay(10000);
          }
          dato = 0;
          datoAnterior = 0;
          contador = 0;
          dato3 = 0;
          datoAnterior3 = 0;
          contador3 = 0;
          contador4 = 0;
          contadore = 0;
          time = 0;
          dato2 = 0;
          datoAnterior2 = 0;
          contador2 = 0;
          datoe_m = 0;
          datoAnteriore_m = 0;
          monedas = 0;
          w = 0;
          x = 0;
          m = 0;
          n = 0;
          nn = 0;
        }
      }
      datoAnteriore_m = datoe_m;
      if (dato3 == 0 && datoAnterior3 == 1)
      {
        apagado10();
        apagado5();
        delay(200);
        contador3++;
        contadore = 0;

        if (contador3 >= 1)//// ES DOS PARA $5
        {
          contador3 = 0;
          contador4++;
          k = 2;
          if (contador4 >= contador)
          {
            apagado10();
            apagado5();
            for (;;)
            {
              Serial3.print("page 2");
              Serial3.write(0xff);
              Serial3.write(0xff);
              Serial3.write(0xff);
              delay(10000);
            }
            dato = 0;
            datoAnterior = 0;
            contador = 0;
            dato3 = 0;
            datoAnterior3 = 0;
            contador3 = 0;
            contador4 = 0;
            contadore = 0;
            time = 0;
            dato2 = 0;
            datoAnterior2 = 0;
            contador2 = 0;
            datoe_m = 0;
            datoAnteriore_m = 0;
            monedas = 0;
            w = 0;
            x = 0;
            m = 0;
            n = 0;
            nn = 0;
          }
        }
      }
      datoAnterior3 = dato3;
    }
    else
    {
      dato3 = digitalRead(h5);
      time = millis() / 1000;
      datoe_m = time;
      if (datoe_m != datoAnteriore_m)
      {
        contadore++;
        if (contadore <= 20)
        {
          encendido5();
        }
        if (contadore > 20 && contadore <= 30)
        {
          reversa_monedas5();
        }
        if (contadore > 30 && contadore <= 40)
        {
          encendido5();
        }
        if (contadore > 40)
        {
          apagado10();
          apagado5();
          for (;;)
          {
            Serial3.print("page 2");
            Serial3.write(0xff);
            Serial3.write(0xff);
            Serial3.write(0xff);
            delay(10000);
          }
          dato = 0;
          datoAnterior = 0;
          contador = 0;
          dato3 = 0;
          datoAnterior3 = 0;
          contador3 = 0;
          contador4 = 0;
          contadore = 0;
          time = 0;
          dato2 = 0;
          datoAnterior2 = 0;
          contador2 = 0;
          datoe_m = 0;
          datoAnteriore_m = 0;
          monedas = 0;
          w = 0;
          x = 0;
          m = 0;
          n = 0;
          nn = 0;
        }
      }
      datoAnteriore_m = datoe_m;

      if (dato3 == 0 && datoAnterior3 == 1)
      {
        apagado10();
        apagado5();
        delay(200);
        contador3++;
        contadore = 0;

        if (contador3 >= 10)
        {
          contador3 = 0;
          contador4++;
          k = 2;
          if (contador4 >= 2)//ES UN 4 PARA MONEDAS DE 5
          {
            apagado10();
            apagado5();
            for (;;)
            {
              tone(buzz, frecuencia);
              delay(250);
              noTone(buzz);
              Serial3.print("page 2");
              Serial3.write(0xff);
              Serial3.write(0xff);
              Serial3.write(0xff);
              delay(10000);
            }
            dato = 0;
            datoAnterior = 0;
            contador = 0;
            dato3 = 0;
            datoAnterior3 = 0;
            contador3 = 0;
            contador4 = 0;
            contadore = 0;
            time = 0;
            dato2 = 0;
            datoAnterior2 = 0;
            contador2 = 0;
            datoe_m = 0;
            datoAnteriore_m = 0;
            monedas = 0;
            w = 0;
            x = 0;
            m = 0;
            n = 0;
            nn = 0;
          }
        }
      }
      datoAnterior3 = dato3;
    }
  }

  if (k == 2)
  {
    if (contador == 0)
    {
      apagado10();
      apagado5();
      for (;;)
      {
        tone(buzz, frecuencia);
        delay(250);
        noTone(buzz);
        Serial3.print("page 2");
        Serial3.write(0xff);
        Serial3.write(0xff);
        Serial3.write(0xff);
        delay(10000);
      }
      dato = 0;
      datoAnterior = 0;
      contador = 0;
      dato3 = 0;
      datoAnterior3 = 0;
      contador3 = 0;
      contador4 = 0;
      contadore = 0;
      time = 0;
      dato2 = 0;
      datoAnterior2 = 0;
      contador2 = 0;
      datoe_m = 0;
      datoAnteriore_m = 0;
      monedas = 0;
      w = 0;
      x = 0;
      m = 0;
      n = 0;
      nn = 0;
    }
    if (contador > 0 && contador < 20)
    {
      dato3 = digitalRead(h10);
      time = millis() / 1000;
      datoe_m = time;
      if (datoe_m != datoAnteriore_m)
      {
        contadore++;
        if (contadore <= 20)
        {
          encendido10();
        }
        if (contadore > 20 && contadore <= 30)
        {
          reversa_monedas10();
        }
        if (contadore > 30 && contadore <= 40)
        {
          encendido10();
        }
        if (contadore > 40)
        {
          apagado10();
          apagado5();
          for (;;)
          {
            tone(buzz, frecuencia);
            delay(250);
            noTone(buzz);
            Serial3.print("page 2");
            Serial3.write(0xff);
            Serial3.write(0xff);
            Serial3.write(0xff);
            delay(10000);
          }
          dato = 0;
          datoAnterior = 0;
          contador = 0;
          dato3 = 0;
          datoAnterior3 = 0;
          contador3 = 0;
          contador4 = 0;
          contadore = 0;
          time = 0;
          dato2 = 0;
          datoAnterior2 = 0;
          contador2 = 0;
          datoe_m = 0;
          datoAnteriore_m = 0;
          monedas = 0;
          w = 0;
          x = 0;
          m = 0;
          n = 0;
          nn = 0;
        }
      }
      datoAnteriore_m = datoe_m;
      if (dato3 == 0 && datoAnterior3 == 1)
      {
        apagado10();
        apagado5();
        delay(200);
        contador3++;
        contadore = 0;
        if (contador3 >= 1)// 2 PARA MONEDAS DE $5
        {
          contador3 = 0;
          contador4++;
          k = 1;
          if (contador4 >= contador)
          {
            apagado10();
            apagado5();
            for (;;)
            {
              tone(buzz, frecuencia);
              delay(250);
              noTone(buzz);
              Serial3.print("page 2");
              Serial3.write(0xff);
              Serial3.write(0xff);
              Serial3.write(0xff);
              delay(10000);
            }
            dato = 0;
            datoAnterior = 0;
            contador = 0;
            dato3 = 0;
            datoAnterior3 = 0;
            contador3 = 0;
            contador4 = 0;
            contadore = 0;
            time = 0;
            dato2 = 0;
            datoAnterior2 = 0;
            contador2 = 0;
            datoe_m = 0;
            datoAnteriore_m = 0;
            monedas = 0;
            w = 0;
            x = 0;
            m = 0;
            n = 0;
            nn = 0;
          }
        }
      }
      datoAnterior3 = dato3;
    }
    else
    {
      dato3 = digitalRead(h10);
      time = millis() / 1000;
      datoe_m = time;
      if (datoe_m != datoAnteriore_m)
      {
        contadore++;
        if (contadore <= 20)
        {
          encendido10();
        }
        if (contadore > 20 && contadore <= 30)
        {
          reversa_monedas10();
        }
        if (contadore > 30 && contadore <= 40)
        {
          encendido10();
        }
        if (contadore > 40)
        {
          apagado10();
          apagado5();
          for (;;)
          {
            tone(buzz, frecuencia);
            delay(250);
            noTone(buzz);
            Serial3.print("page 2");
            Serial3.write(0xff);
            Serial3.write(0xff);
            Serial3.write(0xff);
            delay(10000);
          }
          dato = 0;
          datoAnterior = 0;
          contador = 0;
          dato3 = 0;
          datoAnterior3 = 0;
          contador3 = 0;
          contador4 = 0;
          contadore = 0;
          time = 0;
          dato2 = 0;
          datoAnterior2 = 0;
          contador2 = 0;
          datoe_m = 0;
          datoAnteriore_m = 0;
          monedas = 0;
          w = 0;
          x = 0;
          m = 0;
          n = 0;
          nn = 0;
        }
      }
      datoAnteriore_m = datoe_m;

      if (dato3 == 0 && datoAnterior3 == 1)
      {
        apagado10();
        apagado5();
        delay(200);

        contador3++;
        contadore = 0;

        if (contador3 >= 10)
        {
          contador3 = 0;
          contador4++;
          k = 1;
          if (contador4 >= 2)//4 PARA MONEDAS $5
          {
            apagado10();
            apagado5();
            for (;;)
            {
              tone(buzz, frecuencia);
              delay(250);
              noTone(buzz);
              Serial3.print("page 2");
              Serial3.write(0xff);
              Serial3.write(0xff);
              Serial3.write(0xff);
              delay(10000);
            }
            dato = 0;
            datoAnterior = 0;
            contador = 0;
            dato3 = 0;
            datoAnterior3 = 0;
            contador3 = 0;
            contador4 = 0;
            contadore = 0;
            time = 0;
            dato2 = 0;
            datoAnterior2 = 0;
            contador2 = 0;
            datoe_m = 0;
            datoAnteriore_m = 0;
            monedas = 0;
            w = 0;
            x = 0;
            m = 0;
            n = 0;
            nn = 0;
          }
        }
      }
      datoAnterior3 = dato3;
    }
  }

  if (k == 20)
  {
    if (Serial3.available() > 0) //aqui voy a poner esa parteeeeeeeeeeeeeeeeeeeeeeeeeeeee
    {
      recived = Serial3.readString();
      if (int(recived[0]) == 2)
      {
        while (1)
        {
          ;
        }
      }
    }
    time = millis() / 1000;
    datoqr = time;
    if (datoqr != datoAnteriorqr && resetqr == 1) //W EMPIEZA EN 0 Y PASA A LA ETAPA DE MOSTRAR LA LECTURA DEL BILLETE EN NEXTION
    {
      contadorqr++;
      if (contadorqr == 300)
      {
        while (1)
        {
          digitalWrite(ble_gnd, HIGH);
          tone(buzz, frecuencia);
          delay(1000);
          noTone(buzz);
          delay(1000);
        }
      }
    }
    datoAnteriorqr = datoqr;
    if (Serial2.available())
    {
      Serial.println("entro en blutubi");
      input = Serial2.readString();
      // delay(100);
      Serial.print("INPUT_valor:");
      Serial.println((input.substring(0, 7)));
      Serial.print("input_longitud:");
      Serial.print(input.length());
      if (!Serial2.available())
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
    time = millis() / 1000;
    dato2 = time;
    if (dato2 != datoAnterior2 && w == 1) //W EMPIEZA EN 0 Y PASA A LA ETAPA DE MOSTRAR LA LECTURA DEL BILLETE EN NEXTION
    {
      contador2++;
      reset_bills = 0;
      if (contador2 == 5)
      {
        contador2 = 0;
        x = 1;
        w = 0;
        stopp = 0;
      }
    }
    datoAnterior2 = dato2;
    //////////////////////////////////////////////////////////////////////////////////
    datoreset_bill = time;
    if (datoreset_bill != datoAnteriorreset_bill && reset_bills == 1) //W EMPIEZA EN 0 Y PASA A LA ETAPA DE MOSTRAR LA LECTURA DEL BILLETE EN NEXTION
    {
      contadorreset_bill++;
      if (contadorreset_bill == 60)
      {
        while (1)
        {
          tone(buzz, frecuencia);
          delay(1000);
          noTone(buzz);
          delay(1000);
        }
      }
    }
    datoAnteriorreset_bill = datoreset_bill;
    /////////////////////////////////////////////////////////////////////////////////
    dato = digitalRead(pulsos_bill);

    if (dato == 0 && datoAnterior == 1 && stopp == 1) //CUENTA LOS PULSOS 1 PULSO CADA $10
    {
      z = 0;
      contador3 = 0;
      contador++;
      digitalWrite(enable_bill, HIGH);
      //      valores_enviar = String(contador);
      w = 1;
    }

    datoAnterior = dato;
    dato3 = time;
    if (dato3 != datoAnterior3 && z == 1) //W EMPIEZA EN 0 Y PASA A LA ETAPA DE MOSTRAR LA LECTURA DEL BILLETE EN NEXTION
    {
      contador3++;
      Serial3.print("n11.val=");
      Serial3.print(contador3);
      Serial3.write(0xff);
      Serial3.write(0xff);
      Serial3.write(0xff);
      if (contador3 == 60)
      {
        digitalWrite(enable_bill, HIGH);
        while (1)
        {
          tone(buzz, frecuencia);
          delay(1000);
          noTone(buzz);
          delay(1000);
        };
      }
    }
    datoAnterior3 = dato3;

    if (x == 1) //X EMPIEZA EN 0
    {
      if (contador < 1)
      {
        Serial3.print("tsw b0,0");
        Serial3.write(0xff);
        Serial3.write(0xff);
        Serial3.write(0xff);
      }
      else if (contador > 10)
      {
        contador = 20;
        moneda200++;
        contadort += contador;
        String total_enviar = "";
        String totall = String(contadort * 10);
        total_enviar = totall + ";";
        total_enviar += String(moneda200) + "x200,";
        Serial2.print(total_enviar);
        /*
         moneda200=EEPROM.read(0)+1; 
        EEPROM.update(0,moneda200);*/
        stopp = 0;
      }
      else if (contador > 1 && contador <= 2)
      {
        contador = 2;
        stopp = 0;
        moneda20++;
        contadort += contador;
        String total_enviar = "";
        String totall = String(contadort * 10);
        total_enviar = totall + ";";
        total_enviar += String(moneda20) + "x20,";
        Serial2.print(total_enviar);
        /*moneda20=EEPROM.read(1)+1; 
        EEPROM.update(1,moneda20);*/
      }
      else if (contador > 2 && contador <= 5)
      {
        contador = 5;
        moneda50++;
        stopp = 0;
        contadort += contador;
        String total_enviar = "";
        String totall = String(contadort * 10);
        total_enviar = totall + ";";
        total_enviar += String(moneda50) + "x50,";
        Serial2.print(total_enviar);
        /*moneda50=EEPROM.read(2)+1; 
        EEPROM.update(2,moneda50);*/
      }
      else if (contador > 5 && contador <= 10)
      {
        contador = 10;
        moneda100++;
        contadort += contador;
        String total_enviar = "";
        String totall = String(contadort * 10);
        total_enviar = totall + ";";
        total_enviar += String(moneda100) + "x100,";
        Serial2.print(total_enviar);
        /*moneda200=EEPROM.read(3)+1; 
        EEPROM.update(3,moneda100);*/
        stopp = 0;
      }
      /*
      else if (contador > 20)
      {
        contador = 50;
        moneda500++;
        contadort += contador;
        String total_enviar = "";
        String totall = String(contadort * 10);
        total_enviar = totall + ";";
        total_enviar += String(moneda500) + "x500,";
        Serial2.print(total_enviar);
        /*moneda500=EEPROM.read(4)+1; 
        EEPROM.update(4,moneda200);
        stopp = 0;
      }
*/
      valores();
      x = 0;
      m = 1;
      w = 0;
      nn = 0;
      digitalWrite(enable_bill, LOW);
      stopp = 1;

      contador = 0;
    }

    if (Serial3.available() > 0) //aqui voy a poner esa parteeeeeeeeeeeeeeeeeeeeeeeeeeeee
    {
      recived = Serial3.readString();
      if (int(recived[0]) == 0)
      {
        digitalWrite(enable_bill, LOW);
        if (contador <= 0)
        {
          z = 1;
          Serial3.print("tsw b0,0");
          Serial3.write(0xff);
          Serial3.write(0xff);
          Serial3.write(0xff);
        }
      }
      else if (int(recived[0]) == 3)
      {
        /* String total_enviar = "";
        String totall = String(contadort*10);
        total_enviar = totall + ";";
        total_enviar += String(moneda20)  + ",";
        total_enviar += String(moneda50)  + ",";
        total_enviar += String(moneda100) + ",";
        total_enviar += String(moneda200) + ",";
        //total_enviar += String(moneda500) + "x500";

        Serial2.print(total_enviar);*/
        //Serial1.write("20;1x20,0x50,0x100,0x200,0x500");
        //        Serial.print(total_enviar);
        //Serial2.print(contadort * 10);
        Serial2.write("END");
        Serial.println(contadort * 10);
        delay(200);
        Serial3.print("page 3");
        Serial3.write(0xff);
        Serial3.write(0xff);
        Serial3.write(0xff);
        while (1)
        {
          digitalWrite(enable_bill, HIGH);
          digitalWrite(ble_gnd, HIGH);
        };
      }
    }
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