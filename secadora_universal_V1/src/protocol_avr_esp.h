/*
 * protocol_avr_esp.h
 *
 * Created: 5/13/2025 4:55:57 PM
 *  Author: Erick Gonzalez
 */ 


#ifndef PROTOCOL_AVR_ESP_H_
#define PROTOCOL_AVR_ESP_H_

#define BYTE_DE_INICIO		0xAA
#define BYTE_DE_FIN			0x55
#define DUMMY_BYTE			0x00

#define	MSG_ESTADO_CICLO	1			// como se encuentra
#define	MSG_INICIO_CICLO	2			// inicio ciclo
#define	MSG_FIN_CICLO		3			// fin
#define	MSG_ERROR_CICLO		4			// errores
#define	MSG_COMANDO_OK		5			// ok de ejecucion
#define	MSG_IDS				6			// id
#define	CMD_ACTIVAR			100			
#define	CMD_STATUS			101			// como se encuentra
#define	CMD_SOLICITAR_IDS	102			




typedef struct 
{
	uint8_t tipo_msg;
	uint8_t id_1;
	uint8_t id_2;
} data_IDs;

typedef struct
{
	uint8_t tipo_msg;
	uint8_t id_1;         // id máquina 1
	uint8_t id_2;         // id máquina 2
	uint8_t repeticiones; // duracion del ciclo
} data_CmdActivar;

typedef struct 
{
	uint8_t tipo_msg;			 // = 0x05
	uint8_t id_1;               // id máquina 1
	uint8_t id_2;               // id máquina 2
	uint8_t comando_confirmado; // Código del comando que se ejecutó bien (ej: 100)
} data_ComandoOK;


typedef struct 
{
	uint8_t tipo_msg;
	uint8_t id_1; // id máquina 1
	uint8_t id_2; // id máquina 2
} data_CmdStatus;

typedef struct 
{
	uint8_t tipo_msg;
	uint8_t id_1; // id máquina 1
	uint8_t id_2; // id máquina 2
} data_FinCiclo;

typedef struct
{
	uint8_t tipo_msg;
	uint8_t id_1;            // id máquina 1
	uint8_t id_2;            // id máquina 2
	uint8_t etapa_inicial;   // etapa inicial puede hacer solo cooldown
	uint8_t temperatura_set; // temperatura objetivo
} data_InicioCiclo;

typedef struct 
{
	uint8_t tipo_msg;
	uint8_t id_1;			// id máquina 1
	uint8_t id_2;			// id máquina 2
	uint8_t etapa;			// etapa (solo para secadoras)
	uint8_t paso;			// paso (solo para lavadoras)
	uint8_t tiempo;			// tiempo restante para que termine el ciclo
	uint8_t temperatura;	// temperatura
} data_EstadoCiclo;


#endif /* PROTOCOL_AVR_ESP_H_ */