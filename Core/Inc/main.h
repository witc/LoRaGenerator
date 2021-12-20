/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l0xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stm32l0xx_ll_gpio.h"
#include "stm32l0xx_ll_tim.h"
#include "stm32l0xx_ll_exti.h"
#include "stm32l0xx_ll_adc.h"
#include "stm32l0xx_ll_dma.h"
#include "stm32l0xx_ll_pwr.h"
#include "stm32l0xx_ll_rtc.h"
#include "stm32l0xx_ll_rcc.h"
#include "stm32l0xx_ll_usart.h"


#include "cmsis_os.h"
#include "stdbool.h"
#include "stdint.h"
#include "string.h"
#include "math.h"
#include "compiler.h"
#include "EepromAddress.h"
#include "stm32l0xx_ll_cortex.h"
#include "stm32l0xx_ll_system.h"
#include "LoadConfig.h"
#include "target.h"
//#include "wwdg.h"
//#include"stm32l0xx_hal_gpio.h"


/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

//Doba blik�n� LED indikuj�c� zapnut� stav - 2 minuty
//Perioda blik�n� LED v zapnut�m stavu - 1.5 sec
//Cas na prvni zahlaseni alarmu pokud neprijde zadny impuls z ohrady - 10 sec
//Cas na druhe zahlaseni alarmu pokud neprijde zadny impuls z ohrady  - 15 min
//Perioda odesilani hodnot v bezporuchovem stavu -15 min
//Prumerovani vzorku Klouzavym prumerem - ze 3 ran

#define RUN_WITH_WATCHDOG						1
#define RUN_WITH_DEBUG							0
#define DEBUG_IN_STOP_MODE						0
#define DEBUG_NO_FACTORY_TEST					0


#define CONST_ONE_SEC							(1000)
#define CONST_ONE_MIN							(60000)
#define CONST_ONE_HOUR							(3600000)
#define TIMER_BLINK_PERIOD_FTEST				(0.2*CONST_ONE_SEC)

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
void 	LogError		(uint64_t error);
//#if (RUN_WITH_DEBUG==1)
void 	MeasureStart	(void);
void	MeasureStop		(void);
void 	osDelayWatchdog	(uint32_t msDelay);
bool	M_IsMcuLocked	(void);


//#endif
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define SHORT_PRESS_PERIOD 2
#define LONG_PRESS_PERIOD 40
#define REPEAT_PRESS_PERIOD 50
#define EXTRA_LONG_PRESS 55
#define AUX_8_Pin GPIO_PIN_2
#define AUX_8_GPIO_Port GPIOA
#define AUX_7_Pin GPIO_PIN_3
#define AUX_7_GPIO_Port GPIOA
#define SX1262_NSS_Pin GPIO_PIN_4
#define SX1262_NSS_GPIO_Port GPIOA
#define SX1262_SCK_Pin GPIO_PIN_5
#define SX1262_SCK_GPIO_Port GPIOA
#define SX1262_MISO_Pin GPIO_PIN_6
#define SX1262_MISO_GPIO_Port GPIOA
#define SX1262_MOSI_Pin GPIO_PIN_7
#define SX1262_MOSI_GPIO_Port GPIOA
#define SX1262_NRESET_Pin GPIO_PIN_1
#define SX1262_NRESET_GPIO_Port GPIOB
#define SX1262_DIO_Pin GPIO_PIN_2
#define SX1262_DIO_GPIO_Port GPIOB
#define SX1262_DIO_EXTI_IRQn EXTI2_3_IRQn
#define SX1262_BUSY_Pin GPIO_PIN_10
#define SX1262_BUSY_GPIO_Port GPIOB
#define LED_GREEN_Pin GPIO_PIN_15
#define LED_GREEN_GPIO_Port GPIOB
#define SX1262_TX_ENABLE_Pin GPIO_PIN_8
#define SX1262_TX_ENABLE_GPIO_Port GPIOA
#define USART1_RX_Pin GPIO_PIN_10
#define USART1_RX_GPIO_Port GPIOA
#define AUX_4_Pin GPIO_PIN_12
#define AUX_4_GPIO_Port GPIOA
#define AUX_3_Pin GPIO_PIN_3
#define AUX_3_GPIO_Port GPIOB
#define AUX_2_Pin GPIO_PIN_4
#define AUX_2_GPIO_Port GPIOB
#define LED_BLUE_Pin GPIO_PIN_5
#define LED_BLUE_GPIO_Port GPIOB
#define AUX_6_Pin GPIO_PIN_6
#define AUX_6_GPIO_Port GPIOB
#define AUX_5_Pin GPIO_PIN_7
#define AUX_5_GPIO_Port GPIOB
#define AUX_1_Pin GPIO_PIN_9
#define AUX_1_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

#define BLINK_INFINITY		UINT64_MAX

/*!
 *	Address for Core Task
 */
#define ADDR_TO_CORE_TASK_STATE_CHANGED			255
#define ADDR_TO_CORE_KEYBOARD_EVENT				254
#define ADDR_TO_CORE_RF_DATA_RECEIVED			253
#define ADDR_TO_CORE_SAVE_PAIR_DEVICE			252
#define ADDR_TO_CORE_HEART_BEAT					251
#define ADDR_TO_CORE_SEND_LAST_PACKET			250
#define ADDR_TO_CORE_HANDLE_ADC					249
#define ADDR_TO_CORE_RESET_SYSTEM				248
#define ADDR_TO_CORE_WATCHDOG_ACTIONS			247
#define ADDR_TO_CORE_TX_PERIOD_EXPIRED			246
#define ADDR_TO_CORE_PAIR_STOP					245
#define ADDR_TO_CORE_TX_PACKET_DONE				244
#define ADDR_TO_CORE_EN_SEND_NEXT_PACKET		243
#define ADDR_TO_CORE_CREATE_NEW_PACKET			242
#define ADDR_TO_CORE_FACTORY_TEST_DATA_RX		241
#define ADDR_TO_CORE_SHUTDOWN_ACK				240
#define ADDR_TO_CORE_TX_PERIODIC				239
#define ADDR_TO_CORE_UART_READ_RX_BUFFER		238
#define ADDR_TO_CORE_UART_RX_NEW_PACKET			237
#define ADDR_TO_CORE_TX_RF_TIMEOUT				236

/*!
 *	Data for Core Task
 */
#define DATA_TO_CORE_RF_IS_ON				255
#define DATA_TO_CORE_RF_IS_OFF				254
#define DATA_TO_CORE_SHORT_PRESS_EVENT		253
#define DATA_TO_CORE_LONG_PRESS_EVENT		252
#define DATA_TO_CORE_REPEATE_PRESS_EVENT	251
#define DATA_TO_CORE_IMPULSE_HEART_BEAT		250


/*!
 *	Address for RF Task
 */
#define ADDR_TO_RF_CHANGE_STATE				255
#define ADDR_TO_RF_START_OFF				254
#define ADDR_TO_RF_CMD						253
#define ADDR_TO_RF_SX1262_IRQ				252
#define ADDR_TO_RF_INFO						251
#define ADDR_TO_RF_CHECK_RF_IS_LIVE			250
#define ADDR_TO_RF_SET_RADIO_PARAM			249


/*!
 * Data for RF Task
 */
#define DATA_TO_RF_START_ON					255
#define DATA_TO_RF_START_OFF				254
#define DATA_TO_RF_END_OF_PAIR				253


extern uint64_t	GlBlinkLedGreen;
extern uint64_t	GlBlinkLedRed;
extern uint64_t GlBlinkSound;
extern uint64_t	GlAliveTimer;
extern double	GlSoundSweep;

/**
 *
 */
typedef struct
{
	uint32_t Address;
	uint32_t Data;
	uint64_t RFU;
	uint32_t temp;
	uint32_t temp2;
	void 	 *pointer;

}DATA_QUEUE;




/**
 *
 */
typedef enum
{
	WD_NONE=0,             //!< WD_NONE
	WD_TIME_FOR_WATCHDOG=1,//!< WD_TIME_FOR_WATCHDOG
	WD_RF_IS_ALIVE=2,      //!< WD_RF_IS_ALIVE

}eWatchDog;



/**
 *
 * @param ledOn
 */
void inline LedRadioTXActive(bool ledOn)
{
	if(ledOn == true)
	{
		LL_GPIO_SetOutputPin(LED_BLUE_GPIO_Port,LED_BLUE_Pin);
	}
	else
	{
		LL_GPIO_ResetOutputPin(LED_BLUE_GPIO_Port,LED_BLUE_Pin);
	}
}

void inline LedRadioRXActive(bool ledOn)
{
	if(ledOn == true)
	{
		LL_GPIO_SetOutputPin(LED_GREEN_GPIO_Port,LED_GREEN_Pin);
	}
	else
	{
		LL_GPIO_ResetOutputPin(LED_GREEN_GPIO_Port,LED_GREEN_Pin);
	}
}

//** EEPROM */
#define EE_ADDR_STATE_OF_SYSTEM			(FLASH_EEPROM_BASE)
#define EE_RF_POWER						(EE_ADDR_STATE_OF_SYSTEM)	//uint8_t
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
