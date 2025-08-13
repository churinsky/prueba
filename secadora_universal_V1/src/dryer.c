/*
 * dryer.c
 *
 * Created: 4/30/2025 8:48:31 AM
 *  Author: Investigacion
 */ 
#include "dryer.h"
#include <avr/pgmspace.h>
#include <avr/eeprom.h>
#include "math.h"
#include "comunications.h"
#include "protocol_avr_esp.h"
#include <util/delay.h>


#define E_TIME1			0
#define E_TEMP1			1
#define	E_PDWN_TIME1	2
#define	E_HISTERESIS_L1	3
#define	E_HISTERESIS_H1	4

#define E_TIME2			5
#define E_TEMP2			6
#define	E_PDWN_TIME2	7
#define	E_HISTERESIS_L2	8
#define	E_HISTERESIS_H2	9

#define E_TIME3			10
#define E_TEMP3			11
#define	E_PDWN_TIME3	12
#define	E_HISTERESIS_L3	13
#define	E_HISTERESIS_H3	14
#define	E_TXTO			15
#define	E_MODO			16
#define	E_EXIT			17
#define NUMBER_OF_PARAMETERS	18

#define MAX_TIME_VALUE	99
#define MIN_TIME_VALUE	1

#define MAX_TEMP_VALUE	90
#define MIN_TEMP_VALUE	5

#define MAX_PDWN_TIME_VALUE		10
#define MIN_PDWN_TIME_VALUE		1

#define MAX_HISTL_TEMP_VALUE	15
#define MIN_HISTL_TEMP_VALUE	1

#define MAX_HISTH_TEMP_VALUE	15
#define MIN_HISTH_TEMP_VALUE	1

#define MAX_TXTO_VALUE			25
#define MIN_TXTO_VALUE			1

#define MAX_MODO_VALUE			MONEDEROS1_mode
#define MIN_MODO_VALUE			OPL_mode


#define STEINHART_COEFICIENT_A	1.976148856e-03
#define STEINHART_COEFICIENT_B	0.3985279933e-04
#define STEINHART_COEFICIENT_C	7.623337487e-07
#define VoltageReference_mv		5000
#define voltagePerCount_mv		4.88
#define FIX_RESISTOR_R1_ohms	47000			

#define OPL_CYCLE				0
#define PRESS_BUTTON_TO_START	1
#define DOOR_MESSAGE			2
#define INI_CYCLE				3
#define FINISH_CYCLE			4
#define PRESS_TO_CONTINUE		5

#define KIOS_M					0
#define GET_IDS_M				1
#define EQUIPOS_MAUTRA_M		2

#define BLANK_M					0
#define DASH_SYMBOL_M			1

#define WAITING_TO_BE_ACTIVATED_STATE	0
#define CYCLE_IS_RUNNING_STATE			1
#define PULLDWN_STATE					2


const char OPL_messages[][MAX_LENGH_MESSAGE] =
{
	"OPL",
	"Presione un boton para iniciar",
	"Puerta",
	"iniciando ciclo",
	"Ciclo Terminado - Vuelva Pronto",
	"Presione para continuar"
};

const char KIOSKO_messages[][MAX_LENGH_MESSAGE] =
{
	"KIOS",
	"Obteniendo IDs",
	"Equipos Mautra"
};
const char coinAcceptors_messages[][MAX_LENGH_MESSAGE] =
{
	"    ",
	"----"
};

static uint8_t temporal_Credit = 0;

static void ctrl_outputs(SETSTATE outputs, uint8_t canastaNumber);
static GETSTATE read_inputs(uint8_t canastaNumber);
static void check_Time(uint8_t canastaNumber);
static void checkTemperature(uint8_t canastaNumber);
static void showTemperature(uint8_t canastaNumber);
static void update_eeprom_Parameters(void);
static uint8_t steinhart_Equation(uint16_t adc_Value);
static void clear_ReceiveMessageFlag(uint8_t canastaNumber);
static uint8_t get_CanastaNumber(uint8_t ID_number);
static void progMode(void);
static void check_for_showTemperature_request(uint8_t canasNumber);
static uint8_t tokens_to_Time(uint8_t credit);


uint8_t EEMEM dummyMemory;				//to avoid write in memory 0
uint8_t EEMEM iniFLAG_eeprom;

uint8_t EEMEM time1_eeprom;
uint8_t EEMEM temp1_eeprom;
uint8_t EEMEM pullDwn_time1_eeprom;
uint8_t EEMEM histeresisL1_eeprom;
uint8_t EEMEM histeresisH1_eeprom;

uint8_t EEMEM time2_eeprom;
uint8_t EEMEM temp2_eeprom;
uint8_t EEMEM pullDwn_time2_eeprom;
uint8_t EEMEM histeresisL2_eeprom;
uint8_t EEMEM histeresisH2_eeprom;

uint8_t EEMEM time3_eeprom;
uint8_t EEMEM temp3_eeprom;
uint8_t EEMEM pullDwn_time3_eeprom;
uint8_t EEMEM histeresisL3_eeprom;
uint8_t EEMEM histeresisH3_eeprom;

uint8_t EEMEM timeXtoken_eeprom;

uint8_t EEMEM opMode_eeprom;

volatile uint8_t time1;
volatile uint8_t temp1;
volatile uint8_t pullDwn_time1;
volatile uint8_t histeresisL1;
volatile uint8_t histeresisH1;

volatile uint8_t time2;
volatile uint8_t temp2;
volatile uint8_t pullDwn_time2;
volatile uint8_t histeresisL2;
volatile uint8_t histeresisH2;

volatile uint8_t time3;
volatile uint8_t temp3;
volatile uint8_t pullDwn_time3;
volatile uint8_t histeresisL3;
volatile uint8_t histeresisH3;
volatile uint8_t timeXtoken;
volatile uint8_t opMode;


/* brief set operation mode
 *
 * Function to get the current operation mode
 *  
 *	param: 
 *  return: uint8_t opMode /OPL_mode, Kiosko_mode, Monederos2_mode, Monederos1_mode
 */
uint8_t get_OpMode(void)
{
	return opMode;
}

/* brief Initialiaze variables used by dryer
 *
 * Function to initialize dryer
 *  
 *	param 
 *  return
 */
void dryer_Ini(void)
{
	char delay_Message[] = "----";
	char progMode_Message[] = "PROG";
	if ( (eeprom_read_byte(&iniFLAG_eeprom)) != iniFLAG_Value )
	{
		//system no initialized, load eeprom
		eeprom_update_byte( &iniFLAG_eeprom,  iniFLAG_Value);
		
		//Set parameters with default Values
		eeprom_update_byte( &time1_eeprom,  TIME1);
		eeprom_update_byte( &temp1_eeprom,  TEMPERATURE1);
		eeprom_update_byte( &pullDwn_time1_eeprom,  PULLDWN_TIME1);
		eeprom_update_byte( &histeresisL1_eeprom,  Temp_Histeresis_L);
		eeprom_update_byte( &histeresisH1_eeprom,  Temp_Histeresis_H);
		
		eeprom_update_byte( &time2_eeprom,  TIME2);
		eeprom_update_byte( &temp2_eeprom,  TEMPERATURE2);
		eeprom_update_byte( &pullDwn_time2_eeprom,  PULLDWN_TIME2);
		eeprom_update_byte( &histeresisL2_eeprom,  Temp_Histeresis_L);
		eeprom_update_byte( &histeresisH2_eeprom,  Temp_Histeresis_H);
		
		eeprom_update_byte( &time3_eeprom, TIME3);
		eeprom_update_byte( &temp3_eeprom, TEMPERATURE3);
		eeprom_update_byte( &pullDwn_time3_eeprom, PULLDWN_TIME3);
		eeprom_update_byte( &histeresisL3_eeprom, Temp_Histeresis_L);
		eeprom_update_byte( &histeresisH3_eeprom, Temp_Histeresis_H);
		
		eeprom_update_byte( &timeXtoken_eeprom, TOKEN_TO_TIME);
		eeprom_update_byte( &opMode_eeprom, OPL_mode);
		
	}
	
	//system has already initialized, read eeprom
	time1 = eeprom_read_byte(&time1_eeprom);
	temp1 = eeprom_read_byte(&temp1_eeprom);
	pullDwn_time1 = eeprom_read_byte(&pullDwn_time1_eeprom);
	histeresisL1 = eeprom_read_byte(&histeresisL1_eeprom);
	histeresisH1 = eeprom_read_byte(&histeresisH1_eeprom);
	
	time2 = eeprom_read_byte(&time2_eeprom);
	temp2 = eeprom_read_byte(&temp2_eeprom);
	pullDwn_time2 = eeprom_read_byte(&pullDwn_time2_eeprom);
	histeresisL2 = eeprom_read_byte(&histeresisL2_eeprom);
	histeresisH2 = eeprom_read_byte(&histeresisH2_eeprom);
	
	time3 = eeprom_read_byte(&time3_eeprom);
	temp3 = eeprom_read_byte(&temp3_eeprom);
	pullDwn_time3 = eeprom_read_byte(&pullDwn_time3_eeprom);
	histeresisL3 = eeprom_read_byte(&histeresisL3_eeprom);
	histeresisH3 = eeprom_read_byte(&histeresisH3_eeprom);
	
	timeXtoken = eeprom_read_byte(&timeXtoken_eeprom);
	opMode = eeprom_read_byte(&opMode_eeprom);
	
	
	//Initialize variables	
	dryer[CANASTA1].cycle =0;
	dryer[CANASTA1].Cyclefinished = OFF_;
	dryer[CANASTA1].state = 0;
	dryer[CANASTA1].pullDown_Flag = OFF_;
	dryer[CANASTA1].timer = new_VirtualTimer(0xFF, _1minute);;
	dryer[CANASTA1].counter = 0;
	dryer[CANASTA1].time_to_reach = 0;
	dryer[CANASTA1].current_Time = 0;
	dryer[CANASTA1].pullDown_Time = 0;
	dryer[CANASTA1].dryMachine_outputs_State.ignitor_state = OFF_;
	dryer[CANASTA1].dryMachine_outputs_State.motor_Der_state = OFF_;
	dryer[CANASTA1].dryMachine_outputs_State.motor_Izq_state= OFF_;
	dryer[CANASTA1].dryMachine_outputs_State.ventilador_state= OFF_;
	dryer[CANASTA1].temperature_to_reach = 0;
	dryer[CANASTA1].current_temperature = 0;
	dryer[CANASTA1].DRY_ctrl_outputs =ctrl_outputs;
	dryer[CANASTA1].DRY_read_Sensors =read_inputs;
	dryer[CANASTA1].DRY_ctrl_outputs(dryer[CANASTA1].dryMachine_outputs_State, CANASTA1);			//turn off all outputs
	dryer[CANASTA1].ID = 0;
	dryer[CANASTA1].messageReceived_Flag = 0;
	dryer[CANASTA1].pending_to_activate_Flag = 0;
	dryer[CANASTA1].processingMessageState = 0;
	dryer[CANASTA1].coinAccepted_Flag = 0;
	dryer[CANASTA1].AvailableCredit = 0;
	
	dryer[CANASTA2].cycle =0;
	dryer[CANASTA2].Cyclefinished = OFF_;
	dryer[CANASTA2].state = 0;
	dryer[CANASTA2].pullDown_Flag = OFF_;
	dryer[CANASTA2].timer =  new_VirtualTimer(0xFF, _1minute);
	dryer[CANASTA2].counter = 0;
	dryer[CANASTA2].time_to_reach = 0;
	dryer[CANASTA2].current_Time = 0;
	dryer[CANASTA2].pullDown_Time = 0;
	dryer[CANASTA2].dryMachine_outputs_State.ignitor_state = OFF_;
	dryer[CANASTA2].dryMachine_outputs_State.motor_Der_state = OFF_;
	dryer[CANASTA2].dryMachine_outputs_State.motor_Izq_state= OFF_;
	dryer[CANASTA2].dryMachine_outputs_State.ventilador_state= OFF_;
	dryer[CANASTA2].temperature_to_reach = 0;
	dryer[CANASTA2].current_temperature = 0;
	dryer[CANASTA2].DRY_ctrl_outputs =ctrl_outputs;
	dryer[CANASTA2].DRY_read_Sensors =read_inputs;
	dryer[CANASTA2].DRY_ctrl_outputs(dryer[CANASTA2].dryMachine_outputs_State, CANASTA2);			//turn off all outputs
	dryer[CANASTA2].ID = 0;
	dryer[CANASTA2].messageReceived_Flag = 0;
	dryer[CANASTA2].pending_to_activate_Flag = 0;
	dryer[CANASTA2].processingMessageState = 0;
	dryer[CANASTA2].coinAccepted_Flag = 0;
	dryer[CANASTA2].AvailableCredit = 0;
	
	
	
	string_to_Screen(delay_Message,NO_ANMIMATION,CANASTA1);
	string_to_Screen(delay_Message,NO_ANMIMATION,CANASTA2);
	
	
	tiempo_Retardo(2500);
	
	dryer[CANASTA1].dryMachine_sensorsState = dryer[CANASTA1].DRY_read_Sensors(CANASTA1);
	dryer[CANASTA2].dryMachine_sensorsState = dryer[CANASTA2].DRY_read_Sensors(CANASTA2);

	if (dryer[CANASTA1].dryMachine_sensorsState.btnIZQ_state && dryer[CANASTA2].dryMachine_sensorsState.btnDER_state)
	{
		//prog mode
		string_to_Screen(progMode_Message,BLINK,CANASTA1);
		tiempo_Retardo(5000);
		progMode();
	}

}

/* brief Re-configured parameters
 *
 * Function to change the value of some parameters
 *  
 *	param 
 *  return
 */
static void progMode(void)
{	
	uint8_t exit = 0;
	uint8_t menu_State = 0;
	int8_t index = 0;
	int8_t max_Value = 0;
	int8_t min_Value = 0;
	char parameters_messages[][MAX_LENGH_NO_ANIMATION_MESSAGE] =
	{
		"tie1",
		"tem1",
		"PDT1",
		"HIL1",
		"HIH1",
		"tie2",
		"tem2",
		"PDT2",
		"HIL2",
		"HIH2",
		"tie3",
		"tem3",
		"PDT3",
		"HIL3",
		"HIH3",
		"TXTO",
		"OPMO",
		"Exit"
	};
	char value_Parameters[]=
	{
		0xFF, 0xFF, 0xFF, 0xFF
	};
	
	volatile uint8_t * parameters[NUMBER_OF_PARAMETERS];
	parameters[E_TIME1] = &time1;
	parameters[E_TEMP1] = &temp1;
	parameters[E_PDWN_TIME1] = &pullDwn_time1;
	parameters[E_HISTERESIS_L1] = &histeresisL1;
	parameters[E_HISTERESIS_H1] = &histeresisH1;
	
	parameters[E_TIME2] = &time2;
	parameters[E_TEMP2] = &temp2;
	parameters[E_PDWN_TIME2] = &pullDwn_time2;
	parameters[E_HISTERESIS_L2] = &histeresisL2;
	parameters[E_HISTERESIS_H2] = &histeresisH2;
	
	parameters[E_TIME3] = &time3;
	parameters[E_TEMP3] = &temp3;
	parameters[E_PDWN_TIME3] = &pullDwn_time3;
	parameters[E_HISTERESIS_L3] = &histeresisL3;
	parameters[E_HISTERESIS_H3] = &histeresisH3;
	parameters[E_TXTO] = &timeXtoken;
	parameters[E_MODO] = &opMode;
	parameters[E_EXIT] = &exit;
	
	while (!exit)
	{
		switch(menu_State)
		{		
			//Display parameters to user
			case 0:
			string_to_Screen(parameters_messages[index],NO_ANMIMATION,CANASTA1);
			dryer[CANASTA1].dryMachine_sensorsState = dryer[CANASTA1].DRY_read_Sensors(CANASTA1);
			if (dryer[CANASTA1].dryMachine_sensorsState.btnIZQ_state ||  dryer[CANASTA1].dryMachine_sensorsState.btnCEN_state || dryer[CANASTA1].dryMachine_sensorsState.btnDER_state)
			{
				tiempo_Retardo(75);
				if (dryer[CANASTA1].dryMachine_sensorsState.btnIZQ_state)
				{
					index++;
					if (index > (sizeof(parameters) / sizeof(parameters[0]) - 1) )
					{
						index = 0;
					}
				}
				else if (dryer[CANASTA1].dryMachine_sensorsState.btnCEN_state)
				{
					index--;
					if (index < 0  )
					{
						index = (sizeof(parameters) / sizeof(parameters[0]) - 1);
					}
				}
				else if (dryer[CANASTA1].dryMachine_sensorsState.btnDER_state)
				{
					menu_State++;
				}
				
				while (dryer[CANASTA1].dryMachine_sensorsState.btnIZQ_state || dryer[CANASTA1].dryMachine_sensorsState.btnCEN_state || dryer[CANASTA1].dryMachine_sensorsState.btnDER_state)
				{
					dryer[CANASTA1].dryMachine_sensorsState = dryer[CANASTA1].DRY_read_Sensors(CANASTA1);
					tiempo_Retardo(50);
				}
			}
			break;
			
			//Display the value of parameter selected in case 0
			case 1:
			
				if (index == E_TIME1 || index == E_TIME2 || index == E_TIME3)
				{
					max_Value = MAX_TIME_VALUE;
					min_Value = MIN_TIME_VALUE;
					//time
					value_Parameters[0] = ( (*(parameters[index])) /10 ) + 0x30;
					value_Parameters[1] = ( (*(parameters[index])) %10 ) + 0x30;
					value_Parameters[2] = 'M';
					value_Parameters[3] = 'N';
					
				}
				if ( index == E_TEMP1 || index == E_TEMP2 || index == E_TEMP3 )
				{
					max_Value = MAX_TEMP_VALUE;
					min_Value = MIN_TEMP_VALUE;
					//temperature
					value_Parameters[0] = ( (*(parameters[index])) /10 ) + 0x30;
					value_Parameters[1] = ( (*(parameters[index])) %10 ) + 0x30;
					value_Parameters[2] = ';';	//degree symbol
					value_Parameters[3] = 'C';
					
				}
				if ( index == E_PDWN_TIME1 || index == E_PDWN_TIME2 || index == E_PDWN_TIME3 )
				{
					max_Value = MAX_PDWN_TIME_VALUE;
					min_Value = MIN_PDWN_TIME_VALUE;
					//pull down time
					value_Parameters[0] = ( (*(parameters[index])) /10 ) + 0x30;
					value_Parameters[1] = ( (*(parameters[index])) %10 ) + 0x30;
					value_Parameters[2] = 'M';
					value_Parameters[3] = 'N';
					
				}
				if ( index == E_HISTERESIS_L1 || index == E_HISTERESIS_L2 || index == E_HISTERESIS_L3 )
				{
					max_Value = MAX_HISTL_TEMP_VALUE;
					min_Value = MIN_HISTL_TEMP_VALUE;
					//Histeresis Low
					value_Parameters[0] = ( (*(parameters[index])) /10 ) + 0x30;
					value_Parameters[1] = ( (*(parameters[index])) %10 ) + 0x30;
					value_Parameters[2] = ';';	//degree symbol
					value_Parameters[3] = 'C';
					
				}
				if ( index == E_HISTERESIS_H1 || index == E_HISTERESIS_H2 || index == E_HISTERESIS_H3 )
				{
					max_Value = MAX_HISTH_TEMP_VALUE;
					min_Value = MIN_HISTH_TEMP_VALUE;
					//Histeresis High
					value_Parameters[0] = ( (*(parameters[index])) /10 ) + 0x30;
					value_Parameters[1] = ( (*(parameters[index])) %10 ) + 0x30;
					value_Parameters[2] = ';';	//degree symbol
					value_Parameters[3] = 'C';
					
				}
				if ( index == E_TXTO)
				{
					max_Value = MAX_TXTO_VALUE;
					min_Value = MIN_TXTO_VALUE;
					//Histeresis High
					value_Parameters[0] = ( (*(parameters[index])) /10 ) + 0x30;
					value_Parameters[1] = ( (*(parameters[index])) %10 ) + 0x30;
					value_Parameters[2] = 'M';
					value_Parameters[3] = 'I';
					
				}
				
				if ( index == E_MODO)
				{
					max_Value = MAX_MODO_VALUE;
					min_Value = MIN_MODO_VALUE;
					//Operation Modes, 0 == OPL, 1 == KIOSKO, 2 == 2 coin acceptors, 3 == 1 coin acceptors 
					value_Parameters[0] = 'M';
					value_Parameters[1] = 'O';
					value_Parameters[2] = 'D';
					value_Parameters[3] = ( (*(parameters[index])) %10 ) + 0x30;
					
				}
				if ( index == E_EXIT )
				{
					
					value_Parameters[0] = 'S';
					value_Parameters[1] = 'A';
					value_Parameters[2] = 'V';
					value_Parameters[3] = 'E';
					menu_State++;
				}
				
				dryer[CANASTA1].dryMachine_sensorsState = dryer[CANASTA1].DRY_read_Sensors(CANASTA1);
				if (dryer[CANASTA1].dryMachine_sensorsState.btnIZQ_state ||  dryer[CANASTA1].dryMachine_sensorsState.btnCEN_state || dryer[CANASTA1].dryMachine_sensorsState.btnDER_state)
				{
					tiempo_Retardo(75);
					if (dryer[CANASTA1].dryMachine_sensorsState.btnIZQ_state)
					{
						*parameters[index] += 1;
						if ( *parameters[index] > max_Value)
						{
							*parameters[index] = min_Value;
						}
						
					}
					else if (dryer[CANASTA1].dryMachine_sensorsState.btnCEN_state)
					{
						*parameters[index] -= 1;
						if ( *parameters[index] < min_Value)
						{
							*parameters[index] = max_Value;
						}
					}
					else if (dryer[CANASTA1].dryMachine_sensorsState.btnDER_state)
					{
						
						menu_State--;
					}
					
					while (dryer[CANASTA1].dryMachine_sensorsState.btnIZQ_state || dryer[CANASTA1].dryMachine_sensorsState.btnCEN_state || dryer[CANASTA1].dryMachine_sensorsState.btnDER_state)
					{
						dryer[CANASTA1].dryMachine_sensorsState = dryer[CANASTA1].DRY_read_Sensors(CANASTA1);
						tiempo_Retardo(50);
					}
				}
				string_to_Screen(value_Parameters,NO_ANMIMATION,CANASTA1);
				
			break;
			
			default:
				update_eeprom_Parameters();
				string_to_Screen(value_Parameters,BLINK,CANASTA1);
				tiempo_Retardo(5000);
				//Exit = 1
				*parameters[index] = 1;
				asm("nop");
				
			break;
			
		}
	}

	
}

/* brief Re-configured parameters
 *
 * Update eeprom parameters setting in progMode()
 *  
 *	param 
 *  return
 */
void update_eeprom_Parameters(void)
{
	eeprom_update_byte( &time1_eeprom, time1);
	eeprom_update_byte( &temp1_eeprom, temp1);
	eeprom_update_byte( &pullDwn_time1_eeprom, pullDwn_time1);
	eeprom_update_byte( &histeresisL1_eeprom, histeresisL1);
	eeprom_update_byte( &histeresisH1_eeprom, histeresisH1);
	
	eeprom_update_byte( &time2_eeprom, time2);
	eeprom_update_byte( &temp2_eeprom, temp2);
	eeprom_update_byte( &pullDwn_time2_eeprom, pullDwn_time2);
	eeprom_update_byte( &histeresisL2_eeprom, histeresisL2);
	eeprom_update_byte( &histeresisH2_eeprom, histeresisH2);
	
	eeprom_update_byte( &time3_eeprom, time3);
	eeprom_update_byte( &temp3_eeprom, temp3);
	eeprom_update_byte( &pullDwn_time3_eeprom, pullDwn_time3);
	eeprom_update_byte( &histeresisL3_eeprom, histeresisL3);
	eeprom_update_byte( &histeresisH3_eeprom, histeresisH3);
	eeprom_update_byte( &timeXtoken_eeprom, timeXtoken);
	eeprom_update_byte( &opMode_eeprom, opMode);
	
}

/* brief Handled the logic in operation cycles
 *
 * This function is in charge to ensure the cycles fulfill correctly their functionality
 *  
 *	param 
 *  return
 */
void operationCycles(void)
{
	static uint8_t canastaNumber = CANASTA1;
	uint8_t unidades,decenas,availableTime;
	char availableTime_M[] = {'t','-',0xFF,0xFF};
	//Read inputs
	dryer[canastaNumber].dryMachine_sensorsState = dryer[canastaNumber].DRY_read_Sensors(canastaNumber);
	//if door is closed( door_state = 1), the system is ready to start a cycle
	if (dryer[canastaNumber].dryMachine_sensorsState.door_state)
	{
		switch (dryer[canastaNumber].state)
		{	
			case WAITING_TO_BE_ACTIVATED_STATE:
				if ( !dryer[canastaNumber].cycle )
				{
					if ( opMode == OPL_mode || ( dryer[canastaNumber].AvailableCredit != 0 ) )
					{
						if (opMode == OPL_mode)
						{
							//door close, send message to press button and start cycle,
							string_to_Screen(OPL_messages[PRESS_BUTTON_TO_START],RIGHT_TO_LEFT,canastaNumber);
						}
						else
						{
							//display the time buying by user in Monederos Mode
							time1 = time2 = time3 = dryer[canastaNumber].time_to_reach;
							availableTime = dryer[canastaNumber].time_to_reach - dryer[canastaNumber].current_Time;
							unidades = availableTime % 10;
							decenas = availableTime / 10;
							availableTime_M[2] = decenas + 0x30;
							availableTime_M[3] = unidades + 0x30;
							string_to_Screen(availableTime_M,NO_ANMIMATION,canastaNumber);
						}
						
						if (dryer[canastaNumber].dryMachine_sensorsState.btnIZQ_state ||  dryer[canastaNumber].dryMachine_sensorsState.btnCEN_state || dryer[canastaNumber].dryMachine_sensorsState.btnDER_state)
						{
							//debounce
							tiempo_Retardo(75);
							dryer[canastaNumber].dryMachine_sensorsState = dryer[canastaNumber].DRY_read_Sensors(canastaNumber);
							if (dryer[canastaNumber].dryMachine_sensorsState.btnIZQ_state ||  dryer[canastaNumber].dryMachine_sensorsState.btnCEN_state || dryer[canastaNumber].dryMachine_sensorsState.btnDER_state)
							{
								//Se presiono un boton, iniciar ciclo acorde del boton presionado
								dryer[canastaNumber].state++;
								if (dryer[canastaNumber].dryMachine_sensorsState.btnIZQ_state)
								{
									dryer[canastaNumber].cycle = minimun_TEMPERATURE;
									dryer[canastaNumber].time_to_reach = time1;
									dryer[canastaNumber].temperature_to_reach = temp1;
									dryer[canastaNumber].pullDown_Time = pullDwn_time1;
									dryer[canastaNumber].histeresis_L = histeresisL1;
									dryer[canastaNumber].histeresis_H = histeresisH1;
									ctrl_Leds(0xFF,OFF,canastaNumber);
									ctrl_Leds(LED_IZQ,ON,canastaNumber);
									
								}
								else if (dryer[canastaNumber].dryMachine_sensorsState.btnCEN_state)
								{
									dryer[canastaNumber].cycle = medium_TEMPERATURE;
									dryer[canastaNumber].time_to_reach = time2;
									dryer[canastaNumber].temperature_to_reach = temp2;
									dryer[canastaNumber].pullDown_Time = pullDwn_time2;
									dryer[canastaNumber].histeresis_L = histeresisL2;
									dryer[canastaNumber].histeresis_H = histeresisH2;
									ctrl_Leds(0xFF,OFF,canastaNumber);
									ctrl_Leds(LED_CEN,ON,canastaNumber);
									
								}
								else if (dryer[canastaNumber].dryMachine_sensorsState.btnDER_state)
								{
									dryer[canastaNumber].cycle = max_TEMPERATURE;
									dryer[canastaNumber].time_to_reach = time3;
									dryer[canastaNumber].temperature_to_reach = temp3;
									dryer[canastaNumber].pullDown_Time = pullDwn_time3;
									dryer[canastaNumber].histeresis_L = histeresisL3;
									dryer[canastaNumber].histeresis_H = histeresisH3;
									ctrl_Leds(0xFF,OFF,canastaNumber);
									ctrl_Leds(LED_DER,ON,canastaNumber);
								}
								else
								{
									asm("nop");
								}
								dryer[canastaNumber].AvailableCredit = 0 ;
							}
							else
							{
								asm("nop");
							}
						}
					}
					/****************op_Mode KIOSKO******************/
					else if (opMode == KIOSKO_mode)
					{
						string_to_Screen(KIOSKO_messages[EQUIPOS_MAUTRA_M],RIGHT_TO_LEFT,canastaNumber);
						if ( dryer[canastaNumber].pending_to_activate_Flag)
						{
							//Always activate machine with max_Temperature cycle
							//time to reach is setting in process_Message function
							dryer[canastaNumber].cycle = max_TEMPERATURE;
							dryer[canastaNumber].temperature_to_reach = temp3;
							dryer[canastaNumber].pullDown_Time = pullDwn_time3;
							dryer[canastaNumber].histeresis_L = histeresisL3;
							dryer[canastaNumber].histeresis_H = histeresisH3;
							dryer[canastaNumber].state++;
							dryer[canastaNumber].pending_to_activate_Flag = 0;
							//send message inicio de ciclo
							sendMessage_UART(organize_Message_InicioCiclo( dryer[canastaNumber].ID, dryer[canastaNumber].temperature_to_reach) );
						}
					}
					/**************** op_Mode Monederos2_mode ******************/
					else if(opMode == MONEDEROS2_mode )
					{
						string_to_Screen(KIOSKO_messages[EQUIPOS_MAUTRA_M],RIGHT_TO_LEFT,canastaNumber);
					}
					/**************** Monederos1_mode ******************/
					else if( opMode == MONEDEROS1_mode )
					{
						if (temporal_Credit == 0)
						{
							string_to_Screen(KIOSKO_messages[EQUIPOS_MAUTRA_M],RIGHT_TO_LEFT,canastaNumber);
						}
						
					}
					else
					{
						asm("nop");
					}
				}
				else
				{
					//the cycle was interrupted due to an open door event, It is necesary start with the time and temperature previously selected
					if (temporal_Credit == 0)
					{
						string_to_Screen(OPL_messages[PRESS_TO_CONTINUE],RIGHT_TO_LEFT,canastaNumber);
					}
					
					if (dryer[canastaNumber].dryMachine_sensorsState.btnIZQ_state ||  dryer[canastaNumber].dryMachine_sensorsState.btnCEN_state || dryer[canastaNumber].dryMachine_sensorsState.btnDER_state)
					{
						//debounce
						tiempo_Retardo(75);
						dryer[canastaNumber].dryMachine_sensorsState = dryer[canastaNumber].DRY_read_Sensors(canastaNumber);
						if (dryer[canastaNumber].dryMachine_sensorsState.btnIZQ_state ||  dryer[canastaNumber].dryMachine_sensorsState.btnCEN_state || dryer[canastaNumber].dryMachine_sensorsState.btnDER_state)
						{
							dryer[canastaNumber].state++;
						}
					}
				}
				
				
				break;
				
			//Display time and temperature selected, t1** t2** t3**
			//** It is the time to finish cycle.
			//Turn on motor and ignitor. Ignitor just in case that current temperature < temperature to reach
			case CYCLE_IS_RUNNING_STATE:
				if (dryer[canastaNumber].dryMachine_sensorsState.btnIZQ_state)
				{
					dryer[canastaNumber].cycle = minimun_TEMPERATURE;
					dryer[canastaNumber].temperature_to_reach = temp1;
					dryer[canastaNumber].histeresis_L = histeresisL1;
					dryer[canastaNumber].histeresis_H = histeresisH1;
					ctrl_Leds(0xFF,OFF,canastaNumber);
					ctrl_Leds(LED_IZQ,ON,canastaNumber);
				}
				else if (dryer[canastaNumber].dryMachine_sensorsState.btnCEN_state)
				{
					dryer[canastaNumber].cycle = medium_TEMPERATURE;
					dryer[canastaNumber].temperature_to_reach = temp2;
					dryer[canastaNumber].histeresis_L = histeresisL2;
					dryer[canastaNumber].histeresis_H = histeresisH2;
					ctrl_Leds(0xFF,OFF,canastaNumber);
					ctrl_Leds(LED_CEN,ON,canastaNumber);
				}
				else if (dryer[canastaNumber].dryMachine_sensorsState.btnDER_state)
				{
					dryer[canastaNumber].cycle =max_TEMPERATURE;
					dryer[canastaNumber].temperature_to_reach = temp3;
					dryer[canastaNumber].histeresis_L = histeresisL3;
					dryer[canastaNumber].histeresis_H = histeresisH3;
					ctrl_Leds(0xFF,OFF,canastaNumber);
					ctrl_Leds(LED_DER,ON,canastaNumber);
				}
				
				check_for_showTemperature_request(canastaNumber);
				dryer[canastaNumber].dryMachine_outputs_State.motor_Der_state = ON_;
				checkTemperature(canastaNumber);
				dryer[canastaNumber].DRY_ctrl_outputs(dryer[canastaNumber].dryMachine_outputs_State,canastaNumber);
				if (dryer[canastaNumber].pullDown_Flag)
				{
					dryer[canastaNumber].state++;
				}
				
				break;
				
			//pulldown
			case PULLDWN_STATE:
				if (dryer[canastaNumber].pullDown_Flag)
				{
					if (dryer[canastaNumber].Cyclefinished)
					{
						//turn off all outputs
						dryer[canastaNumber].dryMachine_outputs_State.motor_Der_state = OFF_;
						dryer[canastaNumber].dryMachine_outputs_State.ignitor_state = OFF_;
						dryer[canastaNumber].dryMachine_outputs_State.motor_Izq_state = OFF_;
						dryer[canastaNumber].dryMachine_outputs_State.ventilador_state = OFF_;
						//move to next state
						dryer[canastaNumber].state++;
						dryer[canastaNumber].DRY_ctrl_outputs(dryer[canastaNumber].dryMachine_outputs_State,canastaNumber);
					}
					else
					{
						dryer[canastaNumber].dryMachine_outputs_State.motor_Der_state = ON_;
						dryer[canastaNumber].DRY_ctrl_outputs(dryer[canastaNumber].dryMachine_outputs_State,canastaNumber);
						
					}
					check_for_showTemperature_request(canastaNumber);
					checkTemperature(canastaNumber);
				}
				else
				{
					dryer[canastaNumber].state--;
				}
				
				break;	
				
			default:
				//clean flags and variables related with operation cycle
				dryer[canastaNumber].cycle = 0;
				dryer[canastaNumber].Cyclefinished = OFF_;
				dryer[canastaNumber].pullDown_Flag = OFF_;
				dryer[canastaNumber].current_Time = 0;
				ctrl_Leds(0xFF,OFF,canastaNumber);
				//display message "Ciclo terminado gracias por su preferencia"
				string_to_Screen(OPL_messages[FINISH_CYCLE],RIGHT_TO_LEFT,canastaNumber);
				tiempo_Retardo(100);
				if (dryer[canastaNumber].counter == 100)
				{
					dryer[canastaNumber].state = WAITING_TO_BE_ACTIVATED_STATE;
					dryer[canastaNumber].counter =0;
					
					if (opMode == KIOSKO_mode)
					{
						//Send Fin de ciclo message
						sendMessage_UART(organize_Message_FinCiclo( dryer[canastaNumber].ID ) );
					}
					
				}
				else
				{
					dryer[canastaNumber].counter++;
				}
				
				break;
		}
		
	}
	else
	{
		//Door open, send message to close the door and turn off motor and ignitor
		string_to_Screen(OPL_messages[DOOR_MESSAGE],RIGHT_TO_LEFT,canastaNumber);
		//turn off ignitor
		//turn off motor
		dryer[canastaNumber].dryMachine_outputs_State.ignitor_state = OFF_;
		dryer[canastaNumber].dryMachine_outputs_State.motor_Der_state = OFF_;
		dryer[canastaNumber].dryMachine_outputs_State.motor_Izq_state = OFF_;
		dryer[canastaNumber].dryMachine_outputs_State.ventilador_state = OFF_;
		dryer[canastaNumber].state = 0;
		dryer[canastaNumber].DRY_ctrl_outputs(dryer[canastaNumber].dryMachine_outputs_State, canastaNumber);
	}
	/*if ( (dryer[canastaNumber].cycle) && ( dryer[canastaNumber].state != 0 ) )
	{
		check_Time(canastaNumber);
	}*/
	check_Time(canastaNumber);
	
	if (opMode == KIOSKO_mode)
	{
		if (dryer[canastaNumber].messageReceived_Flag)
		{
			process_Message(get_Messsage_UART());
		}
		clear_ReceiveMessageFlag(0xFF);
	}
	
	if(opMode == MONEDEROS2_mode)
	{
		if (dryer[canastaNumber].coinAccepted_Flag)
		{
			//clean flag
			//increment credit
			//transform tokens to time
			dryer[canastaNumber].coinAccepted_Flag = 0;
			dryer[canastaNumber].AvailableCredit++;
			//time3 = tokens_to_Time(dryer[canastaNumber].AvailableCredit);
			if (dryer[canastaNumber].cycle)
			{	
				//dryer was already activated, increase time
				dryer[canastaNumber].time_to_reach += tokens_to_Time(dryer[canastaNumber].AvailableCredit);
				dryer[canastaNumber].AvailableCredit = 0;	
			}
			else
			{
				//dryer has not be activated, 
				dryer[canastaNumber].time_to_reach = tokens_to_Time(dryer[canastaNumber].AvailableCredit);
			}
			//limit time_to_reach value because it is the maximum value that can be displayed in two digits(2 displays)
			if (dryer[canastaNumber].time_to_reach > 99)
			{
				dryer[canastaNumber].time_to_reach = 99;
			}
		}
		
	}
	
	canastaNumber ^= 1;
}

static uint8_t tokens_to_Time(uint8_t credit)
{
	return (credit * timeXtoken);
}
void OPL(void)
{
	operationCycles();
}

void kiosko(void)
{
	//Get IDs	
	while (dryer[CANASTA1].ID == 0 || dryer[CANASTA2].ID == 0)
	{
		tiempo_Retardo(1500);
		string_to_Screen(KIOSKO_messages[1],RIGHT_TO_LEFT,SCREEN1);
		sendMessage_UART(organize_Message_GetIDs());
		if (dryer[CANASTA1].messageReceived_Flag == 1 || dryer[CANASTA2].messageReceived_Flag == 1)
		{
			process_Message(get_Messsage_UART());
		}
		clear_ReceiveMessageFlag(0xFF);
		asm("nop");
	}
	operationCycles();
}

void coinAcceptors2(void)
{
	operationCycles();
}

void coinAcceptors1(void)
{
	
	char temporalCredit_M[] = {' ','+',0xFF,0xFF};
	static uint8_t canastaWasSelected_Flag = OFF_;
	static uint8_t canastaNumber= 0;
	uint8_t unidades,decenas,availableTime;
	
	if (dryer[CANASTA1].coinAccepted_Flag == ON_ && dryer[CANASTA2].coinAccepted_Flag == ON_)
	{
		if (canastaWasSelected_Flag == OFF_)
		{
			temporal_Credit++;
		}
		else
		{
			dryer[canastaNumber].AvailableCredit++;
		}
		dryer[CANASTA1].coinAccepted_Flag = OFF_;
		dryer[CANASTA2].coinAccepted_Flag = OFF_;
		
	}
	//if door is closed( door_state = 1), the system is ready to continue with its normal behavior
	//if (dryer[CANASTA1].dryMachine_sensorsState.door_state == 0 || dryer[CANASTA2].dryMachine_sensorsState.door_state == 0)
	
	//Read inputs
	dryer[CANASTA1].dryMachine_sensorsState = dryer[CANASTA1].DRY_read_Sensors(CANASTA1);
	dryer[CANASTA2].dryMachine_sensorsState = dryer[CANASTA2].DRY_read_Sensors(CANASTA2);
	
	if (temporal_Credit && canastaWasSelected_Flag == OFF_)
	{
		//display credit bought by user in both screens
		availableTime = temporal_Credit * timeXtoken;
		//limit available time value because it is the maximum value that can be displayed in two digits(2 displays)
		if (availableTime > 99)
		{
			availableTime = 99;
		}
		unidades = availableTime % 10;
		decenas = availableTime / 10;
		temporalCredit_M[2] = decenas + 0x30;
		temporalCredit_M[3] = unidades + 0x30;
		
		
		//hacer que ambas pantallas muestren el credito ingresado
		//esperar a que se presione un boton para saber a que canasta se le agregara ese credito
			
		
		
		if (dryer[CANASTA1].dryMachine_sensorsState.door_state)
		{
			//door is close
			string_to_Screen(temporalCredit_M,BLINK,CANASTA1);	
			if (dryer[CANASTA1].dryMachine_sensorsState.btnIZQ_state ||  dryer[CANASTA1].dryMachine_sensorsState.btnCEN_state || dryer[CANASTA1].dryMachine_sensorsState.btnDER_state)
			{
				//debounce
				tiempo_Retardo(50);
				dryer[CANASTA1].dryMachine_sensorsState = dryer[CANASTA1].DRY_read_Sensors(CANASTA1);
				if (dryer[CANASTA1].dryMachine_sensorsState.btnIZQ_state ||  dryer[CANASTA1].dryMachine_sensorsState.btnCEN_state || dryer[CANASTA1].dryMachine_sensorsState.btnDER_state)
				{
					canastaWasSelected_Flag = ON_;
					canastaNumber = CANASTA1;
					dryer[canastaNumber].AvailableCredit = temporal_Credit;
					temporal_Credit = 0;
					string_to_Screen(coinAcceptors_messages[DASH_SYMBOL_M],BLINK,CANASTA1);
					string_to_Screen(coinAcceptors_messages[BLANK_M],NO_ANMIMATION,CANASTA2);
					tiempo_Retardo(1000);
				}
				
			}
		}
		else
		{
			operationCycles();
		}
		if (dryer[CANASTA2].dryMachine_sensorsState.door_state)
		{
			//door is close
			string_to_Screen(temporalCredit_M,BLINK,CANASTA2);
			if (dryer[CANASTA2].dryMachine_sensorsState.btnIZQ_state ||  dryer[CANASTA2].dryMachine_sensorsState.btnCEN_state || dryer[CANASTA2].dryMachine_sensorsState.btnDER_state)
			{
				//debounce
				tiempo_Retardo(50);
				dryer[CANASTA2].dryMachine_sensorsState = dryer[CANASTA2].DRY_read_Sensors(CANASTA2);
				if (dryer[CANASTA2].dryMachine_sensorsState.btnIZQ_state ||  dryer[CANASTA2].dryMachine_sensorsState.btnCEN_state || dryer[CANASTA2].dryMachine_sensorsState.btnDER_state)
				{
					canastaWasSelected_Flag = ON_;
					canastaNumber = CANASTA2;
					dryer[canastaNumber].AvailableCredit = temporal_Credit;
					temporal_Credit = 0;
					string_to_Screen(coinAcceptors_messages[DASH_SYMBOL_M],BLINK,CANASTA2);
					string_to_Screen(coinAcceptors_messages[BLANK_M],NO_ANMIMATION,CANASTA1);
					tiempo_Retardo(1000);
				}	
			}
		}
		else
		{
			operationCycles();
		}
			
	}
	if (canastaWasSelected_Flag == ON_)
	{
		if (dryer[canastaNumber].cycle)
		{
			//dryer was already activated, increase time
			dryer[canastaNumber].time_to_reach += tokens_to_Time(dryer[canastaNumber].AvailableCredit);
			dryer[canastaNumber].AvailableCredit = 0;
		}
		else
		{
			//dryer has not be activated,
			dryer[canastaNumber].time_to_reach = tokens_to_Time(dryer[canastaNumber].AvailableCredit);
		}
		if (dryer[canastaNumber].AvailableCredit == 0 )
		{
			//credit was already processed
			canastaWasSelected_Flag = OFF_;
		}
		//limit time_to_reach value because it is the maximum value that can be displayed in two digits(2 displays)
		if (dryer[canastaNumber].time_to_reach > 99)
		{
			dryer[canastaNumber].time_to_reach = 99;
		}
		
	}
		
	if (temporal_Credit == 0)
	{
		operationCycles();
	}
		
}

static void check_for_showTemperature_request(uint8_t canastaNumber)
{
	if (dryer[canastaNumber].dryMachine_sensorsState.btnIZQ_state ||  dryer[canastaNumber].dryMachine_sensorsState.btnCEN_state || dryer[canastaNumber].dryMachine_sensorsState.btnDER_state)
	{
		// if button  pressing for 10 seconds, show the current temperature for 2 seconds
		if (dryer[canastaNumber].counter == 170)
		{
			dryer[canastaNumber].counter =0;
			showTemperature(canastaNumber);
			tiempo_Retardo(2000);
			
		}
		else
		{
			dryer[canastaNumber].counter++;
		}
	}
	else
	{
		dryer[canastaNumber].counter =0;
	}
	
}

void process_Message(uint8_t *message)
{
	data_IDs myData_IDs;
	data_CmdActivar myData_cmdActivar;
	data_CmdStatus myData_CmdStatus;
	uint8_t canastaNumber = 0xFF;
	uint8_t etapa = 0;
	switch (message[DATA_RECEIVED_UART])
	{
		case MSG_IDS:
			myData_IDs.tipo_msg = message[DATA_RECEIVED_UART];
			myData_IDs.id_1 = message[DATA_RECEIVED_UART + 1];
			myData_IDs.id_2 = message[DATA_RECEIVED_UART + 2];
			dryer[CANASTA1].ID = myData_IDs.id_1;
			dryer[CANASTA2].ID = myData_IDs.id_2;
			break;
		
		case CMD_ACTIVAR:
			myData_cmdActivar.tipo_msg = message[DATA_RECEIVED_UART];
			myData_cmdActivar.id_1 = message[DATA_RECEIVED_UART + 1];
			myData_cmdActivar.id_2 = message[DATA_RECEIVED_UART + 2];
			myData_cmdActivar.repeticiones = message[DATA_RECEIVED_UART + 3];
			canastaNumber = get_CanastaNumber(myData_cmdActivar.id_1);
			if (canastaNumber != 0xFF)
			{
				if (dryer[canastaNumber].cycle == 0)
				{
					dryer[canastaNumber].time_to_reach = myData_cmdActivar.repeticiones;
					if (dryer[canastaNumber].time_to_reach > 99)
					{
						dryer[canastaNumber].time_to_reach = 99;
					}
					dryer[canastaNumber].pending_to_activate_Flag = 1;
					ctrl_Leds(0xFF,OFF,canastaNumber);
					ctrl_Leds(LED_DER,ON,canastaNumber);
					//send message OK
					sendMessage_UART( organize_Message_OK(dryer[canastaNumber].ID , myData_cmdActivar.tipo_msg) );
				}
				else
				{
					//there is a cycle running, ignore activation
				}
				
			}
			else
			{
				//there is an error with IDs, TODO, implement how to handled this scenario
			}
			break;
		case CMD_STATUS:
			myData_CmdStatus.tipo_msg = message[DATA_RECEIVED_UART];
			myData_CmdStatus.id_1 = message[DATA_RECEIVED_UART + 1];
			myData_CmdStatus.id_2 = message[DATA_RECEIVED_UART + 2];
			canastaNumber = get_CanastaNumber(myData_CmdStatus.id_1);
			//etapa = ( (dryer[canastaNumber].cycle != 0 ) ? CALENTANDO: NADA );
			if (dryer[canastaNumber].cycle != 0)
			{
				etapa = CALENTANDO;
			}
			else
			{
				etapa = NADA;
				dryer[canastaNumber].time_to_reach = 0;
			}
			if ( dryer[canastaNumber].pullDown_Flag )
			{
				etapa = ENFRIANDO;
			}
			//Send Estado ciclo
			sendMessage_UART( organize_Message_EstadoCiclo(dryer[canastaNumber].ID, etapa, DUMMY_BYTE, (dryer[canastaNumber].time_to_reach - dryer[canastaNumber].current_Time),dryer[canastaNumber].current_temperature ) );
			//send message OK
			sendMessage_UART( organize_Message_OK(dryer[canastaNumber].ID , myData_CmdStatus.tipo_msg) );
			
			break;
			
		default:
			break;
	}
	
	//Clean rmessageeceivedFlag
}

/* brief clear messageReceived_Flag
 *
 * Function to delete messageReceived_Flag
 *  
 *	param canastaNumber/ CANASTA1, CANASTA2, 0xFF = delete both flags
 *  return
 */
static void clear_ReceiveMessageFlag(uint8_t canastaNumber)
{
	if (canastaNumber == 0xFF)
	{
		dryer[CANASTA1].messageReceived_Flag = 0;
		dryer[CANASTA2].messageReceived_Flag = 0;
	}
	else
	{
		dryer[canastaNumber].messageReceived_Flag = 0;
	}
	
	
}

/* brief get Canasta Number
 *
 * Function to get canasta number accorging to ID number
 *  
 *	param ID: number of ID received from ESP-12
 *  return canastaNumber
 */
static uint8_t get_CanastaNumber(uint8_t ID_number)
{
	if (dryer[CANASTA1].ID == ID_number)
	{
		return CANASTA1;
	}
	else if (dryer[CANASTA2].ID == ID_number)
	{
		return CANASTA2;
	}
	else
	{
		//ID number not recognized, there is an error
		return 0xFF;
		
	}
	
}

static GETSTATE read_inputs(uint8_t canastaNumber)
{
	GETSTATE a;
	if (canastaNumber == CANASTA1)
	{
		a.door_state = DOOR1_ISCLOSE;
		a.btnIZQ_state = BTN_IZQ_P1_PRESS;
		a.btnCEN_state = BTN_CEN_P1_PRESS;
		a.btnDER_state = BTN_DER_P1_PRESS;
	}
	else if (canastaNumber == CANASTA2)
	{
		a.door_state = DOOR2_ISCLOSE;
		a.btnIZQ_state = BTN_IZQ_P2_PRESS;
		a.btnCEN_state = BTN_CEN_P2_PRESS;
		a.btnDER_state = BTN_DER_P2_PRESS;
	}

	return a;
}

/*  brief  turn on or turn off outputs in machine
 *
 *  This function control the following signals
 *	motor derecha, motor izquierdo, ignitor and ventilador
 *  of each canasta
 *
 *  param - canastaNumber: number of canasta/ CANASTA1 , CANASTA2
 * 
 *
 *  return:
 */
static void ctrl_outputs(SETSTATE outputs, uint8_t canastaNumber)
{
	if (canastaNumber == CANASTA1)
	{
		if (dryer[canastaNumber].dryMachine_outputs_State.ignitor_state)
		{
			IGNITOR1_ON;
		}
		else
		{
			IGNITOR1_OFF;
		}
		if (dryer[canastaNumber].dryMachine_outputs_State.motor_Der_state)
		{
			MOTOR_DER1_ON;
		}
		else
		{
			MOTOR_DER1_OFF;
		}
		if (dryer[canastaNumber].dryMachine_outputs_State.motor_Izq_state)
		{
			MOTOR_IZQ1_ON;
		}
		else
		{
			MOTOR_IZQ1_OFF;
		}
		if (dryer[canastaNumber].dryMachine_outputs_State.ventilador_state)
		{
			VENTILADOR1_ON;
		}
		else
		{
			VENTILADOR1_OFF;
		}
	}
	else if (canastaNumber == CANASTA2)
	{
		if (dryer[canastaNumber].dryMachine_outputs_State.ignitor_state)
		{
			IGNITOR2_ON;
		}
		else
		{
			IGNITOR2_OFF;
		}
		if (dryer[canastaNumber].dryMachine_outputs_State.motor_Der_state)
		{
			MOTOR_DER2_ON;
		}
		else
		{
			MOTOR_DER2_OFF;
		}
		if (dryer[canastaNumber].dryMachine_outputs_State.motor_Izq_state)
		{
			MOTOR_IZQ2_ON;
		}
		else
		{
			MOTOR_IZQ2_OFF;
		}
		if (dryer[canastaNumber].dryMachine_outputs_State.ventilador_state)
		{
			VENTILADOR2_ON;
		}
		else
		{
			VENTILADOR2_OFF;
		}
		
	}
	asm("nop");
}


/*  brief  read ADC in ADC0 and ADC1 channels
 *
 *  This function read ADC in ADC0 and ADC1 where the thermistor1 and thermistor2 is connected
 *
 *  param - canastaNumber: number of canasta/ CANASTA1 , CANASTA2
 * 
 *  return:
 */
static void checkTemperature(uint8_t canastaNumber)
{
	uint16_t adc_value;
	uint16_t adc_filter = 0;
	
	if (canastaNumber == CANASTA1)
	{
		//ADMUX = 0b01000000;
		THERMISTOR1_CH_ON;
	}
	else if (canastaNumber == CANASTA2)
	{
		//ADMUX = 0b01000001;
		THERMISTOR2_CH_ON;
	}
	
	/********** Dummy lecture ****************/
	START_ADC_CONVERSION;
	while(ADC_CONVERSION_IN_PROGRES)
	{
		asm("nop");
		
	}
	CLEAN_COMPLETE_ADC_CONVERSION_FLAG;
	
	//4.88mv per count
	adc_value = ADC;
	/******************************************/
	
	/*just for test, used lm35 sensor, 10mv per 1 celcius
	* Use these lines to test project in proteus
	*/
	/*************************************************************************/
	// (adc_value * (resolution per count) ) / 10
	/*adc_value = (adc_value * 5) /10;
	//dryer[canastaNumber].current_temperature = (uint8_t)adc_value; */
	/************************************************************************/
	
	/* Used the line below in normal operation */
	/*************************************************************************/
	dryer[canastaNumber].current_temperature = steinhart_Equation(adc_value);
	/*************************************************************************/
	for (uint8_t i = 0; i < NUMBER_OF_ADC_SAMPLES; i++)
	{
		START_ADC_CONVERSION;
		while(ADC_CONVERSION_IN_PROGRES)
		{
			asm("nop");
			
		}
		CLEAN_COMPLETE_ADC_CONVERSION_FLAG;
		adc_value = ADC;
		adc_filter  += steinhart_Equation(adc_value);
	}
	//Use average lecture instead just one
	adc_filter = (adc_filter / NUMBER_OF_ADC_SAMPLES) ;
	if (adc_filter >= MAX_ALLOWABLE_TEMPERATURE_TO_SHOW)
	{
		dryer[canastaNumber].current_temperature = MAX_ALLOWABLE_TEMPERATURE_TO_SHOW;	
	}
	else
	{
		dryer[canastaNumber].current_temperature = adc_filter;	
	}
	  
	if (dryer[canastaNumber].current_temperature >= (dryer[canastaNumber].temperature_to_reach + dryer[canastaNumber].histeresis_H ) || dryer[canastaNumber].pullDown_Flag)
	{
		//turn off ignitor
		dryer[canastaNumber].dryMachine_outputs_State.ignitor_state = OFF_;
		
	}
	else if (dryer[canastaNumber].current_temperature <= (dryer[canastaNumber].temperature_to_reach - dryer[canastaNumber].histeresis_L ) )
	{
		//turn on ignitor
		dryer[canastaNumber].dryMachine_outputs_State.ignitor_state = ON_;
		
	}
}

/*  brief  Show temperature
 *
 *  Show temperature in canasta selected
 *
 *  param - canastaNumber: number of canasta/ CANASTA1 , CANASTA2
 * 
 *  return:
 */
static void showTemperature(uint8_t canastaNumber)
{
	char temperature[] = {0xFF,0xFF,';','C'};
	temperature[0] = (dryer[canastaNumber].current_temperature / 10 ) + 0x30;
	temperature[1] = (dryer[canastaNumber].current_temperature % 10) + 0x30; 
	string_to_Screen(temperature,NO_ANMIMATION,canastaNumber);
}

/*  brief  update info displayed to user
 *
 *  This function calculate the time to finish the cycle 
 *	and display to user, also show the temperature selected by the user T1, T2, or T3.
 *
 *  param:	canasta Number
 *
 *  return:
 */
static void check_Time(uint8_t canastaNumber)
{
	uint8_t unidades,decenas,time_to_FinishCycle;
	char cycle[] = {'t',0xFF,0xFF,0xFF};
	time_to_FinishCycle = dryer[canastaNumber].time_to_reach - dryer[canastaNumber].current_Time;
	unidades = time_to_FinishCycle % 10;
	decenas = time_to_FinishCycle / 10;
	cycle[1] = dryer[canastaNumber].cycle + 0x30;
	cycle[2] = decenas + 0x30;
	cycle[3] = unidades + 0x30;
	
	if (dryer[canastaNumber].dryMachine_sensorsState.door_state != 0 && (dryer[canastaNumber].state != WAITING_TO_BE_ACTIVATED_STATE && dryer[canastaNumber].state < PULLDWN_STATE +1 )  && (!temporal_Credit) )
	{
		//update info in Screen if door is close and machine has been activated, and there is not a credit pending to charge(MONEDERO1_mode), otherwise don't update info in screen
		string_to_Screen(cycle,NO_ANMIMATION,canastaNumber);
	}
	else
	{
		asm("nop");
	}
	
	if (dryer[canastaNumber].cycle)
	{
		//check time just when machine is running a cycle
		if ( tiempo_Compara( (dryer[canastaNumber].timer) ,BORRAR) )
		{
			//1 minute transcurred
			dryer[canastaNumber].current_Time++;
		}
		if (time_to_FinishCycle <= dryer[canastaNumber].pullDown_Time)
		{
			dryer[canastaNumber].pullDown_Flag = ON_;
		}
		else
		{
			dryer[canastaNumber].pullDown_Flag = OFF_;
		}
		if (time_to_FinishCycle == 0)
		{
			dryer[canastaNumber].Cyclefinished = ON_;
		}
		
		//Fix: When door is open after current_Time variable reach time_to_reach value, the current_Time continues increment time, this is a not expected behavior
		if (dryer[canastaNumber].current_Time >= dryer[canastaNumber].time_to_reach)
		{
			dryer[canastaNumber].current_Time = dryer[canastaNumber].time_to_reach;
		}
	}
	
}

/*  brief  return current temperature in celcius
 *
 *  This function calculate the current temperature
 *  using steinhart equation.
 *
 *  param:	uint16_t adc_Value:  raw value from ADC 
 *
 *  return: uint8_t temperature in celcius
 */
static uint8_t steinhart_Equation(uint16_t adc_Value)
{
	double R2 = 0;
	double logR2 = 0;
	double temperature_Kelvin;
	uint16_t temperature_Celcius;
	//adc_Value = 1023 - adc_Value;						//Check this line
	//R2 = FIX_RESISTOR_R1_ohms / ( ( VoltageReference_mv / (adc_Value * voltagePerCount_mv) ) - 1 );
	R2 =  FIX_RESISTOR_R1_ohms * ( ( VoltageReference_mv / (adc_Value * voltagePerCount_mv) ) - 1 ) ;
	logR2 = log(R2);
	temperature_Kelvin = (1.0 / ( STEINHART_COEFICIENT_A + (STEINHART_COEFICIENT_B * logR2) + (STEINHART_COEFICIENT_C * (pow(logR2,3)))) );
	temperature_Celcius = (uint8_t)(temperature_Kelvin - 273.15);
	return temperature_Celcius;
}

/*  brief  check if token was accepted by coin validator
 *
 *  
 *  param:	canastaNumber/ CANASTA1, CANASTA2
 *
 *  return: 
 */
void check_TokenAccepted(uint8_t canastaNumber)
{
	static uint8_t numberOftimer_ExtInt[NUMBER_OF_CANASTAS] = {0,0};
	static uint8_t toggle[NUMBER_OF_CANASTAS] = {0,0};
	uint16_t time[NUMBER_OF_CANASTAS] = {0,0};
	if (toggle[canastaNumber] == 0)
	{
		//Create a virtual timer(number of timer assigned by software and free runnning, no compare mode) to count the pulse time generated by coin validator
		numberOftimer_ExtInt[canastaNumber] = new_VirtualTimer(0xFF,0xFFFF);
	}
	else
	{
		//stop and free timer
		time[canastaNumber] =  getTime_VirtualTimer(numberOftimer_ExtInt[canastaNumber]);
		tiempo_Liberar(numberOftimer_ExtInt[canastaNumber]);
		if (time[canastaNumber] > 20 && time[canastaNumber] < 105)
		{
			dryer[canastaNumber].coinAccepted_Flag = ON_;
			if (opMode == MONEDEROS1_mode)
			{
				dryer[CANASTA1].coinAccepted_Flag = ON_;
				dryer[CANASTA2].coinAccepted_Flag = ON_;
			}
		}
		else
		{
			dryer[canastaNumber].coinAccepted_Flag = OFF_;
			if (opMode == MONEDEROS1_mode)
			{
				dryer[CANASTA1].coinAccepted_Flag = OFF_;
				dryer[CANASTA2].coinAccepted_Flag = OFF_;
			}
		} 
		//remove lines below in normal operation
		/*dryer[canastaNumber].coinAccepted_Flag = ON_;
		if (opMode == MONEDEROS1_mode)
		{
			dryer[CANASTA1].coinAccepted_Flag = ON_;
			dryer[CANASTA2].coinAccepted_Flag = ON_;
		}*/
		////////////////////////////////////////
	}
	toggle[canastaNumber] ^= 1;
}