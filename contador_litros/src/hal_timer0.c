#include <avr/io.h>
#include <avr/interrupt.h>
#include "hal_timer0.h"

#ifndef F_CPU
#  define F_CPU 16000000UL
#endif

/* Contador global de milisegundos (lo incrementa la ISR) */
static volatile uint32_t s_ms = 0;

/* ISR cada 1 ms: Timer0 en modo CTC, compara con OCR0A */
ISR(TIMER0_COMPA_vect)
{
    s_ms++;
}

void HAL_Timer0_Init(void)
{
    /* Queremos 1 kHz (1 ms) con F_CPU=16 MHz:
       f = F_CPU / (prescaler * (1 + OCR0A))
       Elegimos prescaler=64 y OCR0A=249 → 16e6 / (64 * 250) = 1000 Hz */
    TCCR0A = (1u << WGM01);                 /* CTC: WGM01=1, WGM00=0 */
    TCCR0B = 0u;                             /* limpia prescaler por ahora */
    OCR0A  = 249u;                           /* TOP para 1 ms */
    TIMSK0 = (1u << OCIE0A);                 /* habilita interrupción compare A */
    TCCR0B = (1u << CS01) | (1u << CS00);    /* prescaler 64: CS01=1, CS00=1 */
    /* Nota: NO llamamos sei() aquí. Hazlo en main cuando todo esté listo. */
}

uint32_t HAL_Millis(void)
{
    /* Lectura atómica de 32 bits en AVR */
    uint32_t ms;
    uint8_t s = SREG;
    cli();
    ms = s_ms;
    SREG = s;
    return ms;
}

void HAL_DelayMs(uint16_t ms)
{
    const uint32_t t0 = HAL_Millis();
    while ((HAL_Millis() - t0) < (uint32_t)ms) {
        /* spin no bloqueante del todo (permite ISR) */
    }
}
