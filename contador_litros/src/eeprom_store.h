#ifndef EEPROM_STORE_H
#define EEPROM_STORE_H

#include <stdbool.h>

/* Guarda/carga factores K en EEPROM con verificación.
 * Retorna true en éxito; false si los datos no son válidos.
 */
bool KStore_Load(float *k_fria_out, float *k_cal_out);
bool KStore_Save(float k_fria, float k_cal);

#endif
