#include <Arduino.h>

#define modelo 0            // modelo de tarjeta nueva 0 vieja 1
#define precio_on 0         // 0 sin precio  en 1 para monedas  2 para banda,  3 solo precio iniciando a mario
#define serial 1            // 0 activa la lectura de banda, 1 activa la puerta 110V y puerta 24+,
#define continental 0       // 0 puerta continental off, 1 puerta continental on
#define activacion_puerta 0 // 0 puerta speedquen 110,  1 puerta continental
#define amortiguador 0      // activa acomodo de ropa 1 y 0 desactiva

#include <TM1637Display.h>
#include "Display.h"
#include "Button.h"
#include "Menu.h"
#include "Led.h"
#include "Seleccion_parametros.h"
#include "Lavado.h"
#include "Programacion.h"
#include "EEPROM.h"
#include <Wire.h>
#include <SoftwareSerial.h> // Incluimos la librería  SoftwareSerialx
#include <avr/wdt.h>
////////////////////////////////////////
int cambio = 1;
int cambio_2 = 1;
int ciclo_str = 0;
int etapa_str = 0;
int paso_str = 0;
int status_etapa = 1;
int status_etapa_1 = 1;
int etapa_2 = 0;
int contador_errores_e6 = 0;
int contador_errores_e5 = 0;
int continua = 1;
int activacion_2 = 0;
int auxiliar_puerta_error = 0;
////////////////////////////////////////
static bool instruccionEnviada = false;
static bool instruccionEnviada1 = false;
static bool instruccionEnviada2 = false;
static bool instruccionEnviada3 = false;
static bool instruccionEnviada4 = false;
static bool instruccionEnviada5 = false;
static bool instruccionEnviada6 = false;
static bool instruccionEnviada7 = false;
static bool instruccionEnviada8 = false;
static bool instruccionEnviada9 = false;
static bool instruccionEnviada10 = false;
static bool instruccionEnviada11 = false;
static bool instruccionEnviada12 = false;
static bool instruccionEnviada13 = false;
static bool instruccionEnviada14 = false;
static bool instruccionEnviada15 = false;
static bool instruccionEnviada16 = false;
static bool instruccionEnviada17 = false;
static bool instruccionEnviada18 = false;
static bool instruccionEnviada29 = false;
static bool instruccionEnviada30 = false;
static bool instruccionEnviada33 = false;
int aux_con_door = 1;
////////////////////////////////////////
const unsigned long tiempoDeseado = 20000;
unsigned long tiempoInicio;
unsigned long lastTime = 0;
String combinedString;

/////////////////////
// actualizar para el pay
/////////////////////
#define eprom 0// actualizar parametros 1 ,, para bloquearlos 0///////////////////////
#define opl 0// para quitar comunicacion y jale opl, 0 ,1 para activar comunicacion y 2 para  2 lo del instalador
// actualizar parametros  1 para que queden bloqueados 0
////////////

int index = 0;
///////////
#if serial == 0
SoftwareSerial BT(51, 50); // Definimos los pines RX y TX del Arduino conectados al Bluetooth
#endif
int contador_moneda = 0;
int enable_monedero = 76;
bool dato_moneda = HIGH;
bool datoAnterior_moneda = HIGH;
////////////////////////
int dosificador1;
int dosificador2;
int dosificador3;
int dosificador4;
int contador_dosificador = 0;
int dosificador = 1;
////////////////////////
int c_usos = 600;
int aux_44 = 0;
int aux_55 = 0;
int ciclo_1 = EEPROM.read(162);
int ciclo_2 = EEPROM.read(163);
int ciclo_3 = EEPROM.read(164);
int ciclo_4 = EEPROM.read(190);
int precio_tibia = EEPROM.read(165);
int precio_caliente = EEPROM.read(166);
int nivel_2 = EEPROM.read(167);
int nivel_3 = EEPROM.read(168);
int valor_moneda = EEPROM.read(169);
/////////////////////////////////////////
int h1uso = EEPROM.read(1010);
int l1uso = EEPROM.read(1009);
int wm_uso = h1uso << 8;

int uso = wm_uso + l1uso;
///////////////////////////////////////////
// int uso = EEPROM.read(1002);
int uso2 = EEPROM.read(195);
int posicion1 = 0;
String palabra1 = "";
int aa, aa1;
int aux1 = 0, aux2 = 101;

int precio_temp = 0;
int precio_ciclo = 0;
int precio_nivel = 0;
int k = 0; //--------------------------> debe estar en cero pero va a hacer en dos para empezarr comunicacon

int h1m = EEPROM.read(302);
int l1m = EEPROM.read(303);
int wm = h1m << 8;
int tiempo_medio = wm + l1m;

int h1b = EEPROM.read(300);
int l1b = EEPROM.read(301);
int wb = h1b << 8;
int tiempo_bajo = wb + l1b;

int h1a = EEPROM.read(304);
int l1a = EEPROM.read(305);
int wa = h1a << 8;
int tiempo_alto = wa + l1a;
////programacion de niveles
int tiempo_llenado;

unsigned long lastDebounceTime = 0;   // the last time the output pin was toggled
unsigned long debounceDelay = 200000; // the debounce time; increase if the output flickers

String recived;
int activacion = 0;
int total = 0;   // the running total
int average = 0; // the average
char bb;
String cc = "";
float voltage;
int contador_programcion = 0;
int val = 0;
int prog = 0;
int etapa = 1;
// int etapa = EEPROM.read(33);/////////////////////////////////////////////
long int segundostotalB = 0;
unsigned long segundosAux = 0;
unsigned long tiempo_aux2 = 0;
int paso = 0;
int contadorP = 0;
int dato_llenado;
int dato_error;
int datoAnterior_error = 0;
int contador_error_llenado;
int tiempo_error_llenado = EEPROM.read(161);
;
;
bool t = 1;
int llenado_error = 1;
int error_llenado = 0;
int dato_desague;
int datoAnterior_llenado = 0;
int contador_llenado = 1;
int datoAnterior_desague = 0;
int contador_desague = 0;
bool llenado = 1;
bool desague = 1;
int datoAnteriorP = 0;
int datoP;
int temperatura = 0;

int ahoras = 0;
int aminutos = 0;
int asegundos = 0;
int segundostotal = 0;
unsigned long time = 0;
int dato2;
int datoAnterior2;
// parametros
int LLENADO_AGIpre;
int DESAGUE_AGIpre;
int CENTRIFUpre;
int nivelde_llenado_prelavado;
int TIEMPO_PRELAVADO;
//--------------------------------------
int LLENADO_AGIlav;
int DESAGUE_AGIlav;
int CENTRIFUlav;
int nivelde_llenado_lavado;
int TIEMPO_LAVADO;
//--------------------------------------
int LLENADO_AGIeng;
int DESAGUE_AGIeng;
int CENTRIFUeng;
int nivelde_llenado_enjugaue;
int TIEMPO_ENJUAGUE;
//--------------------------------------
int LLENADO_AGIex1;
int DESAGUE_AGIex1;
int CENTRIFUex1;
int nivel_de_llenado_ennjuague_extra_1;
int TIEMPO_ENJUAGUE_EXTRA_1;
//-------------------------------------------
int LLENADO_AGIex2;
int DESAGUE_AGIex2;
int CENTRIFUex2;
int nivel_de_llenado_ennjuague_extra_2;
int TIEMPO_ENJUAGUE_EXTRA_2;
//--------------------------------------------
int LLENADO_AGIfin;
int DESAGUE_AGIfin;
int CENTRIFUfin;
int nivel_de_llenado_ennjuague_final;
int TIEMPO_ENJUAGUE_FINAL;
//-----------------------------------------
int TIEMPO_CENTRIFUGADO_FINAL;
int nivel_de_llenado_centrifugado;
//---------------------------------------------
int TIEMPO;
// ------------------------------------------
int tiempo_giro_izquierda;
int tiempo_giro_derecha;
int tiempo_reposo;
//////////////////////
int tiempo_dosificador1_ETAPA1 = EEPROM.read(200);
int tiempo_dosificador1_ETAPA2 = EEPROM.read(201);
int tiempo_dosificador1_ETAPA3 = EEPROM.read(202);
int tiempo_dosificador1_ETAPA4 = EEPROM.read(203);
int tiempo_dosificador1_ETAPA5 = EEPROM.read(204);
int tiempo_dosificador1_ETAPA6 = EEPROM.read(205);

int tiempo_dosificador2_ETAPA1 = EEPROM.read(206);
int tiempo_dosificador2_ETAPA2 = EEPROM.read(207);
int tiempo_dosificador2_ETAPA3 = EEPROM.read(208);
int tiempo_dosificador2_ETAPA4 = EEPROM.read(209);
int tiempo_dosificador2_ETAPA5 = EEPROM.read(210);
int tiempo_dosificador2_ETAPA6 = EEPROM.read(211);

int tiempo_dosificador3_ETAPA1 = EEPROM.read(212);
int tiempo_dosificador3_ETAPA2 = EEPROM.read(213);
int tiempo_dosificador3_ETAPA3 = EEPROM.read(214);
int tiempo_dosificador3_ETAPA4 = EEPROM.read(215);
int tiempo_dosificador3_ETAPA5 = EEPROM.read(216);
int tiempo_dosificador3_ETAPA6 = EEPROM.read(217);

int tiempo_dosificador4_ETAPA1 = EEPROM.read(218);
int tiempo_dosificador4_ETAPA2 = EEPROM.read(219);
int tiempo_dosificador4_ETAPA3 = EEPROM.read(220);
int tiempo_dosificador4_ETAPA4 = EEPROM.read(221);
int tiempo_dosificador4_ETAPA5 = EEPROM.read(222);
int tiempo_dosificador4_ETAPA6 = EEPROM.read(223);

////////////////////
void parametros()
{
  LLENADO_AGIpre = LLENADO_AGIpre_1;
  DESAGUE_AGIpre = LLENADO_AGIpre_1;
  CENTRIFUpre = CENTRIFUpre_1;
  nivelde_llenado_prelavado = nivelde_llenado_prelavado_1;
  TIEMPO_PRELAVADO = LLENADO_AGIpre + DESAGUE_AGIpre + CENTRIFUpre;
  //-------------------------------------
  LLENADO_AGIlav = LLENADO_AGIlav_1;
  DESAGUE_AGIlav = DESAGUE_AGIlav_1;
  CENTRIFUlav = CENTRIFUlav_1;
  nivelde_llenado_lavado = nivelde_llenado_lavado_1;
  TIEMPO_LAVADO = LLENADO_AGIlav + DESAGUE_AGIlav + CENTRIFUlav;
  //--------------------------------------
  LLENADO_AGIeng = LLENADO_AGIeng_1;
  DESAGUE_AGIeng = DESAGUE_AGIeng_1;
  CENTRIFUeng = CENTRIFUeng_1;
  nivelde_llenado_enjugaue = nivelde_llenado_enjugaue_1;
  TIEMPO_ENJUAGUE = LLENADO_AGIeng + DESAGUE_AGIeng + CENTRIFUeng;
  //--------------------------------------
  LLENADO_AGIex1 = LLENADO_AGIex1_1;
  DESAGUE_AGIex1 = DESAGUE_AGIex1_1;
  CENTRIFUex1 = CENTRIFUex1_1;
  nivel_de_llenado_ennjuague_extra_1 = nivel_de_llenado_ennjuague_extra_1_1;
  TIEMPO_ENJUAGUE_EXTRA_1 = LLENADO_AGIex1 + DESAGUE_AGIex1 + CENTRIFUex1;
  //-------------------------------------------
  LLENADO_AGIex2 = LLENADO_AGIex2_1;
  DESAGUE_AGIex2 = DESAGUE_AGIex2_1;
  CENTRIFUex2 = CENTRIFUex2_1;
  nivel_de_llenado_ennjuague_extra_2 = nivel_de_llenado_ennjuague_extra_2_1;
  TIEMPO_ENJUAGUE_EXTRA_2 = LLENADO_AGIex2 + DESAGUE_AGIex2 + CENTRIFUex2;
  //--------------------------------------------
  LLENADO_AGIfin = LLENADO_AGIfin_1;
  DESAGUE_AGIfin = DESAGUE_AGIfin_1;
  CENTRIFUfin = CENTRIFUfin_1;
  nivel_de_llenado_ennjuague_final = nivel_de_llenado_ennjuague_final_1;
  TIEMPO_ENJUAGUE_FINAL = LLENADO_AGIfin + DESAGUE_AGIfin + CENTRIFUfin;
  //-----------------------------------------
  TIEMPO_CENTRIFUGADO_FINAL = TIEMPO_CENTRIFUGADO_FINAL_1;
  nivel_de_llenado_centrifugado = nivel_de_llenado_centrifugado_1;
  //---------------------------------------------
  TIEMPO = TIEMPO_PRELAVADO + TIEMPO_LAVADO + TIEMPO_ENJUAGUE + TIEMPO_ENJUAGUE_EXTRA_1 + TIEMPO_ENJUAGUE_EXTRA_2 + TIEMPO_ENJUAGUE_FINAL + TIEMPO_CENTRIFUGADO_FINAL;
  // ------------------------------------------
  tiempo_giro_izquierda = tiempo_giro_izquierda_1;
  tiempo_giro_derecha = tiempo_giro_derecha_1;
  tiempo_reposo = tiempo_reposo_1;
  temperatura = temperatura_1;
}

// DEFINICIONES PARA LAVADORA
#define PRESOSTATO A0
#define MOTOR_BAJA 34 // motor baja
#define SECUENCIA 39  // cambio de secuencia
#define MOTOR_ALTA 28 // motor alta
#define PUERTA 22
#define DESAGUE 26
#define TAMBOR_DEPOSITO1_FRIA 36
#define TAMBOR_DEPOSITO1_CALIENTE 44
#define DEPOSITO2 23
#define DEPOSITO3 24
#define BUTT_TOP 30 // button top-------------FALTA DEFINIRLOS
#define BUTT_BOT A2 // button bottom------------- FALTA DEFINIRLOS
#define BUTT_nivel 11 // button bottom------------- FALTA DEFINIRLOS
#if serial == 1
#define PUERTA_ON_110 50  // MISO
#define PUERTA_OFF_110 51 // MOSI
#define CONTROL_VAC 52    // SCK
#endif

#if serial == 0
#define PUERTA_ON_110 A4  // MISO
#define PUERTA_OFF_110 A5 // MOSI
#define CONTROL_VAC A6    // SCK
#endif

#define moneda_ A3
////////////////////////////////

// DEFINICIONES PARA DISPLAY
#define CLK 4
#define DIO 2
TM1637Display ddisplay(CLK, DIO);
//////////////////////////

// DEFINICIONES DE LOS BOTONES
#define ciclo 7
#define temper 8
#define agua 9
#define inicio 10
// esta en display.h #define buzzer 13
////////////////////////////

// LEDS
#define tciclo_LED 25
#define ttemperatura_LED 27
#define tagua_LED 29
#define tinicio_LED 32

#define prelavadoLED 37
#define lavadoLED 35
#define enjuagueLED 33
#define enjuagueEXLED 38
#define enjugauefinLED 41
#define cenrifugadoLED 40
#define maquina_on 6
///////////////////////////

void avanzarEtapa(unsigned long tiempoEtapa)
{
  if (segundosAux >= tiempoEtapa)
  {
    if (segundosAux == 1)
    {
      average = 0;
    }
    segundosAux = 0;
    tiempo_aux2 = 0;
    paso = 0;
    etapa++;
  }
}

// NIVELES
int DEFAULT_tipo_ciclo = 1;
int DEFAULT_tipo_temperatura = 1;
int DEFAULT_nivel_agua = 1;
///////////////////////

// CLASE BOTON
Button tipo_ciclo(ciclo);
Button tipo_temperatura(temper);
Button nivel_agua(agua);
Button Inicio(inicio);
/////////////////////////
// CLASDE PROGRAMACION
Programacion programacion;
//////////////////////
// CLASE MENU
Menu menu_temp;
Menu menu_ciclo;
Menu menu_agua;
////////////////////////

// CLASE LED
Led led(tciclo_LED, ttemperatura_LED, tagua_LED, tinicio_LED);
//////////////////////

// CLASE LAVADO
Lavado lavado(PRESOSTATO, MOTOR_BAJA, SECUENCIA, MOTOR_ALTA, PUERTA, DESAGUE, TAMBOR_DEPOSITO1_FRIA, TAMBOR_DEPOSITO1_CALIENTE, DEPOSITO2, DEPOSITO3, BUTT_TOP, BUTT_BOT, PUERTA_ON_110, PUERTA_OFF_110, CONTROL_VAC);
//////////////////////////////////////////////

//////////////////////////////////le movi aqui para etna
void llenado_mojado(int dato_llenado, int nivelde_llenado_prelavado, int tiempo_giro_izquierda, int tiempo_giro_derecha, int tiempo_reposo, int tiempo_aux2, int LLENADO_AGIpre, int temperatura, int ETAPA)
{
  lavado.no_drenado();
  /* Serial.print("tipo agua:");
   Serial.println(DEFAULT_nivel_agua);*/
  if (DEFAULT_nivel_agua == 1)
  {
    tiempo_llenado = 120;
  }
  if (DEFAULT_nivel_agua == 2)
  {
    tiempo_llenado = 120;
  }
  if (DEFAULT_nivel_agua == 3)
  {
    tiempo_llenado = 120;
  }

  if (nivelde_llenado_prelavado > average)
  {
    dosificador = 1;
    if (ETAPA == 1 && temperatura == 0)
    {
      // Serial.println("agua_fria");
      lavado.agua_fria();
    }
    if (ETAPA == 1 && temperatura == 1)
    {
      // Serial.println("agua_tibia");
      lavado.agua_tibia();
    }
    if (ETAPA == 1 && temperatura == 2)
    {
      // Serial.println("agua_caliente");
      lavado.agua_caliente();
    }
  }
  else
  {
    lavado.val_off();
  }

  if (nivelde_llenado_prelavado > average)
  {
    dosificador = 1;
    if (ETAPA == 2 && temperatura == 0)
    {
      // Serial.println("agua_fria");
      lavado.agua_fria();
    }
    if (ETAPA == 2 && temperatura == 1)
    {
      //  Serial.println("agua_tibia");
      lavado.agua_tibia();
    }
    if (ETAPA == 2 && temperatura == 2)
    {
      //  Serial.println("agua_caliente");
      lavado.agua_caliente();
    }
  }
  else
  {
    lavado.val_off();
  }

  if (nivelde_llenado_prelavado > average)
  {
    dosificador = 1;
    if (ETAPA == 3 && temperatura == 0)
    {
      lavado.agua_fria();
      //  Serial.println("agua_fria");
    }

    if (ETAPA == 3 && temperatura == 1)
    {
      lavado.agua_tibia();
      // Serial.println("agua_tibia");
    }
    if (ETAPA == 3 && temperatura == 2)
    {
      lavado.agua_caliente();
      //  Serial.println("agua_caliente");
    }
  }
  else
  {
    lavado.val_off();
  }

  if (nivelde_llenado_prelavado > average)
  {
    dosificador = 1;
    if (ETAPA == 4 && temperatura == 0)
    {
      lavado.agua_fria();
      //  Serial.println("agua_fria");
    }
    if (ETAPA == 4 && temperatura == 1)
    {
      lavado.agua_tibia();
      //  Serial.println("agua_tibia");
    }
    if (ETAPA == 4 && temperatura == 2)
    {
      lavado.agua_caliente();
      //  Serial.println("agua_caliente");
    }
  }
  else
  {
    lavado.val_off();
  }

  if (nivelde_llenado_prelavado > average)
  {
    dosificador = 1;
    if (ETAPA == 5 && temperatura == 0)
    {
      lavado.agua_fria();
      //  Serial.println("agua_fria");
    }
    if (ETAPA == 5 && temperatura == 1)
    {
      lavado.agua_tibia();
      //  Serial.println("agua_tibia");
    }
    if (ETAPA == 5 && temperatura == 2)
    {
      lavado.agua_caliente();
      //  Serial.println("agua_caliente");
    }
  }
  else
  {
    lavado.val_off();
  }

  if (nivelde_llenado_prelavado > average)
  {
    dosificador = 1;
    if (ETAPA == 6 && temperatura == 0)
    {
      lavado.enjuague_final();
    }
    if (ETAPA == 6 && temperatura == 1)
    {
      lavado.enjuague_final();
    }
    if (ETAPA == 6 && temperatura == 2)
    {
      lavado.enjuague_final();
    }
  }
  else
  {
    lavado.val_off();
  }

  if (dato_llenado != datoAnterior_llenado)
  {
    contador_llenado++;
    display.setBrightness(0x0f);
    display.showNumberDec(aminutos, true, 2, 2); // Expect: __04
    if (average <= nivelde_llenado_prelavado && llenado_error == 1)
    {
      // Serial.println("yyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyy");
      time = 0;
      while (1)
      {

        /*
        if ((millis() - lastTime) > 1000)
        {
          //   Serial2.print("[2]");
          ddisplay.clear();
          lastTime = millis();
        }
*/
        display.setBrightness(0x0f);
        display.showNumberDec(aminutos, true, 2, 2); // Expect: __04
        wdt_reset();
        time = millis() / 1000;
        dato_error = time;
        if (dato_error != datoAnterior_error)
        {
          contador_error_llenado++;
          contador_dosificador++;
          ///////////////////////////////////////////BOMBAS DE JABON
          if (ETAPA == 1)
          {
            if (dosificador == 1 && contador_dosificador <= tiempo_dosificador1_ETAPA1)
            {
              digitalWrite(A12, HIGH);
              digitalWrite(A11, LOW);
              digitalWrite(A10, LOW);
              digitalWrite(A9, LOW);
            }
            else
            {
              digitalWrite(A12, LOW);
              digitalWrite(A11, LOW);
              digitalWrite(A10, LOW);
              digitalWrite(A9, LOW);
              dosificador = 2;
              contador_dosificador = 0;
            }
            if (dosificador == 2 && contador_dosificador <= tiempo_dosificador2_ETAPA1)
            {
              digitalWrite(A12, LOW);
              digitalWrite(A11, HIGH);
              digitalWrite(A10, LOW);
              digitalWrite(A9, LOW);
            }
            else
            {
              digitalWrite(A12, LOW);
              digitalWrite(A11, LOW);
              digitalWrite(A10, LOW);
              digitalWrite(A9, LOW);
              dosificador = 3;
              contador_dosificador = 0;
            }
            if (dosificador == 3 && contador_dosificador <= tiempo_dosificador3_ETAPA1)
            {
              digitalWrite(A12, LOW);
              digitalWrite(A11, LOW);
              digitalWrite(A10, HIGH);
              digitalWrite(A9, LOW);
              // activar rele dosificador 3
            }
            else
            {
              digitalWrite(A12, LOW);
              digitalWrite(A11, LOW);
              digitalWrite(A10, LOW);
              digitalWrite(A9, LOW);
              dosificador = 4;
              contador_dosificador = 0;
            }
            if (dosificador == 4 && contador_dosificador <= tiempo_dosificador4_ETAPA1)
            {
              digitalWrite(A12, LOW);
              digitalWrite(A11, LOW);
              digitalWrite(A10, LOW);
              digitalWrite(A9, HIGH);
              // activar rele dosificador 4
            }
            else
            {
              digitalWrite(A12, LOW);
              digitalWrite(A11, LOW);
              digitalWrite(A10, LOW);
              digitalWrite(A9, LOW);
              dosificador = 5;
              contador_dosificador = 0;
            }
          }
          if (ETAPA == 2)
          {
            if (dosificador == 1 && contador_dosificador <= tiempo_dosificador1_ETAPA2)
            {
              digitalWrite(A12, HIGH);
              digitalWrite(A11, LOW);
              digitalWrite(A10, LOW);
              digitalWrite(A9, LOW);
            }
            else
            {
              digitalWrite(A12, LOW);
              digitalWrite(A11, LOW);
              digitalWrite(A10, LOW);
              digitalWrite(A9, LOW);
              dosificador = 2;
              contador_dosificador = 0;
            }
            if (dosificador == 2 && contador_dosificador <= tiempo_dosificador2_ETAPA2)
            {
              digitalWrite(A12, LOW);
              digitalWrite(A11, HIGH);
              digitalWrite(A10, LOW);
              digitalWrite(A9, LOW);
            }
            else
            {
              digitalWrite(A12, LOW);
              digitalWrite(A11, LOW);
              digitalWrite(A10, LOW);
              digitalWrite(A9, LOW);
              dosificador = 3;
              contador_dosificador = 0;
            }
            if (dosificador == 3 && contador_dosificador <= tiempo_dosificador3_ETAPA2)
            {
              digitalWrite(A12, LOW);
              digitalWrite(A11, LOW);
              digitalWrite(A10, HIGH);
              digitalWrite(A9, LOW);
              // activar rele dosificador 3
            }
            else
            {
              digitalWrite(A12, LOW);
              digitalWrite(A11, LOW);
              digitalWrite(A10, LOW);
              digitalWrite(A9, LOW);
              dosificador = 4;
              contador_dosificador = 0;
            }
            if (dosificador == 4 && contador_dosificador <= tiempo_dosificador4_ETAPA2)
            {
              digitalWrite(A12, LOW);
              digitalWrite(A11, LOW);
              digitalWrite(A10, LOW);
              digitalWrite(A9, HIGH);
              // activar rele dosificador 4
            }
            else
            {
              digitalWrite(A12, LOW);
              digitalWrite(A11, LOW);
              digitalWrite(A10, LOW);
              digitalWrite(A9, LOW);
              dosificador = 5;
              contador_dosificador = 0;
            }
          }
          if (ETAPA == 3)
          {
            if (dosificador == 1 && contador_dosificador <= tiempo_dosificador1_ETAPA3)
            {
              digitalWrite(A12, HIGH);
              digitalWrite(A11, LOW);
              digitalWrite(A10, LOW);
              digitalWrite(A9, LOW);
            }
            else
            {
              digitalWrite(A12, LOW);
              digitalWrite(A11, LOW);
              digitalWrite(A10, LOW);
              digitalWrite(A9, LOW);
              dosificador = 2;
              contador_dosificador = 0;
            }
            if (dosificador == 2 && contador_dosificador <= tiempo_dosificador2_ETAPA3)
            {
              digitalWrite(A12, LOW);
              digitalWrite(A11, HIGH);
              digitalWrite(A10, LOW);
              digitalWrite(A9, LOW);
            }
            else
            {
              digitalWrite(A12, LOW);
              digitalWrite(A11, LOW);
              digitalWrite(A10, LOW);
              digitalWrite(A9, LOW);
              dosificador = 3;
              contador_dosificador = 0;
            }
            if (dosificador == 3 && contador_dosificador <= tiempo_dosificador3_ETAPA3)
            {
              digitalWrite(A12, LOW);
              digitalWrite(A11, LOW);
              digitalWrite(A10, HIGH);
              digitalWrite(A9, LOW);
              // activar rele dosificador 3
            }
            else
            {
              digitalWrite(A12, LOW);
              digitalWrite(A11, LOW);
              digitalWrite(A10, LOW);
              digitalWrite(A9, LOW);
              dosificador = 4;
              contador_dosificador = 0;
            }
            if (dosificador == 4 && contador_dosificador <= tiempo_dosificador4_ETAPA3)
            {
              digitalWrite(A12, LOW);
              digitalWrite(A11, LOW);
              digitalWrite(A10, LOW);
              digitalWrite(A9, HIGH);
              // activar rele dosificador 4
            }
            else
            {
              digitalWrite(A12, LOW);
              digitalWrite(A11, LOW);
              digitalWrite(A10, LOW);
              digitalWrite(A9, LOW);
              dosificador = 5;
              contador_dosificador = 0;
            }
          }
          if (ETAPA == 4)
          {
            if (dosificador == 1 && contador_dosificador <= tiempo_dosificador1_ETAPA4)
            {
              digitalWrite(A12, HIGH);
              digitalWrite(A11, LOW);
              digitalWrite(A10, LOW);
              digitalWrite(A9, LOW);
            }
            else
            {
              digitalWrite(A12, LOW);
              digitalWrite(A11, LOW);
              digitalWrite(A10, LOW);
              digitalWrite(A9, LOW);
              dosificador = 2;
              contador_dosificador = 0;
            }
            if (dosificador == 2 && contador_dosificador <= tiempo_dosificador2_ETAPA4)
            {
              digitalWrite(A12, LOW);
              digitalWrite(A11, HIGH);
              digitalWrite(A10, LOW);
              digitalWrite(A9, LOW);
            }
            else
            {
              digitalWrite(A12, LOW);
              digitalWrite(A11, LOW);
              digitalWrite(A10, LOW);
              digitalWrite(A9, LOW);
              dosificador = 3;
              contador_dosificador = 0;
            }
            if (dosificador == 3 && contador_dosificador <= tiempo_dosificador3_ETAPA4)
            {
              digitalWrite(A12, LOW);
              digitalWrite(A11, LOW);
              digitalWrite(A10, HIGH);
              digitalWrite(A9, LOW);
              // activar rele dosificador 3
            }
            else
            {
              digitalWrite(A12, LOW);
              digitalWrite(A11, LOW);
              digitalWrite(A10, LOW);
              digitalWrite(A9, LOW);
              dosificador = 4;
              contador_dosificador = 0;
            }
            if (dosificador == 4 && contador_dosificador <= tiempo_dosificador4_ETAPA4)
            {
              digitalWrite(A12, LOW);
              digitalWrite(A11, LOW);
              digitalWrite(A10, LOW);
              digitalWrite(A9, HIGH);
              // activar rele dosificador 4
            }
            else
            {
              digitalWrite(A12, LOW);
              digitalWrite(A11, LOW);
              digitalWrite(A10, LOW);
              digitalWrite(A9, LOW);
              dosificador = 5;
              contador_dosificador = 0;
            }
          }
          ///////////////////////////////////////////
          contador_llenado++;
          if (llenado == 1)
          {
            if (t == 0)
            {
              if (contador_llenado <= tiempo_giro_izquierda)
              {
                lavado.IZQUIERDA_M();
                //  Serial.println("izquierda");
                // if (contador_error_llenado >= tiempo_llenado)
                if (digitalRead(A0) == 0)
                {
                  average = nivelde_llenado_prelavado + 1;
                  lavado.val_off();
                  /* average = nivelde_llenado_prelavado + 1;
                   Serial.print("average");
                   Serial.println(average);*/
                  contador_llenado = 1;
                  t = 1;
                  llenado_error = 0;
                  llenado = 1;
                  break;
                }
              }
              else
              {
                contador_llenado = 1;
                t = 1;
              }
            }
            if (t == 1)
            {
              if (contador_llenado <= tiempo_reposo)
              {
                lavado.STOP_M();
                //  Serial.println("stop");
                if (contador_error_llenado >= tiempo_error_llenado * 60)
                {
                  lavado.drenado();
                  lavado.STOP_M();
                  lavado.val_off();
                  ddisplay.clear();
                  display.setBrightness(0x0f);
                  display.setSegments(SEG_E1);
                  ciclo_str = 1;
                  etapa_str = DEFAULT_tipo_ciclo;
                  paso_str = 0;
                  String estado_lavadora = "[" + String(ciclo_str) + "," + String(etapa_str) + "," + String(paso_str) + "]\n";
                  Serial2.print(estado_lavadora);
                  delay(3000);
                  if (aux_55 == 0)
                  {
                    // aqui hay que agregar lo de la puerta de la continental
                    lavado.PUERTA_OFF();
#if continental == 1
                    lavado.r_continental_off_1();
                    delay(200);
                    lavado.r_continental_on_2();
                    delay(10000);
                    lavado.r_continental_off_2();
#endif
                  }
                  aux_55 = 1;
                  // Serial2.print("03");
                  while (1)
                  {
                    wdt_reset();
                    tone(buzzer, 2000);
                    delay(1000);
                    noTone(buzzer);
                    delay(1000);
                  }
                  // asm volatile(" jmp 0");
                  // ERROR LLENADO
                }
                // if (contador_error_llenado >= tiempo_llenado)
                if (digitalRead(A0) == 0)
                {
                  lavado.val_off();
                  average = nivelde_llenado_prelavado + 1;
                  /* average = nivelde_llenado_prelavado + 1;
                   Serial.print("average");
                   Serial.println(average);*/
                  contador_llenado = 1;
                  t = 1;
                  llenado_error = 0;
                  llenado = 1;
                  break;
                }
              }
              else
              {
                t = 0;
                contador_llenado = 1;
                llenado = 0;
              }
            }
          }

          if (llenado == 0)
          {
            if (t == 0)
            {
              if (contador_llenado <= tiempo_giro_derecha)
              {
                lavado.DERECHA_M();
                //  Serial.println("derecha");

                // if (contador_error_llenado >= tiempo_llenado)
                if (digitalRead(A0) == 0)
                {
                  lavado.val_off();
                  average = nivelde_llenado_prelavado + 1;
                  /*average = nivelde_llenado_prelavado + 1;
                  Serial.print("average");
                  Serial.println(average);*/
                  contador_llenado = 1;
                  t = 1;
                  llenado_error = 0;
                  llenado = 1;
                  break;
                }
              }
              else
              {
                contador_llenado = 1;
                t = 1;
              }
            }
            if (t == 1)
            {
              if (contador_llenado <= tiempo_reposo)
              {
                lavado.STOP_M();
                //  Serial.println("stop");

                if (contador_error_llenado >= tiempo_error_llenado * 60)
                {
                  // error llenadoooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo e1
                  lavado.drenado();
                  lavado.STOP_M();
                  lavado.val_off();
                  ddisplay.clear();
                  display.setBrightness(0x0f);
                  display.setSegments(SEG_E1);
                  ciclo_str = 1;
                  etapa_str = DEFAULT_tipo_ciclo;
                  paso_str = 0;
                  String estado_lavadora = "[" + String(ciclo_str) + "," + String(etapa_str) + "," + String(paso_str) + "]\n";
                  Serial2.print(estado_lavadora);
                  delay(3000);
                  if (aux_55 == 0)
                  {
                    // aqui hay que agregar lo de la puerta de la continental
                    lavado.PUERTA_OFF();
#if continental == 1
                    lavado.r_continental_off_1();
                    delay(200);
                    lavado.r_continental_on_2();
                    delay(10000);
                    lavado.r_continental_off_2();
#endif
                  }
                  aux_55 = 1;
                  // Serial2.print("03");
                  while (1)
                  {
                    wdt_reset();
                    tone(buzzer, 2000);
                    delay(1000);
                    noTone(buzzer);
                    delay(1000);
                  }
                  // asm volatile(" jmp 0");
                  //  ERROR LLENADO
                }
                // if (contador_error_llenado >= tiempo_llenado)
                if (digitalRead(A0) == 0)
                {
                  lavado.val_off();
                  average = nivelde_llenado_prelavado + 1;
                  /*  average = nivelde_llenado_prelavado + 1;
                    Serial.print("average");
                    Serial.println(average);*/
                  contador_llenado = 1;
                  t = 1;
                  llenado_error = 0;
                  llenado = 1;
                  break;
                }
              }
              else
              {
                t = 0;
                contador_llenado = 1;
                llenado = 1;
              }
            }
          }
        }
        datoAnterior_error = dato_error;
      }
    }
    if (llenado == 1)
    {
      if (t == 0)
      {
        if (contador_llenado <= tiempo_giro_izquierda)
        {
          lavado.IZQUIERDA_M();
          //  Serial.println("izquierda");
        }
        else
        {
          contador_llenado = 1;
          t = 1;
        }
      }
      if (t == 1)
      {
        if (contador_llenado <= tiempo_reposo)
        {
          lavado.STOP_M();
          //  Serial.println("stop");
        }
        else
        {
          t = 0;
          contador_llenado = 1;
          llenado = 0;
        }
      }
    }
    if (llenado == 0)
    {
      if (t == 0)
      {
        if (contador_llenado <= tiempo_giro_derecha)
        {
          lavado.DERECHA_M();
          //  Serial.println("derecha");
        }
        else
        {
          contador_llenado = 1;
          t = 1;
        }
      }
      if (t == 1)
      {
        if (contador_llenado <= tiempo_reposo)
        {
          lavado.STOP_M();
          // Serial.println("stop");
        }
        else
        {
          t = 0;
          contador_llenado = 1;
          llenado = 1;
        }
      }
    }
  }
  datoAnterior_llenado = dato_llenado;
}

void drenado_lavado(int dato_llenado, int nivelde_llenado_prelavado, int tiempo_giro_izquierda, int tiempo_giro_derecha, int tiempo_reposo, int tiempo_aux2, int LLENADO_AGIpre)
{
  lavado.drenado();
  if (dato_llenado != datoAnterior_llenado)
  {
    contador_llenado++;
    ///////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////
    if (llenado == 1)
    {
      if (t == 0)
      {
        if (contador_llenado <= tiempo_giro_izquierda)
        {
          lavado.IZQUIERDA_M();
          // izquierda
        }
        else
        {
          contador_llenado = 1;
          t = 1;
        }
      }

      if (t == 1)
      {
        if (contador_llenado <= tiempo_reposo)
        {
          lavado.STOP_M();
          // stop
          // e2 drenado
        }
        else
        {
          t = 0;
          contador_llenado = 1;
          llenado = 0;
        }
      }
    }
    if (llenado == 0)
    {
      if (t == 0)
      {
        if (contador_llenado <= tiempo_giro_derecha)
        {
          lavado.DERECHA_M();
        }
        else
        {
          t = 1;
          contador_llenado = 1;
        }
      }

      if (t == 1)
      {
        if (contador_llenado <= tiempo_reposo)
        {

          lavado.STOP_M();
        }
        else
        {
          t = 0;
          contador_llenado = 1;
          llenado = 1;
          // stop
        }
      }
    }
  }
  datoAnterior_llenado = dato_llenado;
}

void setup()
{

  // EEPROM.update(101, 2);
  // EEPROM.update(33, 1);
  wdt_disable();
// EEPROM.update(101, 2); // nivel de agua_ normal 165 mediana chica 163
#if serial == 0
  BT.begin(9600);
#endif
#if precio_on == 1
  if (EEPROM.read(1001) == 0)
  {
  }
#endif
  Serial.begin(9600);
  Serial2.begin(115200);

  ddisplay.clear();
  ddisplay.setBrightness(0x0f);

  pinMode(A0, INPUT_PULLUP);

  menu_agua.cases(DEFAULT_nivel_agua, 3);
  menu_temp.cases(DEFAULT_tipo_temperatura, 2);
  menu_ciclo.cases(DEFAULT_tipo_ciclo, 1);
  seleccion_temperatura(DEFAULT_tipo_temperatura);
  seleccion_agua(DEFAULT_nivel_agua);
  seleccion_ciclo(DEFAULT_tipo_ciclo);
  // EEPROM.update(800, 1);

  pinMode(enable_monedero, OUTPUT); /// aun no hace nada el rele por el cambio en el programa
  digitalWrite(enable_monedero, LOW);
  ////////////////////////////////////////////////////////
  pinMode(A12, OUTPUT); // B1
  pinMode(A11, OUTPUT); // B2
  pinMode(A10, OUTPUT); // B3
  pinMode(A9, OUTPUT);  // B4
  ///////////////////////////////////////////////////////
  digitalWrite(A12, LOW);
  digitalWrite(A11, LOW);
  digitalWrite(A10, LOW);
  digitalWrite(A9, LOW);
  ///////////////////////////////////////////////////////
  pinMode(moneda_, INPUT_PULLUP);

  pinMode(prelavadoLED, OUTPUT);
  pinMode(lavadoLED, OUTPUT);
  pinMode(enjuagueLED, OUTPUT);
  pinMode(enjuagueEXLED, OUTPUT);
  pinMode(enjugauefinLED, OUTPUT);
  pinMode(cenrifugadoLED, OUTPUT);
  // pinMode(maquina_on, INPUT_PULLUP);

  digitalWrite(prelavadoLED, LOW);
  digitalWrite(lavadoLED, LOW);
  digitalWrite(enjuagueLED, LOW);
  digitalWrite(enjuagueEXLED, LOW);
  digitalWrite(enjugauefinLED, LOW);
  digitalWrite(cenrifugadoLED, LOW);

  parametros();

  // ddisplay.clear();
  ddisplay.setBrightness(0x0f);

#if (eprom == 1)
  paramreos_eeprom();
#endif
  if (tipo_ciclo.isPressed())
  {
    k = 1;
  }
  if (tipo_temperatura.isPressed())
  {
    //  k = 2;
  }
  if (nivel_agua.isPressed())
  {
    //  k = 3;
  }
  if (Inicio.isPressed())
  {
    //  k = 4;
  }

#if (opl == 1)
  EEPROM.update(1001, 0); // opl por mientras
  ddisplay.clear();
  display.setBrightness(0x0f);
  display.setSegments(SEG_PAY);
#endif
#if (opl == 0)
  EEPROM.update(1001, 1); // opl por mientras
#endif

  // EEPROM.update(400, 0); //
  // EEPROM.update(401, 0); // 0
  // EEPROM.update(402, 0); //
  // EEPROM.update(403, 0); // 8
  // EEPROM.update(404, 0); // 1
  // EEPROM.update(405, 0); //
  // EEPROM.update(406, 0); // 8
  //  EEPROM.update(407, 0); // 1
  //  EEPROM.update(408, 0); //
  //  EEPROM.update(409, 0); // 8
  //  EEPROM.update(410, 0); // 1
  //  EEPROM.update(411, 0); //

  wdt_enable(WDTO_8S);

  //  lavado.no_drenado();
  //  lavado.DERECHA_M();
  //  lavado.enjuague_final();
  // lavado.CENTRIFUGADO();
  // lavado.IZQUIERDA_M();
}
void loop()
{
  switch (k)
  {

  case 1:
    wdt_reset();
    if (aux1 == 0)
    {
      ddisplay.clear();
      aux1++;
    }
    ddisplay.setBrightness(0x0f);
    display.setSegments(SEG_PROG);
    delay(3000);
    ddisplay.clear();
    display.setBrightness(0x0f);
    display.setSegments(SEG_P);
    display.showNumberDec(aux2, true, 3, 1);
    while (1)
    {
      wdt_reset();
      if (tipo_ciclo.isPressed())
      {
        aux2++;

        if (aux2 > 500)
        {
          delay(9000);
          // asm volatile(" jmp 0");
        }
        display.setBrightness(0x0f);
        display.setSegments(SEG_P);
        display.showNumberDec(aux2, true, 3, 1);
      }
      if (tipo_temperatura.isPressed())
      {
        aux2--;

        if (aux2 <= 101)
        {
          aux2 = 101;
        }
        display.setBrightness(0x0f);
        display.setSegments(SEG_P);
        display.showNumberDec(aux2, true, 3, 1);
      }
      if (nivel_agua.isPressed())
      {
        ddisplay.clear();
        int aux3 = EEPROM.read(aux2);
        display.setBrightness(0x0f);
        display.showNumberDec(aux3, true, 3, 1);
        while (1)
        {
          wdt_reset();
          if (tipo_ciclo.isPressed())
          {
            aux3++;

            if (aux3 >= 21)
            {
              aux2 = 20;
            }
            display.setBrightness(0x0f);
            display.showNumberDec(aux3, true, 3, 1);
          }
          if (tipo_temperatura.isPressed())
          {
            aux3--;

            if (aux3 <= 0)
            {
              aux3 = 0;
            }
            display.setBrightness(0x0f);
            display.showNumberDec(aux3, true, 3, 1);
          }
          if (Inicio.isPressed())
          {
            EEPROM.update(aux2, aux3);
            delay(500);
            ddisplay.clear();
            display.setBrightness(0x0f);
            display.setSegments(SEG_P);
            display.showNumberDec(aux2, true, 3, 1);
            break;
          }
        }
      }
    }
    break;

  case 0:
    wdt_reset();

    while (status_etapa_1 == 1)
    {
      /* code */

      unsigned long interval = 500;
      unsigned long previousMillis = millis();

      for (int i = 0; i < 1; i++)
      {
        wdt_reset();
        Serial2.print("status_2\n");
        Serial.print("status_2\n");

        while (millis() - previousMillis < interval)
        {
        }
        status_etapa_1 = 0;
        previousMillis = millis(); // Actualiza el tiempo para la próxima espera
      }
    }

    while (status_etapa == 1)
    {
      /* code */

      unsigned long interval = 500;
      unsigned long previousMillis = millis();

      for (int i = 0; i < 1; i++)
      {
        wdt_reset();
        Serial2.print("status\n");
        // Serial.print("status\n");

        while (millis() - previousMillis < interval)
        {
        }
        status_etapa = 0;
        previousMillis = millis(); // Actualiza el tiempo para la próxima espera
      }
    }

    ////////////////////////////
    /*
    #if continental == 1

        if (digitalRead(BUTT_BOT) == 1)
        {
          aux_con_door = 0;
          lavado.r_continental_off_2();
        }
        while (digitalRead(BUTT_BOT) == 0 && aux_con_door == 1)
        {
          lavado.r_continental_on_2();
          wdt_reset();
        }
    #endif*/
    ////////////////////////////

    while (digitalRead(BUTT_BOT) == 0 && digitalRead(BUTT_TOP) == 0)
    {
      wdt_reset();
      display.setBrightness(0x0f);
      display.setSegments(SEG_E5);
      // Serial.println("32\n");

      // Serial.print(estado_lavadora);
      lavado.PUERTA_ON();
      delay(2000);
      wdt_reset();
      // aqui hay que agregar lo de la puerta de la continental
      lavado.PUERTA_OFF();

      delay(2000);
      contador_errores_e5++;
      if (contador_errores_e5 >= 20)
      {
        ciclo_str = 5;
        etapa_str = DEFAULT_tipo_ciclo;
        paso_str = 0;
        String estado_lavadora = "[" + String(ciclo_str) + "," + String(etapa_str) + "," + String(paso_str) + "]\n";
        Serial2.print(estado_lavadora);
        contador_errores_e5 = 0;
        ;
      }
      if (digitalRead(BUTT_TOP) == 1)
      {
        while (1)
          ;
      }
    }

#if serial == 0

    if (BT.available() > 0) // Si llega un dato por el puerto BT se envía al monitor serial
    {
      String tarjeta = BT.readString();
      if (tarjeta.length() > 20)
      {
        Serial.print(tarjeta.substring(1, 21));
      }

      DEFAULT_tipo_ciclo = tipo_ciclo.pulses(DEFAULT_tipo_ciclo); // limita los estados que puede haber en el switch
      Serial.print(DEFAULT_tipo_ciclo);
      Serial.print(",");
      DEFAULT_tipo_temperatura = tipo_temperatura.pulses(DEFAULT_tipo_temperatura); // limita los estados que puede haber en el switch
      Serial.print(DEFAULT_tipo_temperatura);
      DEFAULT_nivel_agua = nivel_agua.pulses(DEFAULT_nivel_agua); // limita los estados que puede haber en el switch
      Serial.print(",");
      Serial.print(DEFAULT_nivel_agua);

      ////////////
      ddisplay.clear();
      display.setBrightness(0x0f);
      display.setSegments(SEG_SEG);
      ////////////
      //
      if (tarjeta.length() > 20)
      {
        Serial2.print(tarjeta.substring(1, 21));
      }
      Serial2.print(DEFAULT_tipo_ciclo);
      Serial2.print(",");
      Serial2.print(DEFAULT_tipo_temperatura);
      Serial2.print(",");
      Serial2.print(DEFAULT_nivel_agua);
    }

    /*
    /////////////////// lo puse con etna
    aa = String(recived).toInt();
    if (aa > 0)
    {
      Serial2.print("OK");
      activacion = 10;
      ddisplay.clear();
      display.setBrightness(0x0f);
      display.setSegments(SEG_on);
    }
    ////////////////////
    */
    if (Serial2.available() > 0)
    {
      recived = Serial2.readString();
      if (recived.substring(0, 7) == "Precios")
      {
        tone(buzzer, 1000);
        delay(100);
        noTone(buzzer);
        Serial.println("precios_Actualziados");
        posicion1 = recived.indexOf("Precios: ");
        palabra1 = recived.substring(posicion1 + 11, posicion1 + 14);
        Serial.println(palabra1);
        EEPROM.update(500, palabra1.toInt());
        palabra1 = recived.substring(posicion1 + 17, posicion1 + 20);
        Serial.println(palabra1);
        EEPROM.update(501, palabra1.toInt());
        palabra1 = recived.substring(posicion1 + 23, posicion1 + 26);
        Serial.println(palabra1);
        EEPROM.update(502, palabra1.toInt());
        palabra1 = recived.substring(posicion1 + 29, posicion1 + 32);
        Serial.println(palabra1);
        EEPROM.update(503, palabra1.toInt());
        palabra1 = recived.substring(posicion1 + 36, posicion1 + 39);
        Serial.println(palabra1);
        EEPROM.update(504, palabra1.toInt());
      }

      if (recived.substring(0, 9) == "Resultado")
      {

        Serial.println("AQUI2");
        posicion1 = recived.indexOf("Resultado: ");
        palabra1 = recived.substring(posicion1 + 10, posicion1 + 15);
        aa = String(palabra1).toInt();
        aa1 = String(palabra1).toInt();
        Serial.println("xxxxxxxxxxxxxxxxxxxxxxxxxxxx");
        Serial.println(aa);
        Serial.println("xxxxxxxxxxxxxxxxxxxxxxxxxxxx");
        if (aa > 0)
        {
          ddisplay.clear();
          display.setBrightness(0x0f);
          display.setSegments(SEG_on);
          activacion = 10;
          activacion = 10;
          tone(buzzer, 2000);
          delay(100);
          noTone(buzzer);
        }
        else
        {
          tone(buzzer, 100);
          delay(200);
          noTone(buzzer);
          // activacion = 0;
        }
        if (int(recived[0]) == 1) // activacion con parametros
        {
          led.prog_LED_off();
          activacion = 10;
          tone(buzzer, 2000);
          delay(100);
          noTone(buzzer);
        }
      }
    }
#endif
////////////////////////////////////////////////////

///////////////////////////////////////////////////////
#if serial == 1
#if opl == 2
    const int maxExpectedLength = 166;
    char incomingData[maxExpectedLength + 1];
    int index = 0;
    bool endOfData = false;

    while (index < maxExpectedLength && !endOfData)
    {
      wdt_reset();
      if (Serial2.available() > 0)
      {
        char incomingByte = Serial2.read();
        Serial.print(incomingByte);

        incomingData[index] = incomingByte;
        index++;

        if (incomingByte == '}')
        {
          endOfData = true;
        }
      }
    }
    incomingData[index] = '\0'; // Añadir el carácter nulo al final

    if (index <= 10) // cadena corta
    {
    }
    else // cadena larga
    {
    }
#endif

#if opl == 1
    const int expectedLength = 10;
    char incomingData[expectedLength + 1];
    int index = 0;

    while (index < expectedLength && activacion == 0)
    {
      // Serial.print("sssssssssss");
      wdt_reset();
      if (Serial2.available() > 0)
      {
        // Lee el siguiente byte disponible
        char incomingByte = Serial.read();
        Serial.print(incomingByte);
        index++;
      }
      /*   if(EEPROM.read(33)>1){
              parametros();
             activacion = 10;
           break;
         }
         */
    }

    if (Serial2.available() > 0)
    {
      wdt_reset();
      recived = Serial2.readString();
      Serial.print(recived);
      int startPos = recived.indexOf('{');
      int endPos = recived.indexOf('}');
      if (startPos >= 0 && endPos >= 0)
      {
        wdt_reset();
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
        wdt_reset();

        if (numbersArray[0].toInt() > 0)
        {
          wdt_reset();
          parametros();
          activacion = 10;

          unsigned long interval = 1000; // Intervalo de 1 segundo entre cada envío
          unsigned long previousMillis = millis();

          for (int i = 0; i < 4; i++)
          {
            wdt_reset();
            Serial2.print("ok\n");
            // Serial2.print("ok\n"); // Envía el comando "01"
            // Serial.print("ok\n");  // Envía el comando "01"
            //  Espera hasta que haya pasado 1 segundo
            while (millis() - previousMillis < interval)
            {
              // No hacer nada aquí, solo esperar
            }

            previousMillis = millis(); // Actualiza el tiempo para la próxima espera
          }

          ddisplay.clear();
          display.setBrightness(0x0f);
          display.setSegments(SEG_on);
          parametros();

          DEFAULT_tipo_ciclo = numbersArray[1].toInt();
          DEFAULT_tipo_temperatura = numbersArray[2].toInt();
          DEFAULT_nivel_agua = numbersArray[3].toInt();
          seleccion_ciclo(numbersArray[1].toInt());
          seleccion_temperatura(numbersArray[2].toInt());
          seleccion_agua(numbersArray[3].toInt());
          Serial.print(DEFAULT_tipo_ciclo);
          Serial.println(DEFAULT_tipo_ciclo);
          Serial.println(DEFAULT_tipo_temperatura);
          Serial.println(DEFAULT_nivel_agua);
          parametros();
          Serial.print(TIEMPO);
          // Serial.print("CCCCCCCCCCCCFVFDFGDFSDFSDFSDSDSDFSDSDCSDCSDCSDCSDCSDCSDCSDCSD");
        }
      }
      //////////////////////////////////////
      int startPos_3 = recived.indexOf('<');
      int endPos_3 = recived.indexOf('>');
      if (startPos_3 >= 0 && endPos_3 >= 0)
      {
        wdt_reset();
        String numbersString = recived.substring(startPos_3 + 1, endPos_3);

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
        wdt_reset();

        if (numbersArray[0].toInt() > 0)
        {
          activacion_2 = 10;
          // Serial.print("HERE");
        }
      }
      //////////////////////////////////////
      int startPos_1 = recived.indexOf('[');
      int endPos_1 = recived.indexOf(']');
      if (startPos_1 >= 0 && endPos_1 >= 0)
      {
        wdt_reset();
        String numbersString = recived.substring(startPos_1 + 1, endPos_1);

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
        wdt_reset();

        etapa_2 = (numbersArray[0].toInt()) - 2;
        continua = (numbersArray[3].toInt());
        //////////////////////////////////////////////
        Serial.print("etapa_2:");
        Serial.println(etapa_2);

        Serial.print("numero:");
        Serial.println(numbersArray[0].toInt());
        Serial.print("tiempo_preñavado:");
        Serial.println(TIEMPO_PRELAVADO * 60);
        Serial.print("tiempo_lavado:");
        Serial.println(TIEMPO_LAVADO * 60);
        Serial.print("tiempo_enjuague:");
        Serial.println(TIEMPO_ENJUAGUE * 60);
        Serial.print("tiempo_enjuaguex1:");
        Serial.println(TIEMPO_ENJUAGUE_EXTRA_1 * 60);
        Serial.print("tiempo_enjuaguex2:");
        Serial.println(TIEMPO_ENJUAGUE_EXTRA_2 * 60);
        Serial.print("tiempo_enjuagueF:");
        Serial.println(TIEMPO_ENJUAGUE_FINAL * 60);
        Serial.print("tiempo_centrifugadoF:");
        Serial.println(TIEMPO_CENTRIFUGADO_FINAL * 60);
        if (etapa_2 > 0)
        {
          activacion = 10;

          etapa = etapa_2;
          Serial.print("etapa:");
          Serial.println(etapa);
        }

#if opl == 1
        if (etapa_2 < 0)
        {
          if (TIEMPO_PRELAVADO * 60 > 0)
          {
            Serial.println("TIempo_etapa_prelavado");
            etapa = 1;
            if (numbersArray[0].toInt() == 0)
            {
              etapa_2 = 1;
            }
          }
          else if (TIEMPO_LAVADO * 60 > 0)
          {
            Serial.println("TIempo_etapa_lavado");
            etapa = 2;
            if (numbersArray[0].toInt() == 0)
            {
              etapa_2 = 2;
            }
          }
          else if (TIEMPO_ENJUAGUE * 60 > 0)
          {
            Serial.println("TIempo_etapa_enjuague");
            etapa = 3;
            if (numbersArray[0].toInt() == 0)
            {
              etapa_2 = 3;
            }
          }
          else if (TIEMPO_ENJUAGUE_EXTRA_1 * 60 > 0)
          {
            Serial.println("TIempo_etapa_enjuaguex1");
            etapa = 4;
            if (numbersArray[0].toInt() == 0)
            {
              etapa_2 = 4;
            }
          }
          else if (TIEMPO_ENJUAGUE_EXTRA_2 * 60 > 0)
          {
            Serial.println("TIempo_etapa_enjuaguex2");
            etapa = 5;
            if (numbersArray[0].toInt() == 0)
            {
              etapa_2 = 5;
            }
          }
          else if (TIEMPO_ENJUAGUE_FINAL * 60 > 0)
          {
            Serial.println("TIempo_etapa_enjuagueF");
            etapa = 6;
            if (numbersArray[0].toInt() == 0)
            {
              etapa_2 = 6;
            }
          }
          else if (TIEMPO_CENTRIFUGADO_FINAL * 60 > 0)
          {
            Serial.println("TIempo_etapa_Centrifugado");
            etapa = 7;
            if (numbersArray[0].toInt() == 0)
            {
              etapa_2 = 7;
            }
          }
        }
        /*    if (etapa_2 < etapa)
            {
              etapa_2 = etapa;
            }
            else
              etapa = etapa_2;*/
#endif
        // parametros();
      }
      ////////
    }
#endif

#endif

#if precio_on == 3

    if (DEFAULT_tipo_ciclo == 1)
    {
      // precio_ciclo = EEPROM.read(500);
      precio_ciclo = ciclo_1;
    }
    if (DEFAULT_tipo_ciclo == 2)
    {
      // precio_ciclo = EEPROM.read(501);
      precio_ciclo = ciclo_2;
    }
    if (DEFAULT_tipo_ciclo == 3)
    {
      // precio_ciclo = EEPROM.read(502);
      precio_ciclo = ciclo_3;
    }
    if (DEFAULT_tipo_ciclo == 4)
    {
      // precio_ciclo = EEPROM.read(502);
      precio_ciclo = ciclo_4;
    }

    if (DEFAULT_tipo_temperatura == 1)
    {
      precio_temp = 0;
    }
    if (DEFAULT_tipo_temperatura == 2)
    {
      // precio_temp = EEPROM.read(503);
      precio_temp = precio_tibia;
    }
    if (DEFAULT_tipo_temperatura == 3)
    {
      //  precio_temp = EEPROM.read(504);
      precio_temp = precio_caliente;
    }
    if (DEFAULT_nivel_agua == 1)
    {
      precio_nivel = 0;
    }

    if (DEFAULT_nivel_agua == 2)
    {
      precio_nivel = nivel_2;
    }
    if (DEFAULT_nivel_agua == 3)
    {
      precio_nivel = nivel_3;
    }
    /*
          Serial.println(precio_temp);
          Serial.println(precio_ciclo);
          Serial.println(precio_temp + precio_ciclo);
    */
    /////////////////////////////////////////////////////////////

    if (activacion != 10)
    {
      ddisplay.clear();
      display.setBrightness(0x0f);
      display.showNumberDec(((precio_temp + precio_ciclo + precio_nivel) * valor_moneda), true, 3, 1);
    }
    else
    {
      ddisplay.clear();
      display.setBrightness(0x0f);
      display.setSegments(SEG_on);
    }

    // Expect: __04

    while (activacion != 10)
    {
      wdt_reset();
      if (Inicio.isPressed()) // evaluo si el boton fue presionado
      {
        if ((millis() - lastDebounceTime) > debounceDelay)
        {
          while (1)
          {
          }
        }
        if (DEFAULT_tipo_ciclo == 1)
        {
          // precio_ciclo = EEPROM.read(500);
          precio_ciclo = ciclo_1;
        }
        if (DEFAULT_tipo_ciclo == 2)
        {
          // precio_ciclo = EEPROM.read(501);
          precio_ciclo = ciclo_2;
        }
        if (DEFAULT_tipo_ciclo == 3)
        {
          // precio_ciclo = EEPROM.read(502);
          precio_ciclo = ciclo_3;
        }
        if (DEFAULT_tipo_ciclo == 4)
        {
          //  precio_ciclo = EEPROM.read(502);
          precio_ciclo = ciclo_4;
        }
        if (DEFAULT_tipo_temperatura == 1)
        {
          precio_temp = 0;
        }
        if (DEFAULT_tipo_temperatura == 2)
        {
          //  precio_temp = EEPROM.read(503);
          precio_temp = precio_tibia;
        }
        if (DEFAULT_tipo_temperatura == 3)
        {
          //  precio_temp = EEPROM.read(504);
          precio_temp = precio_caliente;
        }
        if (DEFAULT_nivel_agua == 1)
        {
          precio_nivel = 0;
        }

        if (DEFAULT_nivel_agua == 2)
        {
          precio_nivel = nivel_2;
        }
        if (DEFAULT_nivel_agua == 3)
        {
          precio_nivel = nivel_3;
        }
        ddisplay.clear();
        display.setBrightness(0x0f);
        display.showNumberDec(((precio_temp + precio_ciclo + precio_nivel) * valor_moneda), true, 3, 1); // Expect: __04
      }

      if (tipo_ciclo.isPressed()) // evaluo si el boton fue presionado
      {
        if (dato_moneda == HIGH && datoAnterior_moneda == LOW)
        {
          contador_moneda++;
        }
        datoAnterior_moneda = dato_moneda;
        DEFAULT_tipo_ciclo++;
        DEFAULT_tipo_ciclo = tipo_ciclo.pulses(DEFAULT_tipo_ciclo); // limita los estados que puede haber en el switch
        menu_ciclo.cases(DEFAULT_tipo_ciclo, 1);
        seleccion_ciclo(DEFAULT_tipo_ciclo);
        seleccion_temperatura(DEFAULT_tipo_temperatura);
        seleccion_agua(DEFAULT_nivel_agua);
        led.ciclo_LED();
        parametros();

        if (DEFAULT_tipo_ciclo == 1)
        {
          // precio_ciclo = EEPROM.read(500);
          precio_ciclo = ciclo_1;
        }
        if (DEFAULT_tipo_ciclo == 2)
        {
          // precio_ciclo = EEPROM.read(501);
          precio_ciclo = ciclo_2;
        }
        if (DEFAULT_tipo_ciclo == 3)
        {
          // precio_ciclo = EEPROM.read(502);
          precio_ciclo = ciclo_3;
        }
        if (DEFAULT_tipo_ciclo == 4)
        {
          // precio_ciclo = EEPROM.read(502);
          precio_ciclo = ciclo_4;
        }
        if (DEFAULT_tipo_temperatura == 1)
        {
          precio_temp = 0;
        }
        if (DEFAULT_tipo_temperatura == 2)
        {
          // precio_temp = EEPROM.read(503);
          precio_temp = precio_tibia;
        }
        if (DEFAULT_tipo_temperatura == 3)
        {
          // precio_temp = EEPROM.read(504);
          precio_temp = precio_caliente;
        }
        if (DEFAULT_nivel_agua == 1)
        {
          precio_nivel = 0;
        }

        if (DEFAULT_nivel_agua == 2)
        {
          precio_nivel = nivel_2;
        }
        if (DEFAULT_nivel_agua == 3)
        {
          precio_nivel = nivel_3;
        }
        ddisplay.clear();
        display.setBrightness(0x0f);
        display.showNumberDec(((precio_temp + precio_ciclo + precio_nivel) * valor_moneda) - (contador_moneda * valor_moneda), true, 3, 1);
      }

      if (tipo_temperatura.isPressed()) // evaluo si el boton fue presionado
      {
        if (dato_moneda == HIGH && datoAnterior_moneda == LOW)
        {
          contador_moneda++;
        }
        datoAnterior_moneda = dato_moneda;
        DEFAULT_tipo_temperatura++;
        DEFAULT_tipo_temperatura = tipo_temperatura.pulses(DEFAULT_tipo_temperatura); // limita los estados que puede haber en el switch
        menu_temp.cases(DEFAULT_tipo_temperatura, 2);
        seleccion_ciclo(DEFAULT_tipo_ciclo);
        seleccion_temperatura(DEFAULT_tipo_temperatura);
        seleccion_agua(DEFAULT_nivel_agua);
        led.temperatura_LED();
        parametros();

        if (DEFAULT_tipo_ciclo == 1)
        {
          // precio_ciclo = EEPROM.read(500);
          precio_ciclo = ciclo_1;
        }
        if (DEFAULT_tipo_ciclo == 2)
        {
          // precio_ciclo = EEPROM.read(501);
          precio_ciclo = ciclo_2;
        }
        if (DEFAULT_tipo_ciclo == 3)
        {
          //  precio_ciclo = EEPROM.read(502);
          precio_ciclo = ciclo_3;
        }
        if (DEFAULT_tipo_ciclo == 4)
        {
          //  precio_ciclo = EEPROM.read(502);
          precio_ciclo = ciclo_4;
        }
        if (DEFAULT_tipo_temperatura == 1)
        {
          precio_temp = 0;
        }
        if (DEFAULT_tipo_temperatura == 2)
        {
          // precio_temp = EEPROM.read(503);
          precio_temp = precio_tibia;
        }
        if (DEFAULT_tipo_temperatura == 3)
        {
          // precio_temp = EEPROM.read(504);
          precio_temp = precio_caliente;
        }
        if (DEFAULT_nivel_agua == 1)
        {
          precio_nivel = 0;
        }

        if (DEFAULT_nivel_agua == 2)
        {
          precio_nivel = nivel_2;
        }
        if (DEFAULT_nivel_agua == 3)
        {
          precio_nivel = nivel_3;
        }
        ddisplay.clear();
        display.setBrightness(0x0f);
        display.showNumberDec(((precio_temp + precio_ciclo + precio_nivel) * valor_moneda) - (contador_moneda * valor_moneda), true, 3, 1);
      }

      if (nivel_agua.isPressed()) // evaluo si el boton fue presionado
      {
        dato_moneda = digitalRead(moneda_);
        // revisar
        if (dato_moneda == HIGH && datoAnterior_moneda == LOW)
        {
          contador_moneda++;
        }
        datoAnterior_moneda = dato_moneda;

        DEFAULT_nivel_agua++;
        DEFAULT_nivel_agua = nivel_agua.pulses(DEFAULT_nivel_agua); // limita los estados que puede haber en el switch
        menu_agua.cases(DEFAULT_nivel_agua, 3);
        seleccion_ciclo(DEFAULT_tipo_ciclo);
        seleccion_temperatura(DEFAULT_tipo_temperatura);
        seleccion_agua(DEFAULT_nivel_agua);
        led.agua_LED();
        parametros();

        if (DEFAULT_tipo_ciclo == 1)
        {
          // precio_ciclo = EEPROM.read(500);
          precio_ciclo = ciclo_1;
        }
        if (DEFAULT_tipo_ciclo == 2)
        {
          // precio_ciclo = EEPROM.read(501);
          precio_ciclo = ciclo_2;
        }
        if (DEFAULT_tipo_ciclo == 3)
        {
          // precio_ciclo = EEPROM.read(502);
          precio_ciclo = ciclo_3;
        }
        if (DEFAULT_tipo_ciclo == 4)
        {
          // precio_ciclo = EEPROM.read(502);
          precio_ciclo = ciclo_4;
        }
        if (DEFAULT_tipo_temperatura == 1)
        {
          precio_temp = 0;
        }
        if (DEFAULT_tipo_temperatura == 2)
        {
          // precio_temp = EEPROM.read(503);
          precio_temp = precio_tibia;
        }
        if (DEFAULT_tipo_temperatura == 3)
        {
          // precio_temp = EEPROM.read(504);
          precio_temp = precio_caliente;
        }
        if (DEFAULT_nivel_agua == 1)
        {
          precio_nivel = 0;
        }

        if (DEFAULT_nivel_agua == 2)
        {
          precio_nivel = nivel_2;
        }
        if (DEFAULT_nivel_agua == 3)
        {
          precio_nivel = nivel_3;
        }
        ddisplay.clear();
        display.setBrightness(0x0f);
        display.showNumberDec(((precio_temp + precio_ciclo + precio_nivel) * valor_moneda) - (contador_moneda * valor_moneda), true, 3, 1);
      }
      dato_moneda = digitalRead(moneda_);
      // revisar
      if (dato_moneda == HIGH && datoAnterior_moneda == LOW)
      {
        lastDebounceTime = millis();
        debounceDelay = 300000;
        contador_moneda++;
        tone(buzzer, 1000);
        delay(100);
        noTone(buzzer);
        ddisplay.clear();
        display.setBrightness(0x0f);
        display.showNumberDec(((precio_temp + precio_ciclo + precio_nivel) * valor_moneda) - (contador_moneda * valor_moneda), true, 3, 1);
        if (((precio_temp + precio_ciclo + precio_nivel) * valor_moneda) <= contador_moneda * valor_moneda)
        {
          contador_moneda = 0;
          activacion = 10;
          break;
        }
      }
      datoAnterior_moneda = dato_moneda;
    }
#endif

    /////////////////////////////////////////////////
    if (activacion == 0)
    {
      // Serial.println("XXXXXXXXXXXSSDDDFDFSDFSDFSDFSDFS");
      wdt_reset();
      if (tipo_ciclo.isPressed()) // evaluo si el boton fue presionado
      {
        wdt_reset();
        if (dato_moneda == HIGH && datoAnterior_moneda == LOW)
        {
          contador_moneda++;
        }
        datoAnterior_moneda = dato_moneda;
        DEFAULT_tipo_ciclo++;
        DEFAULT_tipo_ciclo = tipo_ciclo.pulses(DEFAULT_tipo_ciclo); // limita los estados que puede haber en el switch
        menu_ciclo.cases(DEFAULT_tipo_ciclo, 1);
        seleccion_ciclo(DEFAULT_tipo_ciclo);
        seleccion_temperatura(DEFAULT_tipo_temperatura);
        seleccion_agua(DEFAULT_nivel_agua);
        led.ciclo_LED();
        parametros();
      }
      /*if (digitalRead(maquina_on) == 0)
      {
        k = 1;
      }*/

      if (tipo_temperatura.isPressed()) // evaluo si el boton fue presionado
      {
        wdt_reset();
        if (dato_moneda == HIGH && datoAnterior_moneda == LOW)
        {
          contador_moneda++;
        }
        datoAnterior_moneda = dato_moneda;
        DEFAULT_tipo_temperatura++;
        if (DEFAULT_tipo_temperatura > 3)
        {
          DEFAULT_tipo_temperatura = 1;
        }
        DEFAULT_tipo_temperatura = tipo_temperatura.pulses(DEFAULT_tipo_temperatura); // limita los estados que puede haber en el switch
        menu_temp.cases(DEFAULT_tipo_temperatura, 2);
        seleccion_ciclo(DEFAULT_tipo_ciclo);             // carga de la eeprom los tiempos
        seleccion_temperatura(DEFAULT_tipo_temperatura); // carga de la eeprom el tipo de temperatura
        seleccion_agua(DEFAULT_nivel_agua);              // carga de la eeprom el nivel de agua
        led.temperatura_LED();
        parametros();
      }

      if (nivel_agua.isPressed()) // evaluo si el boton fue presionado
      {
        wdt_reset();
        if (dato_moneda == HIGH && datoAnterior_moneda == LOW)
        {
          contador_moneda++;
        }
        datoAnterior_moneda = dato_moneda;
        DEFAULT_nivel_agua++;
        if (DEFAULT_nivel_agua > 3)
        {
          DEFAULT_nivel_agua = 1;
        }
        DEFAULT_nivel_agua = nivel_agua.pulses(DEFAULT_nivel_agua); // limita los estados que puede haber en el switch
        menu_agua.cases(DEFAULT_nivel_agua, 3);
        seleccion_ciclo(DEFAULT_tipo_ciclo);
        seleccion_temperatura(DEFAULT_tipo_temperatura);
        seleccion_agua(DEFAULT_nivel_agua);
        led.agua_LED();
        parametros();
      }
      // if (Inicio.isPressed()&&activacion == 10){}
    }
    // if (Inicio.isPressed() || EEPROM.read(33) > 2) // evaluo si el boton fue presionado
    // Serial.print(etapa);
    if (Inicio.isPressed()|| etapa_2>2||activacion_2==10) // evaluo si el boton fue presionado
    {

      if (EEPROM.read(102) == 1)
      {
        ciclo_str = DEFAULT_tipo_ciclo;
        etapa_str = 0; /////0
        paso_str = 51; /////0
        String estado_lavadora = "{" + String(ciclo_str) + "," + String(etapa_str) + "," + String(paso_str) + "}\n";
        Serial2.print(estado_lavadora);
        EEPROM.update(102, 0);
        while (1)
        {
          digitalWrite(prelavadoLED, HIGH);
          digitalWrite(lavadoLED, HIGH);
          digitalWrite(enjuagueLED, HIGH);
          digitalWrite(enjuagueEXLED, HIGH);
          digitalWrite(enjugauefinLED, HIGH);
          digitalWrite(cenrifugadoLED, HIGH);
        }
      }

      if (!instruccionEnviada29)
      {

        ciclo_str = 0;
        etapa_str = DEFAULT_tipo_ciclo;
        paso_str = 0;
        String estado_lavadora = "[" + String(ciclo_str) + "," + String(etapa_str) + "," + String(paso_str) + "]\n";
        Serial2.print(estado_lavadora);
        instruccionEnviada29 = true;
      }

      //  if( EEPROM.read(33)>2)
      //{
      // activacion == 10;
      // }
      wdt_reset();
      digitalWrite(enable_monedero, HIGH);

#if precio_on == 2
      if (DEFAULT_tipo_ciclo == 1)
      {
        precio_ciclo = EEPROM.read(500);
      }
      if (DEFAULT_tipo_ciclo == 2)
      {
        precio_ciclo = EEPROM.read(501);
      }
      if (DEFAULT_tipo_ciclo == 3)
      {
        precio_ciclo = EEPROM.read(502);
      }

      if (DEFAULT_tipo_temperatura == 1)
      {
        precio_temp = 0;
      }
      if (DEFAULT_tipo_temperatura == 2)
      {
        precio_temp = EEPROM.read(503);
      }
      if (DEFAULT_tipo_temperatura == 3)
      {
        precio_temp = EEPROM.read(504);
      }
      if (DEFAULT_nivel_agua == 1)
      {
        precio_nivel = 0;
      }

      if (DEFAULT_nivel_agua == 2)
      {
        precio_nivel = nivel_2;
      }
      if (DEFAULT_nivel_agua == 3)
      {
        precio_nivel = nivel_3;
      }
      /*
            Serial.println(precio_temp);
            Serial.println(precio_ciclo);
            Serial.println(precio_temp + precio_ciclo);
      */
      /////////////////////////////////////////////////////////////
      ddisplay.clear();
      display.setBrightness(0x0f);
      display.showNumberDec(((precio_temp + precio_ciclo + precio_nivel)), true, 3, 1); // Expect: __04
#endif
#if precio_on == 1

      if (DEFAULT_tipo_ciclo == 1)
      {
        // precio_ciclo = EEPROM.read(500);
        precio_ciclo = ciclo_1;
      }
      if (DEFAULT_tipo_ciclo == 2)
      {
        // precio_ciclo = EEPROM.read(501);
        precio_ciclo = ciclo_2;
      }
      if (DEFAULT_tipo_ciclo == 3)
      {
        // precio_ciclo = EEPROM.read(502);
        precio_ciclo = ciclo_3;
      }
      if (DEFAULT_tipo_ciclo == 4)
      {
        // precio_ciclo = EEPROM.read(502);
        precio_ciclo = ciclo_4;
      }

      if (DEFAULT_tipo_temperatura == 1)
      {
        precio_temp = 0;
      }
      if (DEFAULT_tipo_temperatura == 2)
      {
        // precio_temp = EEPROM.read(503);
        precio_temp = precio_tibia;
      }
      if (DEFAULT_tipo_temperatura == 3)
      {
        //  precio_temp = EEPROM.read(504);
        precio_temp = precio_caliente;
      }
      if (DEFAULT_nivel_agua == 1)
      {
        precio_nivel = 0;
      }

      if (DEFAULT_nivel_agua == 2)
      {
        precio_nivel = nivel_2;
      }
      if (DEFAULT_nivel_agua == 3)
      {
        precio_nivel = nivel_3;
      }
      /*
            Serial.println(precio_temp);
            Serial.println(precio_ciclo);
            Serial.println(precio_temp + precio_ciclo);
      */
      /////////////////////////////////////////////////////////////
      ddisplay.clear();
      display.setBrightness(0x0f);
      display.showNumberDec(((precio_temp + precio_ciclo + precio_nivel) * valor_moneda), true, 3, 1); // Expect: __04

      while (activacion != 10)
      {
        if (Inicio.isPressed()) // evaluo si el boton fue presionado
        {
          if ((millis() - lastDebounceTime) > debounceDelay)
          {
            asm volatile(" jmp 0");
          }
          if (DEFAULT_tipo_ciclo == 1)
          {
            // precio_ciclo = EEPROM.read(500);
            precio_ciclo = ciclo_1;
          }
          if (DEFAULT_tipo_ciclo == 2)
          {
            // precio_ciclo = EEPROM.read(501);
            precio_ciclo = ciclo_2;
          }
          if (DEFAULT_tipo_ciclo == 3)
          {
            // precio_ciclo = EEPROM.read(502);
            precio_ciclo = ciclo_3;
          }
          if (DEFAULT_tipo_ciclo == 4)
          {
            //  precio_ciclo = EEPROM.read(502);
            precio_ciclo = ciclo_4;
          }
          if (DEFAULT_tipo_temperatura == 1)
          {
            precio_temp = 0;
          }
          if (DEFAULT_tipo_temperatura == 2)
          {
            //  precio_temp = EEPROM.read(503);
            precio_temp = precio_tibia;
          }
          if (DEFAULT_tipo_temperatura == 3)
          {
            //  precio_temp = EEPROM.read(504);
            precio_temp = precio_caliente;
          }
          if (DEFAULT_nivel_agua == 1)
          {
            precio_nivel = 0;
          }

          if (DEFAULT_nivel_agua == 2)
          {
            precio_nivel = nivel_2;
          }
          if (DEFAULT_nivel_agua == 3)
          {
            precio_nivel = nivel_3;
          }
          ddisplay.clear();
          display.setBrightness(0x0f);
          display.showNumberDec(((precio_temp + precio_ciclo + precio_nivel) * valor_moneda), true, 3, 1); // Expect: __04
        }

        if (tipo_ciclo.isPressed()) // evaluo si el boton fue presionado
        {
          if (dato_moneda == HIGH && datoAnterior_moneda == LOW)
          {
            contador_moneda++;
          }
          datoAnterior_moneda = dato_moneda;
          DEFAULT_tipo_ciclo++;
          DEFAULT_tipo_ciclo = tipo_ciclo.pulses(DEFAULT_tipo_ciclo); // limita los estados que puede haber en el switch
          menu_ciclo.cases(DEFAULT_tipo_ciclo, 1);
          seleccion_ciclo(DEFAULT_tipo_ciclo);
          seleccion_temperatura(DEFAULT_tipo_temperatura);
          seleccion_agua(DEFAULT_nivel_agua);
          led.ciclo_LED();
          parametros();

          if (DEFAULT_tipo_ciclo == 1)
          {
            // precio_ciclo = EEPROM.read(500);
            precio_ciclo = ciclo_1;
          }
          if (DEFAULT_tipo_ciclo == 2)
          {
            // precio_ciclo = EEPROM.read(501);
            precio_ciclo = ciclo_2;
          }
          if (DEFAULT_tipo_ciclo == 3)
          {
            // precio_ciclo = EEPROM.read(502);
            precio_ciclo = ciclo_3;
          }
          if (DEFAULT_tipo_ciclo == 4)
          {
            // precio_ciclo = EEPROM.read(502);
            precio_ciclo = ciclo_4;
          }
          if (DEFAULT_tipo_temperatura == 1)
          {
            precio_temp = 0;
          }
          if (DEFAULT_tipo_temperatura == 2)
          {
            // precio_temp = EEPROM.read(503);
            precio_temp = precio_tibia;
          }
          if (DEFAULT_tipo_temperatura == 3)
          {
            // precio_temp = EEPROM.read(504);
            precio_temp = precio_caliente;
          }
          if (DEFAULT_nivel_agua == 1)
          {
            precio_nivel = 0;
          }

          if (DEFAULT_nivel_agua == 2)
          {
            precio_nivel = nivel_2;
          }
          if (DEFAULT_nivel_agua == 3)
          {
            precio_nivel = nivel_3;
          }
          ddisplay.clear();
          display.setBrightness(0x0f);
          display.showNumberDec(((precio_temp + precio_ciclo + precio_nivel) * valor_moneda) - (contador_moneda * valor_moneda), true, 3, 1);
        }

        if (tipo_temperatura.isPressed()) // evaluo si el boton fue presionado
        {
          if (dato_moneda == HIGH && datoAnterior_moneda == LOW)
          {
            contador_moneda++;
          }
          datoAnterior_moneda = dato_moneda;
          DEFAULT_tipo_temperatura++;
          DEFAULT_tipo_temperatura = tipo_temperatura.pulses(DEFAULT_tipo_temperatura); // limita los estados que puede haber en el switch
          menu_temp.cases(DEFAULT_tipo_temperatura, 2);
          seleccion_ciclo(DEFAULT_tipo_ciclo);
          seleccion_temperatura(DEFAULT_tipo_temperatura);
          seleccion_agua(DEFAULT_nivel_agua);
          led.temperatura_LED();
          parametros();

          if (DEFAULT_tipo_ciclo == 1)
          {
            // precio_ciclo = EEPROM.read(500);
            precio_ciclo = ciclo_1;
          }
          if (DEFAULT_tipo_ciclo == 2)
          {
            // precio_ciclo = EEPROM.read(501);
            precio_ciclo = ciclo_2;
          }
          if (DEFAULT_tipo_ciclo == 3)
          {
            //  precio_ciclo = EEPROM.read(502);
            precio_ciclo = ciclo_3;
          }
          if (DEFAULT_tipo_ciclo == 4)
          {
            //  precio_ciclo = EEPROM.read(502);
            precio_ciclo = ciclo_4;
          }
          if (DEFAULT_tipo_temperatura == 1)
          {
            precio_temp = 0;
          }
          if (DEFAULT_tipo_temperatura == 2)
          {
            // precio_temp = EEPROM.read(503);
            precio_temp = precio_tibia;
          }
          if (DEFAULT_tipo_temperatura == 3)
          {
            // precio_temp = EEPROM.read(504);
            precio_temp = precio_caliente;
          }
          if (DEFAULT_nivel_agua == 1)
          {
            precio_nivel = 0;
          }

          if (DEFAULT_nivel_agua == 2)
          {
            precio_nivel = nivel_2;
          }
          if (DEFAULT_nivel_agua == 3)
          {
            precio_nivel = nivel_3;
          }
          ddisplay.clear();
          display.setBrightness(0x0f);
          display.showNumberDec(((precio_temp + precio_ciclo + precio_nivel) * valor_moneda) - (contador_moneda * valor_moneda), true, 3, 1);
        }

        if (nivel_agua.isPressed()) // evaluo si el boton fue presionado
        {
          dato_moneda = digitalRead(moneda_);
          // revisar
          if (dato_moneda == HIGH && datoAnterior_moneda == LOW)
          {
            contador_moneda++;
          }
          datoAnterior_moneda = dato_moneda;

          DEFAULT_nivel_agua++;
          DEFAULT_nivel_agua = nivel_agua.pulses(DEFAULT_nivel_agua); // limita los estados que puede haber en el switch
          menu_agua.cases(DEFAULT_nivel_agua, 3);
          seleccion_ciclo(DEFAULT_tipo_ciclo);
          seleccion_temperatura(DEFAULT_tipo_temperatura);
          seleccion_agua(DEFAULT_nivel_agua);
          led.agua_LED();
          parametros();

          if (DEFAULT_tipo_ciclo == 1)
          {
            // precio_ciclo = EEPROM.read(500);
            precio_ciclo = ciclo_1;
          }
          if (DEFAULT_tipo_ciclo == 2)
          {
            // precio_ciclo = EEPROM.read(501);
            precio_ciclo = ciclo_2;
          }
          if (DEFAULT_tipo_ciclo == 3)
          {
            // precio_ciclo = EEPROM.read(502);
            precio_ciclo = ciclo_3;
          }
          if (DEFAULT_tipo_ciclo == 4)
          {
            // precio_ciclo = EEPROM.read(502);
            precio_ciclo = ciclo_4;
          }
          if (DEFAULT_tipo_temperatura == 1)
          {
            precio_temp = 0;
          }
          if (DEFAULT_tipo_temperatura == 2)
          {
            // precio_temp = EEPROM.read(503);
            precio_temp = precio_tibia;
          }
          if (DEFAULT_tipo_temperatura == 3)
          {
            // precio_temp = EEPROM.read(504);
            precio_temp = precio_caliente;
          }
          if (DEFAULT_nivel_agua == 1)
          {
            precio_nivel = 0;
          }

          if (DEFAULT_nivel_agua == 2)
          {
            precio_nivel = nivel_2;
          }
          if (DEFAULT_nivel_agua == 3)
          {
            precio_nivel = nivel_3;
          }
          ddisplay.clear();
          display.setBrightness(0x0f);
          display.showNumberDec(((precio_temp + precio_ciclo + precio_nivel) * valor_moneda) - (contador_moneda * valor_moneda), true, 3, 1);
        }
        dato_moneda = digitalRead(moneda_);
        // revisar
        if (dato_moneda == HIGH && datoAnterior_moneda == LOW)
        {
          lastDebounceTime = millis();
          debounceDelay = 300000;
          contador_moneda++;
          tone(buzzer, 1000);
          delay(100);
          noTone(buzzer);
          ddisplay.clear();
          display.setBrightness(0x0f);
          display.showNumberDec(((precio_temp + precio_ciclo + precio_nivel) * valor_moneda) - (contador_moneda * valor_moneda), true, 3, 1);
          if (((precio_temp + precio_ciclo + precio_nivel) * valor_moneda) <= contador_moneda * valor_moneda)
          {
            contador_moneda = 0;
            activacion = 10;
            break;
          }
        }
        datoAnterior_moneda = dato_moneda;
      }
#endif

      if (activacion == 10 || EEPROM.read(1001) == 1)
      {
        // etapa =2;
        wdt_reset();
        uso += 1;
        /*   Serial.print(DEFAULT_tipo_ciclo);
           Serial.println(DEFAULT_tipo_ciclo);
           Serial.println(DEFAULT_tipo_temperatura);
           Serial.println(DEFAULT_nivel_agua);
           parametros();
           Serial.print(TIEMPO);*/
        // LLENADO_AGIpre = LLENADO_AGIpre_1;
        // DESAGUE_AGIpre = LLENADO_AGIpre_1;
        // CENTRIFUpre = CENTRIFUpre_1;
        // nivelde_llenado_prelavado = nivelde_llenado_prelavado_1;
        // TIEMPO_PRELAVADO = LLENADO_AGIpre + DESAGUE_AGIpre + CENTRIFUpre;
        //  Serial.print(EEPROM.read(400));

        //-------------------------------------

#if opl == 0
        if (TIEMPO_PRELAVADO * 60 > 0)
        {
          etapa = 1;
        }
        else if (TIEMPO_PRELAVADO * 60 == 0)
        {
          etapa = 2;
        }
        else if (TIEMPO_LAVADO * 60 == 0)
        {
          etapa = 3;
        }
        else if (TIEMPO_ENJUAGUE * 60 == 0)
        {
          etapa = 4;
        }
        else if (TIEMPO_ENJUAGUE_EXTRA_1 * 60 == 0)
        {
          etapa = 5;
        }
        else if (TIEMPO_ENJUAGUE_EXTRA_2 * 60 == 0)
        {
          etapa = 6;
        }
        else if (TIEMPO_ENJUAGUE_FINAL * 60 == 0)
        {
          etapa = 7;
        }

#endif

        //////////////////////////////////////////////////////////////////
        h1uso = highByte(uso);
        l1uso = lowByte(uso);
        //   EEPROM.update(1010, h1uso);
        //   EEPROM.update(1009, l1uso);
        // EEPROM.update(1002, uso);

        uso2 += 1;
        //   EEPROM.update(195, uso2);
        //////////////////////////////////////////////////////////
        ciclo_str = DEFAULT_tipo_ciclo;

        ddisplay.clear();
        led.inicio_LED();
        segundostotal = TIEMPO * 60;
        ahoras = ((segundostotal / 60) / 60);
        aminutos = (segundostotal / 60) % 60;
        asegundos = segundostotal % 60;

        ahoras = ((segundostotal / 60) / 60);
        aminutos = (segundostotal / 60) % 60;
        asegundos = segundostotal % 60;
        segundostotal = asegundos + (aminutos * 60) + (ahoras * 60 * 60);
        if (segundostotal == 0)
        {
          etapa = 0;
          delay(1000);
          while (1)
            ;
        }
        if (etapa == 1)
        {
          segundostotalB = segundostotal;
        }
        if (etapa == 2)
        {
          segundostotalB = segundostotal - TIEMPO_PRELAVADO * 60;
        }
        if (etapa == 3)
        {
          segundostotalB = segundostotal - (TIEMPO_PRELAVADO * 60 + TIEMPO_LAVADO * 60);
        }
        if (etapa == 4)
        {
          segundostotalB = segundostotal - (TIEMPO_PRELAVADO * 60 + TIEMPO_LAVADO * 60 + TIEMPO_ENJUAGUE * 60);
        }
        if (etapa == 5)
        {
          segundostotalB = segundostotal - (TIEMPO_PRELAVADO * 60 + TIEMPO_LAVADO * 60 + TIEMPO_ENJUAGUE * 60 + TIEMPO_ENJUAGUE_EXTRA_1 * 60);
        }
        if (etapa == 6)
        {
          segundostotalB = segundostotal - (TIEMPO_PRELAVADO * 60 + TIEMPO_LAVADO * 60 + TIEMPO_ENJUAGUE * 60 + TIEMPO_ENJUAGUE_EXTRA_1 * 60 + TIEMPO_ENJUAGUE_EXTRA_2 * 60);
        }
        if (etapa == 7)
        {
          segundostotalB = segundostotal - (TIEMPO_PRELAVADO * 60 + TIEMPO_LAVADO * 60 + TIEMPO_ENJUAGUE * 60 + TIEMPO_ENJUAGUE_EXTRA_1 * 60 + TIEMPO_ENJUAGUE_EXTRA_2 * 60 + TIEMPO_ENJUAGUE_FINAL * 60);
        }

        while (segundostotalB > 0)
        {

          wdt_reset();
          ///////////////////////////////////////////////////////////////////////////////////////
          int p_continental = 5;

          while (digitalRead(BUTT_BOT) == 1 && digitalRead(BUTT_TOP) == 1 && aminutos > 0 && auxiliar_puerta_error == 0 || digitalRead(BUTT_BOT) == 0 && digitalRead(BUTT_TOP) == 1 && aminutos > 0 && auxiliar_puerta_error == 0)
          {
            // Serial.print("XXXXXXXXXXXXXXXXXXX");
            wdt_reset();
            // Serial.print("wwwwwlklsdkfmsdlvmsñdlv,sñdlv,sdñlv,sdñlvmsdñlvmsdñlv,sdñlv,sdñlv,sñdlv,sñld,vñls,dvñls,");

            if (digitalRead(BUTT_BOT) == 0 && digitalRead(BUTT_TOP) == 1)
            {
#if continental == 1

              int aux_rele = 0;
              while (1)
              {
                wdt_reset();
                if ((millis() - lastTime) > 1000)
                {
                  /////////////////////////////////////
                  p_continental--;
                  aux_rele++;
                  lastTime = millis();
                  display.setBrightness(0x0f);
                  display.showNumberDec(p_continental, true, 2, 2); // Expect: __04
                  if (aux_rele <= 1)
                  {
                    // lavado.r_continental_off_1();
                    // delay(200);
                    lavado.r_continental_on_1();
                  }
                  else
                  {

                    // delay(200);
                    // lavado.r_continental_on_2();
                    // delay(3000);
                    // lavado.r_continental_off_2();
                    // delay(200);
                  }
                  // en la pantalla debe aparecer ese contador descendente
                  // mientras se activa un rele donde se activa  4 y 2 activo
                  // este rele permance encendido todo el ciclo
                }
                if (p_continental <= 0)
                {
                  // hay que activar  el otro rele donde se activa 4 y 3
                  // este rele es un pulso

                  auxiliar_puerta_error = 1;

                  break;
                }
              }
#endif
              lavado.PUERTA_ON();
              ddisplay.clear();
              delay(1000);
              wdt_reset();
            }
            if (digitalRead(BUTT_TOP) == 0 && digitalRead(BUTT_BOT) == 0)
            {
              // Serial.print("XXXXXXXXXXXXXXXXXXX");
              // Serial.println("2\n"); // inicio de ciclo
              // continuamos con el ciclo normal de la continental
              ddisplay.clear();
              // auxiliar_puerta_error = 1;

              delay(500);
              break;
            }
            else
            {
              // Serial.print("XXXXXXXXXXXXXXXXXXX");
              //  hay que volver a mandar el el pulso del rele 3 y 4
              wdt_reset();
              p_continental = 5;
              display.setBrightness(0x0f);
              display.setSegments(SEG_DOOR);
              //  Serial.println("33\n");
              delay(3000);

              wdt_reset();
              lavado.drenado();
              lavado.val_off();
              lavado.STOP_M();
              // aqui hay que agregar eso de la continental
              lavado.PUERTA_OFF();

              delay(3000); //////////////////////////////////
              contador_errores_e6++;
              if (contador_errores_e6 >= 20)
              {
                display.setSegments(SEG_E6);
                delay(3000);
                ciclo_str = 6;
                etapa_str = DEFAULT_tipo_ciclo;
                paso_str = 0;
                String estado_lavadora = "[" + String(ciclo_str) + "," + String(etapa_str) + "," + String(paso_str) + "]\n";
                Serial2.print(estado_lavadora);
                contador_errores_e6 = 0;
              }
              wdt_reset();
              // lavado.PUERTA_ON();
              ddisplay.clear();
            }
          }

          /////////////////////////
          /*
                         while (digitalRead(BUTT_BOT) == 1)
                         {
                           wdt_reset();
                           display.setBrightness(0x0f);
                           display.setSegments(SEG_DOOR);
                           if (digitalRead(BUTT_BOT) == 0)
                           {
                             lavado.r_continental_on_2();
                             delay(500);
                             lavado.PUERTA_ON();
                             ddisplay.clear();
                             break;
                           }
                           lavado.val_off();
                           lavado.STOP_M();
                         }
               */
          ////////////////////////
          /*
          time = millis() / 1000;
          dato2 = time;
          if (dato2 != datoAnterior2)
          {
            segundostotalB--;
            segundosAux++;
            tiempo_aux2++;
            switch (etapa)
            {
            case 1:
              avanzarEtapa(TIEMPO_PRELAVADO * 60);
              break;
            case 2:
              avanzarEtapa(TIEMPO_LAVADO * 60);
              break;
            case 3:
              avanzarEtapa(TIEMPO_ENJUAGUE * 60);
              break;
            case 4:
              avanzarEtapa(TIEMPO_ENJUAGUE_EXTRA_1 * 60);
              break;
            case 5:
              avanzarEtapa(TIEMPO_ENJUAGUE_EXTRA_2 * 60);
              break;
            case 6:
              avanzarEtapa(TIEMPO_ENJUAGUE_FINAL * 60);
              break;
            case 7:
              avanzarEtapa(TIEMPO_CENTRIFUGADO_FINAL * 60);
              break;
            }
            datoAnterior2 = dato2;
          }*/
          //////////////////////////////////////////////////////////////////////////////////////////
          aux_44 = 1;
          // delay (1000);

          time = millis() / 1000;
          dato2 = time;
          if (dato2 != datoAnterior2)
          {
            // Serial2.print("[2]");
            segundostotalB--;
            segundosAux++;
            tiempo_aux2++;
            if (segundosAux >= TIEMPO_PRELAVADO * 60 && etapa == 1)
            {

              if (segundosAux == 1)
              {
                average = 0;
              }
              //  EEPROM.update(33, 2);
              // etapa = EEPROM.read(33);
              etapa = 2;
              segundosAux = 0;
              tiempo_aux2 = 0;
              paso = 0;
            }
            if (segundosAux >= TIEMPO_LAVADO * 60 && etapa == 2)
            {
              if (segundosAux == 1)
              {
                average = 0;
              }
              //  Serial.println("LAVADO");
              //  EEPROM.update(33, 3);
              segundosAux = 0;
              etapa = 3;
              tiempo_aux2 = 0;
              paso = 0;
            }
            if (segundosAux >= TIEMPO_ENJUAGUE * 60 && etapa == 3)
            {
              if (segundosAux == 1)
              {
                average = 0;
              }
              //   Serial.println("ENJUAGUE");
              //  EEPROM.update(33, 4);
              segundosAux = 0;
              etapa = 4;
              tiempo_aux2 = 0;
              paso = 0;
            }
            if (segundosAux >= TIEMPO_ENJUAGUE_EXTRA_1 * 60 && etapa == 4)
            {
              if (segundosAux == 1)
              {
                average = 0;
              }
              //    Serial.println("ENJUAGUEX1");
              //   EEPROM.update(33, 5);
              segundosAux = 0;
              etapa = 5;
              tiempo_aux2 = 0;
              paso = 0;
            }
            if (segundosAux >= TIEMPO_ENJUAGUE_EXTRA_2 * 60 && etapa == 5)
            {
              if (segundosAux == 1)
              {
                average = 0;
              }
              //       Serial.println("ENJUAGUEX2");
              //   EEPROM.update(33, 6);
              segundosAux = 0;
              etapa = 6;
              tiempo_aux2 = 0;
              paso = 0;
            }
            if (segundosAux >= TIEMPO_ENJUAGUE_FINAL * 60 && etapa == 6)
            {
              if (segundosAux == 1)
              {
                average = 0;
              }
              //      Serial.println("ENJUAGUEF");
              // EEPROM.update(33, 7);
              segundosAux = 0;
              etapa = 7;
              tiempo_aux2 = 0;
              paso = 0;
            }
            if (segundosAux >= TIEMPO_CENTRIFUGADO_FINAL * 60 && etapa == 7)
            {
              if (segundosAux == 1)
              {
                average = 0;
              }
              //     Serial.println("CENTRIFUGADOF");
              //  EEPROM.update(33, 1);
              contadorP = 0;
              segundosAux = 0;
              etapa = 1;
              // doneeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee
            }
          }
          datoAnterior2 = dato2;

          ahoras = ((segundostotalB / 60) / 60);
          aminutos = (segundostotalB / 60) % 60;
          asegundos = segundostotalB % 60;
          if ((millis() - lastTime) > 1000)
          {
            //   Serial2.print("[2]");
            // ddisplay.clear();
            lastTime = millis();
            //       Serial.println(etapa);

            if (aminutos < 8)
            {
#if continental == 1
              lavado.r_continental_off_1();
              lavado.PUERTA_OFF();
              auxiliar_puerta_error = 1;
#endif
            }
          }
          if (aminutos > 0)
          {
            display.setBrightness(0x0f);

            uint8_t data[4];
            // display.showNumberDecEx(seconds, 0, true, 2, 2); // Obtiene los datos actuales del display
            display.showNumberDec(aminutos, true, 2, 2); // Expect: __04
            // Apagar los primeros dos segmentos
            data[0] = 0x00;
            data[1] = 0x00;
            data[2] = display.encodeDigit(aminutos / 10);
            data[3] = display.encodeDigit(aminutos % 10);
            display.setSegments(data);
          }
          else
          {
            display.setBrightness(0x0f);                  /////////////////////////////////////////////////para soltar la puerta
            display.showNumberDec(asegundos, true, 2, 2); // Expect: __04
            // aqui hay que agregar eso de la puerta continental
            if (asegundos < 60)
            {
#if continental == 1
              //   lavado.r_continental_off_1();
#endif
            }
            lavado.PUERTA_OFF();

            if (asegundos == 0)
            {
#if continental == 1
              /*
                            lavado.r_continental_off_1();
                            delay(200);
                            lavado.r_continental_on_2();
                            delay(4000);
                            wdt_reset();
                            delay(6000);
                            wdt_reset();
                            lavado.r_continental_off_2();
                            */

#endif
              while (1)
              {
                wdt_reset();
                time = millis() / 1000;
                datoP = time;
                if (datoP != datoAnteriorP) // timer para las monedas
                {
                  if (aux_55 <= 0)
                  {
                    // aqui hay que agregar lo de  la puerta de continental
                    lavado.PUERTA_OFF();
                  }
                  aux_55 = 1;
                  ddisplay.clear();
                  display.setBrightness(0x0f);
                  display.setSegments(SEG_DONE);
                  contadorP++;
                  if (contadorP % 2 == 0)
                  {
                    noTone(buzzer);
                  }
                  else
                  {
                    tone(buzzer, 2000);
                  }
                  if (contadorP >= 10)
                  {
                    // aqui hay que agregar lo de la puerta de continental
                    lavado.PUERTA_OFF();

                    // Serial2.print("02");
                    digitalWrite(prelavadoLED, LOW);
                    digitalWrite(lavadoLED, LOW);
                    digitalWrite(enjuagueLED, LOW);
                    digitalWrite(enjuagueEXLED, LOW);
                    digitalWrite(enjugauefinLED, LOW);
                    digitalWrite(cenrifugadoLED, LOW);
                    // digitalWrite(maquina_on, LOW);
                    /* val = analogRead(A0);
                    voltage = val * (5.0 / 1023.0);
                    voltage = voltage * 10000000;*/

                    activacion = 0;
                    etapa = 2;
                    // EEPROM.update(33, 1);

                    delay(9000);
                    // asm volatile(" jmp 0");
                  }
                }
                datoAnteriorP = datoP;
              }
            }
          }
          if (etapa == 1) // prelavado
          {
            digitalWrite(prelavadoLED, HIGH);
            digitalWrite(lavadoLED, LOW);
            digitalWrite(enjuagueLED, LOW);
            digitalWrite(enjuagueEXLED, LOW);
            digitalWrite(enjugauefinLED, LOW);
            digitalWrite(cenrifugadoLED, LOW);
            if (paso == 0)
            {
              if (!instruccionEnviada)
              {
                // Serial.println("11\n");
                ciclo_str = DEFAULT_tipo_ciclo;
                etapa_str = 3;
                paso_str = 11;
                String estado_lavadora = "{" + String(ciclo_str) + "," + String(etapa_str) + "," + String(paso_str) + "}\n";
                Serial2.print(estado_lavadora);

                instruccionEnviada = true;
              }

              if (tiempo_aux2 <= LLENADO_AGIpre * 60)
              {
                time = millis() / 1000;
                dato_llenado = time;
                llenado_mojado(dato_llenado, nivelde_llenado_prelavado, tiempo_giro_izquierda, tiempo_giro_derecha, tiempo_reposo, tiempo_aux2, LLENADO_AGIpre, temperatura, etapa);
              }
              else
              {
                lavado.STOP_M();
                ///////////////////////////////////////////////////////////////////////////////7
                //////////////////////////////////////////////////////////////////////////////
                t = 1;
                llenado_error = 1;
                error_llenado = 0;
                dato_error = 0;
                datoAnterior_error = 0;
                contador_error_llenado = 0;
                dato_llenado = 0;
                dato_desague = 0;
                datoAnterior_llenado = 0;
                contador_llenado = 0;
                datoAnterior_desague = 0;
                contador_desague = 0;
                llenado = 1;
                desague = 1;
                tiempo_aux2 = 0;
                paso = 1;
                average = 0;
              }
            }
            if (paso == 1)
            {
              if (!instruccionEnviada1)
              {
                //  Serial.println("12\n");
                ciclo_str = DEFAULT_tipo_ciclo;
                etapa_str = 3;
                paso_str = 12;
                String estado_lavadora = "{" + String(ciclo_str) + "," + String(etapa_str) + "," + String(paso_str) + "}\n";
                Serial2.print(estado_lavadora);
                instruccionEnviada1 = true;
              }

              if (tiempo_aux2 <= DESAGUE_AGIpre * 60)
              {
                time = millis() / 1000;
                dato_llenado = time;
                drenado_lavado(dato_llenado, 0, tiempo_giro_izquierda, tiempo_giro_derecha, tiempo_reposo, tiempo_aux2, LLENADO_AGIpre);
              }
              else
              {
                lavado.STOP_M();
                ///////////////////////////////////////////////////////////////////////////////
                ///////////////////////////////////////////////////////////////////////////////////////
                t = 1;
                error_llenado = 0;
                dato_error = 0;
                datoAnterior_error = 0;
                contador_error_llenado = 0;
                dato_llenado = 0;
                dato_desague = 0;
                datoAnterior_llenado = 0;
                contador_llenado = 0;
                datoAnterior_desague = 0;
                contador_desague = 0;
                llenado = 1;
                desague = 1;
                tiempo_aux2 = 0;
                average = 0;
                //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////7
                /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                paso = 2;
              }
            }
            if (paso == 2)
            {
              if (!instruccionEnviada2)
              {
                //   Serial.println("13\n");
                ciclo_str = DEFAULT_tipo_ciclo;
                etapa_str = 3;
                paso_str = 13;
                String estado_lavadora = "{" + String(ciclo_str) + "," + String(etapa_str) + "," + String(paso_str) + "}\n";
                Serial2.print(estado_lavadora);
                instruccionEnviada2 = true;
              }
              if (tiempo_aux2 <= CENTRIFUpre * 60)
              {
                if (tiempo_aux2 <= 30)
                {
                  if (tiempo_aux2 >= 29)
                  {
                    //   Serial.println("stop_centrifugado");
                    tiempoInicio = millis();
                    lavado.STOP_M();
                  }
                  else
                  {
                    if (tiempo_aux2 <= 5)
                    {
                      //   Serial.println("stop_centrifugado");
                      lavado.STOP_M();
                    }
                    else
                    {
                      //   Serial.println("derecha_centrifugado");
                      lavado.DERECHA_M();
                    }
                  }
                }
                else
                {
                  if (TIEMPO_CENTRIFUGADO_FINAL * 60 - tiempo_aux2 <= 20)
                  {
                    lavado.STOP_M();
                  }
                  else
                  {
                    unsigned long tiempoActual = millis();
                    unsigned long tiempoTranscurrido = tiempoActual - tiempoInicio;
                    if (tiempoTranscurrido < tiempoDeseado)
                    {
                      // Serial.println("LLenando 20 segundos");
                      lavado.agua_fria_centrifugado();
                    }
                    else
                    {
                      // Serial.println("y NO LLENA");
                      lavado.val_off();
                    }
                    //  Serial.println("centrifugado");
                    lavado.CENTRIFUGADO();
                  }
                }
              }
              else
              {
                lavado.STOP_M();
                t = 1;
                error_llenado = 0;
                dato_error = 0;
                datoAnterior_error = 0;
                contador_error_llenado = 0;
                dato_llenado = 0;
                dato_desague = 0;
                datoAnterior_llenado = 0;
                contador_llenado = 0;
                datoAnterior_desague = 0;
                contador_desague = 0;
                llenado = 1;
                desague = 1;
                tiempo_aux2 = 0;
                paso = 0;
                average = 0;
                ddisplay.clear();
              }
            }
          }
          if (etapa == 2) // lavado
          {
            digitalWrite(prelavadoLED, LOW);
            digitalWrite(lavadoLED, HIGH);
            digitalWrite(enjuagueLED, LOW);
            digitalWrite(enjuagueEXLED, LOW);
            digitalWrite(enjugauefinLED, LOW);
            digitalWrite(cenrifugadoLED, LOW);
            if (paso == 0)
            {
              if (!instruccionEnviada3)
              {
                //   Serial.println("14\n");
                ciclo_str = DEFAULT_tipo_ciclo;
                etapa_str = 4;
                paso_str = 14;
                String estado_lavadora = "{" + String(ciclo_str) + "," + String(etapa_str) + "," + String(paso_str) + "}\n";
                Serial2.print(estado_lavadora);
                instruccionEnviada3 = true;
              }
              if (tiempo_aux2 <= LLENADO_AGIlav * 60)
              {
                time = millis() / 1000;
                dato_llenado = time;
                llenado_mojado(dato_llenado, nivelde_llenado_lavado, tiempo_giro_izquierda, tiempo_giro_derecha, tiempo_reposo, tiempo_aux2, LLENADO_AGIlav, temperatura, etapa);
              }
              else
              {
                lavado.STOP_M();
                ///////////////////////////////////////////////////////////////////////////////7
                //////////////////////////////////////////////////////////////////////////////
                t = 1;
                llenado_error = 1;
                error_llenado = 0;
                dato_error = 0;
                datoAnterior_error = 0;
                contador_error_llenado = 0;
                dato_llenado = 0;
                dato_desague = 0;
                datoAnterior_llenado = 0;
                contador_llenado = 0;
                datoAnterior_desague = 0;
                contador_desague = 0;
                llenado = 1;
                desague = 1;
                average = 0;
                tiempo_aux2 = 0;
                paso = 1;
              }
            }
            if (paso == 1)
            {
              if (!instruccionEnviada4)
              {
                //  Serial.println("15\n");
                ciclo_str = DEFAULT_tipo_ciclo;
                etapa_str = 4;
                paso_str = 15;
                String estado_lavadora = "{" + String(ciclo_str) + "," + String(etapa_str) + "," + String(paso_str) + "}\n";
                Serial2.print(estado_lavadora);
                instruccionEnviada4 = true;
              }
              if (tiempo_aux2 <= DESAGUE_AGIlav * 60)
              {
                time = millis() / 1000;
                dato_llenado = time;
                drenado_lavado(dato_llenado, 0, tiempo_giro_izquierda, tiempo_giro_derecha, tiempo_reposo, tiempo_aux2, LLENADO_AGIlav);
              }
              else
              {
                lavado.STOP_M();
                ///////////////////////////////////////////////////////////////////////////////

                ///////////////////////////////////////////////////////////////////////////////////////
                t = 1;
                error_llenado = 0;
                dato_error = 0;
                datoAnterior_error = 0;
                contador_error_llenado = 0;
                dato_llenado = 0;
                dato_desague = 0;
                datoAnterior_llenado = 0;
                contador_llenado = 0;
                datoAnterior_desague = 0;
                contador_desague = 0;
                llenado = 1;
                desague = 1;
                average = 0;
                tiempo_aux2 = 0;
                paso = 2;
              }
            }
            if (paso == 2)
            {
              if (!instruccionEnviada5)
              {
                //  Serial.println("16\n");
                ciclo_str = DEFAULT_tipo_ciclo;
                etapa_str = 4;
                paso_str = 16;
                String estado_lavadora = "{" + String(ciclo_str) + "," + String(etapa_str) + "," + String(paso_str) + "}\n";
                Serial2.print(estado_lavadora);
                instruccionEnviada5 = true;
              }

              if (tiempo_aux2 <= CENTRIFUlav * 60)
              {
                if (tiempo_aux2 <= 30)
                {
                  if (tiempo_aux2 >= 29)
                  {
                    // Serial.println("stop_centrifugado_no se de que");
                    tiempoInicio = millis();
                    lavado.STOP_M();
                  }
                  else
                  {
                    if (tiempo_aux2 <= 5)
                    {
                      // Serial.println("stop_centrifugado_derecha");
                      lavado.STOP_M();
                    }
                    else
                    {
                      //  Serial.println("derecha_centrifugado");
                      lavado.DERECHA_M();
                    }
                  }
                }
                else
                {
                  if (TIEMPO_CENTRIFUGADO_FINAL * 60 - tiempo_aux2 <= 20)
                  {
                    lavado.STOP_M();
                  }
                  else
                  {
                    unsigned long tiempoActual = millis();
                    unsigned long tiempoTranscurrido = tiempoActual - tiempoInicio;
                    if (tiempoTranscurrido < tiempoDeseado)
                    {
                      //   Serial.println("LLenando 20 segundos");
                      lavado.agua_fria_centrifugado();
                    }
                    else
                    {
                      //    Serial.println("y NO LLENA");
                      lavado.val_off();
                    }
                    //   Serial.println("centrifugado");
                    lavado.CENTRIFUGADO();
                  }
                }
              }
              else
              {
                lavado.STOP_M();
                t = 1;
                error_llenado = 0;
                dato_error = 0;
                datoAnterior_error = 0;
                contador_error_llenado = 0;
                dato_llenado = 0;
                dato_desague = 0;
                datoAnterior_llenado = 0;
                contador_llenado = 0;
                datoAnterior_desague = 0;
                contador_desague = 0;
                llenado = 1;
                desague = 1;
                average = 0;
                tiempo_aux2 = 0;
                paso = 0;
                ddisplay.clear();
              }
            }
          }
          if (etapa == 3) // enjuague
          {
            digitalWrite(prelavadoLED, LOW);
            digitalWrite(lavadoLED, LOW);
            digitalWrite(enjuagueLED, HIGH);
            digitalWrite(enjuagueEXLED, LOW);
            digitalWrite(enjugauefinLED, LOW);
            digitalWrite(cenrifugadoLED, LOW);
            if (paso == 0)
            {
              if (!instruccionEnviada6)
              {
                //       Serial.println("17\n");
                ciclo_str = DEFAULT_tipo_ciclo;
                etapa_str = 5;
                paso_str = 17;
                String estado_lavadora = "{" + String(ciclo_str) + "," + String(etapa_str) + "," + String(paso_str) + "}\n";
                Serial2.print(estado_lavadora);
                instruccionEnviada6 = true;
              }
              if (tiempo_aux2 <= LLENADO_AGIeng * 60)
              {
                time = millis() / 1000;
                dato_llenado = time;
                llenado_mojado(dato_llenado, nivelde_llenado_enjugaue, tiempo_giro_izquierda, tiempo_giro_derecha, tiempo_reposo, tiempo_aux2, LLENADO_AGIeng, temperatura, etapa);
              }
              else
              {
                lavado.STOP_M();
                ///////////////////////////////////////////////////////////////////////////////7
                //////////////////////////////////////////////////////////////////////////////
                t = 1;
                llenado_error = 1;
                error_llenado = 0;
                dato_error = 0;
                datoAnterior_error = 0;
                contador_error_llenado = 0;
                dato_llenado = 0;
                dato_desague = 0;
                average = 0;
                datoAnterior_llenado = 0;
                contador_llenado = 0;
                datoAnterior_desague = 0;
                contador_desague = 0;
                llenado = 1;
                desague = 1;
                tiempo_aux2 = 0;
                paso = 1;
              }
            }
            if (paso == 1)
            {
              if (!instruccionEnviada7)
              {
                //     Serial.println("18\n");
                ciclo_str = DEFAULT_tipo_ciclo;
                etapa_str = 5;
                paso_str = 18;
                String estado_lavadora = "{" + String(ciclo_str) + "," + String(etapa_str) + "," + String(paso_str) + "}\n";
                Serial2.print(estado_lavadora);
                instruccionEnviada7 = true;
              }
              if (tiempo_aux2 <= DESAGUE_AGIeng * 60)
              {
                time = millis() / 1000;
                dato_llenado = time;
                drenado_lavado(dato_llenado, 0, tiempo_giro_izquierda, tiempo_giro_derecha, tiempo_reposo, tiempo_aux2, LLENADO_AGIeng);
              }
              else
              {
                lavado.STOP_M();
                ///////////////////////////////////////////////////////////////////////////////

                ///////////////////////////////////////////////////////////////////////////////////////
                t = 1;
                llenado_error = 1;
                error_llenado = 0;
                dato_error = 0;
                datoAnterior_error = 0;
                contador_error_llenado = 0;
                dato_llenado = 0;
                dato_desague = 0;
                average = 0;
                datoAnterior_llenado = 0;
                contador_llenado = 0;
                datoAnterior_desague = 0;
                contador_desague = 0;
                llenado = 1;
                desague = 1;
                tiempo_aux2 = 0;
                paso = 2;
              }
            }
            if (paso == 2)
            {
              if (!instruccionEnviada8)
              {
                //     Serial.println("19\n");
                ciclo_str = DEFAULT_tipo_ciclo;
                etapa_str = 5;
                paso_str = 19;
                String estado_lavadora = "{" + String(ciclo_str) + "," + String(etapa_str) + "," + String(paso_str) + "}\n";
                Serial2.print(estado_lavadora);
                instruccionEnviada8 = true;
              }
              if (tiempo_aux2 <= CENTRIFUeng * 60)
              {
                if (tiempo_aux2 <= 30)
                {
                  if (tiempo_aux2 >= 29)
                  {
                    // Serial.println("stop_centrifugado");
                    lavado.STOP_M();
                    tiempoInicio = millis();
                  }
                  else
                  {
                    if (tiempo_aux2 <= 5)
                    {
                      // Serial.println("stop_centrifugado");
                      lavado.STOP_M();
                    }
                    else
                    {
                      // Serial.println("derecha_centrifugado");
                      lavado.DERECHA_M();
                    }
                  }
                }
                else
                {
                  if (TIEMPO_CENTRIFUGADO_FINAL * 60 - tiempo_aux2 <= 20)
                  {
                    lavado.STOP_M();
                  }
                  else
                  {
                    unsigned long tiempoActual = millis();
                    unsigned long tiempoTranscurrido = tiempoActual - tiempoInicio;
                    if (tiempoTranscurrido < tiempoDeseado)
                    {
                      // Serial.println("LLenando 20 segundos");
                      lavado.agua_fria_centrifugado();
                    }
                    else
                    {
                      // Serial.println("y NO LLENA");
                      lavado.val_off();
                    }
                    //  Serial.println("centrifugado");
                    lavado.CENTRIFUGADO();
                  }
                }
              }
              else
              {
                lavado.STOP_M();
                t = 1;
                error_llenado = 0;
                dato_error = 0;
                datoAnterior_error = 0;
                contador_error_llenado = 0;
                dato_llenado = 0;
                dato_desague = 0;
                datoAnterior_llenado = 0;
                contador_llenado = 0;
                datoAnterior_desague = 0;
                contador_desague = 0;
                average = 0;
                llenado = 1;
                desague = 1;
                tiempo_aux2 = 0;
                paso = 0;
                ddisplay.clear();
              }
            }
          }
          if (etapa == 4) // enjuague extra1
          {
            digitalWrite(prelavadoLED, LOW);
            digitalWrite(lavadoLED, LOW);
            digitalWrite(enjuagueLED, LOW);
            digitalWrite(enjuagueEXLED, HIGH);
            digitalWrite(enjugauefinLED, LOW);
            digitalWrite(cenrifugadoLED, LOW);
            if (paso == 0)
            {
              if (!instruccionEnviada9)
              {
                //   Serial.println("20\n");
                ciclo_str = DEFAULT_tipo_ciclo;
                etapa_str = 6;
                paso_str = 20;
                String estado_lavadora = "{" + String(ciclo_str) + "," + String(etapa_str) + "," + String(paso_str) + "}\n";
                Serial2.print(estado_lavadora);
                instruccionEnviada9 = true;
              }
              if (tiempo_aux2 <= LLENADO_AGIex1 * 60)
              {
                time = millis() / 1000;
                dato_llenado = time;
                llenado_mojado(dato_llenado, nivel_de_llenado_ennjuague_extra_1, tiempo_giro_izquierda, tiempo_giro_derecha, tiempo_reposo, tiempo_aux2, LLENADO_AGIex1, temperatura, etapa);
              }
              else
              {
                lavado.STOP_M();
                ///////////////////////////////////////////////////////////////////////////////7
                //////////////////////////////////////////////////////////////////////////////
                t = 1;
                llenado_error = 1;
                error_llenado = 0;
                dato_error = 0;
                datoAnterior_error = 0;
                contador_error_llenado = 0;
                dato_llenado = 0;
                average = 0;
                dato_desague = 0;
                datoAnterior_llenado = 0;
                contador_llenado = 0;
                datoAnterior_desague = 0;
                contador_desague = 0;
                llenado = 1;
                desague = 1;
                tiempo_aux2 = 0;
                paso = 1;
              }
            }
            if (paso == 1)
            {
              if (!instruccionEnviada10)
              {
                //    Serial.println("21\n");
                ciclo_str = DEFAULT_tipo_ciclo;
                etapa_str = 6;
                paso_str = 21;
                String estado_lavadora = "{" + String(ciclo_str) + "," + String(etapa_str) + "," + String(paso_str) + "}\n";
                Serial2.print(estado_lavadora);
                instruccionEnviada10 = true;
              }
              if (tiempo_aux2 <= DESAGUE_AGIex1 * 60)
              {
                time = millis() / 1000;
                dato_llenado = time;
                drenado_lavado(dato_llenado, 0, tiempo_giro_izquierda, tiempo_giro_derecha, tiempo_reposo, tiempo_aux2, LLENADO_AGIex1);
              }
              else
              {
                lavado.STOP_M();
                ///////////////////////////////////////////////////////////////////////////////
                ///////////////////////////////////////////////////////////////////////////////////////
                t = 1;
                llenado_error = 1;
                error_llenado = 0;
                dato_error = 0;
                datoAnterior_error = 0;
                contador_error_llenado = 0;
                dato_llenado = 0;
                dato_desague = 0;
                datoAnterior_llenado = 0;
                average = 0;
                contador_llenado = 0;
                datoAnterior_desague = 0;
                contador_desague = 0;
                llenado = 1;
                desague = 1;
                tiempo_aux2 = 0;
                paso = 2;
              }
            }
            if (paso == 2)
            {
              if (!instruccionEnviada11)
              {
                //     Serial.println("22\n");
                ciclo_str = DEFAULT_tipo_ciclo;
                etapa_str = 6;
                paso_str = 22;
                String estado_lavadora = "{" + String(ciclo_str) + "," + String(etapa_str) + "," + String(paso_str) + "}\n";
                Serial2.print(estado_lavadora);
                instruccionEnviada11 = true;
              }
              if (tiempo_aux2 <= CENTRIFUex1 * 60)
              {
                if (tiempo_aux2 <= 30)
                {
                  if (tiempo_aux2 >= 29)
                  {
                    // Serial.println("stop_centrifugado");
                    lavado.STOP_M();
                    tiempoInicio = millis();
                  }
                  else
                  {
                    if (tiempo_aux2 <= 5)
                    {
                      // Serial.println("stop_centrifugado");
                      lavado.STOP_M();
                    }
                    else
                    {
                      // Serial.println("derecha_centrifugado");
                      lavado.DERECHA_M();
                    }
                  }
                }
                else
                {
                  if (TIEMPO_CENTRIFUGADO_FINAL * 60 - tiempo_aux2 <= 20)
                  {
                    lavado.STOP_M();
                  }
                  else
                  {
                    unsigned long tiempoActual = millis();
                    unsigned long tiempoTranscurrido = tiempoActual - tiempoInicio;
                    if (tiempoTranscurrido < tiempoDeseado)
                    {
                      // Serial.println("LLenando 20 segundos");
                      lavado.agua_fria_centrifugado();
                    }
                    else
                    {
                      // Serial.println("y NO LLENA");
                      lavado.val_off();
                    }
                    //  Serial.println("centrifugado");
                    lavado.CENTRIFUGADO();
                  }
                }
              }
              else
              {

                lavado.STOP_M();
                t = 1;
                error_llenado = 0;
                dato_error = 0;
                datoAnterior_error = 0;
                contador_error_llenado = 0;
                dato_llenado = 0;
                dato_desague = 0;
                datoAnterior_llenado = 0;
                average = 0;
                contador_llenado = 0;
                datoAnterior_desague = 0;
                contador_desague = 0;
                llenado = 1;
                desague = 1;
                tiempo_aux2 = 0;
                paso = 0;
                ddisplay.clear();
              }
            }
          }
          if (etapa == 5) // enjuague extra2
          {
            digitalWrite(prelavadoLED, LOW);
            digitalWrite(lavadoLED, LOW);
            digitalWrite(enjuagueLED, LOW);
            digitalWrite(enjuagueEXLED, HIGH);
            digitalWrite(enjugauefinLED, LOW);
            digitalWrite(cenrifugadoLED, LOW);
            if (paso == 0)
            {
              if (!instruccionEnviada12)
              {
                //     Serial.println("23\n");
                ciclo_str = DEFAULT_tipo_ciclo;
                etapa_str = 7;
                paso_str = 23;
                String estado_lavadora = "{" + String(ciclo_str) + "," + String(etapa_str) + "," + String(paso_str) + "}\n";
                Serial2.print(estado_lavadora);
                instruccionEnviada12 = true;
              }
              if (tiempo_aux2 <= LLENADO_AGIex2 * 60)
              {
                time = millis() / 1000;
                dato_llenado = time;
                llenado_mojado(dato_llenado, nivel_de_llenado_ennjuague_extra_2, tiempo_giro_izquierda, tiempo_giro_derecha, tiempo_reposo, tiempo_aux2, LLENADO_AGIex2, temperatura, etapa);
              }
              else
              {
                lavado.STOP_M();
                //////////////////////////////////////////////////////////////////////////////
                t = 1;
                llenado_error = 1;
                error_llenado = 0;
                dato_error = 0;
                datoAnterior_error = 0;
                contador_error_llenado = 0;
                dato_llenado = 0;
                dato_desague = 0;
                datoAnterior_llenado = 0;
                contador_llenado = 0;
                average = 0;
                datoAnterior_desague = 0;
                contador_desague = 0;
                llenado = 1;
                desague = 1;
                tiempo_aux2 = 0;
                paso = 1;
              }
            }
            if (paso == 1)
            {
              if (!instruccionEnviada13)
              {
                //    Serial.println("24\n");
                ciclo_str = DEFAULT_tipo_ciclo;
                etapa_str = 7;
                paso_str = 24;
                String estado_lavadora = "{" + String(ciclo_str) + "," + String(etapa_str) + "," + String(paso_str) + "}\n";
                Serial2.print(estado_lavadora);
                instruccionEnviada13 = true;
              }
              if (tiempo_aux2 <= DESAGUE_AGIex2 * 60)
              {
                time = millis() / 1000;
                dato_llenado = time;
                drenado_lavado(dato_llenado, 0, tiempo_giro_izquierda, tiempo_giro_derecha, tiempo_reposo, tiempo_aux2, LLENADO_AGIex2);
              }
              else
              {
                lavado.STOP_M();
                ///////////////////////////////////////////////////////////////////////////////
                ///////////////////////////////////////////////////////////////////////////////////////
                t = 1;
                error_llenado = 0;
                llenado_error = 1;
                average = 0;
                dato_error = 0;
                datoAnterior_error = 0;
                contador_error_llenado = 0;
                dato_llenado = 0;
                dato_desague = 0;
                datoAnterior_llenado = 0;
                contador_llenado = 0;
                datoAnterior_desague = 0;
                contador_desague = 0;
                llenado = 1;
                desague = 1;
                tiempo_aux2 = 0;
                paso = 2;
              }
            }
            if (paso == 2)
            {
              if (!instruccionEnviada14)
              {
                //    Serial.println("25\n");
                ciclo_str = DEFAULT_tipo_ciclo;
                etapa_str = 7;
                paso_str = 25;
                String estado_lavadora = "{" + String(ciclo_str) + "," + String(etapa_str) + "," + String(paso_str) + "}\n";
                Serial2.print(estado_lavadora);
                instruccionEnviada14 = true;
              }
              if (tiempo_aux2 <= CENTRIFUex2 * 60)
              {
                if (tiempo_aux2 <= 30)
                {

                  if (tiempo_aux2 >= 29)
                  {
                    // Serial.println("stop_centrifugado");
                    lavado.STOP_M();
                    tiempoInicio = millis();
                  }
                  else
                  {
                    if (tiempo_aux2 <= 5)
                    {
                      // Serial.println("stop_centrifugado");
                      lavado.STOP_M();
                    }
                    else
                    {
                      // Serial.println("derecha_centrifugado");
                      lavado.DERECHA_M();
                    }
                  }
                }
                else
                {
                  if (TIEMPO_CENTRIFUGADO_FINAL * 60 - tiempo_aux2 <= 20)
                  {
                    lavado.STOP_M();
                  }
                  else
                  {
                    unsigned long tiempoActual = millis();
                    unsigned long tiempoTranscurrido = tiempoActual - tiempoInicio;
                    if (tiempoTranscurrido < tiempoDeseado)
                    {
                      // Serial.println("LLenando 20 segundos");
                      lavado.agua_fria_centrifugado();
                    }
                    else
                    {
                      // Serial.println("y NO LLENA");
                      lavado.val_off();
                    }
                    //  Serial.println("centrifugado");
                    lavado.CENTRIFUGADO();
                  }
                }
              }
              else
              {
                lavado.STOP_M();
                t = 1;
                error_llenado = 0;
                dato_error = 0;
                datoAnterior_error = 0;
                contador_error_llenado = 0;
                dato_llenado = 0;
                average = 0;
                dato_desague = 0;
                datoAnterior_llenado = 0;
                contador_llenado = 0;
                datoAnterior_desague = 0;
                contador_desague = 0;
                llenado = 1;
                desague = 1;
                tiempo_aux2 = 0;
                paso = 0;
                ddisplay.clear();
              }
            }
          }
          if (etapa == 6) // enjuauge final
          {
            digitalWrite(prelavadoLED, LOW);
            digitalWrite(lavadoLED, LOW);
            digitalWrite(enjuagueLED, LOW);
            digitalWrite(enjuagueEXLED, LOW);
            digitalWrite(enjugauefinLED, HIGH);
            digitalWrite(cenrifugadoLED, LOW);
            if (paso == 0)
            {
              if (!instruccionEnviada15)
              {
                //       Serial.println("26\n");
                ciclo_str = DEFAULT_tipo_ciclo;
                etapa_str = 8;
                paso_str = 26;
                String estado_lavadora = "{" + String(ciclo_str) + "," + String(etapa_str) + "," + String(paso_str) + "}\n";
                Serial2.print(estado_lavadora);
                instruccionEnviada15 = true;
              }
              if (tiempo_aux2 <= LLENADO_AGIfin * 60)
              {
                time = millis() / 1000;
                dato_llenado = time;
                llenado_mojado(dato_llenado, nivel_de_llenado_ennjuague_final, tiempo_giro_izquierda, tiempo_giro_derecha, tiempo_reposo, tiempo_aux2, LLENADO_AGIfin, temperatura, etapa);
              }
              else
              {
                lavado.STOP_M();
                ///////////////////////////////////////////////////////////////////////////////7

                //////////////////////////////////////////////////////////////////////////////
                t = 1;
                llenado_error = 1;
                error_llenado = 0;
                dato_error = 0;
                datoAnterior_error = 0;
                contador_error_llenado = 0;
                dato_llenado = 0;
                average = 0;
                dato_desague = 0;
                datoAnterior_llenado = 0;
                contador_llenado = 0;
                datoAnterior_desague = 0;
                contador_desague = 0;
                llenado = 1;
                desague = 1;
                tiempo_aux2 = 0;
                paso = 1;
              }
            }
            if (paso == 1)
            {
              if (tiempo_aux2 <= DESAGUE_AGIfin * 60)
              {
                if (!instruccionEnviada16)
                {
                  //      Serial.println("27\n");
                  ciclo_str = DEFAULT_tipo_ciclo;
                  etapa_str = 8;
                  paso_str = 27;
                  String estado_lavadora = "{" + String(ciclo_str) + "," + String(etapa_str) + "," + String(paso_str) + "}\n";
                  Serial2.print(estado_lavadora);
                  instruccionEnviada16 = true;
                }
                time = millis() / 1000;
                dato_llenado = time;
                drenado_lavado(dato_llenado, 0, tiempo_giro_izquierda, tiempo_giro_derecha, tiempo_reposo, tiempo_aux2, LLENADO_AGIfin);
              }
              else
              {
                lavado.STOP_M();
                ///////////////////////////////////////////////////////////////////////////////

                ///////////////////////////////////////////////////////////////////////////////////////
                t = 1;
                llenado_error = 1;
                error_llenado = 0;
                dato_error = 0;
                datoAnterior_error = 0;
                contador_error_llenado = 0;
                dato_llenado = 0;
                dato_desague = 0;
                average = 0;
                datoAnterior_llenado = 0;
                contador_llenado = 0;
                datoAnterior_desague = 0;
                contador_desague = 0;
                llenado = 1;
                desague = 1;
                tiempo_aux2 = 0;
                paso = 2;
              }
            }
            if (paso == 2)
            {
              if (!instruccionEnviada17)
              {
                //    Serial.println("28\n");
                ciclo_str = DEFAULT_tipo_ciclo;
                etapa_str = 8;
                paso_str = 28;
                String estado_lavadora = "{" + String(ciclo_str) + "," + String(etapa_str) + "," + String(paso_str) + "}\n";
                Serial2.print(estado_lavadora);
                instruccionEnviada17 = true;
              }
              if (tiempo_aux2 <= CENTRIFUfin * 60)
              {
                if (tiempo_aux2 <= 30)
                {

                  if (tiempo_aux2 >= 29)
                  {
                    // Serial.println("stop_centrifugado");
                    lavado.STOP_M();
                    tiempoInicio = millis();
                  }
                  else
                  {
                    if (tiempo_aux2 <= 5)
                    {
                      // Serial.println("stop_centrifugado");
                      lavado.STOP_M();
                    }
                    else
                    {
                      // Serial.println("derecha_centrifugado");
                      lavado.DERECHA_M();
                    }
                  }
                }
                else
                {
                  if (TIEMPO_CENTRIFUGADO_FINAL * 60 - tiempo_aux2 <= 20)
                  {
                    lavado.STOP_M();
                  }
                  else
                  {
                    unsigned long tiempoActual = millis();
                    unsigned long tiempoTranscurrido = tiempoActual - tiempoInicio;
                    if (tiempoTranscurrido < tiempoDeseado)
                    {
                      // Serial.println("LLenando 20 segundos");
                      lavado.agua_fria_centrifugado();
                    }
                    else
                    {
                      // Serial.println("y NO LLENA");
                      lavado.val_off();
                    }
                    //  Serial.println("centrifugado");
                    lavado.CENTRIFUGADO();
                  }
                }
              }
              else
              {
                lavado.STOP_M();
                t = 1;
                error_llenado = 0;
                dato_error = 0;
                datoAnterior_error = 0;
                contador_error_llenado = 0;
                dato_llenado = 0;
                dato_desague = 0;
                datoAnterior_llenado = 0;
                contador_llenado = 0;
                datoAnterior_desague = 0;
                average = 0;
                contador_desague = 0;
                llenado = 1;
                desague = 1;
                tiempo_aux2 = 0;
                paso = 0;
                ddisplay.clear();
              }
            }
          }
          if (etapa == 7) // centrifugado final
          {
            digitalWrite(prelavadoLED, LOW);
            digitalWrite(lavadoLED, LOW);
            digitalWrite(enjuagueLED, LOW);
            digitalWrite(enjuagueEXLED, LOW);
            digitalWrite(enjugauefinLED, LOW);
            digitalWrite(cenrifugadoLED, HIGH);
            // centrifugadooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooled
            if (paso == 0)
            {
              if (!instruccionEnviada18)
              {
                //     Serial.println("29\n");
                ciclo_str = DEFAULT_tipo_ciclo;
                etapa_str = 9;
                paso_str = 29;
                String estado_lavadora = "{" + String(ciclo_str) + "," + String(etapa_str) + "," + String(paso_str) + "}\n";
                Serial2.print(estado_lavadora);
                instruccionEnviada18 = true;
              }
              if (tiempo_aux2 <= TIEMPO_CENTRIFUGADO_FINAL * 60)
              {
                if (tiempo_aux2 <= 180)
                {

                  if (tiempo_aux2 >= 179)
                  {
                    // Serial.println("stop_centrifugado");
                    lavado.STOP_M();
                  }
                  else
                  {
                    if (tiempo_aux2 <= 5)
                    {
                      // Serial.println("stop_centrifugado");
                      lavado.STOP_M();
                    }
                    else
                    {
#if amortiguador == 1
                      int intervalNumber = (tiempo_aux2 / 10) % 4;

                      // Serial.println(tiempo_aux2);
                      Serial.println(intervalNumber);
                      ////////////////////////////
                      switch (intervalNumber)
                      {
                      case 0:
                        lavado.STOP_M();

                        Serial.println("stop");
                        break;
                      case 1:
                        lavado.IZQUIERDA_M();
                        Serial.println("Izquierda");
                        break;
                      case 2:
                        lavado.STOP_M();

                        Serial.println("stop");
                        break;
                      case 3:
                        lavado.DERECHA_M();
                        Serial.println("derecha");
                        break;

                      default:
                        lavado.STOP_M();
                        break;
                      }

#endif
                      ///////////////////////////////
#if amortiguador == 0
                      // Serial.println("derecha_centrifugado");
                      lavado.DERECHA_M();

#endif
                    }
                  }
                }
                else
                {
                  if (TIEMPO_CENTRIFUGADO_FINAL * 60 - tiempo_aux2 <= 60)
                  {
                    if (!instruccionEnviada30)
                    {
                      ciclo_str = DEFAULT_tipo_ciclo;
                      etapa_str = 0; /////0
                      paso_str = 51; /////0
                      String estado_lavadora = "{" + String(ciclo_str) + "," + String(etapa_str) + "," + String(paso_str) + "}\n";
                      Serial2.print(estado_lavadora);
                      delay(500);
                      ciclo_str = 7;
                      etapa_str = DEFAULT_tipo_ciclo;
                      paso_str = 0;
                      String estado_lavadora_1 = "[" + String(ciclo_str) + "," + String(etapa_str) + "," + String(paso_str) + "]\n";
                      Serial2.print(estado_lavadora_1);
                      unsigned long interval = 1000; // Intervalo de 1 segundo entre cada envío
                      unsigned long previousMillis = millis();
                      for (int i = 0; i < 4; i++)
                      {
                        wdt_reset();
                        Serial2.print("ok_2\n");
                        // Serial2.print("ok\n"); // Envía el comando "01"
                        // Serial.print("ok\n");  // Envía el comando "01"
                        //  Espera hasta que haya pasado 1 segundo
                        while (millis() - previousMillis < interval)
                        {
                          // No hacer nada aquí, solo esperar
                        }
                        previousMillis = millis(); // Actualiza el tiempo para la próxima espera
                      }

                      instruccionEnviada30 = true;
                    }

                    lavado.STOP_M();
                  }
                  else
                  {
#if amortiguador == 1
                    int intervalNumber = (tiempo_aux2 / 120) % 2;

                    // Serial.println(tiempo_aux2);
                    Serial.println(intervalNumber);
                    ////////////////////////////
                    switch (intervalNumber)
                    {
                    case 0:
                      lavado.DERECHA_M();
                      cambio = 1;
                      if (cambio_2 == 1)
                      {
                        lavado.STOP_M();
                        cambio_2 = 0;
                        wdt_reset();
                        delay(6000);
                        wdt_reset();
                        wdt_reset();
                        delay(4000);
                        wdt_reset();
                        wdt_reset();
                        delay(6000);
                        wdt_reset();
                        wdt_reset();
                        delay(4000);
                        wdt_reset();
                      }
                      Serial.println("derecha");
                      break;
                    case 1:
                      if (cambio == 1)
                      {
                        lavado.STOP_M();
                        cambio = 0;
                        wdt_reset();
                        delay(6000);
                        wdt_reset();
                        wdt_reset();
                        delay(4000);
                        wdt_reset();
                        wdt_reset();
                        delay(6000);
                        wdt_reset();
                        wdt_reset();
                        delay(4000);
                        wdt_reset();
                      }
                      lavado.CENTRIFUGADO();
                      cambio_2 = 1;
                      Serial.println("spin");
                      break;

                    default:
                      lavado.STOP_M();
                      break;
                    }
#endif

#if amortiguador == 0
                    //  Serial.println("centrifugado");
                    lavado.CENTRIFUGADO();
#endif
                  }
                }
              }
              else
              {
                lavado.STOP_M();
                t = 1;
                error_llenado = 0;
                dato_error = 0;
                datoAnterior_error = 0;
                contador_error_llenado = 0;
                dato_llenado = 0;
                dato_desague = 0;
                datoAnterior_llenado = 0;
                contador_llenado = 0;
                datoAnterior_desague = 0;
                contador_desague = 0;
                llenado = 1;
                desague = 1;
                tiempo_aux2 = 0;
                paso = 0;
              }
            }
          }
          if (segundostotalB <= 0 || (asegundos <= 0 && aminutos <= 0)) // fin del ciclo
          {
            while (1)
            {
              wdt_reset();
              time = millis() / 1000;
              datoP = time;
              // aqui hay que agregar lo de la puerta de la continental
              lavado.PUERTA_OFF();
#if continental == 1
              lavado.r_continental_off_1();
              delay(200);
              lavado.r_continental_on_2();
              delay(10000);
              lavado.r_continental_off_2();
#endif
              if (datoP != datoAnteriorP) // timer para las monedas
              {
                if (aux_55 == 0)
                {
                  // aqui hay que agregar lo de la puerta de la continental
                  lavado.PUERTA_OFF();
                  // Serial.println("11\n");

#if continental == 1
                  lavado.r_continental_off_1();
                  delay(200);
                  lavado.r_continental_on_2();
                  delay(10000);
                  lavado.r_continental_off_2();
#endif
                }
                aux_55 = 1;
                ddisplay.clear();
                display.setBrightness(0x0f);
                display.setSegments(SEG_DONE);
                contadorP++;

                if (contadorP % 2 == 0)
                {
                  noTone(buzzer);
                }
                else
                {
                  tone(buzzer, 2000);
                }
                if (contadorP >= 10)
                {
                  // Serial2.print("02");
                  digitalWrite(prelavadoLED, LOW);
                  digitalWrite(lavadoLED, LOW);
                  digitalWrite(enjuagueLED, LOW);
                  digitalWrite(enjuagueEXLED, LOW);
                  digitalWrite(enjugauefinLED, LOW);
                  digitalWrite(cenrifugadoLED, LOW);
                  // digitalWrite(maquina_on, LOW);
                  /* val = analogRead(A0);
                  voltage = val * (5.0 / 1023.0);
                  voltage = voltage * 10000000;*/
                  activacion = 0;
                  etapa = 1;
                  // EEPROM.update(33, 1);

                  delay(9000);
                  // asm volatile(" jmp 0");
                }
              }
              datoAnteriorP = datoP;
            }
          }
        }
      }
    }

    break;
  case 2:
    wdt_reset();

    break;

  case 3:
    wdt_reset();
    break;

  case 4:
    wdt_reset();
    break;
  }
}