#define BORRAR 		1
#define NOBORRAR 	0
#define NUMBER_OF_VIRTUALTIMERS		7
//Definimos una estructura de datos
typedef struct
{
	unsigned char numero;
	unsigned int tiempo;				//tiempo en ms del timer, si esta corriendo
	unsigned int tiempo_a_comparar;		//0xFFFF Reservedo = No se requiere comparar
	unsigned corre :1;					//0x0 = Apagado; 0x1 = Corriendo
	unsigned tiempos_iguales :1;		//Comparaci�n igual: No = 0x0; Si = 0x1
} tiempos;
static volatile tiempos tiempo[NUMBER_OF_VIRTUALTIMERS];

void ini_VirtualTimers(void)
{
	unsigned char i;
	
	for (i=0;i<NUMBER_OF_VIRTUALTIMERS;i++)
	{ 
		tiempo[i].numero = 0xFF;
		tiempo[i].tiempo = 0;
		tiempo[i].tiempo_a_comparar = 0xFFFF;
		tiempo[i].corre = 0;
		tiempo[i].tiempos_iguales = 0;
	 }
}

// activa el temporizador numero
// si numero es 0xFF entonces tratar� de asignar un temporizador a numero
// si este esta disponible regresa 0x00 de otra forma devuelve 0xFF
// si no se requiere comparaci�n de tiempo asigne 0xFFFF
unsigned char new_VirtualTimer(unsigned char numero,unsigned int tiempo_a_comparar)
{
	unsigned char i = 0;

	if (numero != 0xFF)
	{		
		while (i<NUMBER_OF_VIRTUALTIMERS)
		{ 
			if (tiempo[i].corre == 0)
				{
					tiempo[i].numero = numero;
					tiempo[i].tiempo = 0;
					tiempo[i].tiempo_a_comparar = tiempo_a_comparar;
					tiempo[i].corre = 1;
					tiempo[i].tiempos_iguales = 0;
					return 0x00;
				}
	 		i++;
	 	}
	}
	else
	{
		while (i<NUMBER_OF_VIRTUALTIMERS)
		{ 
			if (tiempo[i].corre == 0)
				{
					tiempo[i].numero = i;
					tiempo[i].tiempo = 0;
					tiempo[i].tiempo_a_comparar = tiempo_a_comparar;
					tiempo[i].corre = 1;
					tiempo[i].tiempos_iguales = 0;
					return i;
				}
	 		i++;
	 	}
	}
	return 0xFF;
}

// Revisa si se activo la bandera tiempos_iguales
// si es asi devuelve 0xFF de otra forma devuelve 0x00
unsigned char tiempo_Compara(unsigned char numero,unsigned char borrar)
{
	if ((tiempo[numero].tiempos_iguales)&&(tiempo[numero].numero == numero))
	{
		if (borrar)
			tiempo[numero].tiempos_iguales = 0;

		return 0xFF;
	}
	return 0x00;
}

// Libera el temporizador para que este libre para ser usado otra vez
void tiempo_Liberar(unsigned char i)
{	
	tiempo[i].numero = 255;
	tiempo[i].tiempo = 0;
	tiempo[i].tiempo_a_comparar = 0xFFFF;
	tiempo[i].corre = 0;
	tiempo[i].tiempos_iguales = 0;
}

void tiempo_Retardo(unsigned long retardo)
{
	unsigned char i;
	 
	i = new_VirtualTimer(0xFF,retardo);

	while ( !(tiempo_Compara(i,BORRAR)) );

	tiempo_Liberar(i);
}

// Incrementa el temporizador que este corriendo
void virutalTimers_Inc(void)
{
	unsigned char i;
	
	for (i=0;i<NUMBER_OF_VIRTUALTIMERS;i++)
	{ 
		if (tiempo[i].corre)
			{
				tiempo[i].tiempo+=1;
				if ((tiempo[i].tiempo >= tiempo[i].tiempo_a_comparar)&&(tiempo[i].tiempo_a_comparar != 0xFFFF))
				{
					tiempo[i].tiempos_iguales = 1;
					tiempo[i].tiempo = 0;
				}
			}	 	
	 } 
}

void reset_VirtualTimer(unsigned char i)
{
	tiempo[i].tiempo = 0;
}

unsigned int getTime_VirtualTimer(unsigned char timer)
{
	return tiempo[timer].tiempo;
}
void stop_VirtualTimer(unsigned char timer)
{
	tiempo[timer].corre = 0;
}