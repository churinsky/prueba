#ifndef NIVEL_H
#define NIVEL_H


#include<avr/io.h>
#include<stdbool.h>
typedef struct 
{
    volatile uint8_t *ddr;
    volatile uint8_t *port;
    volatile uint8_t *pin_reg;
    uint8_t pin;
    uint8_t pcint_numero;
}Nivel;

void nivel_init(Nivel *nivel);
bool nivel_act(void);
bool nivel_ob(void);
#endif