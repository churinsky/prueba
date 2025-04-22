#ifndef COM_PROTOCOL_H
#define COM_PROTOCOL_H

#include <stdint.h>

// mensajes y comandos
typedef enum
{
    // maquna a wifi
    MSG_ESTADO_CICLO = 1,  // como se encuentra
    MSG_INICIO_CICLO = 2,  // inicio ciclo
    MSG_FIN_CICLO = 3,     // fin
    MSG_ERROR_CICLO = 4,   // errores
    MSG_COMANDO_OK = 5,    // ok de ejecucion
                           // wifi a maquina
    CMD_ACTIVAR = 100,
    CMD_STATUS = 101,
    CMD_RESET = 102
} TipoMensaje;

// errores
typedef enum
{
    ERROR_PUERTA_ABIERTA = 1,
    ERROR_SOBRECALENTAMIENTO = 2,
    ERROR_SENSOR_FALLA = 3,
    ERROR_VALOR_INVALIDO = 4
} CodigoError;

// EStado del ciclo
typedef struct
{
    TipoMensaje tipo_msg;
    uint8_t id;           // id
    uint8_t etapa;        // etapa
    uint8_t paso;         // paso
    uint16_t tiempo;      // iiempo en segundo
    uint8_t temperatura; // temperatura
} EstadoCiclo;

// cilco iniciado
typedef struct
{
    TipoMensaje tipo_msg;
    uint8_t id;              // id
    uint8_t etapa_inicial;   // etapa inicial puede hacer solo cooldown
    uint8_t temperatura_set; // yemperatura objetivo
} InicioCiclo;

// ffin del ciclo
typedef struct
{
    TipoMensaje tipo_msg;
    uint8_t id; // id
} FinCiclo;

// error
typedef struct
{
    TipoMensaje tipo_msg;
    uint8_t id;         // id
    CodigoError codigo; // codifo
} ErrorCiclo;

typedef struct
{
    TipoMensaje tipo_msg;       // = 5
    uint8_t id;                 // id
    uint8_t comando_confirmado; // Código del comando que se ejecutó bien (ej: 100)
} ComandoOK;


// comando activacion
typedef struct
{
    TipoMensaje tipo_msg;
    uint8_t repeticiones; // duracion del ciclo
} CmdActivar;

// comando de estado
typedef struct
{
    TipoMensaje tipo_msg;
} CmdStatus;

// comando de reinicio
typedef struct
{
    TipoMensaje tipo_msg;
} CmdReset;

#endif
