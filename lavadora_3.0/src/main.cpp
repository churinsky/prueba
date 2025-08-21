#include <Arduino.h>

#define modelo 0            // modelo de tarjeta nueva 0 vieja 1
#define precio_on 3         // 0 sin precio    2 para banda,  3 solo precio iniciando a mario
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
#include "EEPROM.h"
#include <Wire.h>
#include <SoftwareSerial.h>
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

static bool instruccionEnviada29 = false;

int aux_con_door = 1;
////////////////////////////////////////
const unsigned long tiempoDeseado = 20000;

unsigned long tiempoInicio;
unsigned long lastTime = 0;
String combinedString;

/////////////////////
// actualizar para el pay
/////////////////////
#define eprom 0 // actualizar parametros 1 ,, para bloquearlos 0///////////////////////
#define opl 0   // para quitar comunicacion y jale opl, 0 ,1 para activar comunicacion y 2 para  2 lo del instalador
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
#define BUTT_TOP 30   // button top-------------FALTA DEFINIRLOS
#define BUTT_BOT A2   // button bottom------------- FALTA DEFINIRLOS
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

// ========================= Helpers locales (mismo archivo) =========================

// -- 1) Selección de válvulas según ETAPA y temperatura (cuando “falta” nivel)
static inline void seleccionar_valvulas_por_etapa_temp(int etapa, int temperatura)
{
  if (etapa == 6)
  { // Enjuague final en cualquier temperatura
    lavado.enjuague_final();
    return;
  }
  switch (temperatura)
  {
  case 0:
    lavado.agua_fria();
    break;
  case 1:
    lavado.agua_tibia();
    break;
  case 2:
    lavado.agua_caliente();
    break;
  default:
    lavado.val_off();
    break;
  }
}

// -- 2) Apagar todas las válvulas
static inline void valvulas_off(void)
{
  lavado.val_off();
}

// -- 3) Aplica o apaga válvulas según si todavía falta nivel
static inline void manejar_valvulas(int nivel_objetivo, int nivel_actual, int etapa, int temperatura)
{
  if (nivel_objetivo > nivel_actual)
  {
    dosificador = 1; // mismo side-effect que el código original
    seleccionar_valvulas_por_etapa_temp(etapa, temperatura);
  }
  else
  {
    valvulas_off();
  }
}

// -- 4) Dosificación por etapa (replica exactamente la secuencia 1→4 con A12..A9)
static inline void dosificadores_off(void)
{
  digitalWrite(A12, LOW);
  digitalWrite(A11, LOW);
  digitalWrite(A10, LOW);
  digitalWrite(A9, LOW);
}

static inline void aplicar_dosificador(uint8_t n)
{
  digitalWrite(A12, (n == 1) ? HIGH : LOW);
  digitalWrite(A11, (n == 2) ? HIGH : LOW);
  digitalWrite(A10, (n == 3) ? HIGH : LOW);
  digitalWrite(A9, (n == 4) ? HIGH : LOW);
}

// Devuelve true si todavía seguimos dosificando (dentro de tiempo) o false si ya terminó el bloque 1..4
static bool correr_dosificacion_por_etapa(int ETAPA_local, uint32_t *pCont)
{
  // Mapeo 1:1 con tus variables de tiempo por etapa (mantén los mismos nombres globales)
  switch (ETAPA_local)
  {
  case 1:
    if (dosificador == 1 && *pCont <= (uint32_t)tiempo_dosificador1_ETAPA1)
    {
      aplicar_dosificador(1);
      return true;
    }
    if (dosificador <= 1)
    {
      dosificador = 2;
      *pCont = 0;
    }
    if (dosificador == 2 && *pCont <= (uint32_t)tiempo_dosificador2_ETAPA1)
    {
      aplicar_dosificador(2);
      return true;
    }
    if (dosificador <= 2)
    {
      dosificador = 3;
      *pCont = 0;
    }
    if (dosificador == 3 && *pCont <= (uint32_t)tiempo_dosificador3_ETAPA1)
    {
      aplicar_dosificador(3);
      return true;
    }
    if (dosificador <= 3)
    {
      dosificador = 4;
      *pCont = 0;
    }
    if (dosificador == 4 && *pCont <= (uint32_t)tiempo_dosificador4_ETAPA1)
    {
      aplicar_dosificador(4);
      return true;
    }
    break;

  case 2:
    if (dosificador == 1 && *pCont <= (uint32_t)tiempo_dosificador1_ETAPA2)
    {
      aplicar_dosificador(1);
      return true;
    }
    if (dosificador <= 1)
    {
      dosificador = 2;
      *pCont = 0;
    }
    if (dosificador == 2 && *pCont <= (uint32_t)tiempo_dosificador2_ETAPA2)
    {
      aplicar_dosificador(2);
      return true;
    }
    if (dosificador <= 2)
    {
      dosificador = 3;
      *pCont = 0;
    }
    if (dosificador == 3 && *pCont <= (uint32_t)tiempo_dosificador3_ETAPA2)
    {
      aplicar_dosificador(3);
      return true;
    }
    if (dosificador <= 3)
    {
      dosificador = 4;
      *pCont = 0;
    }
    if (dosificador == 4 && *pCont <= (uint32_t)tiempo_dosificador4_ETAPA2)
    {
      aplicar_dosificador(4);
      return true;
    }
    break;

  case 3:
    if (dosificador == 1 && *pCont <= (uint32_t)tiempo_dosificador1_ETAPA3)
    {
      aplicar_dosificador(1);
      return true;
    }
    if (dosificador <= 1)
    {
      dosificador = 2;
      *pCont = 0;
    }
    if (dosificador == 2 && *pCont <= (uint32_t)tiempo_dosificador2_ETAPA3)
    {
      aplicar_dosificador(2);
      return true;
    }
    if (dosificador <= 2)
    {
      dosificador = 3;
      *pCont = 0;
    }
    if (dosificador == 3 && *pCont <= (uint32_t)tiempo_dosificador3_ETAPA3)
    {
      aplicar_dosificador(3);
      return true;
    }
    if (dosificador <= 3)
    {
      dosificador = 4;
      *pCont = 0;
    }
    if (dosificador == 4 && *pCont <= (uint32_t)tiempo_dosificador4_ETAPA3)
    {
      aplicar_dosificador(4);
      return true;
    }
    break;

  case 4:
    if (dosificador == 1 && *pCont <= (uint32_t)tiempo_dosificador1_ETAPA4)
    {
      aplicar_dosificador(1);
      return true;
    }
    if (dosificador <= 1)
    {
      dosificador = 2;
      *pCont = 0;
    }
    if (dosificador == 2 && *pCont <= (uint32_t)tiempo_dosificador2_ETAPA4)
    {
      aplicar_dosificador(2);
      return true;
    }
    if (dosificador <= 2)
    {
      dosificador = 3;
      *pCont = 0;
    }
    if (dosificador == 3 && *pCont <= (uint32_t)tiempo_dosificador3_ETAPA4)
    {
      aplicar_dosificador(3);
      return true;
    }
    if (dosificador <= 3)
    {
      dosificador = 4;
      *pCont = 0;
    }
    if (dosificador == 4 && *pCont <= (uint32_t)tiempo_dosificador4_ETAPA4)
    {
      aplicar_dosificador(4);
      return true;
    }
    break;

  case 5:
    if (dosificador == 1 && *pCont <= (uint32_t)tiempo_dosificador1_ETAPA5)
    {
      aplicar_dosificador(1);
      return true;
    }
    if (dosificador <= 1)
    {
      dosificador = 2;
      *pCont = 0;
    }
    if (dosificador == 2 && *pCont <= (uint32_t)tiempo_dosificador2_ETAPA5)
    {
      aplicar_dosificador(2);
      return true;
    }
    if (dosificador <= 2)
    {
      dosificador = 3;
      *pCont = 0;
    }
    if (dosificador == 3 && *pCont <= (uint32_t)tiempo_dosificador3_ETAPA5)
    {
      aplicar_dosificador(3);
      return true;
    }
    if (dosificador <= 3)
    {
      dosificador = 4;
      *pCont = 0;
    }
    if (dosificador == 4 && *pCont <= (uint32_t)tiempo_dosificador4_ETAPA5)
    {
      aplicar_dosificador(4);
      return true;
    }
    break;

  case 6:
    // En etapa 6 no dosificas en tu código original (se deja por simetría)
    break;
  }

  // Si llegamos aquí, se terminó 1..4 → apagar
  dosificadores_off();
  dosificador = 5; // igual que el original
  *pCont = 0;
  return false;
}

// -- 5) Detección de nivel alcanzado (tu criterio actual “duro”)
static inline bool nivel_alcanzado(void)
{
  return (digitalRead(A0) == 0);
}

// -- 6) UI rápida que ya usabas
// Cache local para evitar escrituras innecesarias al display
static int __mm_prev = -1;

static inline void ui_mostrar_minutos(int mm)
{
  // Normaliza rango 0..99
  if (mm < 0)
    mm = 0;
  if (mm > 99)
    mm = 99;

  // Si no cambió, no toques el display
  if (mm == __mm_prev)
    return;
  __mm_prev = mm;

  display.setBrightness(0x0f);

  // Cargamos los 4 segmentos manualmente: dos primeros apagados, dos últimos = minutos
  uint8_t segs[4];
  segs[0] = 0x00;                         // dígito 0 apagado
  segs[1] = 0x00;                         // dígito 1 apagado
  segs[2] = display.encodeDigit(mm / 10); // decenas
  segs[3] = display.encodeDigit(mm % 10); // unidades
  display.setSegments(segs);
}

// -- 7) Manejo del error E1 (SE MANTIENE BLOQUEANTE como en tu código)
static void lanzar_error_llenado_bloqueante(void)
{
  lavado.drenado();
  lavado.STOP_M();
  valvulas_off();

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

  // Bucle infinito con buzzer (idéntico a tu comportamiento)
  while (1)
  {
    wdt_reset();
    tone(buzzer, 2000);
    delay(1000);
    noTone(buzzer);
    delay(1000);
  }
}

// --- Motor: usa el MISMO tipo del contador global (int*)
static void motor_step_int(int *pCont, int t_izq, int t_der, int t_rep)
{
  // seguridad básica: evita valores negativos o cero
  if (*pCont < 1)
  {
    *pCont = 1;
  }

  if (llenado == 1)
  {
    if (t == 0)
    {
      // Giro IZQUIERDA
      if (*pCont <= t_izq)
      {
        lavado.IZQUIERDA_M();
      }
      else
      {
        *pCont = 1;
        t = 1; // pasar a reposo
      }
    }
    else
    { // t == 1
      // REPOSO
      if (*pCont <= t_rep)
      {
        lavado.STOP_M();
      }
      else
      {
        t = 0; // volver a giro
        *pCont = 1;
        llenado = 0; // CAMBIO DE SENTIDO: siguiente bloque será DERECHA
      }
    }
  }
  else
  { // llenado == 0  → bloque DERECHA
    if (t == 0)
    {
      // Giro DERECHA
      if (*pCont <= t_der)
      {
        lavado.DERECHA_M();
      }
      else
      {
        *pCont = 1;
        t = 1; // pasar a reposo
      }
    }
    else
    { // t == 1
      // REPOSO
      if (*pCont <= t_rep)
      {
        lavado.STOP_M();
      }
      else
      {
        t = 0; // volver a giro
        *pCont = 1;
        llenado = 1; // CAMBIO DE SENTIDO: siguiente bloque será IZQUIERDA
      }
    }
  }
}

// =============================== FUNCIÓN PRINCIPAL ===============================
void llenado_mojado(int dato_llenado,
                    int nivelde_llenado_prelavado,
                    int tiempo_giro_izquierda,
                    int tiempo_giro_derecha,
                    int tiempo_reposo,
                    int tiempo_aux2,
                    int LLENADO_AGIpre,
                    int temperatura,
                    int ETAPA)
{
  // 1) Estado base: no drenado
  lavado.no_drenado();

  // 3) Válvulas: “falta nivel?” → abre según ETAPA/temperatura; si no, cierra.
  manejar_valvulas(nivelde_llenado_prelavado, average, ETAPA, temperatura);

  // 4) Ritmo a 1 Hz (solo si cambia el dato)
  if (dato_llenado != datoAnterior_llenado)
  {
    // --- UI (idéntica a tu llamada)
    ui_mostrar_minutos(aminutos);

    // --- Si AÚN falta nivel y está armado el “modo supervisión”, entra al bucle
    if (average <= nivelde_llenado_prelavado && llenado_error == 1)
    {
      // reseteo de “time” (tal como tenías) y lazo de supervisión
      time = 0;

      while (1) // *** se preserva el comportamiento bloqueante ***
      {
        // UI continua
        ui_mostrar_minutos(aminutos);

        // Watchdog
        wdt_reset();

        // Cronometría de error (segundos)
        time = millis() / 1000;
        dato_error = time;
        if (dato_error != datoAnterior_error)
        {
          // ====== Bloque por segundo ======
          contador_error_llenado++;
          contador_dosificador++;

          // 4.1) DOSIFICACIÓN (etapa-dependiente, exactamente como antes)
          (void)correr_dosificacion_por_etapa(ETAPA, (uint32_t *)&contador_dosificador);

          // 4.2) MOTOR: patrón izquierda/reposo/derecha/reposo con tus flags
          contador_llenado++; // avanza con tu tick de 1 Hz
          motor_step_int(&contador_llenado, tiempo_giro_izquierda, tiempo_giro_derecha, tiempo_reposo);

          // 4.3) TIMEOUT DE LLENADO → error E1 bloqueante (igual que original)
          if (contador_error_llenado >= (uint32_t)tiempo_error_llenado * 60U)
          {
            lanzar_error_llenado_bloqueante();
            // no retorna
          }

          // 4.4) NIVEL ALCANZADO (tu criterio actual es A0==0)
          if (nivel_alcanzado())
          {
            valvulas_off();
            average = nivelde_llenado_prelavado + 1; // mismo “truco” que traías
            contador_llenado = 1;
            t = 1;
            llenado_error = 0;
            llenado = 1;
            break; // salir del while(1) como en tu flujo
          }
        }

        // avance de “datoAnterior_error” (igual que traías)
        datoAnterior_error = dato_error;
      }
    }

    // 5) Si NO estamos en el modo supervisión, mantén el patrón de motor normal
    //    (esto replica tus dos bloques “llenado==1” y “llenado==0” fuera del while)
    if (llenado == 1)
    {
      contador_llenado++; // avanza con tu tick de 1 Hz
      motor_step_int(&contador_llenado, tiempo_giro_izquierda, tiempo_giro_derecha, tiempo_reposo);
    }
    else
    {
      contador_llenado++; // avanza con tu tick de 1 Hz
      motor_step_int(&contador_llenado, tiempo_giro_izquierda, tiempo_giro_derecha, tiempo_reposo);
    }
  }

  // 6) Avance de marca 1 Hz (idéntico)
  datoAnterior_llenado = dato_llenado;
}

void drenado_lavado(int dato_llenado,
                    int /*nivelde_llenado_prelavado*/,
                    int tiempo_giro_izquierda,
                    int tiempo_giro_derecha,
                    int tiempo_reposo,
                    int /*tiempo_aux2*/,
                    int /*LLENADO_AGIpre*/)
{
  // 1) Asegura drenado activo (igual que antes)
  lavado.drenado();

  // 2) Tick de 1 Hz: sólo avanza si cambió
  if (dato_llenado != datoAnterior_llenado)
  {
    // Avanza el contador de fase de motor
    contador_llenado++;

    // 3) Patrón IZQ → REPO → DER → REPO (misma lógica que tenías)
    motor_step_int(&contador_llenado, tiempo_giro_izquierda, tiempo_giro_derecha, tiempo_reposo);
  }

  // 4) Cierra el ciclo del tick
  datoAnterior_llenado = dato_llenado;
}

// ===================== Helpers de la SECUENCIA =====================

// 1) Códigos de etapa (mantengo tus números)
enum : int
{
  PRELAV = 1,
  LAV = 2,
  ENJ = 3,
  ENJX1 = 4,
  ENJX2 = 5,
  ENJF = 6,
  SPINF = 7
};
// 2) Pasos dentro de cada etapa
enum : int
{
  PASO_LLENA = 0,
  PASO_DRENA = 1,
  PASO_CENTRI = 2
};

// 3) Enviar estado una sola vez por paso (reemplaza los instruccionEnviadaX sueltos)
static int __ultimoAnuncio = -999;
inline void anunciar_una_vez(int ciclo_id, int etapa_code, int paso_code)
{
  int clave = (ciclo_id * 10000) + (etapa_code * 100) + paso_code;
  if (__ultimoAnuncio != clave)
  {
    String estado = "{" + String(ciclo_id) + "," + String(etapa_code) + "," + String(paso_code) + "}\n";
    Serial2.print(estado);
    __ultimoAnuncio = clave;
  }
}

// 4) Mapea etapa->duraciones (en minutos) y niveles
inline int dur_llenado(int e)
{
  switch (e)
  {
  case PRELAV:
    return LLENADO_AGIpre;
  case LAV:
    return LLENADO_AGIlav;
  case ENJ:
    return LLENADO_AGIeng;
  case ENJX1:
    return LLENADO_AGIex1;
  case ENJX2:
    return LLENADO_AGIex2;
  case ENJF:
    return LLENADO_AGIfin;
  default:
    return 0;
  }
}
inline int dur_desague(int e)
{
  switch (e)
  {
  case PRELAV:
    return DESAGUE_AGIpre;
  case LAV:
    return DESAGUE_AGIlav;
  case ENJ:
    return DESAGUE_AGIeng;
  case ENJX1:
    return DESAGUE_AGIex1;
  case ENJX2:
    return DESAGUE_AGIex2;
  case ENJF:
    return DESAGUE_AGIfin;
  default:
    return 0;
  }
}
inline int dur_centri(int e)
{
  switch (e)
  {
  case PRELAV:
    return CENTRIFUpre;
  case LAV:
    return CENTRIFUlav;
  case ENJ:
    return CENTRIFUeng;
  case ENJX1:
    return CENTRIFUex1;
  case ENJX2:
    return CENTRIFUex2;
  case ENJF:
    return CENTRIFUfin;
  default:
    return (e == SPINF ? TIEMPO_CENTRIFUGADO_FINAL : 0);
  }
}
inline int nivel_objetivo(int e)
{
  switch (e)
  {
  case PRELAV:
    return nivelde_llenado_prelavado;
  case LAV:
    return nivelde_llenado_lavado;
  case ENJ:
    return nivelde_llenado_enjugaue;
  case ENJX1:
    return nivel_de_llenado_ennjuague_extra_1;
  case ENJX2:
    return nivel_de_llenado_ennjuague_extra_2;
  case ENJF:
    return nivel_de_llenado_ennjuague_final;
  default:
    return 0;
  }
}

// 5) Reseteo compacto (es lo que repetías en todos los “else”)
inline void reset_estado_paso()
{
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
}

// 6) Avanza de etapa cuando pasa su tiempo total
inline void intentar_avanzar_etapa()
{
  // idéntico a tu lógica, solo más compacta
  if (etapa == PRELAV && segundosAux >= TIEMPO_PRELAVADO * 60)
  {
    etapa = LAV;
    segundosAux = 0;
    tiempo_aux2 = 0;
    paso = 0;
    average = 0;
  }
  else if (etapa == LAV && segundosAux >= TIEMPO_LAVADO * 60)
  {
    etapa = ENJ;
    segundosAux = 0;
    tiempo_aux2 = 0;
    paso = 0;
    average = 0;
  }
  else if (etapa == ENJ && segundosAux >= TIEMPO_ENJUAGUE * 60)
  {
    etapa = ENJX1;
    segundosAux = 0;
    tiempo_aux2 = 0;
    paso = 0;
    average = 0;
  }
  else if (etapa == ENJX1 && segundosAux >= TIEMPO_ENJUAGUE_EXTRA_1 * 60)
  {
    etapa = ENJX2;
    segundosAux = 0;
    tiempo_aux2 = 0;
    paso = 0;
    average = 0;
  }
  else if (etapa == ENJX2 && segundosAux >= TIEMPO_ENJUAGUE_EXTRA_2 * 60)
  {
    etapa = ENJF;
    segundosAux = 0;
    tiempo_aux2 = 0;
    paso = 0;
    average = 0;
  }
  else if (etapa == ENJF && segundosAux >= TIEMPO_ENJUAGUE_FINAL * 60)
  {
    etapa = SPINF;
    segundosAux = 0;
    tiempo_aux2 = 0;
    paso = 0;
    average = 0;
  }
  else if (etapa == SPINF && segundosAux >= TIEMPO_CENTRIFUGADO_FINAL * 60)
  {
    // fin de ciclo
    contadorP = 0;
    segundosAux = 0;
    etapa = PRELAV; // te dejo igual que tu “done…”
  }
}

// 7) Centrifugado (pega aquí tu cuerpo actual por etapa)
//    Para no cambiar comportamiento, llama a esta función en PASO_CENTRI
inline void run_centrifugado(int e)
{
  // Usa los mismos globals que ya tienes
  // tiempo_aux2 (segundos desde que inició el paso)
  // TIEMPO_CENTRIFUGADO_FINAL, tiempoInicio, tiempoDeseado
  // amortiguador, cambio, cambio_2
  // y las funciones: lavado.STOP_M(), .DERECHA_M(), .CENTRIFUGADO(), .agua_fria_centrifugado(), .val_off()

  // 1) Arranque suave ~30s (idéntico a lo que traías)
  if (tiempo_aux2 <= 30)
  {
    if (tiempo_aux2 >= 29)
    {
      lavado.STOP_M();
      tiempoInicio = millis(); // marcas el inicio para el “shot” de agua
    }
    else
    {
      if (tiempo_aux2 <= 5)
      {
        lavado.STOP_M();
      }
      else
      {
        lavado.DERECHA_M();
      }
    }
    return;
  }

  // 2) Últimos 20s del paso → STOP para no frenar en seco
  int dur = dur_centri(e);               // minutos
  int faltan = (dur * 60) - tiempo_aux2; // segs restantes del paso de centrifugado
  if (faltan <= 50)
  {
    lavado.STOP_M();
    return;
  }

  // 3) “Shot” de agua fría al inicio del centrifugado real (mismo criterio que usabas)
  //    durante 'tiempoDeseado' milisegundos desde que hiciste STOP de rampa.
  unsigned long ahora = millis();
  unsigned long trans = ahora - tiempoInicio;
  if (trans < tiempoDeseado)
  {
    // lavado.agua_fria_centrifugado();
  }
  else
  {
    lavado.val_off();
  }

#if amortiguador == 1
  // Patrón con amortiguador (idéntico a tu lógica)
  // Alterna bloques largos: DERECHA -> STOP -> SPIN -> STOP ...
  // Aquí uso bloques de 120s para el patrón como traías en final,
  // y de 10s en el pre-rampa (ya cubierto arriba).
  int intervalNumber = (tiempo_aux2 / 120) % 2;
  switch (intervalNumber)
  {
  case 0:
    lavado.DERECHA_M();
    cambio = 1;
    if (cambio_2 == 1)
    {
      lavado.STOP_M();
      cambio_2 = 0;
      // tus pausas largas: mantengo la semántica sin saturar el WDT
      for (int i = 0; i < 2; i++)
      {
        wdt_reset();
        delay(6000);
      }
      for (int i = 0; i < 2; i++)
      {
        wdt_reset();
        delay(4000);
      }
    }
    break;
  case 1:
    if (cambio == 1)
    {
      lavado.STOP_M();
      cambio = 0;
      for (int i = 0; i < 2; i++)
      {
        wdt_reset();
        delay(6000);
      }
      for (int i = 0; i < 2; i++)
      {
        wdt_reset();
        delay(4000);
      }
    }
    lavado.CENTRIFUGADO();
    cambio_2 = 1;
    break;
  default:
    lavado.STOP_M();
    break;
  }
#else
  // Sin amortiguador: centrifugado directo
  lavado.CENTRIFUGADO();
#endif
}

// 8) Ejecuta el PASO actual (0 = Llenado, 1 = Drenado, 2 = Centrifugado)
inline void run_paso_actual()
{
  // anuncio único (mantenemos tus códigos de “{ciclo,etapa_alias,paso_alias}”)
  // Para no complicarnos con todos los alias que usabas, mando un solo anuncio por (etapa,paso):
  // Si quieres conservar exactamente 11..29, puedes hacer un map aparte.
  anunciar_una_vez(DEFAULT_tipo_ciclo, etapa + 2, (etapa * 10) + (11 + paso)); // deja el patrón cerca de lo que tenías
  // --- SKIPS por tiempo = 0 ---
  if (paso == PASO_LLENA && dur_llenado(etapa) == 0)
  {
    lavado.STOP_M();
    reset_estado_paso();
    paso = PASO_DRENA; // salta directo a drenado
    return;
  }
  if (paso == PASO_DRENA && dur_desague(etapa) == 0)
  {
    lavado.STOP_M();
    reset_estado_paso();
    paso = PASO_CENTRI; // salta a centrifugado
    return;
  }
  if (paso == PASO_CENTRI && dur_centri(etapa) == 0)
  {
    lavado.STOP_M();
    reset_estado_paso();
    paso = PASO_LLENA; // siguiente etapa arrancará llenando
    ddisplay.clear();
    return;
  }

  // Ejecutar según paso
  if (paso == PASO_LLENA)
  {
    if (tiempo_aux2 <= dur_llenado(etapa) * 60)
    {
      time = millis() / 1000;
      dato_llenado = time;
      llenado_mojado(dato_llenado, nivel_objetivo(etapa), tiempo_giro_izquierda, tiempo_giro_derecha, tiempo_reposo, tiempo_aux2, dur_llenado(etapa), temperatura, etapa);
    }
    else
    {
      lavado.STOP_M();
      reset_estado_paso();
      paso = PASO_DRENA;
    }
  }
  else if (paso == PASO_DRENA)
  {
    if (tiempo_aux2 <= dur_desague(etapa) * 60)
    {
      time = millis() / 1000;
      dato_llenado = time;
      drenado_lavado(dato_llenado, 0, tiempo_giro_izquierda, tiempo_giro_derecha, tiempo_reposo, tiempo_aux2, dur_desague(etapa));
    }
    else
    {
      lavado.STOP_M();
      reset_estado_paso();
      paso = PASO_CENTRI;
    }
  }
  else
  { // PASO_CENTRI
    if (tiempo_aux2 <= dur_centri(etapa) * 60)
    {
      run_centrifugado(etapa);
    }
    else
    {
      lavado.STOP_M();
      reset_estado_paso();
      paso = PASO_LLENA; // siguiente etapa arranca llenando
      ddisplay.clear();
    }
  }
}

// 9) Un tick por segundo: descuenta, suma contadores y decide cambios de etapa
inline void tick_cada_segundo()
{
  segundostotalB--;
  segundosAux++;
  tiempo_aux2++;
  intentar_avanzar_etapa();
}

// Devuelve true cuando la puerta quedó lista para continuar el ciclo
// Devuelve false si salió por error/timeout o porque ya no aplica el bloqueo
bool bloqueo_puerta_loop(unsigned long &lastTime,
                         int &contador_errores_e6,
                         int aminutos,
                         volatile int &auxiliar_puerta_error)
{
  int p_continental = 5;

  // OJO: paréntesis para que no se mezcle el && con el ||
  while ((digitalRead(BUTT_BOT) == 1 && digitalRead(BUTT_TOP) == 1 && aminutos > 0 && auxiliar_puerta_error == 0) ||
         (digitalRead(BUTT_BOT) == 0 && digitalRead(BUTT_TOP) == 1 && aminutos > 0 && auxiliar_puerta_error == 0))
  {
    wdt_reset();

    // Caso: BOTON INFERIOR PRESIONADO, SUPERIOR CERRADO -> iniciar secuencia continental
    if (digitalRead(BUTT_BOT) == 0 && digitalRead(BUTT_TOP) == 1)
    {
#if continental == 1
      int aux_rele = 0;
      while (1)
      {
        wdt_reset();
        if ((millis() - lastTime) > 1000)
        {
          p_continental--;
          aux_rele++;
          lastTime = millis();

          display.setBrightness(0x0f);
          display.showNumberDec(p_continental, true, 2, 2); // __SS

          if (aux_rele <= 1)
          {
            // Primer pulso
            // lavado.r_continental_off_1();
            // delay(200);
            lavado.r_continental_on_1();
          }
          else
          {
            // Si más adelante decides usar el segundo relé, aquí va.
            // lavado.r_continental_on_2();
            // delay(3000);
            // lavado.r_continental_off_2();
          }
        }

        if (p_continental <= 0)
        {
          // Activar la lógica de “puerta lista”
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

    // Caso: AMBOS BOTONES SUELTOS (TOP==0 y BOT==0) -> continuar ciclo
    if (digitalRead(BUTT_TOP) == 0 && digitalRead(BUTT_BOT) == 0)
    {
      ddisplay.clear();
      delay(500);
      return true; // listo para seguir
    }

    // Caso: cualquier otra cosa -> mostrar DOOR, drenar y contabilizar error
    wdt_reset();
    p_continental = 5;

    display.setBrightness(0x0f);
    display.setSegments(SEG_DOOR);
    delay(3000);

    wdt_reset();
    lavado.drenado();
    lavado.val_off();
    lavado.STOP_M();
    lavado.PUERTA_OFF();
    delay(3000);

    contador_errores_e6++;
    if (contador_errores_e6 >= 20)
    {
      display.setSegments(SEG_E6);
      delay(3000);

      // Mantengo tu formato de estado
      int ciclo_str = 6;
      int etapa_str = DEFAULT_tipo_ciclo;
      int paso_str = 0;
      String estado_lavadora = "[" + String(ciclo_str) + "," + String(etapa_str) + "," + String(paso_str) + "]\n";
      Serial2.print(estado_lavadora);

      contador_errores_e6 = 0;
    }

    wdt_reset();
    ddisplay.clear();
  }

  // Se sale del while porque ya no aplica la condición (por tiempo o por estado)
  return false;
}

void setup()
{
  wdt_disable();
#if serial == 0
  BT.begin(9600);
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

#if (opl == 1)
  EEPROM.update(1001, 0); // opl por mientras
  ddisplay.clear();
  display.setBrightness(0x0f);
  display.setSegments(SEG_PAY);
#endif
#if (opl == 0)
  EEPROM.update(1001, 1); // opl por mientras
#endif

  wdt_enable(WDTO_8S);
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
        while (millis() - previousMillis < interval)
        {
        }
        status_etapa = 0;
        previousMillis = millis(); // Actualiza el tiempo para la próxima espera
      }
    }

    while (digitalRead(BUTT_BOT) == 0 && digitalRead(BUTT_TOP) == 0)
    {
      wdt_reset();
      display.setBrightness(0x0f);
      display.setSegments(SEG_E5);
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
       // Serial.print(tarjeta.substring(1, 21));
      }

      DEFAULT_tipo_ciclo = tipo_ciclo.pulses(DEFAULT_tipo_ciclo); // limita los estados que puede haber en el switch
     // Serial.print(DEFAULT_tipo_ciclo);
     // Serial.print(",");
      DEFAULT_tipo_temperatura = tipo_temperatura.pulses(DEFAULT_tipo_temperatura); // limita los estados que puede haber en el switch
     // Serial.print(DEFAULT_tipo_temperatura);
      DEFAULT_nivel_agua = nivel_agua.pulses(DEFAULT_nivel_agua); // limita los estados que puede haber en el switch
     // Serial.print(",");
    //  Serial.print(DEFAULT_nivel_agua);

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
    if (Serial2.available() > 0)
    {
      recived = Serial2.readString();
      if (recived.substring(0, 7) == "Precios")
      {
        tone(buzzer, 1000);
        delay(100);
        noTone(buzzer);
        //   Serial.println("precios_Actualziados");
        posicion1 = recived.indexOf("Precios: ");
        palabra1 = recived.substring(posicion1 + 11, posicion1 + 14);
        //  Serial.println(palabra1);
        EEPROM.update(500, palabra1.toInt());
        palabra1 = recived.substring(posicion1 + 17, posicion1 + 20);
        //     Serial.println(palabra1);
        EEPROM.update(501, palabra1.toInt());
        palabra1 = recived.substring(posicion1 + 23, posicion1 + 26);
        //    Serial.println(palabra1);
        EEPROM.update(502, palabra1.toInt());
        palabra1 = recived.substring(posicion1 + 29, posicion1 + 32);
        //    Serial.println(palabra1);
        EEPROM.update(503, palabra1.toInt());
        palabra1 = recived.substring(posicion1 + 36, posicion1 + 39);
        //   Serial.println(palabra1);
        EEPROM.update(504, palabra1.toInt());
      }

      if (recived.substring(0, 9) == "Resultado")
      {

       
        posicion1 = recived.indexOf("Resultado: ");
        palabra1 = recived.substring(posicion1 + 10, posicion1 + 15);
        aa = String(palabra1).toInt();
        aa1 = String(palabra1).toInt();

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

#if opl == 1
    const int expectedLength = 10;
    char incomingData[expectedLength + 1];
    int index = 0;

    while (index < expectedLength && activacion == 0)
    {
      wdt_reset();
      if (Serial2.available() > 0)
      {
        // Lee el siguiente byte disponible
        char incomingByte = Serial.read();
        index++;
      }
    }

    if (Serial2.available() > 0)
    {
      wdt_reset();
      recived = Serial2.readString();
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
          parametros();
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
        if (etapa_2 > 0)
        {
          activacion = 10;
          etapa = etapa_2;
        }

#if opl == 1
        if (etapa_2 < 0)
        {
          if (TIEMPO_PRELAVADO * 60 > 0)
          {
            etapa = 1;
            if (numbersArray[0].toInt() == 0)
            {
              etapa_2 = 1;
            }
          }
          else if (TIEMPO_LAVADO * 60 > 0)
          {
            etapa = 2;
            if (numbersArray[0].toInt() == 0)
            {
              etapa_2 = 2;
            }
          }
          else if (TIEMPO_ENJUAGUE * 60 > 0)
          {
            etapa = 3;
            if (numbersArray[0].toInt() == 0)
            {
              etapa_2 = 3;
            }
          }
          else if (TIEMPO_ENJUAGUE_EXTRA_1 * 60 > 0)
          {
            etapa = 4;
            if (numbersArray[0].toInt() == 0)
            {
              etapa_2 = 4;
            }
          }
          else if (TIEMPO_ENJUAGUE_EXTRA_2 * 60 > 0)
          {
            etapa = 5;
            if (numbersArray[0].toInt() == 0)
            {
              etapa_2 = 5;
            }
          }
          else if (TIEMPO_ENJUAGUE_FINAL * 60 > 0)
          {
            etapa = 6;
            if (numbersArray[0].toInt() == 0)
            {
              etapa_2 = 6;
            }
          }
          else if (TIEMPO_CENTRIFUGADO_FINAL * 60 > 0)
          {
            etapa = 7;
            if (numbersArray[0].toInt() == 0)
            {
              etapa_2 = 7;
            }
          }
        }
#endif
      }
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
    }

    if (Inicio.isPressed() || etapa_2 > 2 || activacion_2 == 10) // evaluo si el boton fue presionado
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
      /////////////////////////////////////////////////////////////
      ddisplay.clear();
      display.setBrightness(0x0f);
      display.showNumberDec(((precio_temp + precio_ciclo + precio_nivel)), true, 3, 1); // Expect: __04
#endif

      // ===================== INICIO SECUENCIA (drop‑in) =====================
      if (activacion == 10 || EEPROM.read(1001) == 1)
      {
        wdt_reset();
        uso += 1;

        // Elegir etapa inicial igual que tenías (si TIEMPO_PRELAVADO = 0, saltar a la siguiente, etc.)
        // Lo dejo intacto con tu macro opl:
#if opl == 0
        if (TIEMPO_PRELAVADO * 60 > 0)
          etapa = PRELAV;
        else if (TIEMPO_PRELAVADO * 60 == 0)
          etapa = LAV;
        else if (TIEMPO_LAVADO * 60 == 0)
          etapa = ENJ;
        else if (TIEMPO_ENJUAGUE * 60 == 0)
          etapa = ENJX1;
        else if (TIEMPO_ENJUAGUE_EXTRA_1 * 60 == 0)
          etapa = ENJX2;
        else if (TIEMPO_ENJUAGUE_EXTRA_2 * 60 == 0)
          etapa = ENJF;
        else if (TIEMPO_ENJUAGUE_FINAL * 60 == 0)
          etapa = SPINF;
#endif

        // Calcula tiempo total (es tu mismo bloque compactado)
        segundostotal = TIEMPO * 60;
        ahoras = ((segundostotal / 60) / 60);
        aminutos = (segundostotal / 60) % 60;
        asegundos = segundostotal % 60;
        segundostotal = asegundos + (aminutos * 60) + (ahoras * 60 * 60);
        if (segundostotal == 0)
        {
          etapa = 0;
          delay(1000);
          while (1)
          {
          }
        }

        // Ajusta segundostotalB dependiendo de etapa actual (igual que tú)
        if (etapa == PRELAV)
          segundostotalB = segundostotal;
        if (etapa == LAV)
          segundostotalB = segundostotal - TIEMPO_PRELAVADO * 60;
        if (etapa == ENJ)
          segundostotalB = segundostotal - (TIEMPO_PRELAVADO + TIEMPO_LAVADO) * 60;
        if (etapa == ENJX1)
          segundostotalB = segundostotal - (TIEMPO_PRELAVADO + TIEMPO_LAVADO + TIEMPO_ENJUAGUE) * 60;
        if (etapa == ENJX2)
          segundostotalB = segundostotal - (TIEMPO_PRELAVADO + TIEMPO_LAVADO + TIEMPO_ENJUAGUE + TIEMPO_ENJUAGUE_EXTRA_1) * 60;
        if (etapa == ENJF)
          segundostotalB = segundostotal - (TIEMPO_PRELAVADO + TIEMPO_LAVADO + TIEMPO_ENJUAGUE + TIEMPO_ENJUAGUE_EXTRA_1 + TIEMPO_ENJUAGUE_EXTRA_2) * 60;
        if (etapa == SPINF)
          segundostotalB = segundostotal - (TIEMPO_PRELAVADO + TIEMPO_LAVADO + TIEMPO_ENJUAGUE + TIEMPO_ENJUAGUE_EXTRA_1 + TIEMPO_ENJUAGUE_EXTRA_2 + TIEMPO_ENJUAGUE_FINAL) * 60;

        ddisplay.clear();
        led.inicio_LED();

        // === Bucle principal del ciclo ===
        while (segundostotalB > 0)
        {
          wdt_reset();

          // --- (A) BLOQUE PUERTA/CONTINENTAL ---
          // Deja aquí tu bloque tal cual (no lo toco). Solo colócalo antes del “tick” y del “run_paso_actual”.
          //  ... (pega tu bloque de puerta exactamente como está) ...

          if (!bloqueo_puerta_loop(lastTime, contador_errores_e6, aminutos, auxiliar_puerta_error))
          {
            // Si devuelve false, el bucle salió por condición no válida o error;
            // aquí no hacemos nada especial porque tu flujo original solo retomaba.
          }

          // --- (B) TICK CADA SEGUNDO ---
          time = millis() / 1000;
          dato2 = time;
          if (dato2 != datoAnterior2)
          {
            tick_cada_segundo();
            datoAnterior2 = dato2;
          }

          // Actualiza mm:ss y display (dejé tu lógica resumida; puedes conservar tus ifs de puerta si los necesitas)
          ahoras = ((segundostotalB / 60) / 60);
          aminutos = (segundostotalB / 60) % 60;
          asegundos = segundostotalB % 60;

          if (aminutos > 0)
          {
            display.setBrightness(0x0f);
            uint8_t data[4];
            display.showNumberDec(aminutos, true, 2, 2);
            data[0] = 0x00;
            data[1] = 0x00;
            data[2] = display.encodeDigit(aminutos / 10);
            data[3] = display.encodeDigit(aminutos % 10);
            display.setSegments(data);
          }
          else
          {
            display.setBrightness(0x0f);
            display.showNumberDec(asegundos, true, 2, 2);
            // puerta OFF cuando faltan <60s, tal como tenías
            lavado.PUERTA_OFF();
            if (asegundos == 0)
            {
              // FIN visual + buzzer (dejo tu mismo patrón)
              while (1)
              {
                wdt_reset();
                time = millis() / 1000;
                datoP = time;
                if (datoP != datoAnteriorP)
                {
                  ddisplay.clear();
                  display.setBrightness(0x0f);
                  display.setSegments(SEG_DONE);
                  contadorP++;
                  if (contadorP % 2 == 0)
                    noTone(buzzer);
                  else
                    tone(buzzer, 2000);
                  if (contadorP >= 10)
                  {
                    // Apagar todo y reiniciar como tú
                    digitalWrite(prelavadoLED, LOW);
                    digitalWrite(lavadoLED, LOW);
                    digitalWrite(enjuagueLED, LOW);
                    digitalWrite(enjuagueEXLED, LOW);
                    digitalWrite(enjugauefinLED, LOW);
                    digitalWrite(cenrifugadoLED, LOW);
                    activacion = 0;
                    etapa = LAV; // o 1/2 según tu preferencia actual; dejé igual que tu caso
                    while (1)
                    {
                    }
                  }
                }
                datoAnteriorP = datoP;
              }
            }
          }

          // --- (C) LEDs por etapa (idéntico, más compacto) ---
          digitalWrite(prelavadoLED, etapa == PRELAV ? HIGH : LOW);
          digitalWrite(lavadoLED, etapa == LAV ? HIGH : LOW);
          digitalWrite(enjuagueLED, etapa == ENJ ? HIGH : LOW);
          digitalWrite(enjuagueEXLED, (etapa == ENJX1 || etapa == ENJX2) ? HIGH : LOW);
          digitalWrite(enjugauefinLED, etapa == ENJF ? HIGH : LOW);
          digitalWrite(cenrifugadoLED, etapa == SPINF ? HIGH : LOW);

          // --- (D) Ejecutar el paso actual de la etapa ---
          run_paso_actual();
        }
      }
    }

    break;
  }
}