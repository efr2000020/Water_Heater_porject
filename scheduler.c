#include "water_heater.h"


uint8_t Scheduler_uAdd_Task(void (*pFunction)(),const uint16_t DELAY,const uint16_t PERIOD)
{
  /** #Index. 1 */
  uint8_t Index = 0;
  // First find a gap in the array (if there is one)
  while ((Scheduler_Tasks_G[Index].pTask != 0) && (Index < MAXIMUM_NUMBER_OF_TASKS))
  {
    Index++;
  }
  // Have we reached the end of the list?
  if (Index == MAXIMUM_NUMBER_OF_TASKS)
  {
    // Task list is full
    //
    // Set the global error variable
    Error_code_G = ERROR_SCH_TOO_MANY_TASKS;
    // Also return an error code
    return MAXIMUM_NUMBER_OF_TASKS;
  }
  // If we're here, there is a space in the task array
  Scheduler_Tasks_G[Index].pTask = pFunction;
  Scheduler_Tasks_G[Index].Delay = DELAY;
  Scheduler_Tasks_G[Index].Period = PERIOD;
  Scheduler_Tasks_G[Index].RunTask = 0;
  return Index; // return position of task (to allow later deletion)
}


void Scheduler_vInit_t1(void)
{
  Timer_Register_Value_G = 65536-((SHCEDULER_TICK_DURATION_MILLISEC *_XTAL_FREQ)/(4*4*1000));
  // -- [[  IO Configurations ]] --
  TRISB0 = 0;
  RB0 = 0;
  //-- [[ Configure Timer1 To Operate In Timer Mode  ]] --
  //Preload The Value Which We've Calculated To The TMR1 16-Bit Register!
  TMR1 = Timer_Register_Value_G;
  // Choose the local clock source (timer mode)
  TMR1CS = 0;
  // Choose the desired prescaler ratio (1:1)
  T1CKPS0 = 0;
  T1CKPS1 = 1;
  // Event1 = LED ON
  RB0 = 1;
  // Switch ON Timer1 Module!
  TMR1ON = 1;
  // -- [[ Interrupts Configurations ]] --
  TMR1IE = 1; // Timer1 Interrupt Enable Bit
  TMR1IF = 0; // Clear The Interrupt Flag Bit
}

void Scheduler_vStart(void)
{
  PEIE = 1;   // Peripherals Interrupts Enable Bit
  GIE = 1;    // Global Interrupts Enable Bit
}


void Scheduler_vUpdate(void)
{
  TMR1IF = 0; // Clear The Flag Bit
  TMR1 = Timer_Register_Value_G;
  uint8_t Index;
  // NOTE: calculations are in *TICKS* (not milliseconds)
  for (Index = 0; Index < MAXIMUM_NUMBER_OF_TASKS; Index++)
  {
    // Check if there is a task at this location
    if (Scheduler_Tasks_G[Index].pTask)
    {
      if (Scheduler_Tasks_G[Index].Delay == 0)
      {
        // The task is due to run
        Scheduler_Tasks_G[Index].RunTask += 1; // Inc. the 'RunTask' flag
        if (Scheduler_Tasks_G[Index].Period)
        {
          // Schedule periodic tasks to run again
          Scheduler_Tasks_G[Index].Delay = Scheduler_Tasks_G[Index].Period;
        }
      }
      else
      {
        // Not yet ready to run: just decrement the delay
        Scheduler_Tasks_G[Index].Delay -= 1;
      }
    }
  }
}

void Scheduler_vDispatch_Tasks(void)
{
  uint8_t Index;
  // Dispatches (runs) the next task (if one is ready)
  for (Index = 0; Index < MAXIMUM_NUMBER_OF_TASKS; Index++)
  {
    if (Scheduler_Tasks_G[Index].RunTask > 0)
    {
      (*Scheduler_Tasks_G[Index].pTask)(); // Run the task
      Scheduler_Tasks_G[Index].RunTask -= 1; // Reset / reduce RunTask flag
      // Periodic tasks will automatically run again
      // - if this is a 'one shot' task, remove it from the array
      if (Scheduler_Tasks_G[Index].Period == 0)
      {
        Scheduler_Delete_Task(Index);
      }
    }
  }
  // Report system status
  //Scheduler_vReport_Status();
  // The scheduler enters idle mode at this point
  //SCH_Go_To_Sleep();
}

uint8_t Scheduler_Delete_Task(const uint8_t TASK_INDEX)
{
  uint8_t Return_code;

  if (Scheduler_Tasks_G[TASK_INDEX].pTask == 0)
    {
      // No task at this location...
      //
      // Set the global error variable
      Error_code_G = ERROR_SCH_CANNOT_DELETE_TASK;

      // ...also return an error code
      Return_code = RETURN_ERROR;
    }
    else
    {
      Return_code = RETURN_NORMAL;
    }

    Scheduler_Tasks_G[TASK_INDEX].pTask   = 0x0000;
    Scheduler_Tasks_G[TASK_INDEX].Delay   = 0;
    Scheduler_Tasks_G[TASK_INDEX].Period  = 0;

    Scheduler_Tasks_G[TASK_INDEX].RunTask   = 0;

    return Return_code;       // return status
}
