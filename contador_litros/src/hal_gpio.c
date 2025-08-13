#include <avr/io.h>
#include "hal_gpio.h"
#include "app_pins.h"  /* Nombres simbólicos de puertos/bits (ver abajo) */


/* ============================================================================
 * RAZONES DE DISEÑO (resumen):
 *  - LED: salida; se inicia en APAGADO para evitar parpadeos de arranque.
 *  - Botones: entrada + pull-up; lectura activa en bajo.
 *  - Sensores: entrada sin pull-up interno (ya hay 10k externos en tu PCB).
 *  - RMW (Read-Modify-Write): siempre con máscaras para no pisar otros bits.
 *  - No se deshabilitan interrupciones aquí: cada pin usa un bit distinto.
 *    Si un ISR tocara el MISMO puerto/bit, revisa coherencia a nivel de app.
 * ==========================================================================*/

void HAL_GPIO_Init(void)
{
    /* ---------------- LED_PROG (PD5) como SALIDA, inicia en LOW ---------------- */
    LED_PROG_DDR  |= BIT(LED_PROG_BIT);      /* salida */
    LED_PROG_PORT &= (uint8_t)~BIT(LED_PROG_BIT);  /* LED apagado */

    /* ---------------- Botones como ENTRADA con PULL-UP ----------------
     * Pulsador a GND => pull-up ON => lectura en PINR será 0 cuando se presiona
     */
    BTN_MODE_DDR  &= (uint8_t)~BIT(BTN_MODE_BIT);  /* entrada */
    BTN_MODE_PORT |= BIT(BTN_MODE_BIT);            /* pull-up ON */

    BTN_CAL_DDR   &= (uint8_t)~BIT(BTN_CAL_BIT);   /* entrada */
    BTN_CAL_PORT  |= BIT(BTN_CAL_BIT);             /* pull-up ON */

   /* Nota: NO tocamos aquí PD2/PD3 (sensores). Eso lo hará hal_exti. */
}

void HAL_GPIO_UartBufInit(void)
{
    /* Deshabilitar por defecto: ENTRADA + PULL-UP => /OE=1 */
    UART_BUF_OE_DDR  &= (uint8_t)~BIT(UART_BUF_OE_BIT);  // entrada
    UART_BUF_OE_PORT |= BIT(UART_BUF_OE_BIT);            // pull-up ON (solo vale si es entrada)
}

/* en = true  -> habilita (pone /OE=0)  -> SALIDA en 0
 * en = false -> deshabilita (pone /OE=1)-> ENTRADA con pull-up
 */
void HAL_GPIO_UartBufEnable(bool en)
{
    if (en)
    {
        /* Transición a habilitado:
           - Preparar nivel 0
           - Luego pasar a salida (así, al volverse salida, ya está en 0) */
        UART_BUF_OE_PORT &= (uint8_t)~BIT(UART_BUF_OE_BIT);  // nivel 0 preparado
        UART_BUF_OE_DDR  |= BIT(UART_BUF_OE_BIT);            // salida (drive a 0) => /OE=0
    }
    else
    {
        /* Volver a deshabilitar de forma segura:
           - Primero quitar "drive" (entrada)
           - Luego activar pull-up para mantener /OE=1 */
        UART_BUF_OE_DDR  &= (uint8_t)~BIT(UART_BUF_OE_BIT);  // entrada (deja de conducir)
        UART_BUF_OE_PORT |= BIT(UART_BUF_OE_BIT);            // pull-up ON => /OE=1
    }
}
/* ---------------- LED_PROG ---------------- */

void HAL_GPIO_SetLedProg(bool on)
{
    if (on) {
        LED_PROG_PORT |= BIT(LED_PROG_BIT);
    } else {
        LED_PROG_PORT &= (uint8_t)~BIT(LED_PROG_BIT);
    }
}

void HAL_GPIO_ToggleLedProg(void)
{
    /* Toggle seguro del bit sin afectar otros */
    LED_PROG_PORT ^= BIT(LED_PROG_BIT);
}

/* ---------------- Botones (activo en bajo) ----------------
 * true = PRESIONADO (nivel bajo), false = no presionado (nivel alto)
 */

bool HAL_GPIO_ReadBtnMode(void)
{
    return ( (BTN_MODE_PINR & BIT(BTN_MODE_BIT)) == 0u );
}

bool HAL_GPIO_ReadBtnCal(void)
{
    return ( (BTN_CAL_PINR & BIT(BTN_CAL_BIT)) == 0u );
}

