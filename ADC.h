/********************************************//**
* @file ADC.h
*
* @brief this file includes two functions for the ADC module.
********************************************/

/********************************************//**
* Initializes ADC in the MCU
*
* @param void
********************************************/
void ADC_vInit(void);

/********************************************//**
* Reads a certain ADC channel's value and reutrns the result as unsigned intger
*
* @param pin -> the Desired ADC pin
********************************************/
uint16_t ADC_uRead(char pin);
