#include "Arduino.h"
#include "salidas_lavadora.h"
//int mapp=0;
salidas_lavadora::salidas_lavadora(int _motor_L, int _motor_R,int _motor_H, int _door, int _drain,int _T_D_1F,int _T_D_1C, int _dep2, int _dep3)
{
   motor_L=_motor_L; 
   motor_R=_motor_R;
   motor_H=_motor_H;
   door=_door;
   drain=_drain;
   T_D_1F=_T_D_1F;
   T_D_1C=_T_D_1C;
   dep2=_dep2;
   dep3=_dep3;
  
  pinMode(motor_L, OUTPUT);
  pinMode(motor_R, OUTPUT);
  pinMode(motor_H, OUTPUT);
  pinMode(door, OUTPUT);
  pinMode(drain, OUTPUT);
  pinMode(T_D_1F, OUTPUT);
  pinMode(T_D_1C, OUTPUT);
  pinMode(dep2, OUTPUT);
  pinMode(dep3, OUTPUT);
 
   all_off();
}

void salidas_lavadora::all_off()
{
  digitalWrite(motor_L, HIGH);
  digitalWrite(motor_R, HIGH);
  digitalWrite(motor_H, HIGH);
  digitalWrite(door, HIGH);
  digitalWrite(drain, HIGH);
  digitalWrite(T_D_1F, HIGH);
  digitalWrite(T_D_1C, HIGH);
  digitalWrite(dep2, HIGH);
  digitalWrite(dep3, HIGH);
}
void salidas_lavadora::puerta_on()
{
  digitalWrite(door, LOW);
}
void salidas_lavadora::PUERTA_OFF()
{
  digitalWrite(door, HIGH);
}
void salidas_lavadora::izquierda_m()
{
  digitalWrite(motor_L, LOW);
  digitalWrite(motor_R, HIGH);
  digitalWrite(motor_H, HIGH);
}
void salidas_lavadora::derecha_m()
{
  digitalWrite(motor_R, LOW);
  digitalWrite(motor_L, HIGH);
  digitalWrite(motor_H, HIGH);
}
void salidas_lavadora::STOP_M()
{
  digitalWrite(motor_L, HIGH);
  digitalWrite(motor_R, HIGH);
  digitalWrite(motor_H, HIGH);
}
void salidas_lavadora::drenado()
{
  digitalWrite(drain, HIGH);
}
void salidas_lavadora::centrifugado()
{
  digitalWrite(motor_L, HIGH);
  digitalWrite(motor_R, HIGH);
  digitalWrite(motor_H, LOW);
}
void salidas_lavadora::no_drenado()
{
  digitalWrite(drain, LOW);
}
void salidas_lavadora::val_off()
{
  digitalWrite(T_D_1F, HIGH);
  digitalWrite(T_D_1C, HIGH);
  digitalWrite(dep2, HIGH);
  digitalWrite(dep3, HIGH);
}

void salidas_lavadora::agua_fria_lavado()
{
  digitalWrite(T_D_1F, LOW);
  digitalWrite(T_D_1C, HIGH);
  digitalWrite(dep2, LOW);
  digitalWrite(dep3, HIGH);
}
void salidas_lavadora::agua_tibia_lavado()
{
  digitalWrite(T_D_1F, LOW);
  digitalWrite(T_D_1C, LOW);
  digitalWrite(dep2, LOW);
  digitalWrite(dep3, HIGH);
}
void salidas_lavadora::agua_caliente_lavado()
{
  digitalWrite(T_D_1F, HIGH);
  digitalWrite(T_D_1C, LOW);
  digitalWrite(dep2, LOW);
  digitalWrite(dep3, HIGH);
}
void salidas_lavadora::enjuague_final()
{
  digitalWrite(T_D_1F, LOW);
  digitalWrite(T_D_1C, LOW);
  digitalWrite(dep2, LOW);
  digitalWrite(dep3, LOW);
}
void salidas_lavadora::agua_fria_enjuague()
{
  digitalWrite(T_D_1F, LOW);
  digitalWrite(T_D_1C, HIGH);
  digitalWrite(dep2, LOW);
  digitalWrite(dep3, HIGH);
}
void salidas_lavadora::agua_tibia_enjuague()
{
  digitalWrite(T_D_1F, LOW);
  digitalWrite(T_D_1C, LOW);
  digitalWrite(dep2, LOW);
  digitalWrite(dep3, HIGH);
}
void salidas_lavadora::agua_caliente_enjuague()
{
  digitalWrite(T_D_1F, HIGH);
  digitalWrite(T_D_1C, LOW);
  digitalWrite(dep2, LOW);
  digitalWrite(dep3, HIGH);
}

