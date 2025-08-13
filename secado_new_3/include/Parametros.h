#ifndef _PARAMETROS_H_
#define _PARAMETROS_H_
#include <Arduino.h>
#include "EEPROM.h"

int _tiempo_secado;    // 1
int _tiempo_cooldown;  // 2
int _valor_moneda;     // 3----
int _precio_temp1;     // 4
int _temperatura_baja; // 7
int _temperatura_alta; // 8
int _tiempo_adicional; // 13
int _opl;              // 14--
int _tiempo_giro_izquierda_1;
int _tiempo_giro_derecha_1;
int _tiempo_reposo_1;
void paramreos_eeprom()
{
    EEPROM.update(1, 34); // secado tiempo_1
    EEPROM.update(2, 1);  // cooldown
    EEPROM.update(3, 0);  // precio t1
    EEPROM.update(4, 40); // temp baja
    EEPROM.update(5, 55); // temp alta

    EEPROM.update(6, 34);  // secado tiempo_1
    EEPROM.update(7, 1);   // cooldown
    EEPROM.update(8, 0);   // precio t1
    EEPROM.update(9, 55);  // temp baja
    EEPROM.update(10, 65); // temp alta

    EEPROM.update(11, 34     ); // secado tiempo_2
    EEPROM.update(12, 1);  // cooldown
    EEPROM.update(13, 0);  // precio t1
    EEPROM.update(14, 65); // temp baja
    EEPROM.update(15, 75); // temp alta

    EEPROM.update(16, 10); // tiempo extra
    EEPROM.update(17, 30);
    EEPROM.update(18, 10); // valor moneda
    EEPROM.update(19, 30);
    EEPROM.update(20, 30);

    EEPROM.update(21, 12); // tiempo izq
    EEPROM.update(22, 6);  // stop
    EEPROM.update(23, 12); // tiempo der
    EEPROM.update(24, 12); // tiempo izq
    EEPROM.update(25, 6);  // stop
    EEPROM.update(26, 12); // tiempo der
    EEPROM.update(27, 12); // tiempo izq
    EEPROM.update(28, 6);  // stop
    EEPROM.update(29, 12); // tiempo der
}

void temperatura_1(int parametros) // nivel del agua
{
    if (parametros == 1)
    {
        _tiempo_secado = EEPROM.read(1);    // 1
        _tiempo_cooldown = EEPROM.read(2);  // 2
        _precio_temp1 = EEPROM.read(3);     // 4
        _temperatura_baja = EEPROM.read(4); // 7
        _temperatura_alta = EEPROM.read(5); // 8
        _valor_moneda = EEPROM.read(18);
        _tiempo_adicional = EEPROM.read(16); // 13
        _tiempo_giro_izquierda_1 = EEPROM.read(21);
        _tiempo_giro_derecha_1 = EEPROM.read(22);
        _tiempo_reposo_1 = EEPROM.read(23);
    }
    if (parametros == 2)
    {
        _tiempo_secado = EEPROM.read(6);     // 1
        _tiempo_cooldown = EEPROM.read(7);   // 2
        _precio_temp1 = EEPROM.read(8);      // 4
        _temperatura_baja = EEPROM.read(9);  // 7
        _temperatura_alta = EEPROM.read(10); // 8
        _valor_moneda = EEPROM.read(18);
        _tiempo_adicional = EEPROM.read(16); // 13
        _tiempo_giro_izquierda_1 = EEPROM.read(24);
        _tiempo_giro_derecha_1 = EEPROM.read(25);
        _tiempo_reposo_1 = EEPROM.read(26);
    }
    if (parametros == 3)
    {
        _tiempo_secado = EEPROM.read(11);    // 1
        _tiempo_cooldown = EEPROM.read(12);  // 2
        _precio_temp1 = EEPROM.read(13);     // 4
        _temperatura_baja = EEPROM.read(14); // 7
        _temperatura_alta = EEPROM.read(15); // 8
        _valor_moneda = EEPROM.read(18);
        _tiempo_adicional = EEPROM.read(16); // 13
        _tiempo_giro_izquierda_1 = EEPROM.read(27);
        _tiempo_giro_derecha_1 = EEPROM.read(28);
        _tiempo_reposo_1 = EEPROM.read(29);
    }
}

#endif
