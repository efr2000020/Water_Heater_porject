/********************************************//**
* @file water_heater.h
*
* @brief This is the main header file which inlcludes all the files in the project.
*
* This file:\n
* 1- includes the modules of all the headers in the project.\n
* 2- all the MACROS.\n
* 3- all the Global variables.\n
* 4- Compiler and MCU configureation.
/********************************************/

#include "config.h"
#include "MCU_config.h"
#include "STD_macros.h"
#include "seven_segment.h"
#include "scheduler.h"
#include "ADC.h"
#include "scheduler_error_list.h"
#include "EEPROM_I2C.h"




//#define SCH_MAX_TASKS 7

////////////////////////
#define SHCEDULER_TICK_DURATION_MILLISEC 20


#define _XTAL_FREQ 8000000

////////////////////////

#define TEMPERATURE_SENSOR_PIN 2

#define TICK_COUNTER_NUMBER_OF_TASKS 3

#define TICK_COUNTER_LED_ID 0
#define TICK_COUNTER_SSD_ID 1
#define TICK_COUNTER_TEMP_SETTING_ID 2



#define SEVEN_SEGMENT_ON 1
#define SEVEN_SEGMENT_BLINK 2

#define GO 1
#define WAIT 0

#define STOPPED 0
#define COOLING 1
#define HEATING 2

#define SCHEDULER_TICKS_TEMP_READING 5
#define SCHEDULER_TICKS_TEMP_ADJUSTING 5
#define SCHEDULER_TICKS_SSD 1
#define SCHEDULER_TICKS_SETTING_HANDLER 1
#define SCHEDULER_TICKS_LED 1

uint8_t Temperature_Setting_Mode_G = 0 ;
uint8_t Water_Heater_Halt_Flag_G = 1;
uint8_t initialization_state_G = 1;
uint8_t avarage_temperature_G = 0;
uint8_t set_temperature_G ;
uint8_t temperature_mode_G = 0;
uint8_t Temperature_Setting_Seconds_Counter_G = 0;
uint8_t old_PORTB = 0;
uint8_t Water_Heater_Halt_Mode_g =0;
uint16_t EEPROM_Temperature_Adress_G = 0x0000;
uint16_t EEPROM_flag_Adress_G = 0x0004;
uint32_t Timer_Register_Value_G = 0;

#define PORTB_4    (1<<4)
#define PORTB_5    (1<<5)
#define PORTB_6    (1<<6)
#define PORTB_7    (1<<7)




/********************************************//**
 *Get the stored set temperature from the EEPROM if any. Hoverever, temperature was stored in the EEPROM, the programs stores the default temperature value in the EEPROM
 *@param void
  *******************************************/
void Water_Heater_vRetrieve_Temperature(void);

/********************************************//**
 *Initializes The External interrupt as well as the PORT Change interrupt
 *@param void
  *******************************************/
void Water_Heater_vInit_Ext_Interrupts(void);

/********************************************//**
 *Turns off all the input output operations to HALT(freeze), alo prevents  Water_Heater_vRead_Avarage_Temperature(); from calculating or updaing the temperature value
 *@param void
  *******************************************/
void Water_Heater_vTurn_OFF(void);


/********************************************//**
 *This functions turns the Fan On/OFF
 *@param condition
 *use the macro ON to turn fan on.\n
 *use the macro OFF to turn fan off.
  *******************************************/
void Water_Heater_vSwitch_Fan(uint8_t condition);

/********************************************//**
 *This functions turns the Heating Element On/OFF
 *@param condition
 *use the macro ON to turn Heating Element on.\n
 *use the macro OFF to turn Heating Element off.
  *******************************************/
void Water_Heater_vSwitch_Heater(uint8_t condition);

/********************************************//**
 *This functions turns the LED On/OFF
 *@param condition
 *use the macro ON to turn LED on.\n
 *use the macro OFF to turn LED off.
  *******************************************/
void Water_Heater_vSwitch_LED(uint8_t condition);

/********************************************//**
 *Initalize the Heating Element, Fan, and LED Pins
 *as outputs according to the pin configureation
 in the file MCU_config.h
 *@param void
 *use the macro ON to turn Heating Element on.\n
 *use the macro OFF to turn Heating Element off.
  *******************************************/
void Water_Heater_vInit_F_H(void);

/********************************************//**
 *This function is responsible for getting the ADC readings
 *and calculate the current temperature based on the Avarage
 *Value of ADC readings in the last 10 times it was called.
 *This function has 4 operation modes according to the program statues:\n\n

 *1- initialization state, active when initialization_state_G variable = 1:\n
 *in this mode the function reads only the incoming ADC Values withoud any clalculations being done for the fisrt 10 times.\n\n

 *2- Normal operation State, avtibe when initialization_state_G=0 and temperature_mode_G =0:\n
 *In this mode the function takes each new reading and calculates the avarage temperature according to the last 10 readings.\n\n

 *3- Temperature Setting Mode,active when the Temperature_Setting_Mode_G variable =1:\n
 *in this mode the function stops working until the Temperature_Setting_Mode_G=0 or the Temperature_Setting_Seconds_Counter_G > 4\n\n

 *4- Water Heater off mode, active when Water_Heater_Halt_Mode_g =1:\n
 in this mode the functions is called once only to reset the stored temperature values from before the halt of the program.
 *@param void Only called by scheduler
  *******************************************/
void Water_Heater_vRead_Avarage_Temperature(void);

/********************************************//**
 *Takes temperature Values from the Water_Heater_vRead_Avarage_Temperature(); function and turns the Heater ON/FF and the Fan OFF/ON
 *accoring to the specified range of temperature which is set_temperature_G +-5 degrees.\n
 *also it is responsible for setting the temperature_mode_G variable accoring the Water Heater statues of COOLING/Heating
 *@param void Only called by scheduler
  *******************************************/
void Water_Heater_vAdjust_temperature(void);

/********************************************//**
 *Turns LED ON when COOLING , Blinking when HEATING and OFF when Temperature Setting
 *@param void Only called by scheduler
  *******************************************/
void Water_Heater_vUpdate_LED(void);


/********************************************//**
 *This function is only active at when Temperature_Setting_Mode_G =1 or called from directly from the ISR. it detects keys pressed to change the set temprature
 *with respect to the maximum and minumum limits of temperature values, also stops the following functions from working when it is active by Setting temperature_mode_G = 1
 *@param void Only called by Scheduler or ISR
  *******************************************/
void Water_Heater_vTemperture_Setting_Handeller(void);

/********************************************//**
 *Initializes the static Variables inside the Water_Heater_uTick_counter(uint8_t task_id, uint16_t duration_ms, uint8_t number_of_ticks); function
 *@param void
  *******************************************/
void Water_Heater_vTick_counter_Init(void);

/********************************************//**
 *generates sort of a delay for scheduler functions in such a way that certain commands inside the scheduler task are executed ever certain number of ticks\n
 *Returns the macros WAIT/GO to stop/start the commands inside the scheduler task
 *@param task_id ->a unique number given to each task\n
 *@param duration_ms -> the desired delay in milliseconds\n
 *@param number_of_ticks -> the number of scheduler ticks set to this task
  *******************************************/
uint8_t Water_Heater_uTick_counter(uint8_t task_id, uint16_t duration_ms, uint8_t number_of_ticks);
