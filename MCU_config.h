/********************************************//**
* @file MCU_config.h
*
* @brief this file includes pin configureations of the MCU used in this project, note that Registers can be changed here without chaning the main program
********************************************/


#define SEVEN_SEGMENT_DATA_DIRECTION_REGISTER TRISD
#define SEVEN_SEGMENT_DATA_STATE_REGISTER PORTD

#define SEVEN_SEGMENT_ENABLE_DISPLAY_1_BIT 3
#define SEVEN_SEGMENT_ENABLE_DISPLAY_1_DIR_REG TRISA
#define SEVEN_SEGMENT_ENABLE_DISPLAY_1_STATE_REG PORTA

#define SEVEN_SEGMENT_ENABLE_DISPLAY_2_BIT 4
#define SEVEN_SEGMENT_ENABLE_DISPLAY_2_DIR_REG TRISA
#define SEVEN_SEGMENT_ENABLE_DISPLAY_2_STATE_REG PORTA

#define HEATER_BIT 5
#define HEATER_DIR_REG TRISC
#define HEATER_STATE_REG PORTC

#define FAN_BIT 2
#define FAN_DIR_REG TRISC
#define FAN_STATE_REG PORTC

#define LED_BIT 2
#define LED_DIR_REG TRISB
#define LED_STATE_REG PORTB
