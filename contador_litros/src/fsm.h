#ifndef FSM_H
#define FSM_H

#include <stdint.h>
#include <stdbool.h>
#include "flow_counter.h"

typedef enum {
    S_BOOT = 0,
    S_NORMAL,
    S_PROG_FRIA,
    S_PROG_CAL,
    S_SAVE,
    S_ERROR
} FsmState;

typedef struct {
    FsmState   st;
    FlowData   fd;           /* incluye K_fria/K_cal y pulsos */
    float      vol_obj_l;    /* litros objetivo para calibración (10.0) */
    uint32_t   t_led;        /* blink */
    uint32_t   t_tx;         /* telemetría UART */
    bool       last_btn_cal; /* para flancos */
    bool       last_mode_prog;
} Fsm;

void FSM_Init(Fsm *f, float k_fria_init, float k_cal_init);
void FSM_Tick(Fsm *f);

#endif
