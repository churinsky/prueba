#include <avr/interrupt.h>
#include "flow_counter.h"
#include "hal_exti.h"

/* Estado global mínimo (podríamos guardarlo dentro de fd también) */
static FlowData *s_fd = 0;

/* ---- callbacks llamados desde las ISRs de hal_exti ---- */
static void on_pulso_fria(void) { s_fd->pulses_fria++; }
static void on_pulso_cal(void)  { s_fd->pulses_cal++;  }

/* Lectura atómica de 32 bits en AVR (8 bits) */
static uint32_t atomic_read_u32(volatile uint32_t *p)
{
    uint32_t v;
    uint8_t s = SREG;
    cli();
    v = *p;
    SREG = s;
    return v;
}

void FlowCounter_Init(FlowData *fd, float k_fria, float k_cal)
{
    s_fd = fd;
    s_fd->pulses_fria = 0u;
    s_fd->pulses_cal  = 0u;
    s_fd->k_fria = (k_fria > 0.0f) ? k_fria : 450.0f;
    s_fd->k_cal  = (k_cal  > 0.0f) ? k_cal  : 450.0f;
}

void FlowCounter_AttachISRs(void)
{
    /* Registrar nuestros callbacks en hal_exti */
    HAL_EXTI_Init(on_pulso_fria, on_pulso_cal);
}

void FlowCounter_Reset(FlowData *fd)
{
    uint8_t s = SREG; cli();
    fd->pulses_fria = 0u;
    fd->pulses_cal  = 0u;
    SREG = s;
}

uint32_t FlowCounter_ReadPulsesFria(void) { return atomic_read_u32(&s_fd->pulses_fria); }
uint32_t FlowCounter_ReadPulsesCal(void)  { return atomic_read_u32(&s_fd->pulses_cal);  }

float FlowCounter_LitrosFria(const FlowData *fd)
{
    return (fd->k_fria > 0.0f) ? ((float)FlowCounter_ReadPulsesFria() / fd->k_fria) : 0.0f;
}
float FlowCounter_LitrosCal(const FlowData *fd)
{
    return (fd->k_cal  > 0.0f) ? ((float)FlowCounter_ReadPulsesCal()  / fd->k_cal)  : 0.0f;
}
float FlowCounter_LitrosTotal(const FlowData *fd)
{
    return FlowCounter_LitrosFria(fd) + FlowCounter_LitrosCal(fd);
}
