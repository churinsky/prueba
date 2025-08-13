/*
 * dryer.h
 *
 * Created: 4/30/2025 8:48:53 AM
 *  Author: Investigacion
 */ 


#ifndef DRYER_H_
#define DRYER_H_

#include <avr/io.h>
#include "port.h"
#include "tiempo.h"
#include "driver_Display14ssegment.h"


#define OPL_mode									0
#define KIOSKO_mode									1
#define MONEDEROS2_mode								2
#define MONEDEROS1_mode								3

/*etapa*/
#define NADA										0
#define CALENTANDO									1
#define ENFRIANDO									2
/**********/
#define MAX_ALLOWABLE_TEMPERATURE_TO_SHOW			99

#define CANASTA1									0
#define CANASTA2									1
#define ON_											1
#define OFF_										0
#define BORRAR										1
#define _1second									1000
#define _1minute									60000
#define minimun_TEMPERATURE							1
#define medium_TEMPERATURE							2
#define max_TEMPERATURE								3
/*****************DEFAULT VALUES************************/
#define Temp_Histeresis_H							2
#define Temp_Histeresis_L							5

#define TIME1										28
#define TEMPERATURE1								54
#define PULLDWN_TIME1								2

#define TIME2										28
#define TEMPERATURE2								71
#define PULLDWN_TIME2								2

#define TIME3										28
#define TEMPERATURE3								88
#define PULLDWN_TIME3								2

#define TOKEN_TO_TIME								5
/*******************************************************/
#define BTN_IZQ_P1_PRESS						(!((readReg_BTN_IZQ_P1) & (1 << pin_BTN_IZQ_P1)))	//conected to pull ups, ON = logic 1, Off = logic 0
#define BTN_CEN_P1_PRESS						(!((readReg_BTN_CEN_P1) & (1 << pin_BTN_CEN_P1)))	//conected to pull ups, ON = logic 1, Off = logic 0
#define BTN_DER_P1_PRESS						(!((readReg_BTN_DER_P1) & (1 << pin_BTN_DER_P1)))	//conected to pull ups, ON = logic 1, Off = logic 0

#define BTN_IZQ_P2_PRESS						(!((readReg_BTN_IZQ_P2) & (1 << pin_BTN_IZQ_P2)))	//conected to pull ups, ON = logic 1, Off = logic 0
#define BTN_CEN_P2_PRESS						(!((readReg_BTN_CEN_P2) & (1 << pin_BTN_CEN_P2)))	//conected to pull ups, ON = logic 1, Off = logic 0
#define BTN_DER_P2_PRESS						(!((readReg_BTN_DER_P2) & (1 << pin_BTN_DER_P2)))	//conected to pull ups, ON = logic 1, Off = logic 0

#define SWITCH_ON								1
#define SWITCH_OFF								0
#define DOOR1_ISCLOSE							(!((readReg_DOOR1) & (1 << pin_DOOR1)))				//1 = door close, 0 == door open
#define DOOR2_ISCLOSE							(!((readReg_DOOR2) & (1 << pin_DOOR2)))

#define MOTOR_DER1_ON							writeReg_CTRL_MOTORDER1 |= (1 << pin_CTRL_MOTORDER1)
#define MOTOR_DER1_OFF							writeReg_CTRL_MOTORDER1 &= ~(1 << pin_CTRL_MOTORDER1)

#define MOTOR_IZQ1_ON							writeReg_CTRL_MOTORIZQ1 |= (1 << pin_CTRL_MOTORIZQ1)
#define MOTOR_IZQ1_OFF							writeReg_CTRL_MOTORIZQ1 &= ~(1 << pin_CTRL_MOTORIZQ1)

#define IGNITOR1_ON								writeReg_CTRL_IGNITOR1 |= (1 << pin_CTRL_IGNITOR1)
#define IGNITOR1_OFF							writeReg_CTRL_IGNITOR1 &= ~(1 << pin_CTRL_IGNITOR1)

#define VENTILADOR1_ON							writeReg_CTRL_VENTILADOR1 |= (1 << pin_CTRL_VENTILADOR1)
#define VENTILADOR1_OFF							writeReg_CTRL_VENTILADOR1 &= ~(1 << pin_CTRL_VENTILADOR1)

#define MOTOR_DER2_ON							writeReg_CTRL_MOTORDER2 |= (1 << pin_CTRL_MOTORDER2)
#define MOTOR_DER2_OFF							writeReg_CTRL_MOTORDER2 &= ~(1 << pin_CTRL_MOTORDER2)

#define MOTOR_IZQ2_ON							writeReg_CTRL_MOTORIZQ2 |= (1 << pin_CTRL_MOTORIZQ2)
#define MOTOR_IZQ2_OFF							writeReg_CTRL_MOTORIZQ2 &= ~(1 << pin_CTRL_MOTORIZQ2)

#define IGNITOR2_ON								writeReg_CTRL_IGNITOR2 |= (1 << pin_CTRL_IGNITOR2)
#define IGNITOR2_OFF							writeReg_CTRL_IGNITOR2 &= ~(1 << pin_CTRL_IGNITOR2)

#define VENTILADOR2_ON							writeReg_CTRL_VENTILADOR2 |= (1 << pin_CTRL_VENTILADOR2)
#define VENTILADOR2_OFF							writeReg_CTRL_VENTILADOR2 &= ~(1 << pin_CTRL_VENTILADOR2)

#define THERMISTOR1_CH_ON						ADMUX &= 0b11100000
#define THERMISTOR2_CH_ON						ADMUX |= 0b00000001

//Disable UART interrup
#define DISABLE_RX_INTERRUP						UCSRB &= ~(1 << RXCIE);
//Disable INT1 interrup(coin acceptor Canasta2)
#define DISABLE_INT1_INTERRUP					GICR &= ~(1 << INT1);



#define START_ADC_CONVERSION						ADCSRA |= (1 << ADSC)
//#define	ADC_CONVERSION_IN_PROGRES					(ADCSRA & (1 << ADSC))						// 1 = conversion in progress, 0 = conversion finished
#define	ADC_CONVERSION_IN_PROGRES					!(ADCSRA & (1 << ADIF))							// 0 = conversion in progress, 1 = conversion finished
#define CLEAN_COMPLETE_ADC_CONVERSION_FLAG			ADCSRA |= (1 << ADIF);
#define iniFLAG_Addres									1
#define iniFLAG_Value									0xAA

#define NUMBER_OF_ADC_SAMPLES					25


typedef struct
{
	uint8_t motor_Der_state : 1;					//1 = on, 0 =off
	uint8_t motor_Izq_state : 1;					//1 = on, 0 =off
	uint8_t ignitor_state : 1;						//1 = on, 0 =off
	uint8_t ventilador_state :1;					//1 = on, 0 =off
	
}SETSTATE;

typedef struct
{
	uint8_t door_state :1;							//1 = door open, 0 = door close
	uint8_t btnIZQ_state :1;							//1 = boton PRESS, 0 = NOT press
	uint8_t btnCEN_state :1;							//1 = boton PRESS, 0 = NOT press
	uint8_t btnDER_state :1;							//1 = boton PRESS, 0 = NOT press
	
}GETSTATE;

typedef void(*FP_ctrl_outputs)(SETSTATE outputs, uint8_t canastaNumber);
typedef GETSTATE(*FP_read_Sesnors)(uint8_t canastaNumber);

typedef struct
{
	uint8_t cycle : 2;								//0 = No run, waiting to trigger event, 1 = minimun temperature boton izq, 2 = medium temperature boton central, 3 = high temperature boton derecha
	uint8_t Cyclefinished :1;						//flag set when Dry finish the cycle
	uint8_t state:4;								//used to know the current state in state machine
	uint8_t pullDown_Flag :1;						//Flag activeded when pulldown cycle is running
	uint8_t timer;									//timer name
	uint16_t counter;								//general purpose variable
	uint8_t time_to_reach;							// time in minutes selected according with the cycle, it doesnt change during the cycle in OPL or Kiosko mode
	uint8_t current_Time;							// transcured time in minutes
	uint8_t pullDown_Time;							//pull down time in minutres
	uint8_t	temperature_to_reach;					//Temperature tu reach, this value is getting from EEPROM, this value is configurable in progMode
	uint8_t	current_temperature;					//Temperature sensed by thermistor
	uint8_t histeresis_L;
	uint8_t histeresis_H;
	SETSTATE dryMachine_outputs_State;				
	GETSTATE dryMachine_sensorsState;
	uint8_t ID;
	uint8_t pending_to_activate_Flag :1;
	uint8_t processingMessageState :3;
	uint8_t messageReceived_Flag :1;				
	uint8_t coinAccepted_Flag :1;					//Token accepted = 1
	uint8_t AvailableCredit;						//this variable is used to know how many tokens should be processed 
	FP_ctrl_outputs DRY_ctrl_outputs;
	FP_read_Sesnors DRY_read_Sensors;
	
}DRYER;

DRYER dryer[NUMBER_OF_SCREENS];

void operationCycles(void);
void OPL(void);
void kiosko(void);
void coinAcceptors2(void);
void coinAcceptors1(void);
//void ctrl_outputs(SETSTATE outputs, uint8_t canastaNumber);
//GETSTATE read_inputs(uint8_t canastaNumber);
//void check_Time(uint8_t canastaNumber);
//void checkTemperature(uint8_t canastaNumber);
void dryer_Ini(void);
void process_Message(uint8_t *message);
uint8_t get_OpMode(void);
void check_TokenAccepted(uint8_t canastaNumber);
#endif /* DRYER_H_ */