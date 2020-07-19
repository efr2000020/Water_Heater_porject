 /********************************************//**
 * @file main.c
 * @brief This file contains the main Water Heater program function and the ISR funciton.\n
 * All other functions, global variables used are defined in their respective file.
 * @authors
 * Khaled Mohsen    khaledmohsen1400@gmail.com\n
 * Omar Mokhtar     Omar.Mokhtar1@msa.edu.eg\n
 * Mohamed Yousry   Mohamed.Yousry5@msa.edu.eg\n
 * @date 17th July 2020
 *
 * @see Our GitHub Repo for this porject \n https://github.com/efr2000020/Water_Heater_porject.git 
 /********************************************/

 /********************************************//**
 ________________[PIC SIM LAB PINS]______________________
 User manual (Electric Water Heater)

 RB4  =>  UP button   (temp+)
 RB5  =>  Down button (temp-)

 RB0  =>  controller on/off button

 B2  =>  Cooler/Heater blinking LED

 /********************************************/



#include "water_heater.h"



int main(void)
{


    I2C_Master_Init(100000);
    Water_Heater_vRetrieve_Temperature();
    Seven_Segment_vInit();
    ADC_vInit();
    Water_Heater_vInit_F_H();
    Water_Heater_vTick_counter_Init();


    Scheduler_vInit_t1();
    Scheduler_uAdd_Task(Water_Heater_vTemperture_Setting_Handeller,0,SCHEDULER_TICKS_SETTING_HANDLER);
    Scheduler_uAdd_Task(Water_Heater_vRead_Avarage_Temperature,0,SCHEDULER_TICKS_TEMP_READING);
    Scheduler_uAdd_Task(Water_Heater_vAdjust_temperature,0,SCHEDULER_TICKS_TEMP_ADJUSTING);
    Scheduler_uAdd_Task(Seven_Segment_vDisplay_Temperature,0,SCHEDULER_TICKS_SSD);
    Scheduler_uAdd_Task(Water_Heater_vUpdate_LED,0,SCHEDULER_TICKS_LED);

    Scheduler_vStart();
    Water_Heater_vInit_Ext_Interrupts();
    while (1)
    {
      if(Water_Heater_Halt_Flag_G == 1)
      {
        Water_Heater_vTurn_OFF();
      }
      Scheduler_vDispatch_Tasks();
    }
}

void __interrupt() ISR(void)
{
  if(INTF==1)
  {
    if(Water_Heater_Halt_Flag_G == 0)
    Water_Heater_Halt_Flag_G = 1;
    else
    Water_Heater_Halt_Flag_G = 0;
    INTF = 0;
  }
  Water_Heater_vTemperture_Setting_Handeller();
  Scheduler_vUpdate();
}
