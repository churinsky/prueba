#ifndef APP_PINS_H
#define APP_PINS_H

#include <avr/io.h>
#include <stdint.h>

/* Helper para máscara de bit  */
#ifndef BIT
#define BIT(n) ((uint8_t)(1u << (n)))
#endif

/* ===== LED ===== */
#define LED_PROG_DDR DDRD
#define LED_PROG_PORT PORTD
#define LED_PROG_PINR PIND
#define LED_PROG_BIT (5u) /* PD5 */

/* ===== Botones ===== */
#define BTN_CAL_DDR DDRD
#define BTN_CAL_PORT PORTD /* pull-up aquí */
#define BTN_CAL_PINR PIND
#define BTN_CAL_BIT (6u) /* PD6 */

#define BTN_MODE_DDR DDRD
#define BTN_MODE_PORT PORTD /* pull-up aquí */
#define BTN_MODE_PINR PIND
#define BTN_MODE_BIT (7u) /* PD7 */

/* ===== Sensores de flujo ===== */
#define FLOW_FRIA_DDR DDRD
#define FLOW_FRIA_PORT PORTD /* pull-up si se requiere */
#define FLOW_FRIA_PINR PIND
#define FLOW_FRIA_BIT (2u) /* PD2 / INT0 */

#define FLOW_CAL_DDR DDRD
#define FLOW_CAL_PORT PORTD
#define FLOW_CAL_PINR PIND
#define FLOW_CAL_BIT (3u) /* PD3 / INT1 */

/* ===== 74HC125 /OE (activo en bajo) ===== */
#define UART_BUF_OE_DDR    DDRB
#define UART_BUF_OE_PORT   PORTB
#define UART_BUF_OE_PINR   PINB   /* por si algún día quieres leerlo */
#define UART_BUF_OE_BIT    (0u)   /* PB0 */


#endif /* APP_PINS_H */
