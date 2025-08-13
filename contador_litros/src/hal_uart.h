#ifndef HAL_UART_H
#define HAL_UART_H

#include <stddef.h>
#include <stdint.h>

void HAL_UART_Init(uint32_t baud);
/* TX bloqueante, suficiente para debug */
void HAL_UART_TxByte(uint8_t b);
void HAL_UART_TxBuffer(const uint8_t *buf, size_t len);

/* Helpers de depuración opcionales */
void HAL_UART_TxStr(const char *s);
void HAL_UART_TxCRLF(void);
void HAL_UART_TxU32(uint32_t v);   /* imprime decimal simple */

#endif
