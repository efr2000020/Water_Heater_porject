/********************************************//**
* @file seven_segment.h
*
* @brief this file is the module of the Seven Segment diplay dirvers. it has only two functions to start and operate the diplays.
/********************************************/

#define ON          1
#define OFF         0

/********************************************//**
 *Initializes the pins connected to the EEPROM as output accoring to the pin configureation in the file MCU_config.h
 *@param void
  *******************************************/
void Seven_Segment_vInit(void);

/********************************************//**
 *This function has two modes in which it is active:\n
 *1- Temperature Setting mode, active wehn temperature_mode_G = 1\n
 *in this mode the seven segments displays are blinking once every 1 second and show the current set temperature\n\n
 *2- Normal operation mode, avtive when Temperature_Setting_Mode_G==0 and initialization_state_G==0:\n
 *in this mode the displays are active and updating the temperature values every 100 milliseconds
 *@param void
  *******************************************/
void Seven_Segment_vDisplay_Temperature(void);
