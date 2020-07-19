/********************************************//**
* @file scheduler.h
*
* @brief this file includes all the functions, variables and typedefs for the scheduler.
*
* note that the error list in this module is not used
********************************************/


/********************************************//**
* This typedef will be holding all the important data for a scheduler task such as ,
* the pointer to the taks function, the delay before it stats, and the peiod on which it is called
/********************************************/
typedef struct Task_Data{
  // pinter to fucntin that takes void and returns void
  void (*pTask)(void);

  uint16_t Delay;

  uint16_t Period;

  uint8_t  RunTask;
}Task_Data;

#define MAXIMUM_NUMBER_OF_TASKS 5
#define WORD_LENGHT_TIMER_PRE_LOAD 45536

/********************************************//**
* This is the array in which all the scheduler taks pointers to functions
********************************************/
Task_Data Scheduler_Tasks_G[MAXIMUM_NUMBER_OF_TASKS];
uint8_t Error_code_G = 0;

/********************************************//**
 *Adds a function to the array of tasks of the scheduler
 *@param pFunction -> pointer to function, insert here your function name\n
 *@param DELAY -> the delay, mesured in number of scheduler ticks. this indicates the delay the task takes
 *before being executed for the first time
 *@param PERIOD -> the period, mesured in number of scheduler ticks. this indicates the period on which the task will be
 *perodically executed
  *******************************************/
uint8_t Scheduler_uAdd_Task(void (*pFunction)(),const uint16_t DELAY,const uint16_t PERIOD);

/********************************************//**
 *Initializes timer 1 to operate the scheduler
 *@param void
  *******************************************/
void Scheduler_vInit_t1(void);

/********************************************//**
 *Enables the Peripheral interrupts and External interrupts
 *@param void
  *******************************************/
void Scheduler_vStart(void);

/********************************************//**
 *Marks all tasks that are due to run now.
 *@param void
  *******************************************/
void Scheduler_vUpdate(void);

/********************************************//**
 *Runs all the tasks that are due to run now.
 *@param void
  *******************************************/
void Scheduler_vDispatch_Tasks(void);

/********************************************//**
 *Delets a task if it is a oneshot taks (to be executed once).
 *@param void
  *******************************************/
uint8_t Scheduler_Delete_Task(const uint8_t TASK_INDEX);
