#include "led.h"
#include <util/delay.h>

// el structi de los punteros por lo pronto uso el puerto b en el atmega328p
Led led_prueba = {
    .ddr = &DDRB,
    .port = &PORTB,
    .pin = PB0,
    .state = 0
};

int main(void) {
    led_inic(&led_prueba);     // inicializacion de cosas del led
    led_apagar(&led_prueba);      // inicia pagado

    while (1) {
        led_conmutar(&led_prueba);  // Cel parpadeo del led
        _delay_ms(500);       // para probar nadamas la conmuracion me espero un rato
    }

    return 0;
}
