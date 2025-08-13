/*
 * comunications.h
 *
 * Created: 5/13/2025 4:55:06 PM
 *  Author: Erick Gonzalez
 */ 
#include <avr/io.h>

#ifndef COMUNICATIONS_H_
#define COMUNICATIONS_H_

#define READYtoTransmit								(UCSRA & 1 << UDRE)								//Ready to transmit? 1 = ready, 0 = not Ready

#define HEADERBYTE_UART		0
#define LENGHDATA_UART		1
#define DATA_RECEIVED_UART	2
#define FINAL_BYTE_UART		3
#define MANDATORYBYTES		3						//mandatory bytes = HEADERBYTE_UART, LENGHDATA_UART and  FINAL BYTE

void sendMessage_UART(uint8_t * message);
uint8_t ProcessByte_UART(uint8_t data);
uint8_t* organize_Message_GetIDs(void);
uint8_t* get_Messsage_UART(void);
uint8_t* organize_Message_OK(uint8_t ID_number, uint8_t tipo_Msg);
uint8_t* organize_Message_FinCiclo(uint8_t ID_number);
uint8_t* organize_Message_InicioCiclo(uint8_t ID_number, uint8_t temperature);
uint8_t* organize_Message_EstadoCiclo(uint8_t ID_number, uint8_t etapa, uint8_t paso, uint8_t time_to_finishCycle, uint8_t currentTemperature);
#endif /* COMUNICATIONS_H_ */