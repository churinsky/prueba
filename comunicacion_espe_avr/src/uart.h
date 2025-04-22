#ifndef UART_H
#define UART_H

#include <stdint.h>

// pra el envio de info
#define MSG_START_BYTE 0xAA
#define MSG_END_BYTE 0x55
#define MSG_MAX_LEN 16

void uart_init(uint32_t baud);
void uart_send_char(uint8_t c);
void uart_send_char_array(uint8_t *data, uint8_t len);

// formato: START - LEN - DATA - END
void uart_send_message(uint8_t *data, uint8_t len);
#endif
