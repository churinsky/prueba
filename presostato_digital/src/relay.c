#include "relay.h"

void relay_init(Relay *relay)
{
    *(relay->ddr)|=(1<<relay->pin);//como salida
    *(relay->port)&=~(1<<relay->pin);//para que inicie apagado
}

void relay_off(Relay *relay)
{
    *(relay->port)&=~(1<<relay->pin);//para apagarlo
}
void relay_on(Relay *relay)
{
    *(relay->port)|=(1<<relay->pin);//para prenderlo
}