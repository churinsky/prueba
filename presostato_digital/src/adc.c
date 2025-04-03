#include "adc.h"

void adc_init(void)
{
    ADMUX=(1<<REFS0);//referencia avcc
    ADCSRA=(1<<ADEN)|(1<<ADPS0)|(1<<ADPS1)|(1<<ADPS2);//hablitita adc, prescaler 111 pra125khz
}
uint16_t adc_read(uint8_t canal)
{
    canal&=0x07;//proteccionpara no pasar lso 8bits paestemicro del adc
    ADMUX=(ADMUX&0xF8)|canal;// conservo la configuracion de admux y agrego el canal a utilizar
    ADCSRA|=(1<<ADSC);// se activa el bit para iniciar la converison
    while (ADCSRA&(1<<ADSC));// se espera hasta que el bit se ponga en cero para terminar la conversion
    return(ADCL|(ADCH<<8));//resultado
 
}
