#ifndef HAL_EXTI_H
#define HAL_EXTI_H

#include <stdint.h>

/* Tipo de callback sin argumentos para cada pulso */
typedef void (*exti_cb_t)(void);

/* Configura PD2/PD3 como entradas y habilita INT0/INT1 (flanco de bajada) */
void HAL_EXTI_Init(exti_cb_t cb_int0, exti_cb_t cb_int1);

/* Habilitar/Deshabilitar de forma explícita (opcional) */
void HAL_EXTI_Enable(void);
void HAL_EXTI_Disable(void);

#endif
