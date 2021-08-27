/*
 * TaskCore.c
 *
 *  Created on: 12. 3. 2020
 *      Author: jan.ropek
 */

#include "main.h"
#include "TaskCore.h"
#include "ProcessCoreTask.h"
#include "LoRa_Codec.h"
#include "AuxRfProcess.h"
#include "SignalProcessing.h"
#include "TemplateRadioUser.h"
#include "RadioCommands.h"
#include "UartProcess.h"


const uint8_t	defaultTxPacket[]={160,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,35,35,35,35,35,35,35,35,84,101,115,116,111,118,97,99,105,32,112,97,107,101,116,0,228,189};
tPacketParam	PacketParam;

/*!
 *  Freertos objects
 */
extern osMessageQId QueueCoreHandle;
extern osMessageQId QueueRFHandle;
extern osTimerId TimerUartRxCheckHandle;
extern osTimerId TimerRepeateTXHandle;
extern osTimerId TimerTxRfTimeoutHandle;

TaskHandle_t UartTxDoneNotify = NULL;

/*  						*/
size_t volatile Gl_HeapFree;
uint8_t GlUartRxBugger[UART_CIRCLE_MAX_BUFFER_SIZE];

/**
 *
 * @param ReceiveData
 * @param GlobalData
 * @param StateAutomat
 * @param PointerMalloc
 * @return
 */
static void CORE_StateINIT(DATA_QUEUE ReceiveData,
		tCoreGlobalData* coreGlData, tStateCoreAutomat* StateAutomat);
/**
 *
 * @param ReceiveData
 * @param GlobalData
 * @param StateAutomat
 * @param PointerMalloc
 * @return
 */
static void CORE_StateOFF(DATA_QUEUE ReceiveData,
		tCoreGlobalData* coreGlData, tStateCoreAutomat* StateAutomat);
/**
 *
 * @param ReceiveData
 * @param GlobalData
 * @param StateAutomat
 * @param PointerMalloc
 * @return
 */
static void CORE_StateStartON(DATA_QUEUE ReceiveData,
		tCoreGlobalData* coreGlData, tStateCoreAutomat* StateAutomat);


/**
 *
 * @param ReceiveData
 * @param GlobalData
 * @param StateAutomat
 * @param PointerMalloc
 * @return
 */
static void CORE_StateON(DATA_QUEUE ReceiveData,
		tCoreGlobalData* coreGlData, tStateCoreAutomat* StateAutomat);

/**
 *
 * @param ReceiveData
 * @param GlobalData
 * @param StateAutomat
 * @param PointerMalloc
 * @return
 */
static void CORE_StateStartOFF(DATA_QUEUE ReceiveData,
		tCoreGlobalData* coreGlData, tStateCoreAutomat* StateAutomat);

/**
 *
 * @param
 * @param
 * @param
 * @param
 * @return
 */
static void (*StateCORE[])(DATA_QUEUE, tCoreGlobalData*,	tStateCoreAutomat*) =
		{	CORE_StateINIT, CORE_StateOFF, CORE_StateStartON,  CORE_StateON, CORE_StateStartOFF };


/**
 *
 * @param argument
 */
void CallbackRepeateTx(void const * argument)
{
	DATA_QUEUE SendData;
	SendData.pointer = NULL;

	SendData.Address = ADDR_TO_CORE_TX_PERIODIC;
	xQueueSend(QueueCoreHandle,&SendData,portMAX_DELAY);

}

void CallbackUartRxCheck(void const * argument)
{
	DATA_QUEUE SendData;
	SendData.pointer = NULL;

	SendData.Address = ADDR_TO_CORE_UART_READ_RX_BUFFER;
	xQueueSend(QueueCoreHandle,&SendData,portMAX_DELAY);
}

/**
 *
 * @param argument
 */
void CallbackTxRfTimeout(void const * argument)
{
	DATA_QUEUE SendData;
	SendData.pointer = NULL;

	SendData.Address = ADDR_TO_CORE_TX_RF_TIMEOUT;
	xQueueSend(QueueCoreHandle,&SendData,portMAX_DELAY);
}


/**
 *
 * @param htim
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{

	DATA_QUEUE SendData;
	SendData.pointer = NULL;
	portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;

	SendData.Address = ADDR_TO_CORE_UART_READ_RX_BUFFER;
	xQueueSendFromISR(QueueCoreHandle,&SendData,&xHigherPriorityTaskWoken);

	portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

/**
 *
 * @param ReceiveData
 * @param GlobalData
 * @param StateAutomat
 * @param PointerMalloc
 * @return
 */
static void CORE_StateINIT(DATA_QUEUE ReceiveData,tCoreGlobalData* coreGlData, tStateCoreAutomat* StateAutomat)
{
	DATA_QUEUE	SendData;
	SendData.pointer = NULL;


	StateAutomat->PreviousState=StateAutomat->ActualState;
	StateAutomat->ActualState=STATE_CORE_START_ON;

	SendData.Data = DATA_TO_RF_START_ON;
	SendData.Address = ADDR_TO_RF_CHANGE_STATE;
	xQueueSend(QueueRFHandle,&SendData,portMAX_DELAY);

}

/**
 *
 * @param ReceiveData
 * @param GlobalData
 * @param StateAutomat
 * @param PointerMalloc
 * @return
 */
static void CORE_StateOFF(DATA_QUEUE ReceiveData,tCoreGlobalData* coreGlData, tStateCoreAutomat* StateAutomat)
{

}


/**
 *
 * @param ReceiveData
 * @param GlobalData
 * @param StateAutomat
 * @param PointerMalloc
 * @return
 */
static void CORE_StateStartON(DATA_QUEUE ReceiveData,tCoreGlobalData* coreGlData, tStateCoreAutomat* StateAutomat)
{

	static bool tempRfAck;

	if(ReceiveData.Address == ADDR_TO_CORE_TASK_STATE_CHANGED)
	{
		if(ReceiveData.Data == DATA_TO_CORE_RF_IS_ON)		tempRfAck = true;

	}

	if(tempRfAck == true)
	{
		tempRfAck=false;
		StateAutomat->PreviousState=StateAutomat->ActualState;
		StateAutomat->ActualState=STATE_CORE_ON;

		/* Init timers. */

		/* load config */

		LL_GPIO_SetOutputPin(LED_GREEN_GPIO_Port,LED_GREEN_Pin);
		osDelay(200);
		LL_GPIO_ResetOutputPin(LED_GREEN_GPIO_Port,LED_GREEN_Pin);

	}
}



/**
 *
 * @param ReceiveData
 * @param GlobalData
 * @param StateAutomat
 * @param PointerMalloc
 * @return
 */
static void CORE_StateON(DATA_QUEUE ReceiveData,tCoreGlobalData* coreGlData, tStateCoreAutomat* StateAutomat)
{
	//DATA_QUEUE	SendData;
	//SendData.pointer=NULL;
	//static tGeneralPacket	localTxBuffer;
	//static uint8_t			localTxPacketSize=0;
	//uint8_t					*TxBuffer;
	uint8_t					*RxUartMsg = NULL;
	uint8_t					doCheckAgain = 0;
	static uint8_t			localRxPacketBuff[PACKET_MAX_SIZE];
	static uint8_t			lastRxPacketSize;
	static uint8_t			lastRssiPacket;

	Gl_HeapFree=xPortGetMinimumEverFreeHeapSize();


	switch (ReceiveData.Address)
	{
		case ADDR_TO_CORE_RF_DATA_RECEIVED:

			memcpy(&localRxPacketBuff[2],ReceiveData.pointer,ReceiveData.temp);
			lastRssiPacket = (uint8_t) ReceiveData.RFU;
			lastRxPacketSize=ReceiveData.temp;
			localRxPacketBuff[0] = lastRxPacketSize;
			localRxPacketBuff[1] = lastRssiPacket;

			UP_UartSendData(UART_MSG_GET_RX_PACKET,localRxPacketBuff,lastRxPacketSize+2);

			break;

		case ADDR_TO_CORE_TX_PERIODIC:

			break;


		case ADDR_TO_CORE_TX_PACKET_DONE:
		case ADDR_TO_CORE_TX_RF_TIMEOUT:
			if(xTimerIsTimerActive(TimerTxRfTimeoutHandle) == true)	osTimerStop(TimerTxRfTimeoutHandle);

			/* Is there anything to send? */
			if(PacketParam.autoRepeat)
			{
				osTimerStart(TimerTxRfTimeoutHandle,PacketParam.repeatPeriod);
			}

			break;

		case ADDR_TO_CORE_SEND_LAST_PACKET:
			PCT_SendRfPacket();

			break;

		case ADDR_TO_CORE_UART_READ_RX_BUFFER:
			/* periodic scanning circular uart buffer*/
			do
			{
				if(UP_FindAnyMsg(&RxUartMsg,&doCheckAgain)==eUART_MSG_OK)
				{
					PCT_DecodeUartRxMsg(RxUartMsg);
					vPortFree(RxUartMsg);
					RxUartMsg = NULL;
					//LL_GPIO_ResetOutputPin(LED_RED_GPIO_Port,LED_RED_Pin);
					//LL_GPIO_ResetOutputPin(LED_GREEN_GPIO_Port,LED_GREEN_Pin);
				}
			}
			while(doCheckAgain == 1 );

			PCT_ShceduleUartCheck();

			break;

		case ADDR_TO_CORE_UART_RX_NEW_PACKET:
			//RxUartMsg=ReceiveData.pointer;

			break;

		default:
			break;
	}
}




/**
 *
 * @param ReceiveData
 * @param GlobalData
 * @param StateAutomat
 * @param PointerMalloc
 * @return
 */
static void CORE_StateStartOFF(DATA_QUEUE ReceiveData,tCoreGlobalData* coreGlData, tStateCoreAutomat* StateAutomat)
{

	static bool tempRfAck;

	if(ReceiveData.Address == ADDR_TO_CORE_TASK_STATE_CHANGED)
	{
		if(ReceiveData.Data == DATA_TO_CORE_RF_IS_OFF)	tempRfAck = true;
	}

	if(tempRfAck == true)
	{
		tempRfAck = false;

		StateAutomat->PreviousState=StateAutomat->ActualState;
		StateAutomat->ActualState=STATE_CORE_OFF;


		/* Stop all Timers. */


		/* Deinit variables. */

		LL_GPIO_ResetOutputPin(LED_BLUE_GPIO_Port,LED_BLUE_Pin);
		LL_GPIO_ResetOutputPin(LED_GREEN_GPIO_Port,LED_GREEN_Pin);
	}

}

/**
 *
 * @param argument
 */
void StartTaskCore(void const * argument)
{
	DATA_QUEUE ReceiveData;

	static tCoreGlobalData DataTaskCore;
	static tStateCoreAutomat StateAutomat = {STATE_CORE_INIT,STATE_CORE_INIT};
	static portBASE_TYPE ReturnValue;

	CORE_StateINIT(ReceiveData,&DataTaskCore, &StateAutomat);


	for(;;)
	{
		ReturnValue = xQueueReceive(QueueCoreHandle, &ReceiveData, portMAX_DELAY);
		if (ReturnValue == pdPASS)
		{
			StateCORE[StateAutomat.ActualState](ReceiveData, &DataTaskCore, &StateAutomat);

			/* Clear malloc */
			vPortFree(ReceiveData.pointer);
			ReceiveData.pointer=NULL;
		}
	}
}
