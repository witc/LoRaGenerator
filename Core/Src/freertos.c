/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
osThreadId TaskCoreHandle;
uint32_t TaskCoreBuffer[ 350 ];
osStaticThreadDef_t TaskCoreControlBlock;
osThreadId TaskRFHandle;
uint32_t TaskRFBuffer[ 300 ];
osStaticThreadDef_t TaskRFControlBlock;
osMessageQId QueueCoreHandle;
uint8_t QueueCoreBuffer[ 20 * sizeof( DATA_QUEUE ) ];
osStaticMessageQDef_t QueueCoreControlBlock;
osMessageQId QueueRFHandle;
uint8_t QueueRFBuffer[ 20 * sizeof( DATA_QUEUE ) ];
osStaticMessageQDef_t QueueRFControlBlock;
osTimerId TimerRepeateTXHandle;
osStaticTimerDef_t TimerRepeateTXControlBlock;
osTimerId TimerUartRxCheckHandle;
osStaticTimerDef_t TimerUartRxCheckControlBlock;
osTimerId TimerTxRfTimeoutHandle;
osStaticTimerDef_t TimerTxRfTimeoutControlBlock;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
   
/* USER CODE END FunctionPrototypes */

void StartTaskCore(void const * argument);
void StartTaskRF(void const * argument);
void CallbackRepeateTx(void const * argument);
void CallbackUartRxCheck(void const * argument);
void CallbackTxRfTimeout(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* Pre/Post sleep processing prototypes */
void PreSleepProcessing(uint32_t *ulExpectedIdleTime);
void PostSleepProcessing(uint32_t *ulExpectedIdleTime);

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* GetTimerTaskMemory prototype (linked to static allocation support) */
void vApplicationGetTimerTaskMemory( StaticTask_t **ppxTimerTaskTCBBuffer, StackType_t **ppxTimerTaskStackBuffer, uint32_t *pulTimerTaskStackSize );

/* Hook prototypes */
void vApplicationStackOverflowHook(xTaskHandle xTask, signed char *pcTaskName);
void vApplicationMallocFailedHook(void);

/* USER CODE BEGIN 4 */
__weak void vApplicationStackOverflowHook(xTaskHandle xTask, signed char *pcTaskName)
{
   /* Run time stack overflow checking is performed if
   configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2. This hook function is
   called if a stack overflow is detected. */
}
/* USER CODE END 4 */

/* USER CODE BEGIN 5 */
__weak void vApplicationMallocFailedHook(void)
{
   /* vApplicationMallocFailedHook() will only be called if
   configUSE_MALLOC_FAILED_HOOK is set to 1 in FreeRTOSConfig.h. It is a hook
   function that will get called if a call to pvPortMalloc() fails.
   pvPortMalloc() is called internally by the kernel whenever a task, queue,
   timer or semaphore is created. It is also called by various parts of the
   demo application. If heap_1.c or heap_2.c are used, then the size of the
   heap available to pvPortMalloc() is defined by configTOTAL_HEAP_SIZE in
   FreeRTOSConfig.h, and the xPortGetFreeHeapSize() API function can be used
   to query the size of free heap space that remains (although it does not
   provide information on how the remaining heap might be fragmented). */
}
/* USER CODE END 5 */

/* USER CODE BEGIN PREPOSTSLEEP */
__weak void PreSleepProcessing(uint32_t *ulExpectedIdleTime)
{
/* place for user code */
}

__weak void PostSleepProcessing(uint32_t *ulExpectedIdleTime)
{
/* place for user code */
}
/* USER CODE END PREPOSTSLEEP */

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];
  
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}                   
/* USER CODE END GET_IDLE_TASK_MEMORY */

/* USER CODE BEGIN GET_TIMER_TASK_MEMORY */
static StaticTask_t xTimerTaskTCBBuffer;
static StackType_t xTimerStack[configTIMER_TASK_STACK_DEPTH];

void vApplicationGetTimerTaskMemory( StaticTask_t **ppxTimerTaskTCBBuffer, StackType_t **ppxTimerTaskStackBuffer, uint32_t *pulTimerTaskStackSize )
{
  *ppxTimerTaskTCBBuffer = &xTimerTaskTCBBuffer;
  *ppxTimerTaskStackBuffer = &xTimerStack[0];
  *pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
  /* place for user code */
}
/* USER CODE END GET_TIMER_TASK_MEMORY */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
       
  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* Create the timer(s) */
  /* definition and creation of TimerRepeateTX */
  osTimerStaticDef(TimerRepeateTX, CallbackRepeateTx, &TimerRepeateTXControlBlock);
  TimerRepeateTXHandle = osTimerCreate(osTimer(TimerRepeateTX), osTimerOnce, NULL);

  /* definition and creation of TimerUartRxCheck */
  osTimerStaticDef(TimerUartRxCheck, CallbackUartRxCheck, &TimerUartRxCheckControlBlock);
  TimerUartRxCheckHandle = osTimerCreate(osTimer(TimerUartRxCheck), osTimerOnce, NULL);

  /* definition and creation of TimerTxRfTimeout */
  osTimerStaticDef(TimerTxRfTimeout, CallbackTxRfTimeout, &TimerTxRfTimeoutControlBlock);
  TimerTxRfTimeoutHandle = osTimerCreate(osTimer(TimerTxRfTimeout), osTimerOnce, NULL);

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* definition and creation of QueueCore */
  osMessageQStaticDef(QueueCore, 20, DATA_QUEUE, QueueCoreBuffer, &QueueCoreControlBlock);
  QueueCoreHandle = osMessageCreate(osMessageQ(QueueCore), NULL);

  /* definition and creation of QueueRF */
  osMessageQStaticDef(QueueRF, 20, DATA_QUEUE, QueueRFBuffer, &QueueRFControlBlock);
  QueueRFHandle = osMessageCreate(osMessageQ(QueueRF), NULL);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of TaskCore */
  osThreadStaticDef(TaskCore, StartTaskCore, osPriorityNormal, 0, 350, TaskCoreBuffer, &TaskCoreControlBlock);
  TaskCoreHandle = osThreadCreate(osThread(TaskCore), NULL);

  /* definition and creation of TaskRF */
  osThreadStaticDef(TaskRF, StartTaskRF, osPriorityNormal, 0, 300, TaskRFBuffer, &TaskRFControlBlock);
  TaskRFHandle = osThreadCreate(osThread(TaskRF), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartTaskCore */
/**
  * @brief  Function implementing the TaskCore thread.
  * @param  argument: Not used 
  * @retval None
  */
/* USER CODE END Header_StartTaskCore */
__weak void StartTaskCore(void const * argument)
{
  /* USER CODE BEGIN StartTaskCore */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartTaskCore */
}

/* USER CODE BEGIN Header_StartTaskRF */
/**
* @brief Function implementing the TaskRF thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTaskRF */
__weak void StartTaskRF(void const * argument)
{
  /* USER CODE BEGIN StartTaskRF */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartTaskRF */
}

/* CallbackRepeateTx function */
__weak void CallbackRepeateTx(void const * argument)
{
  /* USER CODE BEGIN CallbackRepeateTx */

  /* USER CODE END CallbackRepeateTx */
}

/* CallbackUartRxCheck function */
__weak void CallbackUartRxCheck(void const * argument)
{
  /* USER CODE BEGIN CallbackUartRxCheck */

  /* USER CODE END CallbackUartRxCheck */
}

/* CallbackTxRfTimeout function */
__weak void CallbackTxRfTimeout(void const * argument)
{
  /* USER CODE BEGIN CallbackTxRfTimeout */

  /* USER CODE END CallbackTxRfTimeout */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
     
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
