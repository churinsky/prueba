/*
 * port.h
 *
 * Created: 4/30/2025 8:49:11 AM
 *  Author: Investigacion
 */ 


#ifndef PORT_H_
#define PORT_H_

/******************** Inputs ************************/
#define configReg_PULSOMON1					 		DDRD
#define configReg_PULSOMON2					 		DDRD
#define configReg_OPCION1					 		DDRD
#define configReg_OPCION2					 		DDRD
#define configReg_DOOR1						 		DDRC
#define configReg_DOOR2						 		DDRC
#define configReg_BTN_IZQ_P1						DDRA
#define configReg_BTN_CEN_P1						DDRA
#define configReg_BTN_DER_P1						DDRA
#define configReg_BTN_IZQ_P2						DDRA
#define configReg_BTN_CEN_P2						DDRA
#define configReg_BTN_DER_P2						DDRA
#define configReg_THERMISTOR1						DDRA
#define configReg_THERMISTOR2						DDRA

#define readReg_PULSOMON1							PIND
#define readReg_PULSOMON2					 		PIND
#define readReg_OPCION1								PIND
#define readReg_OPCION2						 		PIND
#define readReg_DOOR1						 		PINC
#define readReg_DOOR2						 		PINC
#define readReg_BTN_IZQ_P1						    PINA
#define readReg_BTN_CEN_P1							PINA
#define readReg_BTN_DER_P1							PINA
#define readReg_BTN_IZQ_P2						    PINA
#define readReg_BTN_CEN_P2							PINA
#define readReg_BTN_DER_P2							PINA
#define readReg_THERMISTOR1							PINA
#define readReg_THERMISTOR2							PINA

#define pin_PULSOMON1								PIND2
#define pin_PULSOMON2								PIND3
#define pin_OPCION1									PIND4
#define pin_OPCION2									PIND5
#define pin_DOOR1									PINC2
#define pin_DOOR2									PINC3
#define pin_BTN_IZQ_P1								PINA2
#define pin_BTN_CEN_P1								PINA3
#define pin_BTN_DER_P1								PINA4
#define pin_BTN_IZQ_P2								PINA5
#define pin_BTN_CEN_P2								PINA6
#define pin_BTN_DER_P2								PINA7

/*Used to activate pull ups*/
#define writeReg_OPCION1			 				PORTD
#define writeReg_OPCION2				 			PORTD
#define writeReg_BTN_IZQ_P1				 			PORTA
#define writeReg_BTN_CEN_P1				 			PORTA
#define writeReg_BTN_DER_P1				 			PORTA
#define writeReg_BTN_IZQ_P2				 			PORTA
#define writeReg_BTN_CEN_P2				 			PORTA
#define writeReg_BTN_DER_P2				 			PORTA
#define writeReg_DOOR1								PORTC
#define writeReg_DOOR2								PORTC

/*ADC channels*/
#define pin_THERMISTOR1								PINA0
#define pin_THERMISTOR2								PINA1
/***************************************************/

/********************* Outputs **********************/
#define configReg_CTRL_BUZZER1			 			DDRB
#define configReg_CTRL_IGNITOR1			 			DDRD
#define configReg_CTRL_MOTORDER1			 		DDRD
#define configReg_CTRL_MOTORIZQ1			 		DDRC
#define configReg_CTRL_VENTILADOR1			 		DDRC
#define configReg_CTRL_VENTILADOR2			 		DDRC
#define configReg_CTRL_MOTORIZQ2			 		DDRC
#define configReg_CTRL_MOTORDER2			 		DDRC
#define configReg_CTRL_IGNITOR2				 		DDRC
#define configReg_CTRL_BUZZER2				 		DDRB

#define writeReg_CTRL_BUZZER1			 			PORTB
#define writeReg_CTRL_IGNITOR1			 			PORTD
#define writeReg_CTRL_MOTORDER1			 			PORTD
#define writeReg_CTRL_MOTORIZQ1			 			PORTC
#define writeReg_CTRL_VENTILADOR1			 		PORTC
#define writeReg_CTRL_VENTILADOR2			 		PORTC
#define writeReg_CTRL_MOTORIZQ2				 		PORTC
#define writeReg_CTRL_MOTORDER2				 		PORTC
#define writeReg_CTRL_IGNITOR2				 		PORTC
#define writeReg_CTRL_BUZZER2				 		PORTB

#define pin_CTRL_BUZZER1			 				PINB7
#define pin_CTRL_IGNITOR1				 			PIND6
#define pin_CTRL_MOTORDER1				 			PIND7
#define pin_CTRL_MOTORIZQ1				 			PINC0
#define pin_CTRL_VENTILADOR1				 		PINC1
#define pin_CTRL_VENTILADOR2				 		PINC4
#define pin_CTRL_MOTORIZQ2					 		PINC5
#define pin_CTRL_MOTORDER2					 		PINC6
#define pin_CTRL_IGNITOR2				 			PINC7
#define pin_CTRL_BUZZER2				 			PINB4

/*74595 */
#define configReg_MR_DISPLAY				 		DDRB
#define configReg_LATCH_DISPLAY				 		DDRB
#define configReg_SER_DAT_DISPLAY					DDRB
#define configReg_SRCLK_DISPLAY						DDRB

#define writeReg_MR_DISPLAY				 			PORTB
#define writeReg_LATCH_DISPLAY				 		PORTB
#define writeReg_SER_DAT_DISPLAY				 	PORTB
#define writeReg_SRCLK_DISPLAY					 	PORTB

#define pin_MR_DISPLAY				 				PINB0
#define pin_LATCH_DISPLAY				 			PINB1
#define pin_SER_DAT_DISPLAY				 			PINB2
#define pin_SRCLK_DISPLAY				 			PINB3
/*********************************************************/



#endif /* PORT_H_ */