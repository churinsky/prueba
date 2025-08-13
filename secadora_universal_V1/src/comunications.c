/*
 * comunications.c
 *
 * Created: 5/13/2025 4:54:45 PM
 *  Author: Erick Gonzalez
 */ 

#include "comunications.h"
#include "protocol_avr_esp.h"


static  uint8_t rxBuffer[15];

void sendMessage_UART(uint8_t * message)
{
	uint8_t i = 0;
	/*while(message[i] != BYTE_DE_FIN)
	{
		if (READYtoTransmit)
		{
			UDR = message[i];
			i++;
		}
	} */
	
	for ( i = 0; i < (message[LENGHDATA_UART] + MANDATORYBYTES); i++ )
	{
		while (!READYtoTransmit)
		{
			asm("nop");
		}
			UDR = message[i];
	}
}

uint8_t* organize_Message_GetIDs(void)
{
	static uint8_t message[7];
	message[0] = BYTE_DE_INICIO;
	message[1] = sizeof(data_IDs);
	message[2] = CMD_SOLICITAR_IDS;
	message[3] = 0x00;
	message[4] = 0x00;
	message[5] = BYTE_DE_FIN;
	
	return message;
}

uint8_t* organize_Message_OK(uint8_t ID_number, uint8_t tipo_Msg)
{
	static uint8_t message[7];
	message[0] = BYTE_DE_INICIO;
	message[1] = sizeof(data_ComandoOK);
	message[2] = MSG_COMANDO_OK;
	message[3] = ID_number;
	message[4] = DUMMY_BYTE;
	message[5] = tipo_Msg;
	message[6] = BYTE_DE_FIN;
	
	return message;
}

uint8_t* organize_Message_FinCiclo(uint8_t ID_number)
{
	static uint8_t message[7];
	message[0] = BYTE_DE_INICIO;
	message[1] = sizeof(data_FinCiclo);
	message[2] = MSG_FIN_CICLO;
	message[3] = ID_number;
	message[4] = DUMMY_BYTE;
	message[5] = BYTE_DE_FIN;
	
	return message;
}

uint8_t* organize_Message_InicioCiclo(uint8_t ID_number, uint8_t temperature)
{
	static uint8_t message[8];
	message[0] = BYTE_DE_INICIO;
	message[1] = sizeof(data_InicioCiclo);
	message[2] = MSG_INICIO_CICLO;
	message[3] = ID_number;
	message[4] = DUMMY_BYTE;
	message[5] = DUMMY_BYTE;			//Etapa inicial, para secadoras mandar 0
	message[6] = temperature;
	message[7] = BYTE_DE_FIN;
	
	return message;
}
uint8_t* organize_Message_EstadoCiclo(uint8_t ID_number, uint8_t etapa, uint8_t paso, uint8_t time_to_finishCycle, uint8_t currentTemperature)
{
	static uint8_t message[10];
	message[0] = BYTE_DE_INICIO;
	message[1] = sizeof(data_EstadoCiclo);
	message[2] = MSG_ESTADO_CICLO;
	message[3] = ID_number;
	message[4] = DUMMY_BYTE;
	message[5] = etapa;			//etapa 0x00 = Nada, 0x01 = Calentando, 0x02 = Enfriando
	message[6] = paso;			//paso 0x00 = Nada, 0x01 = agitado ....... para secadoras mandar 0x00
	message[7] = time_to_finishCycle;
	message[8] = currentTemperature;
	message[9] = BYTE_DE_FIN;
	
	return message;
}

uint8_t ProcessByte_UART(uint8_t data)
{
	static uint8_t state = 0;
	static uint8_t	offsetData =0;
	switch (state)
	{
		//check if the first byte received is the "Bye de inicio", if not, ignore message
		case HEADERBYTE_UART:
			if (data == BYTE_DE_INICIO)
			{
				rxBuffer[HEADERBYTE_UART] = data;
				state++;
			}
			else
			{
				//Error, receive wrong data
				state = HEADERBYTE_UART;
			}
			break;
		case LENGHDATA_UART:
			rxBuffer[LENGHDATA_UART] = data;
			state++;
			break;
		
		case DATA_RECEIVED_UART:
			rxBuffer[DATA_RECEIVED_UART + offsetData] = data;
			offsetData++;
			if ( offsetData >= rxBuffer[LENGHDATA_UART]  )
			{
				//it have received the number of bytes according with LENGHDATA
				state++;
				
			}
			break;
			
		case FINAL_BYTE_UART:
			if (data == BYTE_DE_FIN)
			{
				//reception was completed successfully, we need to process message
				rxBuffer[DATA_RECEIVED_UART + offsetData] = data;
				offsetData = 0;
				state = HEADERBYTE_UART;
				return 1;
			}
			else
			{
				//Error,  we receive wrong data
				state = HEADERBYTE_UART;
				offsetData = 0;
			}
			break; 
			
		
		default:
			asm("nop");
			
			break;
	}
	return 0;
}
uint8_t* get_Messsage_UART(void)
{
	
	return rxBuffer;
}