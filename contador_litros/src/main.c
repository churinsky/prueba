#include <avr/interrupt.h>
#include "hal_timer0.h"
#include "hal_gpio.h"

int main(void)
{
    HAL_GPIO_Init();
    HAL_Timer0_Init();
    sei();                     /* habilita interrupciones globales */

    uint32_t t0 = 0;
    uint8_t led = 0;

    for (;;) {
        uint32_t now = HAL_Millis();
        if ((now - t0) >= 1000u) {   /* manejo de overflow seguro con unsigned */
            t0 = now;
            led ^= 1u;
            HAL_GPIO_SetLedProg(led != 0u);
        }
    }
}
