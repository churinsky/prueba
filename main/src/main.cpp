#include <Arduino.h>
//#include <LiquidCrystal.h>
#include <SPI.h>
#include <EEPROM.h>
#include <Wire.h>
#include "RTClib.h"
//#include <avr/wdt.h>
RTC_DS3231 rtc;
//LiquidCrystal lcd(37, 40, 41, 38, 42, 43);
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

#include <MFRC522.h>
#define SS_PIN 11
#define RST_PIN 5
/////////////////////////////////////////
byte H1 = EEPROM.read(6);
byte L1 = EEPROM.read(7);
int w1 = H1 << 8;
int uso = w1 + L1;
/////////////////////////////////////////////
/*byte H2 = EEPROM.read(9);
byte L2 = EEPROM.read(8);
int w2 = H2 << 8;
int numerodeusos = w2 + L2;*/
/////////////////////////////////////////////

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
/*int uso = EEPROM.read(2350);
int numerodeusos = EEPROM.read(2351);*/
MFRC522 mfrc522(SS_PIN, RST_PIN);
MFRC522::MIFARE_Key key;
byte readbackblock[18];
byte tipo[800] = {"llave"};
String cc = "";
char bb;

String parametros = "";
byte arr[800];
bool t = 1;
int tipo_maquina = 1; //1=speedqueen 0=wascomat modifica la forma de llenado y el uso de la puerta
int paso = 0;
int PRECIO;
int MONEDA;
int program = 0; //*modo progrmaccion on=1, modo proramacion off=0
int segundosAux = 0;
int contador_programacion = 0;
int contador_llenado_1 = 1;
long int segundostotal = 0;
long int segundostotalB = 0;
int dato;
int llenado_error = 1;
int espera_inicio = 0;
int datoAnterior;
int datoP;
int vigencia = 0; //PARA ACTIVAR LA VIGENCIA EN LA TARJETA
int contadorP = 0;
int datoAnteriorP = 0;
int dato2;
int datoAnterior2;
int dato_llenado;
int dato_desague;
int datoAnterior_llenado = 0;
int contador_llenado = 1;
int datoAnterior_desague = 0;
int contador_desague = 0;
bool llenado = 1;
bool desague = 1;
int val = 0;  //variable aux para leer presostato
int mapp = 0; // mapeo del presostato
int error_llenado = 0;
int dato_error;
int datoAnterior_error = 0;
int contador_error_llenado;
String Tag = "a73c0a26";
String tagID = "";

int dato_error_puerta;
int tiempo_error_puerta = 10;
int datoAnterior_error_puerta = 0;
int contador_error_puerta = 0;
float nivel_agua = 0.21;
//0.21
//3.78
//_____________________________________
int precio_fria = 50;     //EEPROM.read(0);
int precio_tibia = 40;    //EEPROM.read(1);
int precio_caliente = 40; //EEPROM.read(2);
//----------------------------------------------
int LLENADO_AGIpre /*EEPROM.read(3);*/;
int DESAGUE_AGIpre /*EEPROM.read(4);*/;
int CENTRIFUpre /*EEPROM.read(5);*/;
int nivelde_llenado_prelavado;
int TIEMPO_PRELAVADO;
//--------------------------------------

int LLENADO_AGIlav; /*EEPROM.read(7);*/
int DESAGUE_AGIlav /*EEPROM.read(8);*/;
int CENTRIFUlav;            /*EEPROM.read(9);*/
int nivelde_llenado_lavado; /*EEPROM.read(10);*/
int TIEMPO_LAVADO;

//--------------------------------------

int LLENADO_AGIeng;           /*EEPROM.read(11);*/
int DESAGUE_AGIeng;           /* EEPROM.read(12);*/
int CENTRIFUeng;              /*EEPROM.read(13);*/
int nivelde_llenado_enjugaue; /*EEPROM.read(14);*/
int TIEMPO_ENJUAGUE;

//--------------------------------------

int LLENADO_AGIex1; /*EEPROM.read(15);*/
int DESAGUE_AGIex1; /* EEPROM.read(16);*/
int CENTRIFUex1;    /*EEPROM.read(17);*/
int nivel_de_llenado_ennjuague_extra_1;
int TIEMPO_ENJUAGUE_EXTRA_1;

//-------------------------------------------

int LLENADO_AGIex2 /*EEPROM.read(19);*/;
int DESAGUE_AGIex2; /*EEPROM.read(20);*/
int CENTRIFUex2 /*EEPROM.read(21);*/;
int nivel_de_llenado_ennjuague_extra_2;
int TIEMPO_ENJUAGUE_EXTRA_2;

//--------------------------------------------

int LLENADO_AGIfin;                   /*EEPROM.read(23);*/
int DESAGUE_AGIfin;                   /*EEPROM.read(24);*/
int CENTRIFUfin;                      /*EEPROM.read(25);*/
int nivel_de_llenado_ennjuague_final; /*EEPROM.read(26);*/
int TIEMPO_ENJUAGUE_FINAL;

//-----------------------------------------

int TIEMPO_CENTRIFUGADO_FINAL; /*EEPROM.read(27);*/
int nivel_de_llenado_centrifugado /*EEPROM.read(28);*/;
//---------------------------------------------

int TIEMPO;

// ------------------------------------------
int tiempo_giro_izquierda; /*EEPROM.read(29);*/
int tiempo_giro_derecha;   /*EEPROM.read(30);*/
int tiempo_reposo;         /*EEPROM.read(31);*/
//-----------------------------------------
int CICLO_DEFAULT /*EEPROM.read(32);*/;

int etapa = EEPROM.read(33); //// HABILITAR PARA CONTINUAR CON EL CICLO DESPUES DE QUE SE VAYA LA LUZ<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
//------------------------------------------------
int promo_lunes_F = EEPROM.read(40);
int promo_martes_F = EEPROM.read(41);
int promo_miercoles_F = EEPROM.read(42);
int promo_jueves_F = EEPROM.read(43);
int promo_viernes_F = EEPROM.read(44);
int promo_sabado_F = EEPROM.read(45);
int promo_domingo_F = EEPROM.read(46);
//---------------------------------------------------
int promo_lunes_C = EEPROM.read(47);
int promo_martes_C = EEPROM.read(48);
int promo_miercoles_C = EEPROM.read(49);
int promo_jueves_C = EEPROM.read(50);
int promo_viernes_C = EEPROM.read(51);
int promo_sabado_C = EEPROM.read(52);
int promo_domingo_C = EEPROM.read(53);
//-------------------------------------------------
int promo_lunes_T = EEPROM.read(54);
int promo_martes_T = EEPROM.read(55);
int promo_miercoles_T = EEPROM.read(56);
int promo_jueves_T = EEPROM.read(57);
int promo_viernes_T = EEPROM.read(58);
int promo_sabado_T = EEPROM.read(59);
int promo_domingo_T = EEPROM.read(60);
//-----------------------------------------
int tiempo_error_llenado = 15; // EEPROM.read(61); /////definir manual
//
int precio_1;
int precio_2;
int precio_3;
int tiempo_aux2 = 0;
int ahoras = 0;
int aminutos = 0;
int asegundos = 0;
int temperatura = 0;
int cambio;
unsigned long time = 0;
//_------------------------------
float voltage;

// ESTADOS DE INICIO DEL BUCLE SWITCH
#define FRIA 0
#define TIBIA 1
#define CALIENTE 2
#define INICIO 3
#define PROG 4
#define NORMAL 5
#define DELICADO 6
#define MUYSUCIO 7

////////////////////////////////////////////
uint8_t estado_2 = 0;
uint8_t estado = 0; /*CICLO_DEFAULT;*/ //estado actual

//SELECCION EN EL PANEL Y MOSTRAR EN LCD|
#define BTN_FRIA 0
#define BTN_CALIENTE 1
#define BTN_TIBIA 2
#define BTN_INICIO 3
#define BTN_PROG 4
#define BTN_NORMAL 5
#define BTN_DELICADO 6
#define BTN_MUYSUCIO 7

//ENTRADAS Y SALIDAS HACIA LA TARHETA DE CONTROL4
#define buzzer 13
#define PRESOSTATO A0
#define MOTOR_BAJA 34 //motor baja   EL 34 ES CORRECTO AQUI se cambnio a 33 en peña
#define SECUENCIA 39  //cambio de secuencia
#define MOTOR_ALTA 28 //motor alta
#define PUERTA 22
#define DESAGUE 26
#define TAMBOR_DEPOSITO1_FRIA 36
#define TAMBOR_DEPOSITO1_CALIENTE 44
#define DEPOSITO2 23
#define DEPOSITO3 24
#define BUTT_TOP 30 //button top-------------FALTA DEFINIRLOS
#define BUTT_BOT A2 //button bottom------------- FALTA DEFINIRLOS
//leds
#define led_lavado 38
#define led_enjuague 41 /// EL 33 ES CORRECTO AQUI se cambio a 34 en peña
#define led_enjuague_f 40
#define led_centrifugado 37
#define led_puerta_cerrada 35
#define led_cloro 33

//uint8_t button_state[5];
//uint8_t button[5] = {7, 9, 8, 10, 6};
uint8_t button_state[8];
uint8_t button[8] = {7, 9, 8, 10, 6, 4, 2, 3};
uint8_t a = 0;
uint8_t b = 0;
uint8_t c = 0;
uint8_t d = 0;
uint8_t e = 0;
uint8_t f = 0;
uint8_t g = 0;
uint8_t h = 0;
//uint8_t data[5] = {a, b, c, d, e};
uint8_t data[8] = {a, b, c, d, e, f, g, h};
uint8_t flancoSubida(int btn) // lectura de flanco para los botones
{
  uint8_t valor_nuevo = digitalRead(button[btn]);
  //delay(100);
  uint8_t result = button_state[btn] != valor_nuevo && valor_nuevo == valor_nuevo == 1;
  button_state[btn] = valor_nuevo;
  return result;
}

int readBlock(int blockNumber, byte arrayAddress[]) // lectura de RFID
{
  int largestModulo4Number = blockNumber / 4 * 4;
  int trailerBlock = largestModulo4Number + 3;
  byte status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, trailerBlock, &key, &(mfrc522.uid));

  if (status != MFRC522::STATUS_OK)
  {
  /*  lcd.setCursor(0, 1);
    lcd.print("ERROR LECTURA TAG");*/
    //Serial.print("PCD_Authenticate() failed: ");
    //Serial.println(mfrc522.GetStatusCodeName(status));
    delay(2000);
    asm volatile(" jmp 0");
    return 3; //return "3" as error message
  }

  byte buffersize = 18;
  status = mfrc522.MIFARE_Read(blockNumber, arrayAddress, &buffersize);
  if (status != MFRC522::STATUS_OK)
  {
    /*lcd.setCursor(0, 1);
    lcd.print("ERROR LECTURA TAG");*/
    //Serial.print("PCD_Authenticate() failed: ");
    //Serial.println(mfrc522.GetStatusCodeName(status));
    delay(2000);
    asm volatile(" jmp 0");
    return 4; //return "4" as error message
  }
}

void PUERTA_ON()
{
  digitalWrite(led_puerta_cerrada, HIGH);
  digitalWrite(PUERTA, LOW);
}
void PUERTA_OFF()
{
  digitalWrite(led_puerta_cerrada, LOW);
  digitalWrite(PUERTA, HIGH);
}

void IZQUIERDA_M()
{
  digitalWrite(SECUENCIA, LOW);
  digitalWrite(MOTOR_BAJA, HIGH);
  digitalWrite(MOTOR_ALTA, HIGH);
}
void DERECHA_M()
{
  digitalWrite(SECUENCIA, HIGH);
  digitalWrite(MOTOR_BAJA, LOW);
  digitalWrite(MOTOR_ALTA, HIGH);
}
void STOP_M()
{
  digitalWrite(MOTOR_BAJA, HIGH);
  digitalWrite(MOTOR_ALTA, HIGH);
  digitalWrite(SECUENCIA, HIGH);
}
void drenado()
{
  digitalWrite(DESAGUE, HIGH);
}

void CENTRIFUGADO()
{
  drenado();
  digitalWrite(SECUENCIA, HIGH);
  digitalWrite(MOTOR_BAJA, HIGH);
  digitalWrite(MOTOR_ALTA, LOW);
}
void no_drenado()
{
  digitalWrite(DESAGUE, LOW);
}

void val_off()
{
  digitalWrite(TAMBOR_DEPOSITO1_FRIA, HIGH);
  digitalWrite(TAMBOR_DEPOSITO1_CALIENTE, HIGH);
  digitalWrite(DEPOSITO2, HIGH);
  digitalWrite(DEPOSITO3, HIGH);
}

void NORMALL()
{
  LLENADO_AGIpre = 0 /*EEPROM.read(3);*/;
  DESAGUE_AGIpre = 0 /*EEPROM.read(4);*/;
  CENTRIFUpre = 0 /*EEPROM.read(5);*/;
  nivelde_llenado_prelavado = 0;
  TIEMPO_PRELAVADO = LLENADO_AGIpre + DESAGUE_AGIpre + CENTRIFUpre;
  //--------------------------------------

  LLENADO_AGIlav = 5; /*EEPROM.read(7);*/
  DESAGUE_AGIlav = 1 /*EEPROM.read(8);*/;
  CENTRIFUlav = 0;             /*EEPROM.read(9);*/
  nivelde_llenado_lavado = 12; /*EEPROM.read(10);*/
  TIEMPO_LAVADO = LLENADO_AGIlav + DESAGUE_AGIlav + CENTRIFUlav;
  //--------------------------------------

  LLENADO_AGIeng = 5;            /*EEPROM.read(11);*/
  DESAGUE_AGIeng = 1;            /* EEPROM.read(12);*/
  CENTRIFUeng = 0;               /*EEPROM.read(13);*/
  nivelde_llenado_enjugaue = 12; /*EEPROM.read(14);*/
  TIEMPO_ENJUAGUE = LLENADO_AGIeng + DESAGUE_AGIeng + CENTRIFUeng;
  //--------------------------------------

  LLENADO_AGIex1 = 0; /*EEPROM.read(15);*/
  DESAGUE_AGIex1 = 0; /* EEPROM.read(16);*/
  CENTRIFUex1 = 0;    /*EEPROM.read(17);*/
  nivel_de_llenado_ennjuague_extra_1 = 0;
  TIEMPO_ENJUAGUE_EXTRA_1 = LLENADO_AGIex1 + DESAGUE_AGIex1 + CENTRIFUex1;
  //-------------------------------------------

  LLENADO_AGIex2 = 0 /*EEPROM.read(19);*/;
  DESAGUE_AGIex2 = 0; /*EEPROM.read(20);*/
  CENTRIFUex2 = 0 /*EEPROM.read(21);*/;
  nivel_de_llenado_ennjuague_extra_2 = 0;
  TIEMPO_ENJUAGUE_EXTRA_2 = LLENADO_AGIex2 + DESAGUE_AGIex2 + CENTRIFUex2;
  //--------------------------------------------

  LLENADO_AGIfin = 0;                   /*EEPROM.read(23);*/
  DESAGUE_AGIfin = 0;                   /*EEPROM.read(24);*/
  CENTRIFUfin = 0;                      /*EEPROM.read(25);*/
  nivel_de_llenado_ennjuague_final = 0; /*EEPROM.read(26);*/
  TIEMPO_ENJUAGUE_FINAL = LLENADO_AGIfin + DESAGUE_AGIfin + CENTRIFUfin;
  //-----------------------------------------

  TIEMPO_CENTRIFUGADO_FINAL = 6; /*EEPROM.read(27);*/
  nivel_de_llenado_centrifugado = 0 /*EEPROM.read(28);*/;
  //---------------------------------------------
  TIEMPO = TIEMPO_PRELAVADO + TIEMPO_LAVADO + TIEMPO_ENJUAGUE + TIEMPO_ENJUAGUE_EXTRA_1 + TIEMPO_ENJUAGUE_EXTRA_2 + TIEMPO_ENJUAGUE_FINAL + TIEMPO_CENTRIFUGADO_FINAL;
  // ------------------------------------------
  tiempo_giro_izquierda = 10; /*EEPROM.read(29);*/
  tiempo_giro_derecha = 10;   /*EEPROM.read(30);*/
  tiempo_reposo = 4;          /*EEPROM.read(31);*/
  /*lcd.setCursor(0, 1);
  lcd.print("NORMAL CH       ");
  lcd.print("  ");*/
  segundostotal = TIEMPO * 60;
  ahoras = ((segundostotal / 60) / 60);
  aminutos = (segundostotal / 60) % 60;
  asegundos = segundostotal % 60;
  ahoras = ((segundostotal / 60) / 60);
  aminutos = (segundostotal / 60) % 60;
  asegundos = segundostotal % 60;
  /*lcd.setCursor(10, 1);
  
  if (aminutos < 10)
    lcd.print("0");
  lcd.print(aminutos);
  lcd.print(":");
  if (asegundos < 10)
    lcd.print("0");
  lcd.print(asegundos);*/
}
void DELICADOO()
{
  LLENADO_AGIpre = 0 /*EEPROM.read(3);*/;
  DESAGUE_AGIpre = 0 /*EEPROM.read(4);*/;
  CENTRIFUpre = 0 /*EEPROM.read(5);*/;
  nivelde_llenado_prelavado = 0;
  TIEMPO_PRELAVADO = LLENADO_AGIpre + DESAGUE_AGIpre + CENTRIFUpre;
  //--------------------------------------

  LLENADO_AGIlav = 6; /*EEPROM.read(7);*/
  DESAGUE_AGIlav = 1 /*EEPROM.read(8);*/;
  CENTRIFUlav = 0;             /*EEPROM.read(9);*/
  nivelde_llenado_lavado = 12; /*EEPROM.read(10);*/
  TIEMPO_LAVADO = LLENADO_AGIlav + DESAGUE_AGIlav + CENTRIFUlav;
  //--------------------------------------

  LLENADO_AGIeng = 5;            /*EEPROM.read(11);*/
  DESAGUE_AGIeng = 1;            /* EEPROM.read(12);*/
  CENTRIFUeng = 0;               /*EEPROM.read(13);*/
  nivelde_llenado_enjugaue = 12; /*EEPROM.read(14);*/
  TIEMPO_ENJUAGUE = LLENADO_AGIeng + DESAGUE_AGIeng + CENTRIFUeng;
  //--------------------------------------

  LLENADO_AGIex1 = 0; /*EEPROM.read(15);*/
  DESAGUE_AGIex1 = 0; /* EEPROM.read(16);*/
  CENTRIFUex1 = 0;    /*EEPROM.read(17);*/
  nivel_de_llenado_ennjuague_extra_1 = 0;
  TIEMPO_ENJUAGUE_EXTRA_1 = LLENADO_AGIex1 + DESAGUE_AGIex1 + CENTRIFUex1;
  //-------------------------------------------

  LLENADO_AGIex2 = 0 /*EEPROM.read(19);*/;
  DESAGUE_AGIex2 = 0; /*EEPROM.read(20);*/
  CENTRIFUex2 = 0 /*EEPROM.read(21);*/;
  nivel_de_llenado_ennjuague_extra_2 = 0;
  TIEMPO_ENJUAGUE_EXTRA_2 = LLENADO_AGIex2 + DESAGUE_AGIex2 + CENTRIFUex2;
  //--------------------------------------------

  LLENADO_AGIfin = 6;                    /*EEPROM.read(23);*/
  DESAGUE_AGIfin = 1;                    /*EEPROM.read(24);*/
  CENTRIFUfin = 0;                       /*EEPROM.read(25);*/
  nivel_de_llenado_ennjuague_final = 12; /*EEPROM.read(26);*/
  TIEMPO_ENJUAGUE_FINAL = LLENADO_AGIfin + DESAGUE_AGIfin + CENTRIFUfin;
  //-----------------------------------------

  TIEMPO_CENTRIFUGADO_FINAL = 6; /*EEPROM.read(27);*/
  nivel_de_llenado_centrifugado = 0 /*EEPROM.read(28);*/;
  //---------------------------------------------
  TIEMPO = TIEMPO_PRELAVADO + TIEMPO_LAVADO + TIEMPO_ENJUAGUE + TIEMPO_ENJUAGUE_EXTRA_1 + TIEMPO_ENJUAGUE_EXTRA_2 + TIEMPO_ENJUAGUE_FINAL + TIEMPO_CENTRIFUGADO_FINAL;
  // ------------------------------------------
  // ------------------------------------------
  tiempo_giro_izquierda = 10; /*EEPROM.read(29);*/
  tiempo_giro_derecha = 10;   /*EEPROM.read(30);*/
  tiempo_reposo = 4;          /*EEPROM.read(31);*/
  /*lcd.setCursor(0, 1);
  lcd.print("NORMAL G            ");
  lcd.print("  ");*/
  // lcd.print("  ");
  segundostotal = TIEMPO * 60;
  ahoras = ((segundostotal / 60) / 60);
  aminutos = (segundostotal / 60) % 60;
  asegundos = segundostotal % 60;
  ahoras = ((segundostotal / 60) / 60);
  aminutos = (segundostotal / 60) % 60;
  asegundos = segundostotal % 60;
 /* lcd.setCursor(10, 1);

  if (aminutos < 10)
    lcd.print("0");
  lcd.print(aminutos);
  lcd.print(":");
  if (asegundos < 10)
    lcd.print("0");
  lcd.print(asegundos);*/
}
void MUYSUCIOO()
{
  LLENADO_AGIpre = 4 /*EEPROM.read(3);*/;
  DESAGUE_AGIpre = 1 /*EEPROM.read(4);*/;
  CENTRIFUpre = 0 /*EEPROM.read(5);*/;
  nivelde_llenado_prelavado = 12;
  TIEMPO_PRELAVADO = LLENADO_AGIpre + DESAGUE_AGIpre + CENTRIFUpre;
  //--------------------------------------

  LLENADO_AGIlav = 5; /*EEPROM.read(7);*/
  DESAGUE_AGIlav = 1 /*EEPROM.read(8);*/;
  CENTRIFUlav = 0;             /*EEPROM.read(9);*/
  nivelde_llenado_lavado = 12; /*EEPROM.read(10);*/
  TIEMPO_LAVADO = LLENADO_AGIlav + DESAGUE_AGIlav + CENTRIFUlav;
  //--------------------------------------

  LLENADO_AGIeng = 3;            /*EEPROM.read(11);*/
  DESAGUE_AGIeng = 1;            /* EEPROM.read(12);*/
  CENTRIFUeng = 0;               /*EEPROM.read(13);*/
  nivelde_llenado_enjugaue = 12; /*EEPROM.read(14);*/
  TIEMPO_ENJUAGUE = LLENADO_AGIeng + DESAGUE_AGIeng + CENTRIFUeng;
  //--------------------------------------

  LLENADO_AGIex1 = 3; /*EEPROM.read(15);*/
  DESAGUE_AGIex1 = 1; /* EEPROM.read(16);*/
  CENTRIFUex1 = 0;    /*EEPROM.read(17);*/
  nivel_de_llenado_ennjuague_extra_1 = 0;
  TIEMPO_ENJUAGUE_EXTRA_1 = LLENADO_AGIex1 + DESAGUE_AGIex1 + CENTRIFUex1;
  //-------------------------------------------

  LLENADO_AGIex2 = 0 /*EEPROM.read(19);*/;
  DESAGUE_AGIex2 = 0; /*EEPROM.read(20);*/
  CENTRIFUex2 = 0 /*EEPROM.read(21);*/;
  nivel_de_llenado_ennjuague_extra_2 = 0;
  TIEMPO_ENJUAGUE_EXTRA_2 = LLENADO_AGIex2 + DESAGUE_AGIex2 + CENTRIFUex2;
  //--------------------------------------------

  LLENADO_AGIfin = 6;                    /*EEPROM.read(23);*/
  DESAGUE_AGIfin = 1;                    /*EEPROM.read(24);*/
  CENTRIFUfin = 0;                       /*EEPROM.read(25);*/
  nivel_de_llenado_ennjuague_final = 12; /*EEPROM.read(26);*/
  TIEMPO_ENJUAGUE_FINAL = LLENADO_AGIfin + DESAGUE_AGIfin + CENTRIFUfin;
  //-----------------------------------------

  TIEMPO_CENTRIFUGADO_FINAL = 6; /*EEPROM.read(27);*/
  nivel_de_llenado_centrifugado = 0 /*EEPROM.read(28);*/;
  //---------------------------------------------
  TIEMPO = TIEMPO_PRELAVADO + TIEMPO_LAVADO + TIEMPO_ENJUAGUE + TIEMPO_ENJUAGUE_EXTRA_1 + TIEMPO_ENJUAGUE_EXTRA_2 + TIEMPO_ENJUAGUE_FINAL + TIEMPO_CENTRIFUGADO_FINAL;
  // ------------------------------------------
  // ------------------------------------------
  tiempo_giro_izquierda = 10; /*EEPROM.read(29);*/
  tiempo_giro_derecha = 10;   /*EEPROM.read(30);*/
  tiempo_reposo = 4;          /*EEPROM.read(31);*/
 /* lcd.setCursor(0, 1);
  lcd.print("HEAVY CH          ");
  lcd.print("  ");*/
  segundostotal = TIEMPO * 60;
  ahoras = ((segundostotal / 60) / 60);
  aminutos = (segundostotal / 60) % 60;
  asegundos = segundostotal % 60;
  ahoras = ((segundostotal / 60) / 60);
  aminutos = (segundostotal / 60) % 60;
  asegundos = segundostotal % 60;
 /* lcd.setCursor(10, 1);*/
  /* if (ahoras < 10)
    lcd.print("0");
  lcd.print(ahoras);
  lcd.print(":");*/
  /*if (aminutos < 10)
    lcd.print("0");
  lcd.print(aminutos);
  lcd.print(":");
  if (asegundos < 10)
    lcd.print("0");
  lcd.print(asegundos);*/
}

void agua_fria() //lavado
{
  digitalWrite(TAMBOR_DEPOSITO1_FRIA, LOW);
  digitalWrite(TAMBOR_DEPOSITO1_CALIENTE, HIGH);
  digitalWrite(DEPOSITO2, LOW);
  digitalWrite(DEPOSITO3, HIGH);
  no_drenado();
}
void agua_tibia() //lavado
{
  digitalWrite(TAMBOR_DEPOSITO1_FRIA, LOW);
  digitalWrite(TAMBOR_DEPOSITO1_CALIENTE, LOW);
  digitalWrite(DEPOSITO2, LOW);
  digitalWrite(DEPOSITO3, HIGH);
  no_drenado();
}
void agua_caliente() //lavado
{
  digitalWrite(TAMBOR_DEPOSITO1_FRIA, HIGH);
  digitalWrite(TAMBOR_DEPOSITO1_CALIENTE, LOW);
  digitalWrite(DEPOSITO2, LOW);
  digitalWrite(DEPOSITO3, HIGH);
  no_drenado();
}

void enjuague_final() //enjuague final
{
  digitalWrite(TAMBOR_DEPOSITO1_FRIA, LOW);
  digitalWrite(TAMBOR_DEPOSITO1_CALIENTE, LOW);
  digitalWrite(DEPOSITO2, LOW);
  digitalWrite(DEPOSITO3, LOW);
  no_drenado();
}

void clock_lav() //temporizador lavadora
{
 /* lcd.setCursor(0, 1);
  if (ahoras < 10)
    lcd.print("0");
  lcd.print(ahoras);
  lcd.print(":");

  if (aminutos < 10)
    lcd.print("0");
  lcd.print(aminutos);

  lcd.print(":");
  if (asegundos < 10)
    lcd.print("0");
  lcd.print(asegundos);*/
}
void PROGG() //LCD modo prog
{
  if (program == 0)
  {
    LLENADO_AGIpre = 4 /*EEPROM.read(3);*/;
    DESAGUE_AGIpre = 1 /*EEPROM.read(4);*/;
    CENTRIFUpre = 0 /*EEPROM.read(5);*/;
    nivelde_llenado_prelavado = 12;
    TIEMPO_PRELAVADO = LLENADO_AGIpre + DESAGUE_AGIpre + CENTRIFUpre;
    //--------------------------------------

    LLENADO_AGIlav = 8; /*EEPROM.read(7);*/
    DESAGUE_AGIlav = 1 /*EEPROM.read(8);*/;
    CENTRIFUlav = 0;             /*EEPROM.read(9);*/
    nivelde_llenado_lavado = 12; /*EEPROM.read(10);*/
    TIEMPO_LAVADO = LLENADO_AGIlav + DESAGUE_AGIlav + CENTRIFUlav;
    //--------------------------------------

    LLENADO_AGIeng = 4;            /*EEPROM.read(11);*/
    DESAGUE_AGIeng = 1;            /* EEPROM.read(12);*/
    CENTRIFUeng = 0;               /*EEPROM.read(13);*/
    nivelde_llenado_enjugaue = 12; /*EEPROM.read(14);*/
    TIEMPO_ENJUAGUE = LLENADO_AGIeng + DESAGUE_AGIeng + CENTRIFUeng;
    //--------------------------------------

    LLENADO_AGIex1 = 3; /*EEPROM.read(15);*/
    DESAGUE_AGIex1 = 1; /* EEPROM.read(16);*/
    CENTRIFUex1 = 0;    /*EEPROM.read(17);*/
    nivel_de_llenado_ennjuague_extra_1 = 0;
    TIEMPO_ENJUAGUE_EXTRA_1 = LLENADO_AGIex1 + DESAGUE_AGIex1 + CENTRIFUex1;
    //-------------------------------------------

    LLENADO_AGIex2 = 0 /*EEPROM.read(19);*/;
    DESAGUE_AGIex2 = 0; /*EEPROM.read(20);*/
    CENTRIFUex2 = 0 /*EEPROM.read(21);*/;
    nivel_de_llenado_ennjuague_extra_2 = 0;
    TIEMPO_ENJUAGUE_EXTRA_2 = LLENADO_AGIex2 + DESAGUE_AGIex2 + CENTRIFUex2;
    //--------------------------------------------

    LLENADO_AGIfin = 8;                    /*EEPROM.read(23);*/
    DESAGUE_AGIfin = 1;                    /*EEPROM.read(24);*/
    CENTRIFUfin = 0;                       /*EEPROM.read(25);*/
    nivel_de_llenado_ennjuague_final = 12; /*EEPROM.read(26);*/
    TIEMPO_ENJUAGUE_FINAL = LLENADO_AGIfin + DESAGUE_AGIfin + CENTRIFUfin;
    //-----------------------------------------

    TIEMPO_CENTRIFUGADO_FINAL = 6; /*EEPROM.read(27);*/
    nivel_de_llenado_centrifugado = 0 /*EEPROM.read(28);*/;
    //---------------------------------------------
    TIEMPO = TIEMPO_PRELAVADO + TIEMPO_LAVADO + TIEMPO_ENJUAGUE + TIEMPO_ENJUAGUE_EXTRA_1 + TIEMPO_ENJUAGUE_EXTRA_2 + TIEMPO_ENJUAGUE_FINAL + TIEMPO_CENTRIFUGADO_FINAL;
    // ------------------------------------------
    tiempo_giro_izquierda = 10; /*EEPROM.read(29);*/
    tiempo_giro_derecha = 10;   /*EEPROM.read(30);*/
    tiempo_reposo = 4;          /*EEPROM.read(31);*/
    /*lcd.setCursor(0, 1);
    lcd.print("HEAVY G             ");
    lcd.print("  ");*/
    segundostotal = TIEMPO * 60;
    ahoras = ((segundostotal / 60) / 60);
    aminutos = (segundostotal / 60) % 60;
    asegundos = segundostotal % 60;
    ahoras = ((segundostotal / 60) / 60);
    aminutos = (segundostotal / 60) % 60;
    asegundos = segundostotal % 60;
    /*lcd.setCursor(10, 1);
   
    if (aminutos < 10)
      lcd.print("0");
    lcd.print(aminutos);
    lcd.print(":");
    if (asegundos < 10)
      lcd.print("0");
    lcd.print(asegundos);*/
  }
  else
  {

   /* lcd.setCursor(0, 0);
    lcd.print("PROGRAMACION                ");
    lcd.setCursor(0, 1);
    lcd.print("APROXIMA RFID                ");*/
  }
}
void FRIAA() //LCD temperatura fria
{
  if (estado_2 == 0)
  {

    //normal
    NORMALL();
  }
  if (estado_2 == 1)
  {
    //delicado
    DELICADOO();
    contador_programacion++;
    Serial.println(contador_programacion);

    if (contador_programacion >= 2 && program == 1)
    {
      tone(buzzer, 2000);
      delay(1000);
      noTone(buzzer);
      contador_programacion = 0;
      program = 0;
    }
    if (contador_programacion >= 2 && program == 0)
    {
      tone(buzzer, 2000);
      delay(1000);
      noTone(buzzer);
      contador_programacion = 0;
      program = 1;
    }
  }
  if (estado_2 == 2)
  {
    //muy sucio
    MUYSUCIOO();
    contador_programacion++;
    Serial.println(contador_programacion);

    if (contador_programacion >= 2 && program == 1)
    {
      tone(buzzer, 2000);
      delay(1000);
      noTone(buzzer);
      contador_programacion = 0;
      asm volatile(" jmp 0");
    }
    if (contador_programacion >= 2 && program == 0)
    {
      tone(buzzer, 2000);
      delay(1000);
      noTone(buzzer);
      contador_programacion = 0;
      asm volatile(" jmp 0");
    }
  }
  if (estado_2 == 3)
  {
    //muy sucio
    PROGG();
  }
  /*lcd.setCursor(0, 0);
  lcd.print("FRIA                   ");
  lcd.print("          ");*/
  PRECIO = precio_fria;
  /*segundostotal = TIEMPO * 60;
  lcd.print(" $");
  lcd.print(PRECIO);
 
  ahoras = ((segundostotal / 60) / 60);
  aminutos = (segundostotal / 60) % 60;
  asegundos = segundostotal % 60;
  ahoras = ((segundostotal / 60) / 60);
  aminutos = (segundostotal / 60) % 60;
  asegundos = segundostotal % 60;
  lcd.setCursor(0, 1);
  if (ahoras < 10)
    lcd.print("0");
  lcd.print(ahoras);
  lcd.print(":");
  if (aminutos < 10)
    lcd.print("0");
  lcd.print(aminutos);
  lcd.print(":");
  if (asegundos < 10)
    lcd.print("0");
  lcd.print(asegundos);
  temperatura = 0;*/
}
void TIBIAA() //LCD temperatura tibia
{
  if (estado_2 == 0)
  {
    //normal
    NORMALL();
  }
  if (estado_2 == 1)
  {
    //delicado
    DELICADOO();
  }
  if (estado_2 == 2)
  {
    //muy sucio
    MUYSUCIOO();
  }
  if (estado_2 == 3)
  {
    //muy sucio
    PROGG();
  }
  /*lcd.setCursor(0, 0);
  lcd.print("TIBIA");*/
  temperatura = 1;
  /*lcd.print("         ");*/
  /*segundostotal = TIEMPO * 60;
  PRECIO = precio_tibia;
  MONEDA = 5;
  lcd.print(" $");
  lcd.print(PRECIO);
  lcd.print("         ");
  ahoras = ((segundostotal / 60) / 60);
  aminutos = (segundostotal / 60) % 60;
  asegundos = segundostotal % 60;
  ahoras = ((segundostotal / 60) / 60);
  aminutos = (segundostotal / 60) % 60;
  asegundos = segundostotal % 60;
  lcd.setCursor(0, 1);
  if (ahoras < 10)
    lcd.print("0");
  lcd.print(ahoras);
  lcd.print(":");
  if (aminutos < 10)
    lcd.print("0");
  lcd.print(aminutos);
  lcd.print(":");
  if (asegundos < 10)
    lcd.print("0");
  lcd.print(asegundos);*/
}
void CALIENTEE() //LCD temperatura caliente
{
  if (estado_2 == 0)
  {
    //normal
    NORMALL();
  }
  if (estado_2 == 1)
  {
    //delicado
    DELICADOO();
  }
  if (estado_2 == 2)
  {
    //muy sucio
    MUYSUCIOO();
  }
  if (estado_2 == 3)
  {
    //muy sucio
    PROGG();
  }
  /*lcd.setCursor(0, 0);
  lcd.print("CALIENTE");
  temperatura = 2;
  lcd.print("         ");*/
  /*segundostotal = TIEMPO * 60;
  PRECIO = precio_caliente;
  MONEDA = 5;
  lcd.print(" $");
  lcd.print(PRECIO);
  lcd.print("         ");
  ahoras = ((segundostotal / 60) / 60);
  aminutos = (segundostotal / 60) % 60;
  asegundos = segundostotal % 60;
  ahoras = ((segundostotal / 60) / 60);
  aminutos = (segundostotal / 60) % 60;
  asegundos = segundostotal % 60;
  lcd.setCursor(0, 1);
  if (ahoras < 10)
    lcd.print("0");
  lcd.print(ahoras);
  lcd.print(":");
  if (aminutos < 10)
    lcd.print("0");
  lcd.print(aminutos);
  lcd.print(":");
  if (asegundos < 10)
    lcd.print("0");
  lcd.print(asegundos);*/
}
void INICIOO() //LCD inicio de ciclo
{
 /* lcd.setCursor(0, 0);
  lcd.print("LAVAR            ");*/
}

void agua_fria_enjuagues() //enjuagues
{
  digitalWrite(TAMBOR_DEPOSITO1_FRIA, LOW);
  digitalWrite(TAMBOR_DEPOSITO1_CALIENTE, HIGH);
  digitalWrite(DEPOSITO2, LOW);
  digitalWrite(DEPOSITO3, HIGH);
  no_drenado();
}
void agua_tibia_enjuagues() //enjuagues
{
  digitalWrite(TAMBOR_DEPOSITO1_FRIA, LOW);
  digitalWrite(TAMBOR_DEPOSITO1_CALIENTE, LOW);
  digitalWrite(DEPOSITO2, LOW);
  digitalWrite(DEPOSITO3, HIGH);
  no_drenado();
}
void agua_caliente_enjuagues() //enjuagues
{
  digitalWrite(TAMBOR_DEPOSITO1_FRIA, HIGH);
  digitalWrite(TAMBOR_DEPOSITO1_CALIENTE, LOW);
  digitalWrite(DEPOSITO2, LOW);
  digitalWrite(DEPOSITO3, HIGH);
  no_drenado();
}

void llenado_mojado(int dato_llenado, int nivelde_llenado_prelavado, int tiempo_giro_izquierda, int tiempo_giro_derecha, int tiempo_reposo, int tiempo_aux2, int LLENADO_AGIpre, int temperatura, int ETAPA)
{
  val = analogRead(A0);
  //Serial.print("voltage:");
  //  float voltage = val * (5.0 / 1023.0);
  //Serial.println(voltage);
  /*
  if (voltage > 4.27 && voltage < 4.43)
  {
    mapp = (voltage - (4.28)) * 100;
  }
  else if (voltage < 4.28)
  {
    mapp = 0;
  }
  else if (voltage > 4.43)
  {
    mapp = 16;
  }*/
  no_drenado();
  if (nivelde_llenado_prelavado > mapp)
  {
    if (ETAPA == 1 && temperatura == 0)
    {
      // Serial.println("agua_fria");
      agua_fria();
    }
    if (ETAPA == 1 && temperatura == 1)
    {
      // Serial.println("agua_tibia");
      agua_tibia();
    }
    if (ETAPA == 1 && temperatura == 2)
    {
      // Serial.println("agua_caliente");
      agua_caliente();
    }
  }
  else
  {
    val_off();
  }

  if (nivelde_llenado_prelavado > mapp)
  {
    if (ETAPA == 2 && temperatura == 0)
    {
      //Serial.println("agua_fria");
      agua_fria();
    }
    if (ETAPA == 2 && temperatura == 1)
    {
      //Serial.println("agua_tibia");
      agua_tibia();
    }
    if (ETAPA == 2 && temperatura == 2)
    {
      // Serial.println("agua_caliente");
      agua_caliente();
    }
  }
  else
  {
    val_off();
  }

  if (nivelde_llenado_prelavado > mapp)
  {
    if (ETAPA == 3 && temperatura == 0)
    {
      agua_fria();
    }

    if (ETAPA == 3 && temperatura == 1)
    {
      agua_tibia();
    }
    if (ETAPA == 3 && temperatura == 2)
    {
      agua_caliente();
    }
  }
  else
  {
    val_off();
  }

  if (nivelde_llenado_prelavado > mapp)
  {
    if (ETAPA == 4 && temperatura == 0)
    {
      agua_fria();
    }
    if (ETAPA == 4 && temperatura == 1)
    {
      agua_tibia();
    }
    if (ETAPA == 4 && temperatura == 2)
    {
      agua_caliente();
    }
  }
  else
  {
    val_off();
  }

  if (nivelde_llenado_prelavado > mapp)
  {
    if (ETAPA == 5 && temperatura == 0)
    {
      agua_fria();
    }
    if (ETAPA == 5 && temperatura == 1)
    {
      agua_tibia();
    }
    if (ETAPA == 5 && temperatura == 2)
    {
      agua_caliente();
    }
  }
  else
  {
    val_off();
  }

  if (nivelde_llenado_prelavado > mapp)
  {
    if (ETAPA == 6 && temperatura == 0)
    {
      enjuague_final();
    }
    if (ETAPA == 6 && temperatura == 1)
    {
      enjuague_final();
    }
    if (ETAPA == 6 && temperatura == 2)
    {
      enjuague_final();
    }
  }
  else
  {
    val_off();
  }

  if (dato_llenado != datoAnterior_llenado)
  {
    contador_llenado++;
    /*Serial.print("contador_llenado:");
    Serial.println(contador_llenado);

    Serial.print("mapp:");
    Serial.println(mapp);
    Serial.print("np:");
    Serial.println(nivelde_llenado_prelavado);
    Serial.print("llenado_error:");
    Serial.println(llenado_error);
    Serial.println("wwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwww");*/
    if (mapp <= nivelde_llenado_prelavado && llenado_error == 1)
    {
      // Serial.println("yyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyy");
      time = 0;
      while (1)
      {
        val = analogRead(A0);
        float voltage2 = val * (5.0 / 1023.0);
       /* lcd.setCursor(8, 1);*/
        //lcd.print(voltage2);
        //lcd.print(mapp);
        if (voltage2 > voltage + 0.1 && voltage2 < voltage + 5.00)
        {
          mapp = (voltage2 - voltage) * 100;
        }
        else if (voltage2 < voltage)
        {
          mapp = 0;
        }
        else if (voltage2 > voltage + 5.00)
        {
          mapp = 500;
        }
        time = millis() / 1000;
        dato_error = time;
        /* Serial.print("mapp:");
        Serial.println(mapp);
        Serial.print("np:");
        Serial.println(nivelde_llenado_prelavado);
        Serial.print("llenado_error:");
        Serial.println(llenado_error);
        Serial.println("zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz");

        Serial.print("time:");
        Serial.println(dato_error);
         Serial.print("dato anterior:");
        Serial.println(datoAnterior_error);*/

        if (dato_error != datoAnterior_error)
        {

          contador_error_llenado++;
          contador_llenado++;
          /* Serial.print("contador llenado:");
          Serial.println(contador_llenado);
          Serial.print("contador error llenado:");
          Serial.println(contador_error_llenado);*/
          if (llenado == 1)
          {
            if (t == 0)
            {
              if (contador_llenado <= tiempo_giro_izquierda)
              {
                IZQUIERDA_M();
                Serial.println("izquierda");
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
                STOP_M();
                Serial.println("stop");
                if (tiempo_reposo >= 3)
                {
                  val = analogRead(A0);
                  float voltage2 = val * (5.0 / 1023.0);
                /*  lcd.setCursor(8, 1);*/
                  //lcd.print(voltage2);
                  Serial.println(voltage);
                  if (contador_error_llenado >= tiempo_error_llenado * 60)
                  {
                   /* lcd.clear();
                    lcd.setCursor(1, 0);
                    lcd.print("ERROR LLENADO         ");*/
                    drenado();
                    STOP_M();
                    val_off();
                    delay(3000);
                    PUERTA_OFF();
                    while (1)
                    {
                      tone(buzzer, 2000);
                      delay(1000);
                      noTone(buzzer);
                      delay(1000);
                    }
                    asm volatile(" jmp 0");
                    //ERROR LLENADO
                  }
                  if (voltage2 > voltage + 0.1 && voltage2 < voltage + 5.00)
                  {
                    mapp = (voltage2 - voltage) * 100;
                  }
                  else if (voltage2 < voltage)
                  {
                    mapp = 0;
                  }
                  else if (voltage2 > voltage + 5.00)
                  {
                    mapp = 500;
                  }
                  if (mapp >= nivelde_llenado_prelavado)
                  {
                    contador_llenado = 1;
                    t = 1;
                    llenado_error = 0;
                    llenado = 1;
                    break;
                  }
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
                DERECHA_M();
                Serial.println("derecha");
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
                STOP_M();
                Serial.print("stop");
                if (tiempo_reposo >= 3)
                {
                  val = analogRead(A0);
                  float voltage2 = val * (5.0 / 1023.0);
                 /* lcd.setCursor(8, 1);*/
                  //lcd.print(voltage2);
                  //lcd.print(mapp);
                  Serial.println(voltage);
                  if (contador_error_llenado >= tiempo_error_llenado * 60)
                  {
                   /* lcd.clear();
                    lcd.setCursor(1, 0);
                    lcd.print("ERROR LLENADO           ");*/
                    drenado();
                    STOP_M();
                    val_off();
                    delay(3000);
                    PUERTA_OFF();
                    while (1)
                    {
                      tone(buzzer, 2000);
                      delay(1000);
                      noTone(buzzer);
                      delay(1000);
                    }
                    asm volatile(" jmp 0");
                    //ERROR LLENADO
                  }
                  if (voltage2 > voltage + 0.1 && voltage2 < voltage + 5.00)
                  {
                    mapp = (voltage2 - voltage) * 100;
                  }
                  else if (voltage2 < voltage)
                  {
                    mapp = 0;
                  }
                  else if (voltage2 > voltage + 5.00)
                  {
                    mapp = 500;
                  }
                  if (mapp >= nivelde_llenado_prelavado)
                  {
                    contador_llenado = 1;
                    t = 1;
                    llenado_error = 0;
                    llenado = 1;
                    break;
                  }
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
          IZQUIERDA_M();
          Serial.println("izquierda");
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
          STOP_M();
          Serial.println("stop");
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
          DERECHA_M();
          Serial.println("derecha");
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
          STOP_M();
          Serial.println("stop");
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

void lavado(int dato_llenado, int nivelde_llenado_prelavado, int tiempo_giro_izquierda, int tiempo_giro_derecha, int tiempo_reposo, int tiempo_aux2, int LLENADO_AGIpre)
{
  drenado();
  if (dato_llenado != datoAnterior_llenado)
  {
    contador_llenado++;
    if (llenado == 1)
    {
      if (t == 0)
      {
        if (contador_llenado <= tiempo_giro_izquierda)
        {
          /* lcd.setCursor(12, 0);
          lcd.print("I1");*/
          IZQUIERDA_M();
          //izquierda
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
          /* lcd.setCursor(12, 0);
          lcd.print("S1");*/
          STOP_M();
          val = analogRead(A0);
          float voltage2 = val * (5.0 / 1023.0);
         /* lcd.setCursor(8, 1);*/
          //lcd.print(voltage2);
          //lcd.print(mapp);
          if (voltage2 > voltage + 0.1 && voltage2 < voltage + 5.00)
          {
            mapp = (voltage2 - voltage) * 100;
          }
          else if (voltage2 < voltage)
          {
            mapp = 0;
          }
          else if (voltage2 > voltage + 5.00)
          {
            mapp = 500;
          }
          if (LLENADO_AGIpre * 60 - tiempo_aux2 <= 15 && mapp >= nivelde_llenado_prelavado)
          {
            time = 0;
            while (1)
            {
              time = millis() / 1000;
              dato_error = time;
              if (dato_error != datoAnterior_error)
              {
                contador_error_llenado++;
                if (contador_error_llenado >= tiempo_error_llenado * 60)
                {
                  asm volatile(" jmp 0");
                  //ERROR LLENADO
                }
                val = analogRead(A0);
                float voltage2 = val * (5.0 / 1023.0);
              /*  lcd.setCursor(8, 1);*/
                //lcd.print(voltage2);
                //lcd.print(mapp);
                if (voltage2 > voltage + 0.1 && voltage2 < voltage + 5.00)
                {
                  mapp = (voltage2 - voltage) * 100;
                }
                else if (voltage2 < voltage)
                {
                  mapp = 0;
                }
                else if (voltage2 > voltage + 5.00)
                {
                  mapp = 500;
                }
                if (mapp >= nivelde_llenado_prelavado)
                {
                  break;
                }
              }
              datoAnterior_error = dato_error;
            }
          }
          //stop
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
          /*lcd.setCursor(12, 0);
          lcd.print("D1");*/
          DERECHA_M();
          //izquierda
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
          /* lcd.setCursor(12, 0);
          lcd.print("S1");*/
          STOP_M();
          val = analogRead(A0);
          float voltage2 = val * (5.0 / 1023.0);
         /*lcd.setCursor(8, 1);*/
          //lcd.print(voltage2);
          //lcd.print(mapp);
          if (voltage2 > voltage + 0.1 && voltage2 < voltage + 5.00)
          {
            mapp = (voltage2 - voltage) * 100;
          }
          else if (voltage2 < voltage)
          {
            mapp = 0;
          }
          else if (voltage2 > voltage + 5.00)
          {
            mapp = 500;
          }
          if (LLENADO_AGIpre * 60 - tiempo_aux2 <= 15 && mapp >= nivelde_llenado_prelavado)
          {
            time = 0;

            while (1)
            {
              time = millis() / 1000;
              dato_error = time;
              if (dato_error != datoAnterior_error)
              {
                contador_error_llenado++;
                if (contador_error_llenado >= tiempo_error_llenado * 60)
                {
                  asm volatile(" jmp 0");
                  //ERROR LLENADO
                }
                val = analogRead(A0);
                float voltage2 = val * (5.0 / 1023.0);
                /*lcd.setCursor(8, 1);*/
                //lcd.print(voltage2);
                //lcd.print(mapp);
                if (voltage2 > voltage + 0.1 && voltage2 < voltage + 5.00)
                {
                  mapp = (voltage2 - voltage) * 100;
                }
                else if (voltage2 < voltage)
                {
                  mapp = 0;
                }
                else if (voltage2 > voltage + 5.00)
                {
                  mapp = 500;
                }
              }
              datoAnterior_error = dato_error;
            }
          }
        }
        else
        {
          t = 0;
          contador_llenado = 1;
          llenado = 1;
          //stop
        }
      }
    }
  }
  datoAnterior_llenado = dato_llenado;
}

void setup()
{
 /* EEPROM.update(6, 0);
  EEPROM.update(7, 0);*/
  EEPROM.update(8, 244);
  EEPROM.update(9, 1);
  //wdt_disable();
  //EEPROM.update(33, 1);
  Serial.begin(9600);
  //año,mes,dia,hora,min,sec
  /*EEPROM.update(2351,200);
  EEPROM.update(2350,0);*/
  //------------------------------------------------------------------------------------------------------
  /*
  Serial.print("precio_fria>>");
  Serial.println(EEPROM.read(0));
  Serial.print("precio_tibia>>");
  Serial.println(EEPROM.read(1));
  Serial.print("precio_caliente>>");
  Serial.println(EEPROM.read(2));
  //----------------------------------------------
  Serial.print("LLENADO_AGIpre>>");
  Serial.println(EEPROM.read(3));
  Serial.print("DESAGUE_AGIpre>>");
  Serial.println(EEPROM.read(4));
  Serial.print("CENTRIFUpre>>");
  Serial.println(EEPROM.read(5));
  Serial.print("nivelde_llenado_prelavado>>");
  Serial.println(EEPROM.read(6));
  //--------------------------------------

  Serial.print("LLENADO_AGIlav>>");
  Serial.println(EEPROM.read(7));
  Serial.print("DESAGUE_AGIlav>>");
  Serial.println(EEPROM.read(8));
  Serial.print("CENTRIFUlav>>");
  Serial.println(EEPROM.read(9));
  Serial.print("nivelde_llenado_lavado>>");
  Serial.println(EEPROM.read(10));
  //--------------------------------------

  Serial.print("LLENADO_AGIeng>>");
  Serial.println(EEPROM.read(11));
  Serial.print("DESAGUE_AGIeng>>");
  Serial.println(EEPROM.read(12));
  Serial.print("CENTRIFUeng>>");
  Serial.println(EEPROM.read(13));
  Serial.print("nivelde_llenado_enjugaue>>");
  Serial.println(EEPROM.read(14));
  //--------------------------------------

  Serial.print("LLENADO_AGIex1>>");
  Serial.println(EEPROM.read(15));
  Serial.print("DESAGUE_AGIex1>>");
  Serial.println(EEPROM.read(16));
  Serial.print("CENTRIFUex1>>");
  Serial.println(EEPROM.read(17));
  Serial.print("nivel_de_llenado_ennjuague_extra_1>>");
  Serial.println(EEPROM.read(18));
  //-------------------------------------------

  Serial.print("LLENADO_AGIex2>>");
  Serial.println(EEPROM.read(19));
  Serial.print("DESAGUE_AGIex2>>");
  Serial.println(EEPROM.read(20));
  Serial.print("CENTRIFUex2>>");
  Serial.println(EEPROM.read(21));
  Serial.print("nivel_de_llenado_ennjuague_extra_2>>");
  Serial.println(EEPROM.read(22));
  //--------------------------------------------

  Serial.print("LLENADO_AGIfin>>");
  Serial.println(EEPROM.read(23));
  Serial.print("DESAGUE_AGIfin>>");
  Serial.println(EEPROM.read(24));
  Serial.print("CENTRIFUfin>>");
  Serial.println(EEPROM.read(25));
  Serial.print("nivel_de_llenado_ennjuague_final>>");
  Serial.println(EEPROM.read(26));
  //-----------------------------------------

  Serial.print("TIEMPO_CENTRIFUGADO_FINAL>>");
  Serial.println(EEPROM.read(27));
  Serial.print("nivel_de_llenado_centrifugado>>");
  Serial.println(EEPROM.read(28));
  //---------------------------------------------
  // ------------------------------------------
  Serial.print("tiempo_giro_izquierda>>");
  Serial.println(EEPROM.read(29));
  Serial.print("tiempo_giro_derecha>>");
  Serial.println(EEPROM.read(30));
  Serial.print("tiempo_reposo>>");
  Serial.println(EEPROM.read(31));
  //-----------------------------------------
  Serial.print("CICLO_DEFAULT>>");
  Serial.println(EEPROM.read(32));
  Serial.print("etapa>>");
  Serial.println(EEPROM.read(33));
  //------------------------------------------------
  Serial.print("promo_lunes_F>>");
  Serial.println(EEPROM.read(40));
  Serial.print("promo_martes_F>>");
  Serial.println(EEPROM.read(41));
  Serial.print("promo_miercoles_F>>");
  Serial.println(EEPROM.read(42));
  Serial.print("promo_jueves_F>>");
  Serial.println(EEPROM.read(43));
  Serial.print("promo_viernes_F>>");
  Serial.println(EEPROM.read(44));
  Serial.print("promo_sabado_F>>");
  Serial.println(EEPROM.read(45));
  Serial.print("promo_domingo_F>>");
  Serial.println(EEPROM.read(46));
  //---------------------------------------------------
  Serial.print("promo_lunes_C>>");
  Serial.println(EEPROM.read(47));
  Serial.print("promo_martes_C>>");
  Serial.println(EEPROM.read(48));
  Serial.print("promo_miercoles_C>>");
  Serial.println(EEPROM.read(49));
  Serial.print("promo_jueves_C>>");
  Serial.println(EEPROM.read(50));
  Serial.print("promo_viernes_C>>");
  Serial.println(EEPROM.read(51));
  Serial.print("promo_sabado_C>>");
  Serial.println(EEPROM.read(52));
  Serial.print("promo_domingo_C>>");
  Serial.println(EEPROM.read(53));
  //-------------------------------------------------
  Serial.print("promo_lunes_T>>");
  Serial.println(EEPROM.read(54));
  Serial.print("promo_martes_T>>");
  Serial.println(EEPROM.read(55));
  Serial.print("promo_miercoles_T>>");
  Serial.println(EEPROM.read(56));
  Serial.print("promo_jueves_T>>");
  Serial.println(EEPROM.read(57));
  Serial.print("promo_viernes_T>>");
  Serial.println(EEPROM.read(58));
  Serial.print("promo_sabado_T>>");
  Serial.println(EEPROM.read(59));
  Serial.print("promo_domingo_T>>");
  Serial.println(EEPROM.read(60));
  //-----------------------------------------
  Serial.print("tiempo_error_llenado>>");
  Serial.println(EEPROM.read(61));
  //------------------------------------------------------------------------------------------------------
*/
  rtc.begin();
  Wire.begin();
  /*lcd.begin(16, 2);*/
  SPI.begin();
  mfrc522.PCD_Init();
  ///////////////////////////////LLAVEPARA RFID
  key.keyByte[0] = 0xA3;
  key.keyByte[1] = 0xB2;
  key.keyByte[2] = 0xC1;
  key.keyByte[3] = 0xC3;
  key.keyByte[4] = 0xB2;
  key.keyByte[5] = 0xAF;
  //////////////////////////////////////////////////////////////
  val = analogRead(A0);
  voltage = val * (5.0 / 1023.0);

  pinMode(button[BTN_FRIA], INPUT_PULLUP);
  pinMode(button[BTN_CALIENTE], INPUT_PULLUP);
  pinMode(button[BTN_TIBIA], INPUT_PULLUP);
  pinMode(button[BTN_INICIO], INPUT_PULLUP);
  pinMode(button[BTN_PROG], INPUT_PULLUP);
  pinMode(button[BTN_NORMAL], INPUT_PULLUP);
  pinMode(button[BTN_DELICADO], INPUT_PULLUP);
  pinMode(button[BTN_MUYSUCIO], INPUT_PULLUP);

  pinMode(BUTT_TOP, INPUT_PULLUP);
  pinMode(BUTT_BOT, INPUT_PULLUP);
  //leds
  pinMode(led_lavado, OUTPUT);
  pinMode(led_enjuague, OUTPUT);
  pinMode(led_enjuague_f, OUTPUT);
  pinMode(led_centrifugado, OUTPUT);
  pinMode(led_puerta_cerrada, OUTPUT);
  pinMode(led_cloro, OUTPUT);

  digitalWrite(led_lavado, HIGH);
  digitalWrite(led_enjuague, HIGH);
  digitalWrite(led_enjuague_f, HIGH);
  digitalWrite(led_centrifugado, HIGH);
  digitalWrite(led_puerta_cerrada, HIGH);
  digitalWrite(led_cloro, HIGH);
  pinMode(32, OUTPUT);
  pinMode(29, OUTPUT);
  pinMode(27, OUTPUT);
  pinMode(25, OUTPUT);
 digitalWrite(32, HIGH);
  digitalWrite(29, HIGH);
  digitalWrite(27, HIGH);
  digitalWrite(25, HIGH);
  
  if (!rtc.begin())
  {
    if (vigencia == 1)
    {
      //  Serial.println("Couldn't find RTC");
      asm volatile(" jmp 0");
    }
  }
  if (rtc.lostPower())
  {
    // precio_fria = 10;
    // precio_tibia = 20;
    //precio_caliente = 30;
    //rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    if (vigencia == 1)
    {
      Serial.println("ERROR_CADUCIDAD");
      delay(3000);
      asm volatile(" jmp 0");
    }
    //rtc.adjust(DateTime(2000 + EEPROM.read(6), EEPROM.read(7), EEPROM.read(8), EEPROM.read(9), EEPROM.read(10), EEPROM.read(11)));
  }

  DateTime t2(DateTime(2000 + EEPROM.read(0), EEPROM.read(1), EEPROM.read(2), EEPROM.read(3), EEPROM.read(4), EEPROM.read(5)));
  //año/mes/dia/hor/MIN/SEC
  DateTime now = rtc.now();
  TimeSpan t3 = t2 - now;
 /* lcd.setCursor(0, 0);
  lcd.print("USO:");
  if (uso < 100)
    lcd.print("0");
  if (uso < 10)
    lcd.print("0");
  lcd.print(uso);
  lcd.print("de");
  lcd.print(numerodeusos);*/
  /*lcd.print("A:");
  lcd.print((now.year()) - 2000, DEC);
  lcd.print('/');
  lcd.print(now.month(), DEC);
  lcd.print('/');
  lcd.print(now.day(), DEC);
  lcd.print(' ');
  if (now.hour() < 10)
    lcd.print("0");
  lcd.print(now.hour(), DEC);
  lcd.print(':');
  if (now.minute() < 10)
    lcd.print("0");
  lcd.print(now.minute(), DEC);*/
  /*lcd.setCursor(0, 1);
  lcd.print("C:");
  lcd.print((t2.year() - 2000), DEC);
  lcd.print('/');
  lcd.print(t2.month(), DEC);
  lcd.print('/');
  lcd.print(t2.day(), DEC);
  lcd.print(' ');
  if (t2.hour() < 10)
    lcd.print("0");
  lcd.print(t2.hour(), DEC);
  lcd.print(':');
  if (t2.minute() < 10)
    lcd.print("0");
  lcd.print(t2.minute(), DEC);*/
  if (vigencia == 1)
  {
    if (t3.days() <= 0 && t3.hours() <= 0 && t3.seconds() <= 0 )/*|| uso > numerodeusos*/
    {
      Serial.print("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"); //0
      estado = PROG;
      program = 1;
      PROGG();
    }
  }
/*
  Serial.print(t2.year(), DEC);
  Serial.print('/');
  Serial.print(t2.month(), DEC);
  Serial.print('/');
  Serial.print(t2.day(), DEC);
  Serial.print(' ');
  Serial.print(t2.hour(), DEC);
  Serial.print(':');
  Serial.print(t2.minute(), DEC);

  Serial.print(t3.days()); //0
  Serial.print('/');
  Serial.print(t3.hours());   //10
  Serial.print(t3.minutes()); //0
  Serial.print(t3.seconds());*/
  // wdt_enable(WDTO_8S);
  /* else
  {
    DateTime now = rtc.now();
    Serial.println(now.dayOfTheWeek());
    rtc.adjust(DateTime(2000 + EEPROM.read(34), EEPROM.read(35), EEPROM.read(36), EEPROM.read(37), EEPROM.read(38), EEPROM.read(39)));
    if (now.dayOfTheWeek() == 0)
    {
      precio_fria = promo_domingo_F;
      precio_tibia = promo_domingo_T;
      precio_caliente = promo_domingo_C;
    }
    if (now.dayOfTheWeek() == 1)
    {
      precio_fria = promo_lunes_F;
      precio_tibia = promo_lunes_T;
      precio_caliente = promo_lunes_C;
    }
    if (now.dayOfTheWeek() == 2)
    {
      precio_fria = promo_martes_F;
      precio_tibia = promo_martes_T;
      precio_caliente = promo_martes_C;
    }
    if (now.dayOfTheWeek() == 3)
    {
      precio_fria = promo_miercoles_F;
      precio_tibia = promo_miercoles_T;
      precio_caliente = promo_miercoles_C;
    }
    if (now.dayOfTheWeek() == 4)
    {
      precio_fria = promo_jueves_F;
      precio_tibia = promo_jueves_T;
      precio_caliente = promo_jueves_C;
    }
    if (now.dayOfTheWeek() == 5)
    {
      precio_fria = promo_viernes_F;
      precio_tibia = promo_viernes_T;
      precio_caliente = promo_viernes_C;
    }
    if (now.dayOfTheWeek() == 6)
    {
      precio_fria = promo_sabado_F;
      precio_tibia = promo_sabado_T;
      precio_caliente = promo_sabado_C;
    }
  }*/
  pinMode(MOTOR_BAJA, OUTPUT);
  pinMode(MOTOR_ALTA, OUTPUT);
  pinMode(SECUENCIA, OUTPUT);
  pinMode(PUERTA, OUTPUT);
  pinMode(DESAGUE, OUTPUT);
  pinMode(TAMBOR_DEPOSITO1_FRIA, OUTPUT);
  pinMode(TAMBOR_DEPOSITO1_CALIENTE, OUTPUT);
  pinMode(DEPOSITO2, OUTPUT);
  pinMode(DEPOSITO3, OUTPUT);

  digitalWrite(MOTOR_BAJA, HIGH);
  digitalWrite(MOTOR_ALTA, HIGH);
  digitalWrite(SECUENCIA, HIGH);
  digitalWrite(PUERTA, HIGH);
  digitalWrite(DESAGUE, HIGH);
  digitalWrite(TAMBOR_DEPOSITO1_FRIA, HIGH);
  digitalWrite(TAMBOR_DEPOSITO1_CALIENTE, HIGH);
  digitalWrite(DEPOSITO2, HIGH);
  digitalWrite(DEPOSITO3, HIGH);

  button_state[0] = HIGH;
  button_state[1] = HIGH;
  button_state[2] = HIGH;
  button_state[3] = HIGH;
  button_state[4] = HIGH;
  button_state[5] = HIGH;
  button_state[6] = HIGH;
  button_state[7] = HIGH;
  PUERTA_ON();
  delay(3000);
 /* lcd.clear();*/
  PUERTA_OFF();
  tone(buzzer, 2000);
  delay(250);
  noTone(buzzer);

  if (estado == 0)
  {
    FRIAA();
  }

  if (estado == 1)
  {
    TIBIAA();
  }
  if (estado == 2)
  {
    CALIENTEE();
  }
}
void loop()
{
  //wdt_disable();

  switch (estado)
  {
  case FRIA:
    if (digitalRead(button[BTN_TIBIA]) == 0)
    {
      estado = TIBIA;
      TIBIAA();
      tone(buzzer, 2000);
      delay(100);
      noTone(buzzer);
      break;
    }
    if (digitalRead(button[BTN_PROG]) == 0)
    {
      if (program == 0)
      {
        estado_2 = 3;
        PROGG();
        tone(buzzer, 2000);
        delay(100);
        noTone(buzzer);
        break;
      }
      else
      {
        estado = PROG;
        PROGG();
        tone(buzzer, 2000);
        delay(100);
        noTone(buzzer);
        break;
      }
    }
    if (digitalRead(button[BTN_CALIENTE]) == 0)
    {
      estado = CALIENTE;
      CALIENTEE();
      tone(buzzer, 2000);
      delay(100);
      noTone(buzzer);
      break;
    }
    if (digitalRead(button[BTN_INICIO]) == 0)
    {
      while (digitalRead(BUTT_BOT) == 1)
      {
      /*  lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("CIERRA LA PUERTA           ");*/
        if (digitalRead(BUTT_BOT) == 0)
        {
          break;
        }
        tone(buzzer, 2000);
        delay(100);
        noTone(buzzer);
      }
      PUERTA_ON();
      delay(1000);
      if (digitalRead(BUTT_TOP) == 1)
      {
        if (digitalRead(BUTT_BOT) == 0)
        {
          val_off();
          STOP_M();
          while (1)
          {
            ////////////////////////////////////////////////////
            time = millis() / 1000;
            dato_error_puerta = time;
            if (dato_error_puerta != datoAnterior_error_puerta)
            {
              contador_error_puerta++;
              if (contador_error_puerta >= tiempo_error_puerta * 60)
              {
              /*  lcd.clear();
                lcd.setCursor(1, 0);
                lcd.print("ERROR BLOQUEO           ");*/
                while (1)
                {
                  tone(buzzer, 2000);
                  delay(1000);
                  noTone(buzzer);
                  delay(1000);
                }
              }
            }
            dato_error_puerta = datoAnterior_error_puerta;
            ////////////////////////////////////////////////////////////
            digitalWrite(led_puerta_cerrada, LOW);
            delay(250);
            digitalWrite(led_puerta_cerrada, HIGH);
            delay(250);
            if (digitalRead(BUTT_TOP) == 0)
            {
              break;
            }
          }
        }
      }
      else
      {
        noTone(buzzer);
        estado = INICIO;
        ///////////////////////////////////////
        H1 = EEPROM.read(6);
        L1 = EEPROM.read(7);
        w1 = H1 << 8;
        uso = w1 + L1;
        uso += 1;
        H1 = highByte(uso);
        EEPROM.update(6, H1);
        L1 = lowByte(uso);
        EEPROM.update(7, L1);
       /* w1 = H1 << 8;
        uso = w1 + L1;*/
        ////////////////////////////////////////////
        /*uso = EEPROM.read(2350) + 1;
        EEPROM.update(2350, uso);*/
      /*  lcd.clear();*/
        tone(buzzer, 2000);
        delay(100);
        noTone(buzzer);
      }
      break;
    }
    if (digitalRead(button[BTN_NORMAL]) == 0)
    {
      estado_2 = 0;
      NORMALL();
      tone(buzzer, 2000);
      delay(100);
      noTone(buzzer);
      break;
    }
    if (digitalRead(button[BTN_DELICADO]) == 0)
    {
      estado_2 = 1;
      DELICADOO();
      tone(buzzer, 2000);
      delay(100);
      noTone(buzzer);
      break;
    }
    if (digitalRead(button[BTN_MUYSUCIO]) == 0)
    {
      estado_2 = 2;
      MUYSUCIOO();
      tone(buzzer, 2000);
      delay(100);
      noTone(buzzer);
      break;
    }
    break;
  case TIBIA:
    if (digitalRead(button[BTN_FRIA]) == 0)
    {
      estado = FRIA;
      FRIAA();
      tone(buzzer, 2000);
      delay(100);
      noTone(buzzer);
      break;
    }
    if (digitalRead(button[BTN_PROG]) == 0)
    {
      if (program == 0)
      {
        estado_2 = 3;
        PROGG();
        tone(buzzer, 2000);
        delay(100);
        noTone(buzzer);
        break;
      }
      else
      {
        estado = PROG;
        PROGG();
        tone(buzzer, 2000);
        delay(100);
        noTone(buzzer);
        break;
      }
    }
    if (digitalRead(button[BTN_CALIENTE]) == 0)
    {
      estado = CALIENTE;
      CALIENTEE();
      tone(buzzer, 2000);
      delay(100);
      noTone(buzzer);
      break;
    }
    if (digitalRead(button[BTN_INICIO]) == 0)
    {
      while (digitalRead(BUTT_BOT) == 1)
      {
       /* lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("CIERRA LA PUERTA         ");*/
        if (digitalRead(BUTT_BOT) == 0)
        {
          break;
        }
        tone(buzzer, 2000);
        delay(100);
        noTone(buzzer);
      }
      PUERTA_ON();
      delay(1000);
      if (digitalRead(BUTT_TOP) == 1)
      {
        if (digitalRead(BUTT_BOT) == 0)
        {
          val_off();
          STOP_M();
          while (1)
          {
            ////////////////////////////////////////////////////
            time = millis() / 1000;
            dato_error_puerta = time;
            if (dato_error_puerta != datoAnterior_error_puerta)
            {
              contador_error_puerta++;
              if (contador_error_puerta >= tiempo_error_puerta * 60)
              {
             /*   lcd.clear();
                lcd.setCursor(1, 0);
                lcd.print("ERROR BLOQUEO          ");*/
                while (1)
                {
                  tone(buzzer, 2000);
                  delay(1000);
                  noTone(buzzer);
                  delay(1000);
                }
              }
            }
            dato_error_puerta = datoAnterior_error_puerta;
            ////////////////////////////////////////////////////////////
            digitalWrite(led_puerta_cerrada, LOW);
            delay(250);
            digitalWrite(led_puerta_cerrada, HIGH);
            delay(250);
            if (digitalRead(BUTT_TOP) == 0)
            {
              break;
            }
          }
        }
      }
      else
      {
        estado = INICIO;
        ///////////////////////////////////////
        H1 = EEPROM.read(6);
        L1 = EEPROM.read(7);
        w1 = H1 << 8;
        uso = w1 + L1;
        uso += 1;
        H1 = highByte(uso);
        EEPROM.update(6, H1);
        L1 = lowByte(uso);
        EEPROM.update(7, L1);
       /* w1 = H1 << 8;
        uso = w1 + L1;*/
        ////////////////////////////////////////////
        /* uso = EEPROM.read(2350) + 1;
        EEPROM.update(2350, uso);*/
      /*  lcd.clear();*/
        tone(buzzer, 2000);
        delay(100);
        noTone(buzzer);
      }
      break;
    }
    if (digitalRead(button[BTN_NORMAL]) == 0)
    {
      estado_2 = 0;
      NORMALL();
      tone(buzzer, 2000);
      delay(100);
      noTone(buzzer);
      break;
    }
    if (digitalRead(button[BTN_DELICADO]) == 0)
    {
      estado_2 = 1;
      DELICADOO();
      tone(buzzer, 2000);
      delay(100);
      noTone(buzzer);
      break;
    }
    if (digitalRead(button[BTN_MUYSUCIO]) == 0)
    {
      estado_2 = 2;
      MUYSUCIOO();
      tone(buzzer, 2000);
      delay(100);
      noTone(buzzer);
      break;
    }
    break;
  case CALIENTE:
    if (digitalRead(button[BTN_TIBIA]) == 0)
    {
      estado = TIBIA;
      TIBIAA();
      tone(buzzer, 2000);
      delay(100);
      noTone(buzzer);
      break;
    }
    if (digitalRead(button[BTN_PROG]) == 0)
    {
      if (program == 0)
      {
        estado_2 = 3;
        PROGG();
        tone(buzzer, 2000);
        delay(100);
        noTone(buzzer);
        break;
      }
      else
      {
        estado = PROG;
        PROGG();
        tone(buzzer, 2000);
        delay(100);
        noTone(buzzer);
        break;
      }
    }
    if (digitalRead(button[BTN_FRIA]) == 0)
    {
      estado = FRIA;
      FRIAA();
      tone(buzzer, 2000);
      delay(100);
      noTone(buzzer);
      break;
    }
    if (digitalRead(button[BTN_INICIO]) == 0)
    {
      while (digitalRead(BUTT_BOT) == 1)
      {
      /*  lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("CIERRA LA PUERTA           ");*/
        if (digitalRead(BUTT_BOT) == 0)
        {
          break;
        }
        tone(buzzer, 2000);
        delay(100);
        noTone(buzzer);
      }
      PUERTA_ON();
      delay(1000);
      if (digitalRead(BUTT_TOP) == 1)
      {
        if (digitalRead(BUTT_BOT) == 0)
        {
          val_off();
          STOP_M();
          while (1)
          {
            ////////////////////////////////////////////////////
            time = millis() / 1000;
            dato_error_puerta = time;
            if (dato_error_puerta != datoAnterior_error_puerta)
            {
              contador_error_puerta++;
              if (contador_error_puerta >= tiempo_error_puerta * 60)
              {
              /*  lcd.clear();
                lcd.setCursor(1, 0);
                lcd.print("ERROR BLOQUEO         ");*/
                while (1)
                {
                  tone(buzzer, 2000);
                  delay(1000);
                  noTone(buzzer);
                  delay(1000);
                }
              }
            }
            dato_error_puerta = datoAnterior_error_puerta;
            ////////////////////////////////////////////////////////////
            digitalWrite(led_puerta_cerrada, LOW);
            delay(250);
            digitalWrite(led_puerta_cerrada, HIGH);
            delay(250);
            if (digitalRead(BUTT_TOP) == 0)
            {
              break;
            }
          }
        }
      }
      else
      {
        estado = INICIO;
        ///////////////////////////////////////
        H1 = EEPROM.read(6);
        L1 = EEPROM.read(7);
        w1 = H1 << 8;
        uso = w1 + L1;
        uso += 1;
        H1 = highByte(uso);
        EEPROM.update(6, H1);
        L1 = lowByte(uso);
        EEPROM.update(7, L1);
        /*w1 = H1 << 8;
        uso = w1 + L1;*/
        ////////////////////////////////////////////
        /* uso = EEPROM.read(2350) + 1;
        EEPROM.update(2350, uso);*/
       /* lcd.clear();*/
        tone(buzzer, 2000);
        delay(100);
        noTone(buzzer);
      }
      break;
    }
    if (digitalRead(button[BTN_NORMAL]) == 0)
    {
      estado_2 = 0;
      NORMALL();
      tone(buzzer, 2000);
      delay(100);
      noTone(buzzer);
      break;
    }
    if (digitalRead(button[BTN_DELICADO]) == 0)
    {
      estado_2 = 1;
      DELICADOO();
      tone(buzzer, 2000);
      delay(100);
      noTone(buzzer);
      break;
    }
    if (digitalRead(button[BTN_MUYSUCIO]) == 0)
    {
      estado_2 = 2;
      MUYSUCIOO();
      tone(buzzer, 2000);
      delay(100);
      noTone(buzzer);
      break;
    }
    break;
  case NORMAL:
    if (digitalRead(button[BTN_TIBIA]) == 0)
    {
      estado = TIBIA;
      TIBIAA();
      tone(buzzer, 2000);
      delay(100);
      noTone(buzzer);
      break;
    }
    if (digitalRead(button[BTN_PROG]) == 0)
    {
      if (program == 0)
      {
        estado_2 = 3;
        PROGG();
        tone(buzzer, 2000);
        delay(100);
        noTone(buzzer);
        break;
      }
      else
      {
        estado = PROG;
        PROGG();
        tone(buzzer, 2000);
        delay(100);
        noTone(buzzer);
        break;
      }
    }
    if (digitalRead(button[BTN_FRIA]) == 0)
    {
      estado = FRIA;
      FRIAA();
      tone(buzzer, 2000);
      delay(100);
      noTone(buzzer);
      break;
    }
    if (digitalRead(button[BTN_INICIO]) == 0)
    {
      while (digitalRead(BUTT_BOT) == 1)
      {
      /*  lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("CIERRA LA PUERTA          ");*/
        if (digitalRead(BUTT_BOT) == 0)
        {
          break;
        }
        tone(buzzer, 2000);
        delay(100);
        noTone(buzzer);
      }
      PUERTA_ON();
      delay(1000);
      if (digitalRead(BUTT_TOP) == 1)
      {
        if (digitalRead(BUTT_BOT) == 0)
        {
          val_off();
          STOP_M();
          while (1)
          {
            ////////////////////////////////////////////////////
            time = millis() / 1000;
            dato_error_puerta = time;
            if (dato_error_puerta != datoAnterior_error_puerta)
            {
              contador_error_puerta++;
              if (contador_error_puerta >= tiempo_error_puerta * 60)
              {
              /*  lcd.clear();
                lcd.setCursor(1, 0);
                lcd.print("ERROR BLOQUEO            ");*/
                while (1)
                {
                  tone(buzzer, 2000);
                  delay(1000);
                  noTone(buzzer);
                  delay(1000);
                }
              }
            }
            dato_error_puerta = datoAnterior_error_puerta;
            ////////////////////////////////////////////////////////////
            digitalWrite(led_puerta_cerrada, LOW);
            delay(250);
            digitalWrite(led_puerta_cerrada, HIGH);
            delay(250);
            if (digitalRead(BUTT_TOP) == 0)
            {
              break;
            }
          }
        }
      }
      else
      {
        estado = INICIO;
        ///////////////////////////////////////
        H1 = EEPROM.read(6);
        L1 = EEPROM.read(7);
        w1 = H1 << 8;
        uso = w1 + L1;
        uso += 1;
        H1 = highByte(uso);
        EEPROM.update(6, H1);
        L1 = lowByte(uso);
        EEPROM.update(7, L1);
        /*w1 = H1 << 8;
        uso = w1 + L1;*/
        ////////////////////////////////////////////
        /*uso = EEPROM.read(2350) + 1;
        EEPROM.update(2350, uso);*/
       /* lcd.clear();*/
        tone(buzzer, 2000);
        delay(100);
        noTone(buzzer);
      }
      break;
    }
    if (digitalRead(button[BTN_CALIENTE]) == 0)
    {
      estado = CALIENTE;
      CALIENTEE();
      tone(buzzer, 2000);
      delay(100);
      noTone(buzzer);
      break;
    }
    if (digitalRead(button[BTN_DELICADO]) == 0)
    {
      estado_2 = 1;
      DELICADOO();
      tone(buzzer, 2000);
      delay(100);
      noTone(buzzer);
      break;
    }
    if (digitalRead(button[BTN_MUYSUCIO]) == 0)
    {
      estado_2 = 2;
      MUYSUCIOO();
      tone(buzzer, 2000);
      delay(100);
      noTone(buzzer);
      break;
    }
    break;
  case DELICADO:
    if (digitalRead(button[BTN_TIBIA]) == 0)
    {
      estado = TIBIA;
      TIBIAA();
      tone(buzzer, 2000);
      delay(100);
      noTone(buzzer);
      break;
    }
    if (digitalRead(button[BTN_PROG]) == 0)
    {
      if (program == 0)
      {
        estado_2 = 3;
        PROGG();
        tone(buzzer, 2000);
        delay(100);
        noTone(buzzer);
        break;
      }
      else
      {
        estado = PROG;
        PROGG();
        tone(buzzer, 2000);
        delay(100);
        noTone(buzzer);
        break;
      }
    }
    if (digitalRead(button[BTN_FRIA]) == 0)
    {
      estado = FRIA;
      FRIAA();
      tone(buzzer, 2000);
      delay(100);
      noTone(buzzer);
      break;
    }
    if (digitalRead(button[BTN_INICIO]) == 0)
    {
      while (digitalRead(BUTT_BOT) == 1)
      {
       /* lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("CIERRA LA PUERTA             ");*/
        if (digitalRead(BUTT_BOT) == 0)
        {
          break;
        }
        tone(buzzer, 2000);
        delay(100);
        noTone(buzzer);
      }
      PUERTA_ON();
      delay(1000);
      if (digitalRead(BUTT_TOP) == 1)
      {
        if (digitalRead(BUTT_BOT) == 0)
        {
          val_off();
          STOP_M();
          while (1)
          {
            ////////////////////////////////////////////////////
            time = millis() / 1000;
            dato_error_puerta = time;
            if (dato_error_puerta != datoAnterior_error_puerta)
            {
              contador_error_puerta++;
              if (contador_error_puerta >= tiempo_error_puerta * 60)
              {
              /*  lcd.clear();
                lcd.setCursor(1, 0);
                lcd.print("ERROR BLOQUEO             ");*/
                while (1)
                {
                  tone(buzzer, 2000);
                  delay(1000);
                  noTone(buzzer);
                  delay(1000);
                }
              }
            }
            dato_error_puerta = datoAnterior_error_puerta;
            ////////////////////////////////////////////////////////////
            digitalWrite(led_puerta_cerrada, LOW);
            delay(250);
            digitalWrite(led_puerta_cerrada, HIGH);
            delay(250);
            if (digitalRead(BUTT_TOP) == 0)
            {
              break;
            }
          }
        }
      }
      else
      {
        estado = INICIO;
        ///////////////////////////////////////
        H1 = EEPROM.read(6);
        L1 = EEPROM.read(7);
        w1 = H1 << 8;
        uso = w1 + L1;
        uso += 1;
        H1 = highByte(uso);
        EEPROM.update(6, H1);
        L1 = lowByte(uso);
        EEPROM.update(7, L1);
       /* w1 = H1 << 8;
        uso = w1 + L1;*/
        ////////////////////////////////////////////
        /*uso = EEPROM.read(2350) + 1;
        EEPROM.update(2350, uso);*/
      /*  lcd.clear();*/
        tone(buzzer, 2000);
        delay(100);
        noTone(buzzer);
      }
      break;
    }
    if (digitalRead(button[BTN_CALIENTE]) == 0)
    {
      estado = CALIENTE;
      CALIENTEE();
      tone(buzzer, 2000);
      delay(100);
      noTone(buzzer);
      break;
    }
    if (digitalRead(button[BTN_NORMAL]) == 0)
    {
      estado_2 = 0;
      NORMALL();
      tone(buzzer, 2000);
      delay(100);
      noTone(buzzer);
      break;
    }
    if (digitalRead(button[BTN_MUYSUCIO]) == 0)
    {
      estado_2 = 2;
      MUYSUCIOO();
      tone(buzzer, 2000);
      delay(100);
      noTone(buzzer);
      break;
    }
    break;
  case MUYSUCIO:
    if (digitalRead(button[BTN_TIBIA]) == 0)
    {
      estado = TIBIA;
      TIBIAA();
      tone(buzzer, 2000);
      delay(100);
      noTone(buzzer);
      break;
    }
    if (digitalRead(button[BTN_PROG]) == 0)
    {
      if (program == 0)
      {
        estado_2 = 3;
        PROGG();
        tone(buzzer, 2000);
        delay(100);
        noTone(buzzer);
        break;
      }
      else
      {
        estado = PROG;
        PROGG();
        tone(buzzer, 2000);
        delay(100);
        noTone(buzzer);
        break;
      }
    }
    if (digitalRead(button[BTN_FRIA]) == 0)
    {
      estado = FRIA;
      FRIAA();
      tone(buzzer, 2000);
      delay(100);
      noTone(buzzer);
      break;
    }
    if (digitalRead(button[BTN_INICIO]) == 0)
    {
      while (digitalRead(BUTT_BOT) == 1)
      {
       /* lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("CIERRA LA PUERTA            ");*/
        if (digitalRead(BUTT_BOT) == 0)
        {
          break;
        }
        tone(buzzer, 2000);
        delay(100);
        noTone(buzzer);
      }
      PUERTA_ON();
      delay(1000);
      if (digitalRead(BUTT_TOP) == 1)
      {
        if (digitalRead(BUTT_BOT) == 0)
        {
          val_off();
          STOP_M();
          while (1)
          {
            ////////////////////////////////////////////////////
            time = millis() / 1000;
            dato_error_puerta = time;
            if (dato_error_puerta != datoAnterior_error_puerta)
            {
              contador_error_puerta++;
              if (contador_error_puerta >= tiempo_error_puerta * 60)
              {
               /* lcd.clear();
                lcd.setCursor(1, 0);
                lcd.print("ERROR BLOQUEO           ");*/
                while (1)
                {
                  tone(buzzer, 2000);
                  delay(1000);
                  noTone(buzzer);
                  delay(1000);
                }
              }
            }
            dato_error_puerta = datoAnterior_error_puerta;
            ////////////////////////////////////////////////////////////
            digitalWrite(led_puerta_cerrada, LOW);
            delay(250);
            digitalWrite(led_puerta_cerrada, HIGH);
            delay(250);

            if (digitalRead(BUTT_TOP) == 0)
            {
              break;
            }
          }
        }
      }
      else
      {
        estado = INICIO;
        ///////////////////////////////////////
        H1 = EEPROM.read(6);
        L1 = EEPROM.read(7);
        w1 = H1 << 8;
        uso = w1 + L1;
        uso += 1;
        H1 = highByte(uso);
        EEPROM.update(6, H1);
        L1 = lowByte(uso);
        EEPROM.update(7, L1);
        /*w1 = H1 << 8;
        uso = w1 + L1;*/
        ////////////////////////////////////////////
        /*uso = EEPROM.read(2350) + 1;
        EEPROM.update(2350, uso);*/
       /* lcd.clear();*/
        tone(buzzer, 2000);
        delay(100);
        noTone(buzzer);
      }
      break;
    }
    if (digitalRead(button[BTN_CALIENTE]) == 0)
    {
      estado = CALIENTE;
      CALIENTEE();
      tone(buzzer, 2000);
      delay(100);
      noTone(buzzer);
      break;
    }
    if (digitalRead(button[BTN_NORMAL]) == 0)
    {
      estado_2 = 0;
      NORMALL();
      tone(buzzer, 2000);
      delay(100);
      noTone(buzzer);
      break;
    }
    if (digitalRead(button[BTN_DELICADO]) == 0)
    {
      estado_2 = 2;
      DELICADOO();
      tone(buzzer, 2000);
      delay(100);
      noTone(buzzer);
      break;
    }
    break;
  case PROG:
    if (program == 0)
    {
      if (digitalRead(button[BTN_TIBIA]) == 0)
      {
        estado = TIBIA;
        TIBIAA();
        tone(buzzer, 2000);
        delay(100);
        noTone(buzzer);
        break;
      }
      if (digitalRead(button[BTN_MUYSUCIO]) == 0)
      {
        tone(buzzer, 2000);
        delay(100);
        noTone(buzzer);
        break;
      }
      if (digitalRead(button[BTN_FRIA]) == 0)
      {
        estado = FRIA;
        FRIAA();
        tone(buzzer, 2000);
        delay(100);
        noTone(buzzer);
        break;
      }
      if (digitalRead(button[BTN_INICIO]) == 0)
      {
        while (digitalRead(BUTT_BOT) == 1)
        {
        /*  lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("CIERRA LA PUERTA          ");*/
          if (digitalRead(BUTT_BOT) == 0)
          {
            break;
          }
          tone(buzzer, 2000);
          delay(100);
          noTone(buzzer);
        }
        PUERTA_ON();
        delay(1000);
        if (digitalRead(BUTT_TOP) == 1)
        {
          if (digitalRead(BUTT_BOT) == 0)
          {
            val_off();
            STOP_M();
            while (1)
            {
              ////////////////////////////////////////////////////
              time = millis() / 1000;
              dato_error_puerta = time;
              if (dato_error_puerta != datoAnterior_error_puerta)
              {
                contador_error_puerta++;
                if (contador_error_puerta >= tiempo_error_puerta * 60)
                {
                /*  lcd.clear();
                  lcd.setCursor(1, 0);
                  lcd.print("ERROR BLOQUEO          ");*/
                  while (1)
                  {
                    tone(buzzer, 2000);
                    delay(1000);
                    noTone(buzzer);
                    delay(1000);
                  }
                }
              }
              dato_error_puerta = datoAnterior_error_puerta;
              ////////////////////////////////////////////////////////////
              digitalWrite(led_puerta_cerrada, LOW);
              delay(250);
              digitalWrite(led_puerta_cerrada, HIGH);
              delay(250);
              if (digitalRead(BUTT_TOP) == 0)
              {
                break;
              }
            }
          }
        }
        else
        {
          estado = INICIO;
          ///////////////////////////////////////
          H1 = EEPROM.read(6);
          L1 = EEPROM.read(7);
          w1 = H1 << 8;
          uso = w1 + L1;
          uso += 1;
          H1 = highByte(uso);
          EEPROM.update(6, H1);
          L1 = lowByte(uso);
          EEPROM.update(7, L1);
         /* w1 = H1 << 8;
          uso = w1 + L1;*/
          ////////////////////////////////////////////
          /*  uso = EEPROM.read(2350) + 1;
          EEPROM.update(2350, uso);*/
         /* lcd.clear();*/
          tone(buzzer, 2000);
          delay(100);
          noTone(buzzer);
        }
        break;
      }
      if (digitalRead(button[BTN_CALIENTE]) == 0)
      {
        estado = CALIENTE;
        CALIENTEE();
        tone(buzzer, 2000);
        delay(100);
        noTone(buzzer);
        break;
      }
      if (digitalRead(button[BTN_NORMAL]) == 0)
      {
        estado_2 = 0;
        NORMALL();
        tone(buzzer, 2000);
        delay(100);
        noTone(buzzer);
        break;
      }
      if (digitalRead(button[BTN_DELICADO]) == 0)
      {
        estado_2 = 2;
        DELICADOO();
        tone(buzzer, 2000);
        delay(100);
        noTone(buzzer);
        break;
      }
      break;
    }
    else
    {
      time = millis() / 1000;
      datoP = time;
      Serial.println(datoP);
      if (datoP != datoAnteriorP) //15 segundos en espera en modo programacion
      {
        contadorP++;
      /*  lcd.setCursor(14, 1);
        if (contadorP < 10)
          lcd.print("0");
        lcd.print(contadorP);*/
        if (contadorP % 2 == 0)
        {
          noTone(buzzer);
        }
        else
        {
          tone(buzzer, 2000);
        }
        if (contadorP >= 15)
        {
          asm volatile(" jmp 0");
        }
      }
      datoAnteriorP = datoP;
      if (!mfrc522.PICC_IsNewCardPresent())
        return;
      if (!mfrc522.PICC_ReadCardSerial())
        return;

      tagID = "";
      for (uint8_t i = 0; i < 4; i++)
      {
        tagID.concat(String(mfrc522.uid.uidByte[i], HEX));
      }

    /*  lcd.clear();
      lcd.setCursor(0, 0);
      lcd.println("ACTUALIZANDO... ");*/
      int i = 1;
      int t = 0;
      //  if (tagID == Tag)
      //  {
      for (;;) // lectura y escritura de paprametros en EEPROM
      {
        if (i > 2 && (i + 1) % 4 == 0)
        {
          i++;
        }
        readBlock(i, readbackblock);
        i++;
        Serial.print("block ");
        Serial.print(i - 1);
        Serial.print(":");
        for (int j = 0; j < 16; j++)
        {
          EEPROM.update(t, readbackblock[j]);
          Serial.print(readbackblock[j]);
         /* lcd.setCursor(0, 1);
          lcd.print("NO RETIRE RFID           ");*/
          t++;
          tone(buzzer, 1000);
        }
        Serial.println("");
        if (i == 63)
          break;
        if (t == 1000)
          t = 0;
      }
      // }
      /* else
      {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.println("TAG INCORRECTO  ");
      }*/

      mfrc522.PICC_HaltA();
      mfrc522.PCD_StopCrypto1();
      delay(1000);
      asm volatile(" jmp 0");
    }
  case INICIO:
    segundostotal = asegundos + (aminutos * 60) + (ahoras * 60 * 60);
    if (segundostotal == 0)
    {
      etapa = 0;
     /* lcd.clear();
      lcd.setCursor(5, 0);
      lcd.print("CICLO");
      lcd.setCursor(3, 1);
      lcd.print("FINALIZADO");*/
      delay(1000);
      asm volatile(" jmp 0");
    }
    if (etapa == 1)
    {
     /* lcd.setCursor(14, 1);
      lcd.print("E1");*/
      segundostotalB = segundostotal;
    }
    if (etapa == 2)
    {
    /*  lcd.setCursor(14, 1);
      lcd.print("E2");*/
      segundostotalB = segundostotal - TIEMPO_PRELAVADO * 60;
    }
    if (etapa == 3)
    {
    /*  lcd.setCursor(14, 1);
      lcd.print("E3");*/
      segundostotalB = segundostotal - (TIEMPO_PRELAVADO * 60 + TIEMPO_LAVADO * 60);
    }
    if (etapa == 4)
    {
    /*  lcd.setCursor(14, 1);
      lcd.print("E4");*/
      segundostotalB = segundostotal - (TIEMPO_PRELAVADO * 60 + TIEMPO_LAVADO * 60 + TIEMPO_ENJUAGUE * 60);
    }
    if (etapa == 5)
    {
      /*lcd.setCursor(14, 1);
      lcd.print("E5");*/
      segundostotalB = segundostotal - (TIEMPO_PRELAVADO * 60 + TIEMPO_LAVADO * 60 + TIEMPO_ENJUAGUE * 60 + TIEMPO_ENJUAGUE_EXTRA_1 * 60);
    }
    if (etapa == 6)
    {
     /* lcd.setCursor(14, 1);
      lcd.print("E6");*/
      segundostotalB = segundostotal - (TIEMPO_PRELAVADO * 60 + TIEMPO_LAVADO * 60 + TIEMPO_ENJUAGUE * 60 + TIEMPO_ENJUAGUE_EXTRA_1 * 60 + TIEMPO_ENJUAGUE_EXTRA_2 * 60);
    }
    if (etapa == 7)
    {
      segundostotalB = segundostotal - (TIEMPO_PRELAVADO * 60 + TIEMPO_LAVADO * 60 + TIEMPO_ENJUAGUE * 60 + TIEMPO_ENJUAGUE_EXTRA_1 * 60 + TIEMPO_ENJUAGUE_EXTRA_2 * 60 + TIEMPO_ENJUAGUE_FINAL * 60);
    }
    while (segundostotalB > 0)
    {
      while (digitalRead(BUTT_BOT) == 1)
      {
       /* lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("CIERRA LA PUERTA");*/
        if (digitalRead(BUTT_BOT) == 0 && digitalRead(BUTT_TOP) == 1)
        {
       /*   lcd.clear();*/
          break;
        }
        tone(buzzer, 2000);
        delay(100);
        noTone(buzzer);
        val_off();
        STOP_M();
      }
      if (digitalRead(BUTT_BOT) == 0 && digitalRead(BUTT_TOP) == 1)
      {
        val_off();
        STOP_M();
        while (1)
        {
          if (digitalRead(BUTT_TOP) == 0)
          {
            break;
          }
        }
      }

      //delay (1000);
      time = millis() / 1000;
      dato2 = time;
      if (dato2 != datoAnterior2)
      {
        segundostotalB--;
        segundosAux++;
        tiempo_aux2++;
        if (segundosAux >= TIEMPO_PRELAVADO * 60 && etapa == 1)
        {
         /* lcd.setCursor(14, 1);
          lcd.print("E2");
          EEPROM.update(33, 2);*/
          etapa = EEPROM.read(33);
          segundosAux = 0;
          tiempo_aux2 = 0;
          paso = 0;
        }
        if (segundosAux >= TIEMPO_LAVADO * 60 && etapa == 2)
        {
         /* lcd.setCursor(14, 1);
          lcd.print("E3");*/
          segundosAux = 0;
          EEPROM.update(33, 3);
          etapa = EEPROM.read(33);
          tiempo_aux2 = 0;
          paso = 0;
        }
        if (segundosAux >= TIEMPO_ENJUAGUE * 60 && etapa == 3)
        {
         /* lcd.setCursor(14, 1);
          lcd.print("E4");*/
          segundosAux = 0;
          EEPROM.update(33, 4);
          etapa = EEPROM.read(33);
          tiempo_aux2 = 0;
          paso = 0;
        }
        if (segundosAux >= TIEMPO_ENJUAGUE_EXTRA_1 * 60 && etapa == 4)
        {
        /*  lcd.setCursor(14, 1);
          lcd.print("E5");*/
          segundosAux = 0;
          EEPROM.update(33, 5);
          etapa = EEPROM.read(33);
          tiempo_aux2 = 0;
          paso = 0;
        }
        if (segundosAux >= TIEMPO_ENJUAGUE_EXTRA_2 * 60 && etapa == 5)
        {
         /* lcd.setCursor(14, 1);
          lcd.print("E6");*/
          segundosAux = 0;
          EEPROM.update(33, 6);
          etapa = EEPROM.read(33);
          tiempo_aux2 = 0;
          paso = 0;
        }
        if (segundosAux >= TIEMPO_ENJUAGUE_FINAL * 60 && etapa == 6)
        {
       /*   lcd.setCursor(14, 1);
          lcd.print("E7");*/
          segundosAux = 0;
          EEPROM.update(33, 7);
          etapa = EEPROM.read(33);
          tiempo_aux2 = 0;
          paso = 0;
        }
        if (segundosAux >= TIEMPO_CENTRIFUGADO_FINAL * 60 && etapa == 7)
        {
          contadorP = 0;
          EEPROM.update(33, 1);
          segundosAux = 0;
          etapa = EEPROM.read(33);
         /* lcd.clear();
          lcd.setCursor(5, 0);
          lcd.print("CICLO");
          lcd.setCursor(3, 1);
          lcd.print("FINALIZADO");*/
        }
      }
      datoAnterior2 = dato2;
      ahoras = ((segundostotalB / 60) / 60);
      aminutos = (segundostotalB / 60) % 60;
      asegundos = segundostotalB % 60;
      clock_lav();
      if (etapa == 1) //prelavado
      {
       /* lcd.setCursor(0, 0);
        lcd.print("PRELAVADO          ");*/
        digitalWrite(led_lavado, HIGH);
        if (paso == 0)
        {
          if (tiempo_aux2 <= LLENADO_AGIpre * 60)
          {
          /*  lcd.setCursor(14, 0);
            lcd.print("P1");*/
            time = millis() / 1000;
            dato_llenado = time;
            llenado_mojado(dato_llenado, nivelde_llenado_prelavado, tiempo_giro_izquierda, tiempo_giro_derecha, tiempo_reposo, tiempo_aux2, LLENADO_AGIpre, temperatura, etapa);
          }
          else
          {
            STOP_M();
            ///////////////////////////////////////////////////////////////////////////////7
            while (1)
            {
              val = analogRead(A0);
              float voltage2 = val * (5.0 / 1023.0);
             /* lcd.setCursor(8, 1);*/
              //lcd.print(voltage2);
              //lcd.print(mapp);
              if (voltage2 > voltage + 0.1 && voltage2 < voltage + 5.00)
              {
                mapp = (voltage2 - voltage) * 100;
              }
              else if (voltage2 < voltage)
              {
                mapp = 0;
              }
              else if (voltage2 > voltage + 5.00)
              {
                mapp = 500;
              }
              drenado();
              time = millis() / 1000;
              dato_error = time;
              if (dato_error != datoAnterior_error)
              {
                contador_error_llenado++;
                if (mapp <= 6)
                {
                  break;
                }
                if (contador_error_llenado >= tiempo_error_llenado * 60)
                {
                 /* lcd.clear();
                  lcd.setCursor(1, 0);
                  lcd.print("ERROR DRENADO           ");*/
                  drenado();
                  STOP_M();
                  val_off();
                  delay(3000);
                  PUERTA_OFF();
                  asm volatile(" jmp 0");
                  //ERROR NO HAY DRENADO
                }
              }
              datoAnterior_error = dato_error;
            }

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
          }
        }
        if (paso == 1)
        {
          if (tiempo_aux2 <= DESAGUE_AGIpre * 60)
          {
           /* lcd.setCursor(14, 0);
            lcd.print("P2");*/
            time = millis() / 1000;
            dato_llenado = time;
            lavado(dato_llenado, 0, tiempo_giro_izquierda, tiempo_giro_derecha, tiempo_reposo, tiempo_aux2, LLENADO_AGIpre);
          }
          else
          {
            STOP_M();
            ///////////////////////////////////////////////////////////////////////////////
            while (1)
            {
              val = analogRead(A0);
              float voltage2 = val * (5.0 / 1023.0);
            /*  lcd.setCursor(8, 1);*/
              //lcd.print(voltage2);
              //lcd.print(mapp);
              if (voltage2 > voltage + 0.1 && voltage2 < voltage + 5.00)
              {
                mapp = (voltage2 - voltage) * 100;
              }
              else if (voltage2 < voltage)
              {
                mapp = 0;
              }
              else if (voltage2 > voltage + 5.00)
              {
                mapp = 500;
              }
              drenado();
              time = millis() / 1000;
              dato_error = time;
              if (dato_error != datoAnterior_error)
              {
                contador_error_llenado++;
                if (mapp <= 6)
                {
                  break;
                }
                if (contador_error_llenado >= tiempo_error_llenado * 60)
                {
                /*  lcd.clear();
                  lcd.setCursor(1, 0);
                  lcd.print("ERROR DRENADO           ");*/
                  drenado();
                  STOP_M();
                  val_off();
                  delay(3000);
                  PUERTA_OFF();
                  asm volatile(" jmp 0");
                }
              }
              datoAnterior_error = dato_error;
            }

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
            //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////7
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            paso = 2;
          }
        }
        if (paso == 2)
        {
          if (tiempo_aux2 <= CENTRIFUpre * 60)
          {
            if (CENTRIFUpre * 60 - tiempo_aux2 <= 60)
            {
             /* lcd.setCursor(14, 0);
              lcd.print("P3");*/
              STOP_M();
            }
            else
            {
             /* lcd.setCursor(14, 0);
              lcd.print("P3");*/
              CENTRIFUGADO();
            }
          }
          else
          {
            STOP_M();
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
      if (etapa == 2) //lavado
      {
       /* lcd.setCursor(0, 0);
        lcd.print("LAVADO          ");*/
        digitalWrite(led_lavado, HIGH);
        if (paso == 0)
        {
          if (tiempo_aux2 <= LLENADO_AGIlav * 60)
          {
           /* lcd.setCursor(14, 0);
            lcd.print("P1");*/
            time = millis() / 1000;
            dato_llenado = time;
            llenado_mojado(dato_llenado, nivelde_llenado_lavado, tiempo_giro_izquierda, tiempo_giro_derecha, tiempo_reposo, tiempo_aux2, LLENADO_AGIlav, temperatura, etapa);
          }
          else
          {
            STOP_M();
            ///////////////////////////////////////////////////////////////////////////////7
            while (1)
            {
              val = analogRead(A0);
              float voltage2 = val * (5.0 / 1023.0);
             /* lcd.setCursor(8, 1);*/
              //lcd.print(voltage2);
              //lcd.print(mapp);
              if (voltage2 > voltage + 0.1 && voltage2 < voltage + 5.00)
              {
                mapp = (voltage2 - voltage) * 100;
              }
              else if (voltage2 < voltage)
              {
                mapp = 0;
              }
              else if (voltage2 > voltage + 5.00)
              {
                mapp = 500;
              }
              drenado();
              time = millis() / 1000;
              dato_error = time;
              if (dato_error != datoAnterior_error)
              {
                contador_error_llenado++;
                if (mapp <= 6)
                {
                  break;
                }
                if (contador_error_llenado >= tiempo_error_llenado * 60)
                {
                 /* lcd.clear();
                  lcd.setCursor(1, 0);
                  lcd.print("ERROR DRENADO          ");*/
                  drenado();
                  STOP_M();
                  val_off();
                  delay(3000);
                  PUERTA_OFF();
                  asm volatile(" jmp 0");
                }
              }
              datoAnterior_error = dato_error;
            }

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
          }
        }
        if (paso == 1)
        {
          if (tiempo_aux2 <= DESAGUE_AGIlav * 60)
          {
           /* lcd.setCursor(14, 0);
            lcd.print("P2");*/
            time = millis() / 1000;
            dato_llenado = time;
            lavado(dato_llenado, 0, tiempo_giro_izquierda, tiempo_giro_derecha, tiempo_reposo, tiempo_aux2, LLENADO_AGIlav);
          }
          else
          {
            STOP_M();
            ///////////////////////////////////////////////////////////////////////////////
            while (1)
            {
              val = analogRead(A0);
              float voltage2 = val * (5.0 / 1023.0);
             /* lcd.setCursor(8, 1);*/
              //lcd.print(voltage2);
              //lcd.print(mapp);
              if (voltage2 > voltage + 0.1 && voltage2 < voltage + 5.00)
              {
                mapp = (voltage2 - voltage) * 100;
              }
              else if (voltage2 < voltage)
              {
                mapp = 0;
              }
              else if (voltage2 > voltage + 5.00)
              {
                mapp = 500;
              }
              drenado();
              time = millis() / 1000;
              dato_error = time;
              if (dato_error != datoAnterior_error)
              {
                contador_error_llenado++;
                if (mapp <= 6)
                {
                  break;
                }
                if (contador_error_llenado >= tiempo_error_llenado * 60)
                {
                 /* lcd.clear();
                  lcd.setCursor(1, 0);
                  lcd.print("ERROR DRENADO           ");*/
                  drenado();
                  STOP_M();
                  val_off();
                  delay(3000);
                  PUERTA_OFF();
                  asm volatile(" jmp 0");
                }
              }
              datoAnterior_error = dato_error;
            }

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
            paso = 2;
          }
        }
        if (paso == 2)
        {
          if (tiempo_aux2 <= CENTRIFUlav * 60)
          {
            if (CENTRIFUlav * 60 - tiempo_aux2 <= 60)
            {
              /*lcd.setCursor(14, 0);
              lcd.print("P3");*/
              STOP_M();
            }
            else
            {
             /* lcd.setCursor(14, 0);
              lcd.print("P3");*/
              CENTRIFUGADO();
            }
          }
          else
          {
            STOP_M();
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
      if (etapa == 3) //enjuague
      {
      /*  lcd.setCursor(0, 0);
        lcd.print("ENJUAGUE          ");*/
        digitalWrite(led_enjuague, HIGH);
        digitalWrite(led_cloro, HIGH);
        if (paso == 0)
        {
          if (tiempo_aux2 <= LLENADO_AGIeng * 60)
          {
          /*  lcd.setCursor(14, 0)*;
            lcd.print("P1");*/
            time = millis() / 1000;
            dato_llenado = time;
            llenado_mojado(dato_llenado, nivelde_llenado_enjugaue, tiempo_giro_izquierda, tiempo_giro_derecha, tiempo_reposo, tiempo_aux2, LLENADO_AGIeng, temperatura, etapa);
          }
          else
          {
            STOP_M();
            ///////////////////////////////////////////////////////////////////////////////7
            while (1)
            {
              val = analogRead(A0);
              float voltage2 = val * (5.0 / 1023.0);
           /*   lcd.setCursor(8, 1);*/
              //lcd.print(voltage2);
              //lcd.print(mapp);
              if (voltage2 > voltage + 0.1 && voltage2 < voltage + 5.00)
              {
                mapp = (voltage2 - voltage) * 100;
              }
              else if (voltage2 < voltage)
              {
                mapp = 0;
              }
              else if (voltage2 > voltage + 5.00)
              {
                mapp = 500;
              }
              drenado();
              time = millis() / 1000;
              dato_error = time;
              if (dato_error != datoAnterior_error)
              {
                contador_error_llenado++;
                if (mapp <= 6)
                {
                  break;
                }
                if (contador_error_llenado >= tiempo_error_llenado * 60)
                {
                /*  lcd.clear();
                  lcd.setCursor(1, 0);
                  lcd.print("ERROR DRENADO           ");*/
                  drenado();
                  STOP_M();
                  val_off();
                  delay(3000);
                  PUERTA_OFF();
                  asm volatile(" jmp 0");
                }
              }
              datoAnterior_error = dato_error;
            }

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
          }
        }
        if (paso == 1)
        {
          if (tiempo_aux2 <= DESAGUE_AGIeng * 60)
          {
          /*  lcd.setCursor(14, 0);
            lcd.print("P2");*/
            time = millis() / 1000;
            dato_llenado = time;
            lavado(dato_llenado, 0, tiempo_giro_izquierda, tiempo_giro_derecha, tiempo_reposo, tiempo_aux2, LLENADO_AGIeng);
          }
          else
          {
            STOP_M();
            ///////////////////////////////////////////////////////////////////////////////
            while (1)
            {
              val = analogRead(A0);
              float voltage2 = val * (5.0 / 1023.0);
            /*  lcd.setCursor(8, 1);*/
              //lcd.print(voltage2);
              //lcd.print(mapp);
              if (voltage2 > voltage + 0.1 && voltage2 < voltage + 5.00)
              {
                mapp = (voltage2 - voltage) * 100;
              }
              else if (voltage2 < voltage)
              {
                mapp = 0;
              }
              else if (voltage2 > voltage + 5.00)
              {
                mapp = 500;
              }
              drenado();
              time = millis() / 1000;
              dato_error = time;
              if (dato_error != datoAnterior_error)
              {
                contador_error_llenado++;
                if (mapp <= 6)
                {
                  break;
                }
                if (contador_error_llenado >= tiempo_error_llenado * 60)
                {
                /*  lcd.clear();
                  lcd.setCursor(1, 0);
                  lcd.print("ERROR DRENADO              ");*/
                  drenado();
                  STOP_M();
                  val_off();
                  delay(3000);
                  PUERTA_OFF();
                  asm volatile(" jmp 0");
                }
              }
              datoAnterior_error = dato_error;
            }

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
          if (tiempo_aux2 <= CENTRIFUeng * 60)
          {
            if (CENTRIFUeng * 60 - tiempo_aux2 <= 60)
            {
             /* lcd.setCursor(14, 0);
              lcd.print("P3");*/
              STOP_M();
            }
            else
            {
            /*  lcd.setCursor(14, 0);
              lcd.print("P3");*/
              CENTRIFUGADO();
            }
          }
          else
          {
            STOP_M();
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
      if (etapa == 4) //enjuague extra1
      {
      /*  lcd.setCursor(0, 0);
        lcd.print("ENJ. EXTRA 1              ");*/
        digitalWrite(led_enjuague, HIGH);
        if (paso == 0)
        {
          if (tiempo_aux2 <= LLENADO_AGIex1 * 60)
          {
           /* lcd.setCursor(14, 0);
            lcd.print("P1");*/
            time = millis() / 1000;
            dato_llenado = time;
            llenado_mojado(dato_llenado, nivel_de_llenado_ennjuague_extra_1, tiempo_giro_izquierda, tiempo_giro_derecha, tiempo_reposo, tiempo_aux2, LLENADO_AGIex1, temperatura, etapa);
          }
          else
          {
            STOP_M();
            ///////////////////////////////////////////////////////////////////////////////7
            while (1)
            {
              val = analogRead(A0);
              float voltage2 = val * (5.0 / 1023.0);
             /* lcd.setCursor(8, 1);*/
              //lcd.print(voltage2);
              //lcd.print(mapp);
              if (voltage2 > voltage + 0.1 && voltage2 < voltage + 5.00)
              {
                mapp = (voltage2 - voltage) * 100;
              }
              else if (voltage2 < voltage)
              {
                mapp = 0;
              }
              else if (voltage2 > voltage + 5.00)
              {
                mapp = 500;
              }
              drenado();
              time = millis() / 1000;
              dato_error = time;
              if (dato_error != datoAnterior_error)
              {
                contador_error_llenado++;
                if (mapp <= 6)
                {
                  break;
                }
                if (contador_error_llenado >= tiempo_error_llenado * 60)
                {
               /*   lcd.clear();
                  lcd.setCursor(1, 0);
                  lcd.print("ERROR DRENADO           ");*/
                  drenado();
                  STOP_M();
                  val_off();
                  delay(3000);
                  PUERTA_OFF();
                  asm volatile(" jmp 0");
                }
              }
              datoAnterior_error = dato_error;
            }

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
          }
        }
        if (paso == 1)
        {
          if (tiempo_aux2 <= DESAGUE_AGIex1 * 60)
          {
           /* lcd.setCursor(14, 0);
            lcd.print("P2");*/
            time = millis() / 1000;
            dato_llenado = time;
            lavado(dato_llenado, 0, tiempo_giro_izquierda, tiempo_giro_derecha, tiempo_reposo, tiempo_aux2, LLENADO_AGIex1);
          }
          else
          {
            STOP_M();
            ///////////////////////////////////////////////////////////////////////////////
            while (1)
            {
              val = analogRead(A0);
              float voltage2 = val * (5.0 / 1023.0);
            /*  lcd.setCursor(8, 1);*/
              //lcd.print(voltage2);
              //lcd.print(mapp);
              if (voltage2 > voltage + 0.1 && voltage2 < voltage + 5.00)
              {
                mapp = (voltage2 - voltage) * 100;
              }
              else if (voltage2 < voltage)
              {
                mapp = 0;
              }
              else if (voltage2 > voltage + 5.00)
              {
                mapp = 500;
              }
              drenado();
              time = millis() / 1000;
              dato_error = time;
              if (dato_error != datoAnterior_error)
              {
                contador_error_llenado++;
                if (mapp <= 6)
                {
                  break;
                }
                if (contador_error_llenado >= tiempo_error_llenado * 60)
                {
                 /* lcd.clear();
                  lcd.setCursor(1, 0);
                  lcd.print("ERROR DRENADO                ");*/
                  drenado();
                  STOP_M();
                  val_off();
                  delay(3000);
                  PUERTA_OFF();
                  asm volatile(" jmp 0");
                }
              }
              datoAnterior_error = dato_error;
            }

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
          if (tiempo_aux2 <= CENTRIFUex1 * 60)
          {
            if (CENTRIFUex1 * 60 - tiempo_aux2 <= 60)
            {
            /*  lcd.setCursor(14, 0);
              lcd.print("P3");*/
              STOP_M();
            }
            else
            {
            /*  lcd.setCursor(14, 0);
              lcd.print("P3");*/
              CENTRIFUGADO();
            }
          }
          else
          {

            STOP_M();
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
      if (etapa == 5) //enjuague extra2
      {
       /* lcd.setCursor(0, 0);
        lcd.print("ENJ. EXTRA 2                 ");*/
        digitalWrite(led_enjuague, HIGH);
        if (paso == 0)
        {
          if (tiempo_aux2 <= LLENADO_AGIex2 * 60)
          {
           /* lcd.setCursor(14, 0);
            lcd.print("P1");*/
            time = millis() / 1000;
            dato_llenado = time;
            llenado_mojado(dato_llenado, nivel_de_llenado_ennjuague_extra_2, tiempo_giro_izquierda, tiempo_giro_derecha, tiempo_reposo, tiempo_aux2, LLENADO_AGIex2, temperatura, etapa);
          }
          else
          {
            STOP_M();
            while (1)
            {
              val = analogRead(A0);
              float voltage2 = val * (5.0 / 1023.0);
             /* lcd.setCursor(8, 1);*/
              //lcd.print(voltage2);
              //lcd.print(mapp);
              if (voltage2 > voltage + 0.1 && voltage2 < voltage + 5.00)
              {
                mapp = (voltage2 - voltage) * 100;
              }
              else if (voltage2 < voltage)
              {
                mapp = 0;
              }
              else if (voltage2 > voltage + 5.00)
              {
                mapp = 500;
              }
              drenado();
              time = millis() / 1000;
              dato_error = time;
              if (dato_error != datoAnterior_error)
              {
                contador_error_llenado++;
                if (mapp <= 6)
                {
                  break;
                }
                if (contador_error_llenado >= tiempo_error_llenado * 60)
                {
                /*  lcd.clear();
                  lcd.setCursor(1, 0);
                  lcd.print("ERROR DRENADO           ");*/
                  drenado();
                  STOP_M();
                  val_off();
                  delay(3000);
                  PUERTA_OFF();
                  asm volatile(" jmp 0");
                }
              }
              datoAnterior_error = dato_error;
            }

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
          }
        }
        if (paso == 1)
        {
          if (tiempo_aux2 <= DESAGUE_AGIex2 * 60)
          {
           /* lcd.setCursor(14, 0);
            lcd.print("P2");*/
            time = millis() / 1000;
            dato_llenado = time;
            lavado(dato_llenado, 0, tiempo_giro_izquierda, tiempo_giro_derecha, tiempo_reposo, tiempo_aux2, LLENADO_AGIex2);
          }
          else
          {
            STOP_M();
            ///////////////////////////////////////////////////////////////////////////////
            while (1)
            {
              val = analogRead(A0);
              float voltage2 = val * (5.0 / 1023.0);
              /*lcd.setCursor(8, 1);*/
              //lcd.print(voltage2);
              //lcd.print(mapp);
              if (voltage2 > voltage + 0.1 && voltage2 < voltage + 5.00)
              {
                mapp = (voltage2 - voltage) * 100;
              }
              else if (voltage2 < voltage)
              {
                mapp = 0;
              }
              else if (voltage2 > voltage + 5.00)
              {
                mapp = 500;
              }
              drenado();
              time = millis() / 1000;
              dato_error = time;
              if (dato_error != datoAnterior_error)
              {
                contador_error_llenado++;
                if (mapp <= 6)
                {
                  break;
                }
                if (contador_error_llenado >= tiempo_error_llenado * 60)
                {
                /*  lcd.clear();
                  lcd.setCursor(1, 0);
                  lcd.print("ERROR DRENADO          ");*/
                  drenado();
                  STOP_M();
                  val_off();
                  delay(3000);
                  PUERTA_OFF();
                  asm volatile(" jmp 0");
                }
              }
              datoAnterior_error = dato_error;
            }

            ///////////////////////////////////////////////////////////////////////////////////////
            t = 1;
            error_llenado = 0;
            llenado_error = 1;
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
          if (tiempo_aux2 <= CENTRIFUex2 * 60)
          {
            if (CENTRIFUex2 * 60 - tiempo_aux2)
            {
            /*  lcd.setCursor(14, 0);
              lcd.print("P3");*/
              STOP_M();
            }
            else
            {
            /*  lcd.setCursor(14, 0);
              lcd.print("P3");*/
              CENTRIFUGADO();
            }
          }
          else
          {
            STOP_M();
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
      if (etapa == 6) //enjuauge final
      {
       /* lcd.setCursor(0, 0);
        lcd.print("ENJ. FINAL            ");*/
        digitalWrite(led_enjuague_f, HIGH);
        if (paso == 0)
        {
          if (tiempo_aux2 <= LLENADO_AGIfin * 60)
          {
           /* lcd.setCursor(14, 0);
            lcd.print("P1");*/
            time = millis() / 1000;
            dato_llenado = time;
            llenado_mojado(dato_llenado, nivel_de_llenado_ennjuague_final, tiempo_giro_izquierda, tiempo_giro_derecha, tiempo_reposo, tiempo_aux2, LLENADO_AGIfin, temperatura, etapa);
          }
          else
          {
            STOP_M();
            ///////////////////////////////////////////////////////////////////////////////7
            while (1)
            {
              val = analogRead(A0);
              float voltage2 = val * (5.0 / 1023.0);
             /* lcd.setCursor(8, 1);*/
              //lcd.print(voltage2);
              //lcd.print(mapp);
              if (voltage2 > voltage + 0.1 && voltage2 < voltage + 5.00)
              {
                mapp = (voltage2 - voltage) * 100;
              }
              else if (voltage2 < voltage)
              {
                mapp = 0;
              }
              else if (voltage2 > voltage + 5.00)
              {
                mapp = 500;
              }
              drenado();
              time = millis() / 1000;
              dato_error = time;
              if (dato_error != datoAnterior_error)
              {
                contador_error_llenado++;
                if (mapp <= 6)
                {
                  break;
                }
                if (contador_error_llenado >= tiempo_error_llenado * 60)
                {
                 /* lcd.clear();
                  lcd.setCursor(1, 0);
                  lcd.print("ERROR DRENADO          ");*/
                  drenado();
                  STOP_M();
                  val_off();
                  delay(3000);
                  PUERTA_OFF();
                  asm volatile(" jmp 0");
                }
              }
              datoAnterior_error = dato_error;
            }

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
          }
        }
        if (paso == 1)
        {
          if (tiempo_aux2 <= DESAGUE_AGIfin * 60)
          {
           /* lcd.setCursor(14, 0);
            lcd.print("P2");*/
            time = millis() / 1000;
            dato_llenado = time;
            lavado(dato_llenado, 0, tiempo_giro_izquierda, tiempo_giro_derecha, tiempo_reposo, tiempo_aux2, LLENADO_AGIfin);
          }
          else
          {
            STOP_M();
            ///////////////////////////////////////////////////////////////////////////////
            while (1)
            {
              val = analogRead(A0);
              float voltage2 = val * (5.0 / 1023.0);
             /* lcd.setCursor(8, 1);*/
              //lcd.print(voltage2);
              //lcd.print(mapp);
              if (voltage2 > voltage + 0.1 && voltage2 < voltage + 5.00)
              {
                mapp = (voltage2 - voltage) * 100;
              }
              else if (voltage2 < voltage)
              {
                mapp = 0;
              }
              else if (voltage2 > voltage + 5.00)
              {
                mapp = 500;
              }
              drenado();
              time = millis() / 1000;
              dato_error = time;
              if (dato_error != datoAnterior_error)
              {
                contador_error_llenado++;
                if (mapp <= 6)
                {
                  break;
                }
                if (contador_error_llenado >= tiempo_error_llenado * 60)
                {
                /*  lcd.clear();
                  lcd.setCursor(1, 0);
                  lcd.print("ERROR DRENADO           ");*/
                  drenado();
                  STOP_M();
                  val_off();
                  delay(3000);
                  PUERTA_OFF();
                  asm volatile(" jmp 0");
                }
              }
              datoAnterior_error = dato_error;
            }

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
          if (tiempo_aux2 <= CENTRIFUfin * 60)
          {
            if (CENTRIFUfin * 60 - tiempo_aux2 <= 60)
            {
             /* lcd.setCursor(14, 0);
              lcd.print("P3");*/
              STOP_M();
            }
            else
            {
            /*  lcd.setCursor(14, 0);
              lcd.print("P3");*/
              CENTRIFUGADO();
            }
          }
          else
          {
            STOP_M();
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
      if (etapa == 7) //centrifugado final
      {
       /* lcd.setCursor(0, 0);
        lcd.print("CENTRIFUGADO        ");*/
        digitalWrite(led_centrifugado, HIGH);
        if (paso == 0)
        {
          if (tiempo_aux2 <= TIEMPO_CENTRIFUGADO_FINAL * 60)
          {
            if (TIEMPO_CENTRIFUGADO_FINAL * 60 - tiempo_aux2 <= 60)
            {
            /*  lcd.setCursor(14, 0);
              lcd.print("P3");*/
              STOP_M();
            }
            else
            {
            /*  lcd.setCursor(14, 0);
              lcd.print("P3");*/
              CENTRIFUGADO();
            }
          }
          else
          {
            STOP_M();
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
            paso = 1;
          }
        }
      }
      if (segundostotalB <= 0) //fin del ciclo
      {
        while (1)
        {
          time = millis() / 1000;
          datoP = time;
          if (datoP != datoAnteriorP) //timer para las monedas
          {
            digitalWrite(led_lavado, LOW);
            digitalWrite(led_enjuague, LOW);
            digitalWrite(led_enjuague_f, LOW);
            digitalWrite(led_centrifugado, LOW);
            digitalWrite(led_puerta_cerrada, LOW);
            digitalWrite(led_cloro, LOW);
           /* lcd.clear();
            lcd.setCursor(5, 0);
            lcd.print("CICLO");
            lcd.setCursor(3, 1);
            lcd.print("FINALIZADO");*/
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
              digitalWrite(led_lavado, LOW);
              digitalWrite(led_enjuague, LOW);
              digitalWrite(led_enjuague_f, LOW);
              digitalWrite(led_centrifugado, LOW);
              digitalWrite(led_puerta_cerrada, LOW);
              digitalWrite(led_cloro, LOW);
              PUERTA_OFF();
              etapa = 1;
              EEPROM.update(33, 1);

              asm volatile(" jmp 0");
            }
          }
          datoAnteriorP = datoP;
        }
      }
    }

    break;
  }
  delay(1);
}