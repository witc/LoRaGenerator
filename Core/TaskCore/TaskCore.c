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


const uint8_t	defaultTxPacket[]={160,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,35,35,35,35,35,35,35,35,84,101,115,116,111,118,97,99,105,32,112,97,107,101,116,0,228,189};
tPacketParam	PacketParam;

/*!
 *  Freertos objects
 */
extern osMessageQId QueueCoreHandle;
extern osMessageQId QueueRFHandle;
extern osTimerId TimerUartRxCheckHandle;
extern osTimerId TimerRepeateTXHandle;

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
		tCoreGlobalData* GlobalData, tStateCoreAutomat* StateAutomat);
/**
 *
 * @param ReceiveData
 * @param GlobalData
 * @param StateAutomat
 * @param PointerMalloc
 * @return
 */
static void CORE_StateOFF(DATA_QUEUE ReceiveData,
		tCoreGlobalData* GlobalData, tStateCoreAutomat* StateAutomat);
/**
 *
 * @param ReceiveData
 * @param GlobalData
 * @param StateAutomat
 * @param PointerMalloc
 * @return
 */
static void CORE_StateStartON(DATA_QUEUE ReceiveData,
		tCoreGlobalData* GlobalData, tStateCoreAutomat* StateAutomat);


/**
 *
 * @param ReceiveData
 * @param GlobalData
 * @param StateAutomat
 * @param PointerMalloc
 * @return
 */
static void CORE_StateON(DATA_QUEUE ReceiveData,
		tCoreGlobalData* GlobalData, tStateCoreAutomat* StateAutomat);

/**
 *
 * @param ReceiveData
 * @param GlobalData
 * @param StateAutomat
 * @param PointerMalloc
 * @return
 */
static void CORE_StateStartOFF(DATA_QUEUE ReceiveData,
		tCoreGlobalData* GlobalData, tStateCoreAutomat* StateAutomat);

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
static void CORE_StateINIT(DATA_QUEUE ReceiveData,tCoreGlobalData* GlobalData, tStateCoreAutomat* StateAutomat)
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
static void CORE_StateOFF(DATA_QUEUE ReceiveData,tCoreGlobalData* GlobalData, tStateCoreAutomat* StateAutomat)
{
	if(ReceiveData.Address == ADDR_TO_CORE_KEYBOARD_EVENT)
	{

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
static void CORE_StateStartON(DATA_QUEUE ReceiveData,tCoreGlobalData* GlobalData, tStateCoreAutomat* StateAutomat)
{
	DATA_QUEUE	SendData;
	SendData.pointer=NULL;

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

		LL_GPIO_SetOutputPin(LED_GREEN_GPIO_Port,LED_GREEN_Pin);
		osDelay(200);
		LL_GPIO_ResetOutputPin(LED_GREEN_GPIO_Port,LED_GREEN_Pin);

		/*Uart  Init DMA */
		LL_DMA_DisableChannel(DMA1,LL_DMA_CHANNEL_3);
		LL_DMA_ConfigTransfer(DMA1, LL_DMA_CHANNEL_3,
		                        LL_DMA_DIRECTION_PERIPH_TO_MEMORY |
								LL_DMA_PRIORITY_LOW              |
								LL_DMA_MODE_CIRCULAR              |
		                        LL_DMA_PERIPH_NOINCREMENT         |
		                        LL_DMA_MEMORY_INCREMENT           |
		                        LL_DMA_PDATAALIGN_BYTE            |
		                        LL_DMA_MDATAALIGN_BYTE);

		LL_DMA_ConfigAddresses(DMA1, LL_DMA_CHANNEL_3,
		                         LL_USART_DMA_GetRegAddr(USART1, LL_USART_DMA_REG_DATA_RECEIVE),
		                         (uint32_t)GlUartRxBugger,LL_DMA_GetDataTransferDirection(DMA1, LL_DMA_CHANNEL_3));

		LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_3, UART_CIRCLE_MAX_BUFFER_SIZE);
		LL_DMA_SetPeriphRequest(DMA1, LL_DMA_CHANNEL_3, LL_DMA_REQUEST_3);

		/* Enable DMA RX Interrupt */
		LL_USART_EnableDMAReq_RX(USART1);
		/* Enable DMA Channel Rx */
		LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_3);

	//	osTimerStart(TimerUartRxCheckHandle,TIME_TO_CHECK_UART_RX_BUFFER);
		HAL_NVIC_DisableIRQ(TIM6_IRQn);
		HAL_NVIC_ClearPendingIRQ(TIM6_IRQn);
		HAL_NVIC_EnableIRQ(TIM6_IRQn);

		LL_TIM_SetAutoReload(TIM6,__LL_TIM_CALC_ARR(32000000,LL_TIM_GetPrescaler(TIM6),UART_CHECK_FREQUENCY));   //(1/MINIMAL_SIZE_USART_RX_MSG)
		LL_TIM_EnableIT_UPDATE(TIM6);
		LL_TIM_SetCounter(TIM6,0);
		LL_TIM_EnableCounter(TIM6);

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
static void CORE_StateON(DATA_QUEUE ReceiveData,tCoreGlobalData* GlobalData, tStateCoreAutomat* StateAutomat)
{
	DATA_QUEUE	SendData;
	SendData.pointer=NULL;
	static GeneralPacketsUpOrDown_t	localTxBuffer;
	static uint8_t			localTxPacketSize=0;
	uint8_t					*TxBuffer;
	uint8_t					*RxUartMsg;

	Gl_HeapFree=xPortGetMinimumEverFreeHeapSize();


	switch (ReceiveData.Address)
	{
		case ADDR_TO_CORE_RF_DATA_RECEIVED:


			break;

		case ADDR_TO_CORE_TX_PACKET_DONE:
			//if(xTimerIsTimerActive(TimerRfTxTimeoutHandle) == true)	osTimerStop(TimerRfTxTimeoutHandle);

			//osTimerStart(TimerEnSendPacketHandle,TIME_TO_EN_SEND_NEXT_PACKET);	// Po kazdem odeslanem paketu cekame na mozny prijem dat

			osTimerStart(TimerRepeateTXHandle,300);

			break;

		case ADDR_TO_CORE_TX_PERIODIC:


			break;

		case ADDR_TO_CORE_SEND_LAST_PACKET:
			if(RC_GetSizeOfSavedPacket()!=0)
			{
				SendData.Address=ADDR_TO_RF_CMD;
				SendData.Data=RF_CMD_SEND_UNIVERSAL_PAYLOAD_NOW;
				xQueueSend(QueueRFHandle,&SendData,portMAX_DELAY);
			}
			else
			{
				//todo send error
			}
			break;

		case ADDR_TO_CORE_UART_READ_RX_BUFFER:

			PCT_FindAnyMsg();

		//	LL_TIM_EnableCounter(TIM6);

			//osTimerStart(TimerUartRxCheckHandle,TIME_TO_CHECK_UART_RX_BUFFER);
			break;

		case ADDR_TO_CORE_UART_RX_NEW_PACKET:
			RxUartMsg=ReceiveData.pointer;
			PCT_DecodeUartRxMsg(RxUartMsg);
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
static void CORE_StateStartOFF(DATA_QUEUE ReceiveData,tCoreGlobalData* GlobalData, tStateCoreAutomat* StateAutomat)
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

		LL_GPIO_ResetOutputPin(LED_RED_GPIO_Port,LED_RED_Pin);
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
