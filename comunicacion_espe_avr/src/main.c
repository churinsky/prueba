#include "uart.h"
#include "protocol.h"
#include <util/delay.h>
// estado de maquina 
EstadoCiclo maquina =
    {
        .tipo_msg = 2,
        .id = 10,
        .etapa = 1,
        .paso = 2,
        .tiempo = 28,
        .temperatura = 1};

//ejemplo
CmdStatus estado =
    {
        .tipo_msg = CMD_STATUS};

int main(void)
{
    //inicializacion UART
    uart_init(9600);

    while (1)
    {
        /* ejemplo para enviar cosas
        uart_send_message((uint8_t*)&estado, sizeof(estado));
        _delay_ms(500);
        */
    }
    return 0;
}
