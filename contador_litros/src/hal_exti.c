#include <avr/io.h>
#include <avr/interrupt.h>
#include "hal_exti.h"
#include "app_pins.h"

/* Callbacks registrados */
static exti_cb_t s_cb0 = 0;
static exti_cb_t s_cb1 = 0;

void HAL_EXTI_Init(exti_cb_t cb_int0, exti_cb_t cb_int1)
{
    s_cb0 = cb_int0;
    s_cb1 = cb_int1;

    /* 1) Configurar PD2/PD3 como ENTRADAS.
       Tus sensores tienen pull-up EXTERNO en la placa, así que interno OFF. */
    FLOW_FRIA_DDR &= (uint8_t)~BIT(FLOW_FRIA_BIT);  /* entrada */
    FLOW_CAL_DDR &= (uint8_t)~BIT(FLOW_CAL_BIT);    /* entrada */
    FLOW_FRIA_PORT &= (uint8_t)~BIT(FLOW_FRIA_BIT); /* pull-up interno OFF */
    FLOW_CAL_PORT &= (uint8_t)~BIT(FLOW_CAL_BIT);   /* pull-up interno OFF */

    /* 2) Configurar el tipo de flanco.
       Con pull-up (externo) típico, el sensor “tira” a GND -> flanco de BAJADA.
       INT0 (ISC01:ISC00) = 10b, INT1 (ISC11:ISC10) = 10b  => FALLING EDGE.
    */
    EICRA = (1u << ISC01) | (1u << ISC11);

    /* 3) Limpiar flags pendientes */
    EIFR = (1u << INTF0) | (1u << INTF1);

    /* 4) Habilitar interrupciones INT0/INT1 */
    EIMSK = (1u << INT0) | (1u << INT1);
    
    /* 5) Limpiar flags pendientes */
    EIFR = (1u << INTF0) | (1u << INTF1);
}

void HAL_EXTI_Enable(void)
{
    EIMSK |= (1u << INT0) | (1u << INT1);
}

void HAL_EXTI_Disable(void)
{
    EIMSK &= (uint8_t)~((1u << INT0) | (1u << INT1));
}

/* ISRs: llaman a los callbacks si existen */
ISR(INT0_vect)
{
    if (s_cb0 != 0)
    {
        s_cb0();
    }
}

ISR(INT1_vect)
{
    if (s_cb1 != 0)
    {
        s_cb1();
    }
}
