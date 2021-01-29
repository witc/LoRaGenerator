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
#define DEBUG_STUCK_IN_WD_FAIL					0
#define DEBUG_STAY_OFF_AFTER_RESET				0


#define VOLUME_SOUND							22	//puvodne 22


#define CONST_ONE_SEC							(1000)
#define CONST_ONE_MIN							(60000)
#define CONST_ONE_HOUR							(3600000)
#define TIMER_BLINK_PERIOD_FTEST				(0.2*CONST_ONE_SEC)
#define TIME_LED_ON								(90)		// pri periodeickem blikani po stisku tlacitka
#define TIMER_BLINK_PERIOD						(1.5*CONST_ONE_SEC)
#define TIME_SOUND_BEEP_PULSE					(90)
#define TIME_SOUND_BEEP_PULSE_LONG				(500)
#define DURATION_ALIVE_SHOW_ACTIVITY			(((120*CONST_ONE_SEC*2)/TIMER_BLINK_PERIOD)-2)/*change only first const*/
#define IMPULSE_HEARTBEAT_PERIOD				(4*CONST_ONE_SEC)
#define TX_PERIOD								(15*CONST_ONE_MIN)	//15 min
#define	DURATION_RX_WINDOW_1					(300)
#define TIME_TO_ENABLE_TRIGGER					(380)
#define PAIR_TIME_LIMIT							(2*CONST_ONE_MIN)
#define HYSTERESIS_PULSE						(7)	//5 = 500 V
#define MAX_ATTEMPTS_TO_SEND_DATA				(7)
#define MIN_ATTEMPTS_TO_SEND_DATA				(2)
/* Time on air of packet. */
#define	TOA_OF_MONITOR_INFO						320

/* LBT settings */
#define MAX_LBT_TRY								10
#define NO_OF_CADS_IN_LBT						3
#define RANDOM_DELAY_BEF_FIRST_TX				500
#define RANDOM_DELAY_BETWEEN_CAD				600
#define MAX_TIME_BEF_SEND_PACKET				(100+RANDOM_DELAY_BEF_FIRST_TX+MAX_LBT_TRY*RANDOM_DELAY_BETWEEN_CAD)

#define TIME_RF_TX_TIMEOUT						(MAX_TIME_BEF_SEND_PACKET+TOA_OF_MONITOR_INFO)
#define TIME_TO_REPEATE_PACKET					(TIME_RF_TX_TIMEOUT+100)
#define TIME_TO_EN_SEND_NEXT_PACKET				(700)	/* Cekame na mozny prijem po odvysilani */
#define	TIME_TO_TRY_SEND_NEXT_PACKET			(500)	/* */
#define MAX_TIME_MEAS_BATT						20

#define RSSI_TEST_MIN							(-150)
#define RSSI_TEST_MAX							(-50)



#define LENGTH_IMPULSE_FILTER					3
#define ADC_BATT_BUFFER_SIZE					8	//kazdou hodnotu 4x kanal+referenci
#define ADC_IMPULSE_BUFFER_SIZE					8	//kazdou hodnotu 4x kanal+referenci

#define BATT_ALARM_LEVEL_BLINK					10	// na GW 10%
#define BATT_ALARM_LEVEL_SOUND					5	// na GW 5%
#define BATT_DIVIDER							(0.253731)		// Napetovy delic u baterie

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
#define RF_TX_ENABLE_Pin GPIO_PIN_3
#define RF_TX_ENABLE_GPIO_Port GPIOA
#define RF_NSS_Pin GPIO_PIN_4
#define RF_NSS_GPIO_Port GPIOA
#define SX1262_SCK_Pin GPIO_PIN_5
#define SX1262_SCK_GPIO_Port GPIOA
#define SX1262_MISO_Pin GPIO_PIN_6
#define SX1262_MISO_GPIO_Port GPIOA
#define SX1262_MOSI_Pin GPIO_PIN_7
#define SX1262_MOSI_GPIO_Port GPIOA
#define RF_NRESET_Pin GPIO_PIN_0
#define RF_NRESET_GPIO_Port GPIOB
#define RF_BUSY_Pin GPIO_PIN_1
#define RF_BUSY_GPIO_Port GPIOB
#define RF_DIO_Pin GPIO_PIN_2
#define RF_DIO_GPIO_Port GPIOB
#define RF_DIO_EXTI_IRQn EXTI2_3_IRQn
#define ATTEN_SCK_Pin GPIO_PIN_10
#define ATTEN_SCK_GPIO_Port GPIOB
#define RF_MISO_Pin GPIO_PIN_14
#define RF_MISO_GPIO_Port GPIOB
#define ATTEN_MOSI_Pin GPIO_PIN_15
#define ATTEN_MOSI_GPIO_Port GPIOB
#define ATTEN_LE1_Pin GPIO_PIN_8
#define ATTEN_LE1_GPIO_Port GPIOA
#define ATTEN_LE2_Pin GPIO_PIN_9
#define ATTEN_LE2_GPIO_Port GPIOA
#define LED_RED_Pin GPIO_PIN_5
#define LED_RED_GPIO_Port GPIOB
#define LED_GREEN_Pin GPIO_PIN_9
#define LED_GREEN_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

#define LOCAL_KEY			"Dogtrace Fencee"

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
	STATE_CORE_INIT,
	STATE_CORE_OFF,
	STATE_CORE_START_ON,
	STATE_CORE_ON,
	STATE_CORE_START_OFF,
	STATE_CORE_ERROR			// pokud zabral watchdog
}eStateCoreSystem;


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
 */
typedef enum
{
	STATE_RF_INIT,     //!< STATE_INIT
	STATE_RF_OFF,      //!< STATE_OFF
	STATE_RF_START_ON, //!< STATE_START_ON
	STATE_RF_ON,       //!< STATE_ON
	STATE_RF_START_OFF,//!< STATE_START_OFF
	STATE_RF_ERROR			// pokud zabral watchdog

}eStateRfSystem;

/**
 * Core task
 */
typedef struct
{
	eStateCoreSystem	ActualState;
	eStateCoreSystem	PreviousState;

} tStateCoreAutomat;

/**
 * RF task
 */
typedef struct
{
	eStateRfSystem	ActualState;
	eStateRfSystem	PreviousState;

} tStateRfAutomat;
//eStateRfSystem

//** EEPROM */
#define EE_ADDR_STATE_OF_SYSTEM			(FLASH_EEPROM_BASE)
#define EE_RF_POWER						(EE_ADDR_STATE_OF_SYSTEM)	//uint8_t
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
