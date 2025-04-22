#include "uart.h"
#include <avr/io.h>

#include "protocol_control.h" 
#include <avr/interrupt.h>

void uart_init(uint32_t baud)
{
    uint16_t ubrr = (F_CPU / (16UL * baud)) - 1;

    UBRR0H = (ubrr >> 8);
    UBRR0L = ubrr;

    // habilita transmisión, recepcion y interrupcion en rx
    UCSR0B = (1 << RXEN0) | (1 << TXEN0)| (1 << RXCIE0);

    // formato: 8 bits, 1 bit de stop, sin paridad
    UCSR0C = (1 << UCSZ00) | (1 << UCSZ01);

    sei(); // Hhabilitar interrupciones globales

}

void uart_send_char(uint8_t c)
{
    while (!(UCSR0A & (1 << UDRE0)))
        ; // Espera a que el buffer esté vacío
    UDR0 = c;
}

void uart_send_char_array(uint8_t *data, uint8_t len) {
    for (uint8_t i = 0; i < len; i++) {
        uart_send_char(data[i]);
    }
}

void uart_send_message(uint8_t *data, uint8_t len) {
    uart_send_char(MSG_START_BYTE);  // inicio
    uart_send_char(len);             // tamaño del mensaje
    uart_send_char_array(data, len); // datos
    uart_send_char(MSG_END_BYTE);    // fin
}

ISR(USART_RX_vect)
{
    uint8_t byte = UDR0;
    Protocol_ProcessByte(byte);
}