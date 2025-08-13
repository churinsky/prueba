#include <avr/io.h>
#include "hal_uart.h"

/* UBRR = F_CPU/(16*baud) - 1  (modo normal, U2X0=0) */
static inline uint16_t calc_ubrr(uint32_t f_cpu, uint32_t baud)
{
    return (uint16_t)((f_cpu / (16UL * baud)) - 1UL);
}

void HAL_UART_Init(uint32_t baud)
{
    /* Asíncrono, 8N1, normal speed */
    const uint16_t ubrr = calc_ubrr(F_CPU, baud);
    UBRR0H = (uint8_t)(ubrr >> 8);
    UBRR0L = (uint8_t)(ubrr & 0xFFu);

    UCSR0A = 0u;                     /* U2X0=0 */
    UCSR0B = (1u << TXEN0);          /* TX habilitado (RX opcional después) */
    UCSR0C = (1u << UCSZ01) | (1u << UCSZ00);  /* 8-bit, 1 stop, no parity */
}

void HAL_UART_TxByte(uint8_t b)
{
    /* Espera a que el buffer esté vacío */
    while ((UCSR0A & (1u << UDRE0)) == 0u) { /* spin */ }
    UDR0 = b;
}

void HAL_UART_TxBuffer(const uint8_t *buf, size_t len)
{
    for (size_t i = 0; i < len; i++) {
        HAL_UART_TxByte(buf[i]);
    }
}

void HAL_UART_TxStr(const char *s)
{
    while (*s != '\0') {
        HAL_UART_TxByte((uint8_t)*s++);
    }
}

void HAL_UART_TxCRLF(void)
{
    HAL_UART_TxByte('\r');
    HAL_UART_TxByte('\n');
}

/* Decimal sin %printf (evitamos <stdio.h>) */
void HAL_UART_TxU32(uint32_t v)
{
    char buf[10];
    int8_t i = 0;

    if (v == 0u) {
        HAL_UART_TxByte('0');
        return;
    }
    while (v > 0u && i < (int8_t)sizeof(buf)) {
        buf[i++] = (char)('0' + (v % 10u));
        v /= 10u;
    }
    while (--i >= 0) {
        HAL_UART_TxByte((uint8_t)buf[i]);
    }
}
