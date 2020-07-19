#include "water_heater.h"

void ADC_vInit(void)
{
    ADCON0 |= 0x41;
    ADCON1 |= 0xC0;
}
uint16_t ADC_uRead(char pin)
{
    TRISA |= (1<<pin);
    ADCON0 &= 0xC5;
    ADCON0 |= pin<<3;
    __delay_ms(2);
    GO_nDONE = 1;
    while(GO_nDONE);
    return ((ADRESH<<8)+ADRESL);
}
