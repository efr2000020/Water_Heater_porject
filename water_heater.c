#include "water_heater.h"


void Water_Heater_vSwitch_Fan(uint8_t condition)
{
    if(condition==ON)
    SETBIT(FAN_STATE_REG,FAN_BIT);
    else
    CLRBIT(FAN_STATE_REG,FAN_BIT);
}

void Water_Heater_vSwitch_Heater(uint8_t condition)
{
  if(condition==ON)
  SETBIT(HEATER_STATE_REG,HEATER_BIT);
  else
  CLRBIT(HEATER_STATE_REG,HEATER_BIT);
}

void Water_Heater_vInit_F_H(void)
{
  CLRBIT(HEATER_DIR_REG,HEATER_BIT);
  CLRBIT(FAN_DIR_REG,FAN_BIT);
  CLRBIT(LED_DIR_REG,LED_BIT);
}

void Water_Heater_vSwitch_LED(uint8_t condition)
{
  if(condition==ON)
  SETBIT(LED_STATE_REG,LED_BIT);
  else
  CLRBIT(LED_STATE_REG,LED_BIT);
}


void Water_Heater_vRead_Avarage_Temperature(void)
{
  if (Temperature_Setting_Mode_G == 1 && Temperature_Setting_Seconds_Counter_G < 4)
  {
    return;
  }
  static uint16_t ADC_readings[10] = {0,0,0,0,0,0,0,0,0,0};
  static uint8_t  ADC_readings_index = 0;
  static uint16_t ADC_readings_sum;

  uint16_t ADC_current_reading;
  uint16_t ADC_oldest_reading;

  if(Water_Heater_Halt_Mode_g>0)
  {
    Water_Heater_Halt_Mode_g =0;
    ADC_readings_index =0;
    ADC_readings_sum =0;
  }
  if(initialization_state_G == 0)
  {
    ADC_oldest_reading = ADC_readings[ADC_readings_index];
    ADC_readings_sum -= ADC_oldest_reading;
  }

  ADC_current_reading = ADC_uRead(TEMPERATURE_SENSOR_PIN);
  ADC_readings[ADC_readings_index] = ADC_current_reading;
  ADC_readings_sum += ADC_current_reading;
  ADC_readings_index++;

  if(ADC_readings_index>9)
  {
    if(initialization_state_G == 1)
    {
      initialization_state_G = 0;
    }
    ADC_readings_index = 0;
  }

  avarage_temperature_G = (((ADC_readings_sum*1.0)*4.8828125)/100.0); // consider update (1)
}

void Water_Heater_vAdjust_temperature(void)
{
  if(initialization_state_G == 0 && Temperature_Setting_Mode_G==0)
  {
    if(avarage_temperature_G>= (set_temperature_G+5))
    {
      Water_Heater_vSwitch_Fan(ON);
      Water_Heater_vSwitch_Heater(OFF);
      temperature_mode_G = COOLING;
    }
    if(avarage_temperature_G<= (set_temperature_G-5))
    {
      Water_Heater_vSwitch_Heater(ON);
      Water_Heater_vSwitch_Fan(OFF);
      temperature_mode_G = HEATING;
    }
  }
  if (initialization_state_G == 0 && Temperature_Setting_Mode_G==1)
  {
    Water_Heater_vSwitch_Heater(OFF);
    Water_Heater_vSwitch_Fan(OFF);
    temperature_mode_G = OFF;
  }
}
uint8_t Water_Heater_uTick_counter(uint8_t task_id, uint16_t duration_ms, uint8_t number_of_ticks)
{
  static  uint16_t target_count[TICK_COUNTER_NUMBER_OF_TASKS];
  static  uint16_t current_count[TICK_COUNTER_NUMBER_OF_TASKS];

  if (duration_ms == 0)
  {
    current_count[task_id]=0;
    return WAIT;
  }

  target_count[task_id] = duration_ms/(number_of_ticks*SHCEDULER_TICK_DURATION_MILLISEC);
  if(target_count[task_id]<=1)
  {
    return GO;
  }
  current_count[task_id] ++;

  if (current_count[task_id]>=target_count[task_id])
  {
    current_count[task_id]=0;
    return GO;
  }
  return WAIT;
}

void Water_Heater_vTick_counter_Init(void)
{
  for(uint8_t i=0;i<TICK_COUNTER_NUMBER_OF_TASKS;i++)
  {
    Water_Heater_uTick_counter(i,0,0);
  }
}

void Water_Heater_vUpdate_LED(void)
{
  uint8_t timing_condition = WAIT;
  if(initialization_state_G==1 || temperature_mode_G==OFF)
  {
    Water_Heater_vSwitch_LED(OFF);
  }
  else if(temperature_mode_G == HEATING)
  {
    timing_condition = Water_Heater_uTick_counter(TICK_COUNTER_LED_ID,200,SCHEDULER_TICKS_LED);
    if(timing_condition == GO)
    {
      TOGBIT(LED_STATE_REG,LED_BIT);
    }
   }
  else if(temperature_mode_G == COOLING)
  {
    SETBIT(LED_STATE_REG,LED_BIT);
  }

}

void Water_Heater_vTemperture_Setting_Handeller(void)
{
  static uint8_t temperature_setting_flag = 0;
  uint8_t timing_condition = WAIT;
  uint8_t key_pressed;
  uint8_t new_PORTB = PORTB;

  if (temperature_setting_flag==0 && RBIF==0)
  {
    // not in temprature setting mode
    return;
  }

  if(temperature_setting_flag==0 && RBIF==1)
  {
    temperature_setting_flag = 1;
    Temperature_Setting_Mode_G = 1;

    new_PORTB = PORTB;
    RBIF=0;
    return;
  }
  if(temperature_setting_flag==1 && RBIF==1)
  {
    key_pressed =  old_PORTB ^ new_PORTB;

    if(key_pressed & PORTB_4)
    {
      set_temperature_G += 5;
    }
    else if(key_pressed & PORTB_5)
    {
      set_temperature_G -= 5;
    }
    else
    {
      // junk Interrupt
      temperature_setting_flag = 1;
      Temperature_Setting_Mode_G = 1;
    }
    if(set_temperature_G >= 65)
    {
      set_temperature_G = 65;
    }
    if(set_temperature_G <= 35)
    {
      set_temperature_G = 35;
    }
    Temperature_Setting_Seconds_Counter_G = 0;
    new_PORTB = PORTB;
    RBIF=0;
    return;
   }

   timing_condition = Water_Heater_uTick_counter(TICK_COUNTER_TEMP_SETTING_ID,1000,SCHEDULER_TICKS_SETTING_HANDLER);
   if(timing_condition == GO)
   {
     Temperature_Setting_Seconds_Counter_G ++;
   }
   if(Temperature_Setting_Seconds_Counter_G > 5)
   {
     //end temperature setting mode
     Temperature_Setting_Mode_G = 0;
     temperature_setting_flag = 0;
     Temperature_Setting_Seconds_Counter_G = 0;
     //CALL THE EEPROM TO STORE THE NEW SET TEMPERATURE
     EEPROM_Write(EEPROM_Temperature_Adress_G, set_temperature_G); // Write 0x04 @ 0x0020
   }
}

void Water_Heater_vRetrieve_Temperature(void)
{
  uint8_t first_time_flag = EEPROM_Read(EEPROM_flag_Adress_G);
  __delay_ms(10);

  if(first_time_flag != 0x01)
  {
  EEPROM_Write(EEPROM_flag_Adress_G, 0x01);
  __delay_ms(10);
  EEPROM_Write(EEPROM_Temperature_Adress_G, 0x3C);
  __delay_ms(10);
  set_temperature_G = 60;
  }
  else
  {
    set_temperature_G = EEPROM_Read(EEPROM_Temperature_Adress_G);
    __delay_ms(10);
  }
}

void Water_Heater_vInit_Ext_Interrupts(void)
{
  SETBIT(TRISB,5);
  SETBIT(TRISB,4);

  old_PORTB = PORTB;
  RBIF =0;
  CLR_REGISTER_8BIT(OPTION_REG);
  INTCON |= 0b00011000;

  SETBIT(TRISB,0);
  INTEDG = 0;
  INTF = 0;
  INTE = 1;

//  GIE = 1;
}

void Water_Heater_vTurn_OFF(void)
{
  Water_Heater_Halt_Mode_g = 1;
  initialization_state_G = 1;
  Temperature_Setting_Mode_G = 0;
  Temperature_Setting_Seconds_Counter_G =0;

  CLRBIT(SEVEN_SEGMENT_ENABLE_DISPLAY_1_STATE_REG, SEVEN_SEGMENT_ENABLE_DISPLAY_1_BIT);
  CLRBIT(SEVEN_SEGMENT_ENABLE_DISPLAY_2_STATE_REG, SEVEN_SEGMENT_ENABLE_DISPLAY_2_BIT);
  Water_Heater_vSwitch_Heater(OFF);
  Water_Heater_vSwitch_Fan(OFF);
  Water_Heater_vSwitch_LED(OFF);
  while (Water_Heater_Halt_Flag_G != 0)
  {
    // Halt
  }
}
