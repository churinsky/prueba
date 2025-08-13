#include "fsm.h"
#include "hal_gpio.h"
#include "hal_exti.h"
#include "hal_timer0.h"
#include "hal_uart.h"
#include "eeprom_store.h"

#ifndef VOL_OBJ_L
#define VOL_OBJ_L (10.0f)
#endif

static inline bool edge_fall(bool now, bool *last)
{
    bool fell = (!now) && (*last);
    *last = now;
    return fell;
}

static void tx_total(const Fsm *f)
{
    HAL_GPIO_UartBufEnable(true);
    HAL_UART_TxByte('[');
    float lt = FlowCounter_LitrosTotal(&f->fd);
    if (lt < 0.0f) lt = 0.0f;
    HAL_UART_TxU32((uint32_t)lt);   /* entero truncado */
    HAL_UART_TxByte(']');
    HAL_UART_TxCRLF();
    HAL_GPIO_UartBufEnable(false);
}

void FSM_Init(Fsm *f, float k_fria_init, float k_cal_init)
{
    HAL_GPIO_Init();
    HAL_GPIO_UartBufInit();
    HAL_Timer0_Init();
    HAL_UART_Init(9600);

    /* Cargar K de EEPROM; si inválido, usar defaults e inicializar EEPROM */
    float kf = k_fria_init, kc = k_cal_init;
    if (!KStore_Load(&kf, &kc)) {
        (void)KStore_Save(kf, kc);  /* crea estructura válida por si no existía */
    }

    FlowCounter_Init(&f->fd, kf, kc);
    FlowCounter_AttachISRs();

    f->st = S_BOOT;
    f->vol_obj_l = VOL_OBJ_L;
    f->t_led = HAL_Millis();
    f->t_tx  = HAL_Millis();
    f->last_btn_cal   = HAL_GPIO_ReadBtnCal();
    f->last_mode_prog = HAL_GPIO_ReadBtnMode();

    HAL_GPIO_SetLedProg(false);
}

void FSM_Tick(Fsm *f)
{
    const uint32_t now = HAL_Millis();

    /* LED: normal = 1s, programación = 200ms */
    const uint16_t period = (f->st == S_NORMAL) ? 1000u : 200u;
    if ((now - f->t_led) >= period) { f->t_led = now; HAL_GPIO_ToggleLedProg(); }

    /* Entradas (activo en bajo) */
    bool mode_prog = HAL_GPIO_ReadBtnMode();
    bool btn_cal   = HAL_GPIO_ReadBtnCal();

    switch (f->st) {
    case S_BOOT:
        /* Selección de modo al arranque con MODO_PROG (PD7) */
        if (mode_prog) {
            FlowCounter_Reset(&f->fd);
            f->st = S_PROG_FRIA;
        } else {
            f->st = S_NORMAL;
        }
        break;

    case S_NORMAL:
        /* Telemetría periódica */
        if ((now - f->t_tx) >= 500u) { f->t_tx = now; tx_total(f); }
        /* Permitir entrar a programación si el usuario activa MODO_PROG */
        if (mode_prog && !f->last_mode_prog) {
            FlowCounter_Reset(&f->fd);
            f->st = S_PROG_FRIA;
        }
        break;

    case S_PROG_FRIA: {
        /* Usuario confirma al alcanzar 10 L reales: botón CAL (PD6) */
        if (edge_fall(!btn_cal, &f->last_btn_cal)) {
            const uint32_t p = FlowCounter_ReadPulsesFria();
            if (p > 0u) {
                f->fd.k_fria = (float)p / f->vol_obj_l; /* K = pulsos/L */
            }
            FlowCounter_Reset(&f->fd);
            f->st = S_PROG_CAL;
        }
        }
        break;

    case S_PROG_CAL: {
        if (edge_fall(!btn_cal, &f->last_btn_cal)) {
            const uint32_t p = FlowCounter_ReadPulsesCal();
            if (p > 0u) {
                f->fd.k_cal = (float)p / f->vol_obj_l;
            }
            f->st = S_SAVE;
        }
        }
        break;

    case S_SAVE:
        if (KStore_Save(f->fd.k_fria, f->fd.k_cal)) {
            /* OK → volver a normal */
            FlowCounter_Reset(&f->fd);
            f->st = S_NORMAL;
        } else {
            f->st = S_ERROR;
        }
        break;

    case S_ERROR:
        /* Parpadeo ya es rápido; podríamos añadir patrón especial */
        /* Salir de error si usuario desactiva modo prog */
        if (!mode_prog) f->st = S_NORMAL;
        break;
    }

    /* Guardar estado de botones/selector */
    f->last_mode_prog = mode_prog;
    /* last_btn_cal se actualiza dentro de edge_fall() */
}
