/*
 * driver_74595.c
 *
 * Created: 2/20/2025 10:38:49 AM
 *  Author: Investigacion
 */ 
#include <avr/io.h>
#include "driver_74595.h"




/*
uint8_t *dato : pointer to datos to be sending

*/
void sendDato(void *dato, uint8_t lenghData, volatile uint8_t *portMR, uint8_t pinMR, volatile uint8_t *portLATCH, uint8_t pinLATCH, volatile uint8_t *portSERDAT, uint8_t pinSERDAT, volatile uint8_t *portSRCLK, uint8_t pinSRCLK)
{
	clear_Data_74595(portMR, pinMR, portSRCLK, pinSRCLK);
	//*portMR |= (1 <<pinMR);
	uint8_t *myData = (uint8_t *)dato;
	for(int8_t i = (lenghData -1 )  ; i >= 0; i--)
	{
		for (uint8_t j = 0; j < 8; j++)
		{
			if(myData[i] & (1 << j) )
			{
				writeBit(1,portSERDAT,pinSERDAT,portSRCLK,pinSRCLK);
			}
			else
			{
				writeBit(0,portSERDAT,pinSERDAT,portSRCLK,pinSRCLK);
			}
		}
	}
	showData(portLATCH,pinLATCH);
}



void clear_Data_74595(volatile uint8_t *portMR, uint8_t pinMR, volatile uint8_t *portSRCLK, uint8_t pinSRCLK )
{
	*portMR &= ~(1 << pinMR);
	*portSRCLK |= (1 << pinSRCLK);
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	//_delay_ms(1);
	*portSRCLK &= ~(1 << pinSRCLK);
	*portMR |= (1 <<pinMR);
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
}
void writeBit(uint8_t bitValue,volatile uint8_t *portSERDAT, uint8_t pinSERDAT, volatile uint8_t *portSRCLK, uint8_t pinSRCLK)
{
	if(bitValue)
	{
		*portSERDAT |= (1 << pinSERDAT);
		*portSRCLK |= (1 << pinSRCLK);
		//_delay_ms(1);
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		*portSRCLK &= ~(1 << pinSRCLK);
		*portSERDAT &= ~(1 << pinSERDAT);
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
	}
	else
	{
		*portSRCLK |= (1 << pinSRCLK);
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		//_delay_ms(1);
		*portSRCLK &= ~(1 << pinSRCLK);
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
	}
}

void showData(volatile uint8_t *portLATCH, uint8_t pinLATCH)
{
	*portLATCH |= (1 << pinLATCH);
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	//_delay_ms(1);
	*portLATCH &= ~(1 << pinLATCH);
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
}



