#include "uart.h"
#include <avr/io.h>

void uart_init(uint32_t baund)
{
    // ubrr=(f_cpu/(16*baund))-1
    uint16_t ubrr = (F_CPU / (16UL * baund)) - 1;
    // asig ubrr
    UBRR0H = (ubrr >> 8);
    UBRR0L = ubrr;

    // habilitar transmision y recepcion
    UCSR0B = (1 << TXEN0) | (1 << RXEN0);
    // configuracion 8n1
    UCSR0C = (1 << UCSZ00) | (1 << UCSZ01);
}
void uart_send_char(char c)
{
    while (!(UCSR0A & (1 << UDRE0)))
        ;

    UDR0 = c;
}
void uart_send_string(const char *str)
{
    while (*str)
    {
        uart_send_char(*str++);
    }
}