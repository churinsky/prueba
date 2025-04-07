#include "led.h"

//para configurar el led
void led_inic(Led *led) {
    *(led->ddr) |= (1 << led->pin);
}
//para prender el led
void led_encede(Led *led) {
    *(led->port) |= (1 << led->pin);
    led->state = 1;
}
//para apagar el led
void led_apagar(Led *led) {
    *(led->port) &= ~(1 << led->pin);
    led->state = 0;
}
//para conmutar el led  con el xor
void led_conmutar(Led *led) {
    *(led->port) ^= (1 << led->pin);
    led->state = !led->state;
}
//para saber si el led sta encendido o apafado
uint8_t led_estado(Led *led) {
    return led->state;
}
