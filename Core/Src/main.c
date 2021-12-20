/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
#include "main.h"
#include "cmsis_os.h"
#include "dma.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "TaskRF.h"
#include "TaskCore.h"

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

/* USER CODE BEGIN PV */
extern osMessageQId QueueRFHandle;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void MX_FREERTOS_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */


/**
 *
 * @param error
 */
void LogError(uint64_t error)
{
#if (RUN_WITH_DEBUG==1)
	taskDISABLE_INTERRUPTS();

	while(1);

#else
	HAL_RCC_DeInit();
	HAL_NVIC_SystemReset();
	while(1);	//pripadne to schodi WD
#endif

}

#if (RUN_WITH_DEBUG==1)
void MeasureStart()
{
	LL_GPIO_SetOutputPin(TestPoint_GPIO_Port,TestPoint_Pin);
}

void MeasureStop()
{
	LL_GPIO_ResetOutputPin(TestPoint_GPIO_Port,TestPoint_Pin);
}
#endif



/**
 *
 * @return
 */
bool M_IsMcuLocked()
{
//	bool pulseCalibOK;
//	bool rfRssiCalibOK;
//	bool rfTxCalibOK;
	FLASH_OBProgramInitTypeDef flashTemp;

	/* cteni trva 27 us*/
	HAL_FLASHEx_OBGetConfig(&flashTemp);
	if(OB_RDP_LEVEL_0 != flashTemp.RDPLevel)
	{
		return true;
	}

//	memcpy(&pulseCalibOK,(uint8_t*)EEPROM_PULSE_CALIB_OK,1);
//	memcpy(&rfRssiCalibOK,(uint8_t*)EEPROM_PULSE_RSSI_OK,1);
//	memcpy(&rfTxCalibOK,(uint8_t*)EEPROM_RF_TX_OK,1);
//
//	if(rfTxCalibOK&&pulseCalibOK&&rfRssiCalibOK)
//	{
//		return true;
//	}

	return false;

}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	// __DMB(); //ARM says to use a DMB instruction before relocating VTOR */
  SCB->VTOR = 0x8004000; //We relocate vector table to the APP sector
				 // __DSB(); //ARM says to use a DSB instruction just after relocating VTOR */
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
  /* Systick turn of -we use a different timer*/
  //SysTick->CTRL&=~ SysTick_CTRL_ENABLE_Msk;


//  DBGMCU->CR |= DBGMCU_CR_DBG_SLEEP | DBGMCU_CR_DBG_STOP | DBGMCU_CR_DBG_STANDBY;
  __HAL_RCC_DBGMCU_CLK_ENABLE();

  LL_DBGMCU_EnableDBGSleepMode();
  LL_DBGMCU_EnableDBGStopMode();

  LL_DBGMCU_APB1_GRP1_FreezePeriph(	LL_DBGMCU_APB1_GRP1_TIM2_STOP|LL_DBGMCU_APB1_GRP1_TIM3_STOP|LL_DBGMCU_APB1_GRP1_TIM6_STOP|LL_DBGMCU_APB1_GRP1_TIM7_STOP
		  |LL_DBGMCU_APB1_GRP1_RTC_STOP|LL_DBGMCU_APB1_GRP1_WWDG_STOP | LL_DBGMCU_APB1_GRP1_IWDG_STOP);

  LL_DBGMCU_APB2_GRP1_FreezePeriph(LL_DBGMCU_APB2_GRP1_TIM22_STOP|LL_DBGMCU_APB2_GRP1_TIM21_STOP);

  LL_DBGMCU_APB1_GRP1_FreezePeriph(LL_DBGMCU_APB1_GRP1_WWDG_STOP);


  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_TIM6_Init();
  MX_SPI1_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
  //NVIC_ClearPendingIRQ(SysTick_IRQn);
  //NVIC_EnableIRQ(SysTick_IRQn);

  /* USER CODE END 2 */

  /* Call init function for freertos objects (in freertos.c) */
  MX_FREERTOS_Init();
  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLLMUL_4;
  RCC_OscInitStruct.PLL.PLLDIV = RCC_PLLDIV_2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */


/**
 *
 * @param GPIO_Pin
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	DATA_QUEUE SendData;
	SendData.pointer=NULL;
	signed portBASE_TYPE xHigherPriorityTaskWoken;
	xHigherPriorityTaskWoken=pdFALSE;

	if(GPIO_Pin==SX1262_DIO_Pin)
	{
		SendData.Address=ADDR_TO_RF_SX1262_IRQ;
		if(xQueueSendFromISR(QueueRFHandle,&SendData,&xHigherPriorityTaskWoken)!=pdPASS)
		{
			LogError(1289);
		}
		portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
	}
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
	LogError(6835168);
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
