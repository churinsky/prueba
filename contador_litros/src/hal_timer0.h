#ifndef HAL_TIMER0_H
#define HAL_TIMER0_H

#include <stdint.h>

/* Inicializa Timer0 para generar un “tick” de 1 ms (F_CPU = 16 MHz). */
void     HAL_Timer0_Init(void);

/* Tiempo desde el arranque en milisegundos (lectura atómica). */
uint32_t HAL_Millis(void);

/* Delay no bloqueante (usa HAL_Millis internamente). */
void     HAL_DelayMs(uint16_t ms);

#endif
