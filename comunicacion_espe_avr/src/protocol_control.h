#ifndef PROTOCOL_CONTROL_H
#define PROTOCOL_CONTROL_H

#include <stdint.h>
#include "protocol.h"

// Solo funciones para enviar
void Protocol_SendInicioCiclo(InicioCiclo *ptr);
void Protocol_SendEstado(EstadoCiclo *ptr);
void Protocol_SendFinCiclo(FinCiclo *ptr);
void Protocol_SendError(ErrorCiclo *ptr);
void Protocol_SendOK(ComandoOK *ptr);
uint8_t Protocol_GetRepeticiones(void);
void Protocol_ProcessByte(uint8_t byte);
#endif
