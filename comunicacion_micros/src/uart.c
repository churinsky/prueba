#include "uart.h"
#include <avr/io.h>

void uart_init(uint32_t baud)
{
    // set baud rate
    uint16_t ubrr = (F_CPU / (16UL * baud)) - 1;
    UBRR0H = ubrr >> 8;
    UBRR0L = ubrr;
    // enable reciver and transmitter
    UCSR0B = (1 << RXEN0) | (1 << TXEN0);
    // set frame format
    UCSR0C = (1 << UCSZ00) | (1 << UCSZ01);
}

void uart_send_char(uint8_t c)
{
    while (!(UCSR0A & (1 << UDRE0)))
        ;
    UDR0 = c;
}

uint8_t uart_receive_char(void)
{
    while (!(UCSR0A & (1 << RXC0)))
        ;
    return UDR0;
}