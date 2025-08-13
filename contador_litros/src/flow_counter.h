#ifndef FLOW_COUNTER_H
#define FLOW_COUNTER_H

#include <stdint.h>
#include <stdbool.h>

/* Estructura de estado del conteo */
typedef struct {
    volatile uint32_t pulses_fria;
    volatile uint32_t pulses_cal;
    float k_fria;      /* pulsos por litro */
    float k_cal;
} FlowData;

/* API */
void FlowCounter_Init(FlowData *fd, float k_fria, float k_cal);
void FlowCounter_AttachISRs(void);     /* registra callbacks en hal_exti */
void FlowCounter_Reset(FlowData *fd);

uint32_t FlowCounter_ReadPulsesFria(void);
uint32_t FlowCounter_ReadPulsesCal(void);

float FlowCounter_LitrosFria(const FlowData *fd);
float FlowCounter_LitrosCal(const FlowData *fd);
float FlowCounter_LitrosTotal(const FlowData *fd);

#endif
