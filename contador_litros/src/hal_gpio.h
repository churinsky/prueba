#ifndef HAL_GPIO_H
#define HAL_GPIO_H

#include <stdint.h>
#include <stdbool.h>

/* =============================================================================
 * HAL GPIO — ATmega328P
 * Este módulo inicializa y maneja las señales digitales de:
 *   - LED_PROG (PD5)
 *   - BTN_MODE  (PD7, activo en bajo, pull-up interno)
 *   - BTN_CAL   (PD6, activo en bajo, pull-up interno)
 * No configura UART/SPI/TWI aquí — eso va en sus HALs respectivos.
 * ============================================================================*/

/* Inicializa la dirección y el estado eléctrico de LED, botones y sensores. */
void HAL_GPIO_Init(void);

/* ---------------- LED_PROG (PD5) ---------------- */
void HAL_GPIO_SetLedProg(bool on);
void HAL_GPIO_ToggleLedProg(void);

/* --------------- Botones (activo en bajo) --------------- */
/* Lecturas “semánticas”: true = PRESIONADO, false = no presionado */
bool HAL_GPIO_ReadBtnMode(void);
bool HAL_GPIO_ReadBtnCal(void);

/* /OE del 74HC125 (activo en bajo) */
void HAL_GPIO_UartBufInit(void);         // PB0 = entrada + pull-up (deshabilitado)
void HAL_GPIO_UartBufEnable(bool en);    // true: salida=0 (habilita) | false: entrada+pull-up (deshabilita)


#endif /* HAL_GPIO_H */
