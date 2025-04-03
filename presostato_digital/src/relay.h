#ifndef RELAY_H
#define RELAY_H
#include <avr/io.h>
#include<util/delay.h>
typedef struct 
{
volatile uint8_t *ddr;
volatile uint8_t *port;
uint8_t pin;
}Relay;

void relay_init(Relay *relay);
void relay_on(Relay *relay);
void relay_off(Relay *relay);

#endif