#ifndef TIMER_H
#define TIMER_H
#include <avr/io.h>

typedef struct
{
    volatile uint32_t contador_milli;
} Timer;

void timer_init(Timer *t);
uint32_t millis(void);

#endif