#include "adc.h"
#include "relay.h"
#include "nivel.h"
#include "timer.h"
#include <avr/io.h>
#include <util/delay.h>
// me falta hacer la recalibracion automatica
// me falta hacer la recepcion de niveles de tarjeta mautra a presostato

#define alpha 0.1                    // para el filtro recomendado para el presostato
#define nivel_vacio_cm 1.0           // nivel de vacio
#define tiempo_de_recalibracion 1000 // tiempo en vacio para recalibrar lo voy a usar apra calcular el vacio
#define voltaje_offset 0.2           // ofset mpx5010
#define sensibilidad 0.45            // 450mV/kpa
#define umbral_activacio_cm_1 20.0     // nivel al que se activa el rele
#define umbral_activacio_cm_2 25.0     // nivel al que se activa el rele
#define umbral_activacio_cm_3 30.0     // nivel al que se activa el rele
#define cm_a_kpas 10.2
// ariables
float voltajeFiltrado = 0.0;
float presionAtmosferica = 0.0;
float voltageSum = 0.0;

Relay rele = {.ddr = &DDRD, .port = &PORTD, .pin = PD5};                                         // cosas del relay
Nivel nl = {.ddr = &DDRB, .port = &PORTB, .pin_reg = &PINB, .pin = PB1, .pcint_numero = PCINT1}; // cosas para el nivel seleccionar un nivel que viene del display aun no hay nada

int main(void)
{
    // incializaciones de rele y lo del nivel
    relay_init(&rele);
    nivel_init(&nl);
    adc_init(); // Avref, prescales 111
    // inicializacion del voltaje de filtrado pal filtro
    for (int i = 0; i < 20; i++)
    {
        float lectura_i = adc_read(5);
        voltageSum += lectura_i * (5.0 / 1023.0);
        _delay_ms(10);
    }
    voltajeFiltrado = voltageSum / 20;
    presionAtmosferica = (voltajeFiltrado - voltaje_offset) / sensibilidad;
    while (1)
    {
        // filtro recomendado para el presostato media movil ponderada
        int lectura_n = adc_read(5) * (5.0 / 1023.0);
        voltajeFiltrado = (alpha * lectura_n) + ((1 - alpha) * voltajeFiltrado);

        // conversion kpascales
        float presion_kpa = (voltajeFiltrado - voltaje_offset) / sensibilidad;
        float presionRel = presion_kpa - presionAtmosferica;
        if (presionRel < 0)
            presionRel = 0;
        // control del nivel del presostato a la tarjeta de laavadora mautra
        float nivel_cm = presionRel * cm_a_kpas;
        if (nivel_cm > umbral_activacio_cm_1)
        {
            relay_on(&rele);
            _delay_ms(100);
        }
        else
        {
            relay_off(&rele);
            _delay_ms(100);
        }
        _delay_ms(100);
    }
    return 0;
}