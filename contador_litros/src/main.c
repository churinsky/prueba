#include <avr/interrupt.h>
#include "fsm.h"
#include "hal_uart.h"
#include "hal_gpio.h"

/* K por defecto (si EEPROM vacía). Partimos de ~450 pulsos/L típico YF-B6. */
#ifndef K_FRIA_DEF
#define K_FRIA_DEF (450.0f)
#endif
#ifndef K_CAL_DEF
#define K_CAL_DEF  (450.0f)
#endif

int main(void)
{
    Fsm fsm;
    FSM_Init(&fsm, K_FRIA_DEF, K_CAL_DEF);

    /* Señal de arranque por UART (opcional) */
    HAL_GPIO_UartBufEnable(true);
    HAL_UART_TxStr("Medidor Agua - Init\r\n");
    HAL_GPIO_UartBufEnable(false);

    sei(); /* habilita interrupciones globales después de inicializar HALs */

    for (;;) {
        FSM_Tick(&fsm);
        /* Aquí podríamos dormir entre ticks para ahorrar (si se habilita sleep). */
    }
}
