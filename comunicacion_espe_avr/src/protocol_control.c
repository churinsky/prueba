#include "protocol_control.h"
#include "protocol.h"
#include "uart.h"


static uint8_t repeticiones_guardadas = 0;  // variable local

// para alrecepcion
typedef enum
{
    RX_STATE_WAIT_START = 0,
    RX_STATE_WAIT_LEN = 1,
    RX_STATE_RECEIVING = 2,
    RX_STATE_WAIT_END = 3
} RxState;

static RxState rx_state = RX_STATE_WAIT_START;
static uint8_t rx_len = 0;
static uint8_t rx_index = 0;
static uint8_t rx_buffer[MSG_MAX_LEN];

// las cosas que van a hacer los comandos
void Protocol_HandleCommand(uint8_t *data, uint8_t len)
{
    if (len < 1)
        return; // Mínimo debe venir el tipo_msg

    // nada mas un cast
    TipoMensaje tipo = (TipoMensaje)data[0];

    switch (tipo)
    {

    case CMD_ACTIVAR:
    {
        if (len == sizeof(CmdActivar))
        {
            CmdActivar *cmd = (CmdActivar *)data;

            // Aquí van las cosas para cmd->repeticiones
            repeticiones_guardadas = cmd->repeticiones;
            // Enviar OK de respuesta
            ComandoOK ok = {
                .tipo_msg = MSG_COMANDO_OK,
                .id = 1, // o el id de tu máquina
                .comando_confirmado = CMD_ACTIVAR};
            Protocol_SendOK(&ok);
        }
        break;
    }

    case CMD_STATUS:
    {
        extern EstadoCiclo maquina;
        Protocol_SendEstado(&maquina);
        break;
    }

    case CMD_RESET:
    {
        // falta poner algo para el reiciico
        ComandoOK ok = {
            .tipo_msg = MSG_COMANDO_OK,
            .id = 1,
            .comando_confirmado = CMD_RESET};
        Protocol_SendOK(&ok);
        break;
    }

    default:
        // Comando no reconocido, se podria meter un error y cosas
        break;
    }
}

// de lo que llega
void Protocol_ProcessByte(uint8_t byte)
{
    switch (rx_state)
    {

    case RX_STATE_WAIT_START:
        if (byte == MSG_START_BYTE)
        {
            rx_index = 0;
            rx_state = RX_STATE_WAIT_LEN;
        }
        break;

    case RX_STATE_WAIT_LEN:
        if (byte > 0 && byte <= MSG_MAX_LEN)
        {
            rx_len = byte;
            rx_index = 0;
            rx_state = RX_STATE_RECEIVING;
        }
        else
        {
            rx_state = RX_STATE_WAIT_START;
        }
        break;

    case RX_STATE_RECEIVING:
        rx_buffer[rx_index] = byte;
        rx_index++;
        if (rx_index >= rx_len)
        {
            rx_state = RX_STATE_WAIT_END;
        }
        break;

    case RX_STATE_WAIT_END:
        if (byte == MSG_END_BYTE)
        {
            Protocol_HandleCommand(rx_buffer, rx_len);
        }
        rx_state = RX_STATE_WAIT_START;
        break;
    }
}

// un reser por si se ocupa aun no hace nada
void Protocol_ResetReceiver(void)
{
    rx_state = RX_STATE_WAIT_START;
    rx_len = 0;
    rx_index = 0;
    for (uint8_t i = 0; i < MSG_MAX_LEN; i++)
    {
        rx_buffer[i] = 0;
    }
}



void Protocol_SendInicioCiclo(InicioCiclo *ptr)
{
    uart_send_message((uint8_t *)ptr, sizeof(InicioCiclo));
}

void Protocol_SendEstado(EstadoCiclo *ptr)
{
    uart_send_message((uint8_t *)ptr, sizeof(EstadoCiclo));
}

void Protocol_SendFinCiclo(FinCiclo *ptr)
{
    uart_send_message((uint8_t *)ptr, sizeof(FinCiclo));
}

void Protocol_SendError(ErrorCiclo *ptr)
{
    uart_send_message((uint8_t *)ptr, sizeof(ErrorCiclo));
}

void Protocol_SendOK(ComandoOK *ptr)
{
    uart_send_message((uint8_t *)ptr, sizeof(ComandoOK));
}

uint8_t Protocol_GetRepeticiones(void)
{
    return repeticiones_guardadas;
}