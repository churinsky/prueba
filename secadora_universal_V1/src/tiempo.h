#ifndef TIEMPO_H
#define TIEMPO_H

	#define BORRAR 		1
	#define NOBORRAR 	0
	void tiempo_Retardo(unsigned long retardo);
	void virutalTimers_Inc(void);
	void ini_VirtualTimers(void);
	unsigned char new_VirtualTimer(unsigned char numero,unsigned int tiempo_a_comparar);
	unsigned char tiempo_Compara(unsigned char numero,unsigned char borrar);
	void tiempo_Liberar(unsigned char i);	
	void reset_VirtualTimer(unsigned char i);
	uint16_t getTime_VirtualTimer(uint8_t timer);
	void stop_VirtualTimer(unsigned char timer);

#endif
