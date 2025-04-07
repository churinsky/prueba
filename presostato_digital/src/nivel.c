#include "nivel.h"

static Nivel *nl_ptr = 0;                         // lo utilizo para lo de interrupciones
#define PCINT0_PIN_MASK ((1 << PB0) | (1 << PB1)) // una mascara apra luego evitar que seleccionesn algo fuera del vector
static volatile uint8_t pulsos = 0;
static volatile uint32_t tiempo_ultimo_pulso = 0;
static volatile uint8_t nivel_actual = 0;

void nivel_init(Nivel *nivel)
{
    *(nivel->ddr) &= ~(1 << nivel->pin); // configuraciona comoentrada
    *(nivel->port) |= (1 << nivel->pin); // pullup
}
bool nivel_select(Nivel *nivel)
{
    return !(*(nivel->pin_reg) & (1 << nivel->pin));
}