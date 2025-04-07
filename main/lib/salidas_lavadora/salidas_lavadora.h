#ifndef salidas_lavadora_h
#define salidas_lavadora_h

#include "Arduino.h"

class salidas_lavadora
{
      int motor_L;
      int motor_R;
      int motor_H;
      int door;
      int drain;
      int T_D_1F;
      int T_D_1C;
      int dep2;
      int dep3;

public:
      salidas_lavadora(int _motor_L, int _motor_R, int _motor_H, int _door, int _drain, int _T_D_1F, int _T_D_1C, int _dep2, int _dep3);
      void puerta_on();
      void PUERTA_OFF();
      void izquierda_m();
      void derecha_m();
      void STOP_M();
      void drenado();
      void centrifugado();
      void no_drenado();
      void val_off();
      void agua_fria_lavado();
      void agua_tibia_lavado();
      void agua_caliente_lavado();
      void enjuague_final();
      void agua_fria_enjuague();
      void agua_tibia_enjuague();
      void agua_caliente_enjuague();
      void all_off();
      //  void llenado_movimiento(int dato_llenado, int nivelde_llenado_prelavado, int tiempo_giro_izquierda, int tiempo_giro_derecha, int tiempo_reposo, int tiempo_aux2, int LLENADO_AGIpre, int temperatura, int ETAPA);
};

#endif
