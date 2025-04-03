#include "nivel.h"

static Nivel *nl_ptr=0;// lo utilizo para lo de interrupciones
#define PCINT0_PIN_MASK ((1<<PB0)|(1<<PB1))// una mascara apra luego evitar que seleccionesn algo fuera del vector
static volatile uint8_t pulsos=0;
static volatile uint32_t tiempo_ultimo_pulso=0;
static volatile uint8_t nivel_actual=0;

void nivel_init(Nivel *nivel)
{
    nl_ptr=nivel;
    if(!(PCINT0_PIN_MASK&(1<<nivel->pin)))// lo estoy usando para solo poder validar que se modifique pb1 o pb0 de la interrupcion
    {
        return;// no entra no permite configurarr
    }

    *(nivel->ddr)&=~(1<<nivel->pin);//configuraciona comoentrada
    *(nivel->port)|=(1<<nivel->pin);// pullup
    PCMSK0|=(1<<nivel->pcint_numero);//habilitando interrupciones en pb1 o para pb0
    PCICR|=(1<<PCIE0);// habilitando pci0
    sei();//habilita interrupciones globales
}
bool nivel_act(void)
{
    return nivel_actual;
}