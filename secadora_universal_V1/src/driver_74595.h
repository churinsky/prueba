/*
 * driver_74595.h
 *
 * Created: 2/20/2025 10:43:53 AM
 *  Author: Investigacion
 */ 


#ifndef DRIVER_74595_H_
#define DRIVER_74595_H_
void sendDato(void *dato, uint8_t lenght_Data, volatile uint8_t *portMR, uint8_t pinMR, volatile uint8_t *portLATCH, uint8_t pinLATCH, volatile uint8_t *portSERDAT, uint8_t pinSERDAT, volatile uint8_t *portSRCLK, uint8_t pinSRCLK);
void writeBit(uint8_t bitValue,volatile uint8_t *portSERDAT, uint8_t pinSERDAT, volatile uint8_t *portSRCLK, uint8_t pinSRCLK);
void showData(volatile uint8_t *portLATCH, uint8_t pinLATCH);
void clear_Data_74595(volatile uint8_t *portMR, uint8_t pinMR, volatile uint8_t *portSRCLK, uint8_t pinSRCLK );



#endif /* DRIVER_74595_H_ */