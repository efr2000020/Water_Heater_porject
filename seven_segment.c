#include "water_heater.h"


void Seven_Segment_vDisplay_Temperature(void)
{
  const uint8_t segments_code[10]  = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};
  uint8_t temprature;
  uint8_t most_significant_figure  ;
  uint8_t least_significant_figure ;
  uint8_t timing_condition = WAIT ;
  static uint8_t blinking_flag =0 ;

  if(initialization_state_G == 0 && Temperature_Setting_Mode_G == 0)
  {
    temprature = avarage_temperature_G;
    blinking_flag = 0;
    timing_condition = WAIT;
  }
  else if (Temperature_Setting_Mode_G == 1)
  {
    temprature = set_temperature_G;
    timing_condition = Water_Heater_uTick_counter(TICK_COUNTER_SSD_ID,200,SCHEDULER_TICKS_SSD);
  }
  else
  {
    temprature = 0;
    return;
  }
  if (temprature>99)
  {
    temprature = 99;
  }
  most_significant_figure  = temprature/10;
  least_significant_figure = temprature%10;

  if (timing_condition == GO)
  {
    blinking_flag++;
    CLRBIT(SEVEN_SEGMENT_ENABLE_DISPLAY_1_STATE_REG, SEVEN_SEGMENT_ENABLE_DISPLAY_1_BIT);
    CLRBIT(SEVEN_SEGMENT_ENABLE_DISPLAY_2_STATE_REG, SEVEN_SEGMENT_ENABLE_DISPLAY_2_BIT);
    if(blinking_flag>1)
    {
      blinking_flag = 0;
      timing_condition = WAIT;
    }
  }
  if(timing_condition == WAIT && blinking_flag==0)
  {
    SEVEN_SEGMENT_DATA_STATE_REGISTER = segments_code[least_significant_figure];
    CLRBIT(SEVEN_SEGMENT_ENABLE_DISPLAY_1_STATE_REG, SEVEN_SEGMENT_ENABLE_DISPLAY_1_BIT);
    SETBIT(SEVEN_SEGMENT_ENABLE_DISPLAY_2_STATE_REG, SEVEN_SEGMENT_ENABLE_DISPLAY_2_BIT);
    __delay_ms(15);

    SEVEN_SEGMENT_DATA_STATE_REGISTER = segments_code[most_significant_figure];
    SETBIT(SEVEN_SEGMENT_ENABLE_DISPLAY_1_STATE_REG, SEVEN_SEGMENT_ENABLE_DISPLAY_1_BIT);
    CLRBIT(SEVEN_SEGMENT_ENABLE_DISPLAY_2_STATE_REG, SEVEN_SEGMENT_ENABLE_DISPLAY_2_BIT);
    __delay_ms(5);
  }
}

void Seven_Segment_vInit()
{
    CLR_REGISTER_8BIT(SEVEN_SEGMENT_DATA_DIRECTION_REGISTER);
    CLRBIT(SEVEN_SEGMENT_ENABLE_DISPLAY_1_DIR_REG, SEVEN_SEGMENT_ENABLE_DISPLAY_1_BIT);
    CLRBIT(SEVEN_SEGMENT_ENABLE_DISPLAY_2_DIR_REG, SEVEN_SEGMENT_ENABLE_DISPLAY_2_BIT);

    CLRBIT(SEVEN_SEGMENT_ENABLE_DISPLAY_1_STATE_REG, SEVEN_SEGMENT_ENABLE_DISPLAY_1_BIT);
    CLRBIT(SEVEN_SEGMENT_ENABLE_DISPLAY_2_STATE_REG, SEVEN_SEGMENT_ENABLE_DISPLAY_2_BIT);
}


/*
void Seven_Segment_vUpdate(uint8_t temprature) // if the the type selected in less then 1 second in blinking mode it will be 1 second automatically
{
    static uint16_t tick_counter = 0;
    static uint8_t  tick_counter_flag = 0;
    uint16_t ticks_threshold = 1000/(SHCEDULER_TICK_DURATION_MILLISEC*2);
    if (temprature>100)
    {
        temprature=99;
    }


    if(Temperature_Setting_Mode_G)
    {
      if(tick_counter>=0 && tick_counter<ticks_threshold)
      {
        Seven_Segment_vDisplay_Figures(segments_code[most_significant_figure], segments_code[least_significant_figure]);
        tick_counter++;
      }
      else if(tick_counter>=ticks_threshold && tick_counter<ticks_threshold*2)
      {
        Seven_Segment_vDisplay_Figures(0x00,0x00);
        tick_counter++;
      }
      else
      {
        tick_counter = 0;
      }
    }
    else
    {
      Seven_Segment_vDisplay_Figures(segments_code[most_significant_figure], segments_code[least_significant_figure]);
      tick_counter = 0;
    }
}
*/
