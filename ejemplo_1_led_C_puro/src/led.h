#ifndef LED_H
#define LED_H

#include <stdint.h>
#include <avr/io.h>

typedef struct
{
    volatile uint8_t *ddr;  // aqui la configurascion de los registros
    volatile uint8_t *port; // para poner el puerto
    uint8_t pin;            // el bit
    uint8_t state;          // y para almacenar estado, la voy a usar mas adelante en este aun no sirve de nada
} Led;

// las funciones que ando usando
void led_inic(Led *led);
void led_encender(Led *led);
void led_apagar(Led *led);
void led_conmutar(Led *led);
uint8_t led_estado(Led *led);

#endif
