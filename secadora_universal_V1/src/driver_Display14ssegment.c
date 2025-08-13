/*
 * driver_Display7ssegment.c
 *
 * Created: 2/21/2025 10:25:17 AM
 *  Author: Investigacion
 */ 
#include <avr/io.h>
#include "driver_74595.h"
#include "driver_Display14ssegment.h"


uint16_t display[NUMBER_OF_SCREENS][NUMBER_OF_DISPLAYS];												//save decoded value of each display
uint16_t buffer[NUMBER_OF_SCREENS][MAX_LENGH_MESSAGE];													//save decoded value when the data to show is bigger than the number of displays
uint8_t data_to_74595[(sizeof(DISPLAY_AND_LEDS_74595)/sizeof(uint8_t)) * NUMBER_OF_SCREENS];			//used to save the data that will be sent to 74595 shift registers, 1 byte per register, data_to_74595[5] = 6 74595 shift registers
DISPLAY_AND_LEDS_74595 screen[NUMBER_OF_SCREENS];
ASCII_TO_14SEGMENT_DISPLAY decoder;
CONTROL_SCREEN ctrlScreen[NUMBER_OF_SCREENS];

/*control signals of 74595*/
volatile uint8_t *port_MR;
uint8_t pin_MR;
volatile uint8_t *port_LATCH;
uint8_t pin_LATCH;
volatile uint8_t *port_SERDAT;
uint8_t pin_SERDAT; 
volatile uint8_t *port_SRCLK;
uint8_t pin_SRCLK;
/***************************/


static void clean_Buffer(uint8_t numberofscreen)
{
	for (uint8_t i = 0; i < ((sizeof(buffer[numberofscreen]))/sizeof(buffer[numberofscreen][0])); i++ )
	{
		buffer[numberofscreen][i] = SPACE_BLANK_14S;
	}
	ctrlScreen[numberofscreen].counter = 0;
	ctrlScreen[numberofscreen].a = 0;
	ctrlScreen[numberofscreen].number_of_BytesToScreen =0;
}

static void data_to_Blink(uint8_t numberofscreen)
{
	if (ctrlScreen[numberofscreen].counter > BLINK_TIME)
	{
		ctrlScreen[numberofscreen].counter = 0;
	}
	if (ctrlScreen[numberofscreen].counter > BLINK_TIME/2 )
	{
		//turn off screen to blink the screen
		screen[numberofscreen].controlDisplays_and_Leds &= ~(1 << ctrl_DIG1) & ~(1 << ctrl_DIG2) & ~(1 << ctrl_DIG3) & ~(1 << ctrl_DIG4) ; /* Apagamos todos los displays*/
	}
	
	ctrlScreen[numberofscreen].counter = ctrlScreen[numberofscreen].counter + REFRESH_TIME_ms;
}

static void right_to_Left(uint8_t numberofscreen)
{

	if (ctrlScreen[numberofscreen].counter > (SPEED_TEXT_ms/REFRESH_TIME_ms) )
	{
		display[numberofscreen][DISPLAY1] = display[numberofscreen][DISPLAY2];
		display[numberofscreen][DISPLAY2] = display[numberofscreen][DISPLAY3];
		display[numberofscreen][DISPLAY3] = display[numberofscreen][DISPLAY4];
		display[numberofscreen][DISPLAY4] = buffer[numberofscreen][ctrlScreen[numberofscreen].a];
		ctrlScreen[numberofscreen].a++;
		ctrlScreen[numberofscreen].counter = 0;
	}
	if ( ctrlScreen[numberofscreen].a > ctrlScreen[numberofscreen].number_of_BytesToScreen )
	{
		ctrlScreen[numberofscreen].a = 0;
	}
	ctrlScreen[numberofscreen].counter++;
}

/*
	brief: compare message vs data in buffer, if is the same, return 1, otherwise return 0
*/
static uint8_t isTheSameMEssage(const char *message, uint8_t numberofscreen, uint8_t animation)
{
	uint8_t i = 0;
	if (animation == NO_ANMIMATION)
	{
		return 0;
	}
	if (animation == BLINK)
	{
		for (uint8_t j=0; j < NUMBER_OF_DISPLAYS; j++)
		{
			if (display[numberofscreen][j] != decoder_14Segments(message[j]) )
			{
				return 0;
			}
		}
		//return 1;
	}
	else
	{
		while(message[i] )
		{	
			if (buffer[numberofscreen][i] == decoder_14Segments(message[i]))
			{
				i++;
			}
			else
			{
				return 0;
			}
		}
		//return 1;
	}
	return 1;
}
 
/*data[0]: First display from left to right*/
void data_to_Screen(void * data, uint8_t animation,uint8_t numberofscreen)
{
	uint16_t * mydata = (uint16_t*)data;
	clean_Buffer(numberofscreen);
	if (animation == RIGHT_TO_LEFT )
	{
		//mydata[NUMBER_OF_DISPLAYS] = SPACE_BLANK;
		for (uint8_t i=0; i < NUMBER_OF_DISPLAYS; i++)
		{
			display[numberofscreen][i] =  SPACE_BLANK_14S;
			buffer[numberofscreen][i] = mydata[i];
		}	
		
	}
	else
	{
		for (uint8_t i=0; i < NUMBER_OF_DISPLAYS; i++)
		{
			display[numberofscreen][i] =  mydata[i];
		}
	}
	ctrlScreen[numberofscreen].number_of_BytesToScreen = NUMBER_OF_DISPLAYS ;
	ctrlScreen[numberofscreen].animation_in_Course = animation;
}

void string_to_Screen(const char * data, uint8_t animation, uint8_t numberofscreen)
{
	const char *mydata = data;
	uint8_t i = 0;
	if ( !(isTheSameMEssage(mydata, numberofscreen, animation)) )
	{
		clean_Buffer(numberofscreen);
		if (animation == RIGHT_TO_LEFT)
		{
			while(mydata[i] && (i < MAX_LENGH_MESSAGE) )
			{
				buffer[numberofscreen][i] = mydata[i];
				buffer[numberofscreen][i] = decoder_14Segments(buffer[numberofscreen][i]);
				i++;
			}
			ctrlScreen[numberofscreen].number_of_BytesToScreen = i;
			
			for (uint8_t i=0; i < NUMBER_OF_DISPLAYS; i++)
			{
				display[numberofscreen][i] =  SPACE_BLANK_14S;
			}
		}
		else
		{
			for (uint8_t i=0; i < NUMBER_OF_DISPLAYS; i++)
			{
				display[numberofscreen][i] = decoder_14Segments(mydata[i]);
			}
			ctrlScreen[numberofscreen].number_of_BytesToScreen = NUMBER_OF_DISPLAYS;
		}
	}
	ctrlScreen[numberofscreen].animation_in_Course = animation;
	/*clean_Buffer(numberofscreen);
	if (animation == RIGHT_TO_LEFT)
	{
		while(mydata[i] && (i < MAX_LENGH_MESSAGE) )
		{
			buffer[numberofscreen][i] = mydata[i];
			buffer[numberofscreen][i] = decoder_14Segments(buffer[numberofscreen][i]);
			i++;
		}
		ctrlScreen[numberofscreen].number_of_BytesToScreen = i;
		
		for (uint8_t i=0; i < NUMBER_OF_DISPLAYS; i++)
		{
			display[numberofscreen][i] =  SPACE_BLANK_14S;
		}
	}
	else
	{
		for (uint8_t i=0; i < NUMBER_OF_DISPLAYS; i++)
		{
			display[numberofscreen][i] = decoder_14Segments(mydata[i]);
		}
		ctrlScreen[numberofscreen].number_of_BytesToScreen = NUMBER_OF_DISPLAYS;
	}
	ctrlScreen[numberofscreen].animation_in_Course = animation; */
}

/*Send information previuosly loaded in string_to_screen() or data_to_screen()*/
void updateScreen(uint8_t numberofScreen)
{
	uint8_t arrayOffset = 0;
	screen[numberofScreen].controlDisplays_and_Leds &= ~(1 << ctrl_DIG1) & ~(1 << ctrl_DIG2) & ~(1 << ctrl_DIG3) & ~(1 << ctrl_DIG4) ;					//Turn off displays
	screen[numberofScreen].controlDisplays_and_Leds  |= (1 << ctrlScreen[numberofScreen].ctrl_Display);
	
	if (ctrlScreen[numberofScreen].animation_in_Course == BLINK)
	{
		data_to_Blink(numberofScreen);
		
	}
	if (ctrlScreen[numberofScreen].animation_in_Course == RIGHT_TO_LEFT)
	{
		right_to_Left(numberofScreen);
	}
	
	//Load  data of the next display to show
	screen[numberofScreen].screenData.screenData14seg = display[numberofScreen][ctrlScreen[numberofScreen].display_to_show];
	if(numberofScreen)
	{
		arrayOffset = 3;
	}
	
	//Load data to memory that will be sent to 74595 shift registers
	data_to_74595[0 + arrayOffset] = screen[numberofScreen].screenData.SCREENDATA_TO_74595.screendata_MSB;
	data_to_74595[1 + arrayOffset] = screen[numberofScreen].screenData.SCREENDATA_TO_74595.screendata_LSB;
	data_to_74595[2 + arrayOffset] = screen[numberofScreen].controlDisplays_and_Leds;

	ctrlScreen[numberofScreen].display_to_show++;
	ctrlScreen[numberofScreen].ctrl_Display--;
	if (ctrlScreen[numberofScreen].display_to_show > DISPLAY4)
	{
		ctrlScreen[numberofScreen].display_to_show = DISPLAY1;
		ctrlScreen[numberofScreen].ctrl_Display = ctrl_DIG1;
	}
	sendDato(data_to_74595,(sizeof(screen)), port_MR, pin_MR, port_LATCH, pin_LATCH, port_SERDAT, pin_SERDAT, port_SRCLK, pin_SRCLK);
	
}

uint8_t decoder_7Segments(uint8_t character)
{
	static uint8_t ini_Decoder = 0;
	uint8_t value =0;
	if (!ini_Decoder)
	{
		ini_Decoder = 1;
	}
	
	if (character == ' ')
	{
		value = SPACE_BLANK;
	}
	else if ( character == '-')
	{
		value = MEDIUM_SCORE;
	} 
	
	else if(character >= 0x30 && character <= 0x39)
	{
		//character is a number
		value = decoder.numbers[character - 0x30];
	}
	else if (character >= 0x41 && character <= 0x5A)
	{
		//character is a letter (upercase)
		value = decoder.letters[character - 0x41];
	}
	else if (character >= 0x61 && character <= 0x7A)
	{
		//character is a letter (lowercase)
		value = decoder.letters[character - 0x61];
	}
	return value;
	
}

uint16_t decoder_14Segments(uint8_t character)
{
	
	uint16_t value =0;
	
	if (character == ' ')
	{
		value = SPACE_BLANK_14S;
	}
	else if ( character == '-')
	{
		value = MEDIUM_SCORE_14S;
	}
	else if ( character == ';')
	{
		value = DEGREE_SYMBOL_14S;
	}
	else if ( character == '+')
	{
		value = PLUS_SYMBOL_14S;
	}
	else if(character >= 0x30 && character <= 0x39)
	{
		//character is a number
		value = decoder.numbers[character - 0x30];
	}
	else if (character >= 0x41 && character <= 0x5A)
	{
		//character is a letter (upercase)
		value = decoder.letters[character - 0x41];
	}
	else if (character >= 0x61 && character <= 0x7A)
	{
		//character is a letter (lowercase)
		value = decoder.letters[character - 0x61];
	}
	return value;
	
}


void iniScreen(volatile uint8_t *portMR, uint8_t pinMR, volatile uint8_t *portLATCH, uint8_t pinLATCH, volatile uint8_t *portSERDAT, uint8_t pinSERDAT, volatile uint8_t *portSRCLK, uint8_t pinSRCLK,uint8_t totalNumberOfScreens)
{
	/*control signals of 74595*/
	port_MR = portMR;
	pin_MR = pinMR;
	port_LATCH = portLATCH;
	pin_LATCH = pinLATCH;
	port_SERDAT = portSERDAT;
	pin_SERDAT = pinSERDAT;
	port_SRCLK = portSRCLK;
	pin_SRCLK = pinSRCLK;
	
	decoder.numbers[0] = NUMBER0_14S ;
	decoder.numbers[1] = NUMBER1_14S ;
	decoder.numbers[2] = NUMBER2_14S ;
	decoder.numbers[3] = NUMBER3_14S ;
	decoder.numbers[4] = NUMBER4_14S ;
	decoder.numbers[5] = NUMBER5_14S ;
	decoder.numbers[6] = NUMBER6_14S ;
	decoder.numbers[7] = NUMBER7_14S ;
	decoder.numbers[8] = NUMBER8_14S ;
	decoder.numbers[9] = NUMBER9_14S ;
	
	decoder.letters[0] = LETRA_A_14S;
	decoder.letters[1] = LETRA_B_14S;
	decoder.letters[2] = LETRA_C_14S;
	decoder.letters[3] = LETRA_D_14S;
	decoder.letters[4] = LETRA_E_14S;
	decoder.letters[5] = LETRA_F_14S;
	decoder.letters[6] = LETRA_G_14S;
	decoder.letters[7] = LETRA_H_14S;
	decoder.letters[8] = LETRA_I_14S;
	decoder.letters[9] = LETRA_J_14S;
	decoder.letters[10] = LETRA_K_14S;
	decoder.letters[11] = LETRA_L_14S;
	decoder.letters[12] = LETRA_M_14S;
	decoder.letters[13] = LETRA_N_14S;
	decoder.letters[14] = LETRA_O_14S;
	decoder.letters[15] = LETRA_P_14S;
	decoder.letters[16] = LETRA_Q_14S;
	decoder.letters[17] = LETRA_R_14S;
	decoder.letters[18] = LETRA_S_14S;
	decoder.letters[19] = LETRA_T_14S;
	decoder.letters[20] = LETRA_U_14S;
	decoder.letters[21] = LETRA_V_14S;
	decoder.letters[22] = LETRA_W_14S;
	decoder.letters[23] = LETRA_X_14S;
	decoder.letters[24] = LETRA_Y_14S;
	decoder.letters[25] = LETRA_Z_14S;
	
	for (uint8_t i = 0; i < NUMBER_OF_SCREENS; i++)
	{
		ctrlScreen[i].counter = 0;
		ctrlScreen[i].a = 0;
		ctrlScreen[i].number_of_BytesToScreen = 0;
		ctrlScreen[i].display_to_show = DISPLAY1;
		ctrlScreen[i].ctrl_Display = ctrl_DIG1;
	}
	
}

 
void ctrl_Leds(LEDs_RegisterPosition ledName, uint8_t ON_OFF, uint8_t numberOfScreen)
{	
	if (ledName != 0xFF)
	{
		if (ON_OFF)
		{
			screen[numberOfScreen].controlDisplays_and_Leds |= ((1 << (7 - (ledName % 8))));
		}
		else
		{
			screen[numberOfScreen].controlDisplays_and_Leds &= (~(1 << (7 - (ledName % 8))));
		}
	}
	else
	{
		//Turn off all leds
		screen[numberOfScreen].controlDisplays_and_Leds &= 0b11110000;
		
	}	
}
