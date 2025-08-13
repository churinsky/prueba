/*
 * driver_Display7ssegment.h
 *
 * Created: 2/21/2025 11:34:22 AM
 *  Author: Investigacion
 */ 


#ifndef DRIVER_DISPLAY14SSEGMENT_H_
#define DRIVER_DISPLAY14SSEGMENT_H_

#define NUMBER_OF_DISPLAYS			4
#define DISPLAY1					0
#define DISPLAY2					1
#define DISPLAY3					2
#define DISPLAY4					3
/*1.- Register 74595*/
/* Data segment 1
MSb = Semgment C
0b10000000 = Turn on Segment C (Check schematic)
*/

/*2.- Shift Register 74595*/
/* Data Segment 2
MSb = Semgment B
0b10000000 = Turn on Segment C (Check schematic)
*/

/*3.- Shift Register 74595*/
#define ctrl_DIG1					7
#define ctrl_DIG2					6
#define ctrl_DIG3					5
#define ctrl_DIG4					4

/***********************************/


/***********************************/

#define NUMBER_OF_SCREENS			2
#define NUMBER_OF_CANASTAS			NUMBER_OF_SCREENS
#define SCREEN1						0
#define SCREEN2						1
#define MAX_LENGH_MESSAGE			35
#define MAX_LENGH_NO_ANIMATION_MESSAGE			5
#define NO_ANMIMATION				0
#define RIGHT_TO_LEFT				1
#define BLINK						2
#define BLINK_TIME					500
#define REFRESH_TIME_ms				2
#define SPEED_TEXT_ms				150



/********************14 segment Decoder CC*********************/
#define NUMBER0_14S				0xE2CA
#define NUMBER1_14S				0x8088
#define NUMBER2_14S				0x4686
#define NUMBER3_14S				0x8682
#define NUMBER4_14S				0x84C4
#define NUMBER5_14S				0x8646
#define NUMBER6_14S				0xC646
#define NUMBER7_14S				0x8082
#define NUMBER8_14S				0xC6C6
#define NUMBER9_14S				0x86C6

#define LETRA_A_14S				0xC4C6
#define LETRA_B_14S				0x9692
#define LETRA_C_14S				0x4242
#define LETRA_D_14S				0x9292
#define LETRA_E_14S				0x4246
#define LETRA_F_14S				0x4046
#define LETRA_G_14S				0xC642
#define LETRA_H_14S				0xC4C4
#define LETRA_I_14S				0x1212
#define LETRA_J_14S				0xC280
#define LETRA_K_14S				0x484C
#define LETRA_L_14S				0x4240
#define LETRA_M_14S				0xC0E8
#define LETRA_N_14S				0xC8E0
#define LETRA_O_14S				0xC2C2
#define LETRA_P_14S				0x44C6
#define LETRA_Q_14S				0xCAC2
#define LETRA_R_14S				0x4CC6
#define LETRA_S_14S				0x8646
#define LETRA_T_14S				0x1012
#define LETRA_U_14S				0xC2C0
#define LETRA_V_14S				0x6048
#define LETRA_W_14S				0xE8C0
#define LETRA_X_14S				0x2828
#define LETRA_Y_14S				0x1028
#define LETRA_Z_14S				0x220A
#define SPACE_BLANK_14S			0x0000	
#define MEDIUM_SCORE_14S		0x0404
#define DEGREE_SYMBOL_14S		0x04C6
#define PLUS_SYMBOL_14S			0x1414

/********************7 segment Decoder CA*********************/

#define SPACE_BLANK			0xFF
#define MEDIUM_SCORE		((~0b00000010) & 0x00FF)
#define NUMBER0				((~0b11111100) & 0x00FF)
#define NUMBER1				((~0b01100000) & 0x00FF)
#define NUMBER2				((~0b11011010) & 0x00FF)
#define NUMBER3				((~0b11110010) & 0x00FF) 
#define NUMBER4				((~0b01100110) & 0x00FF)
#define NUMBER5				((~0b10110110) & 0x00FF) 
#define NUMBER6				((~0b10111110) & 0x00FF)
#define NUMBER7				((~0b11100000) & 0x00FF)
#define NUMBER8				((~0b11111110) & 0x00FF)
#define NUMBER9				((~0b11110110) & 0x00FF) 

#define LETRA_A				((~0b11101110) & 0x00FF)
#define LETRA_B				((~0b00111110) & 0x00FF)
#define LETRA_C				((~0b10011100) & 0x00FF)
#define LETRA_D				((~0b01111010) & 0x00FF)
#define LETRA_E				((~0b10011110) & 0x00FF)
#define LETRA_F				((~0b10001110) & 0x00FF)
#define LETRA_G				NUMBER6
#define LETRA_H				((~0b01101110) & 0x00FF)
#define LETRA_I				((~0b00001100) & 0x00FF)
#define LETRA_J				((~0b01111000) & 0x00FF)
#define LETRA_K				SPACE_BLANK
#define LETRA_L				((~0b00011100) & 0x00FF)
#define LETRA_M				SPACE_BLANK
#define LETRA_N				((~0b00101010) & 0x00FF)
#define LETRA_O				((~0b00111010) & 0x00FF)
#define LETRA_P				((~0b11001110) & 0x00FF)
#define LETRA_Q				((~0b11100110) & 0x00FF)
#define LETRA_R				((~0b00001010) & 0x00FF)
#define LETRA_S				NUMBER5
#define LETRA_T				((~0b00011110) & 0x00FF)
#define LETRA_U				((~0b01111100) & 0x00FF)
#define LETRA_V				SPACE_BLANK
#define LETRA_W				SPACE_BLANK
#define LETRA_X				SPACE_BLANK
#define LETRA_Y				((~0b01110110) & 0x00FF) 
#define LETRA_Z				NUMBER2
/******************************************************************/
#define ON					1
#define OFF					0


/* Register position per screen */
typedef enum
{
	/**** FIRST 74595 *************/
	RESERVED_0 = 0,					//MSb
	RESERVED_1,
	RESERVED_2,
	RESERVED_3,
	RESERVED_4,
	RESERVED_5,
	RESERVED_6,
	RESERVED_7,
	/*****************************/
	RESERVED_8,
	RESERVED_9,
	RESERVED_10,
	RESERVED_11,
	RESERVED_12,
	RESERVED_13,
	RESERVED_14,
	RESERVED_15,
	/*****************************/

	RESERVED_16,
	RESERVED_17,
	RESERVED_18,
	RESERVED_19,
	LED_IZQ,
	LED_CEN,
	LED_DER,
	RESERVED_20,
	/*****************************/
	
} LEDs_RegisterPosition;

/*Union used to display characters in displays, since there are 14 segments displays we need 2 bytes for each displays but the driver 74595 just admit send 1 byte at time*/
typedef union
{
	struct
	{
		uint8_t screendata_LSB;	
		uint8_t screendata_MSB;
	}SCREENDATA_TO_74595;
	
	uint16_t screenData14seg;	
}SCREENDATA;

/*Structure used to format data that will be sent to Screen(data to displays, data to control displays and to drive Leds in PCB "Tarjeta Display 14 segmentos")*/
typedef struct
{
	SCREENDATA screenData;						
	uint8_t controlDisplays_and_Leds;
} DISPLAY_AND_LEDS_74595;

/*Save values that are related with the control of data that will display in screen*/
typedef struct
{
	uint8_t ctrl_Display;						//signal to turn on or turn off displays
	uint8_t display_to_show;					//number of display to show
	uint8_t animation_in_Course;				//what animation is in course 
	uint16_t counter;							//used in blink or right to left functions
	uint8_t a;									//used in blink or right to left functions
	uint8_t number_of_BytesToScreen;			//bytes to be transmited to screen	
} CONTROL_SCREEN;

typedef struct
{
	uint16_t numbers[10];
	uint16_t letters[26];
	
} ASCII_TO_14SEGMENT_DISPLAY;



void data_to_Screen(void * data, uint8_t animation,uint8_t numberofscreen);
void string_to_Screen(const char * data, uint8_t animation,uint8_t numberofscreen);
void updateScreen(uint8_t numberofScreen);
uint8_t decoder_7Segments(uint8_t character);
uint16_t decoder_14Segments(uint8_t character);
void iniScreen(volatile uint8_t *portMR, uint8_t pinMR, volatile uint8_t *portLATCH, uint8_t pinLATCH, volatile uint8_t *portSERDAT, uint8_t pinSERDAT, volatile uint8_t *portSRCLK, uint8_t pinSRCLK, uint8_t totalNumberOfScreens);
//static void clean_Buffer(uint8_t numberofscreen);
//static void data_to_Blink(uint8_t numberofscreen);
//static void right_to_Left(uint8_t numberofscreen);
void ctrl_Leds(LEDs_RegisterPosition ledName, uint8_t ON_OFF, uint8_t numberOfScreen);
#endif /* DRIVER_DISPLAY14SSEGMENT_H_ */