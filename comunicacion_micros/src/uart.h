#ifndef UART_H
#define UART_H
#include <stdint.h>

void uart_init(uint32_t baud);
void uart_send_char(uint8_t c);
#endif