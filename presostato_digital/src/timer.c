#include "timer.h"
#include <avr/interrupt.h>
static volatile uint32_t contador_ms = 0;

void timer_init(void)
{
    TCCR0A = (1 << WGM01);              // para que se configure modod ctc 0b010
    TCCR0B = (1 << CS01) | (1 << CS00); // prescalar /64
    OCR0A = 249;                        // hasta donde quiero que cuente
    TIMSK0 = (1 << OCIE0A);             // habilita la interrupcion comparacion a ocr0a
    contador_ms = 0;                    // inicializo en 0
    TCNT0 = 0;                          // inicializo en 0
    sei();                              // habilitando interrupciones globales
}
uint32_t millis()
{
    uint32_t valor;
    cli();
    valor = contador_ms;
    sei();
    return valor;
}
ISR(TIMER0_COMPA_vect)
{
    contador_ms++;
}