/*
 * TaskRF.c
 *
 *  Created on: 12. 3. 2020
 *      Author: jan.ropek
 */


#include "main.h"
#include "TaskRF.h"
#include "radio.h"
#include "TemplateRadioUser.h"
#include "ProcessCoreTask.h"
#include "ProcessRFTask.h"
#include "RadioCommands.h"
/*!
 *  Freertos objects
 */
extern osMessageQId		 QueueCoreHandle;
extern osMessageQId		 QueueRFHandle;
extern osTimerId 		 TimerLEDHandle;
extern osTimerId TimerRepeateTXHandle;
extern SPI_HandleTypeDef hspi2;


tRadioParam 	RadioParam;
/**
 *
 * @param ReceiveData
 * @param GlobalData
 * @param StateAutomat
 * @param PointerMalloc
 * @return
 */
static void RF_StateINIT(DATA_QUEUE ReceiveData,
		tRfGlobalData* GlobalData, tStateRfAutomat* StateAutomat,
		void** PointerMalloc);
/**
 *
 * @param ReceiveData
 * @param GlobalData
 * @param StateAutomat
 * @param PointerMalloc
 * @return
 */
static void RF_StateOFF(DATA_QUEUE ReceiveData,
		tRfGlobalData* GlobalData, tStateRfAutomat* StateAutomat,
		void** PointerMalloc);
/**
 *
 * @param ReceiveData
 * @param GlobalData
 * @param StateAutomat
 * @param PointerMalloc
 * @return
 */
static void RF_StateStartON(DATA_QUEUE ReceiveData,
		tRfGlobalData* GlobalData, tStateRfAutomat* StateAutomat,
		void** PointerMalloc);
/**
 *
 * @param ReceiveData
 * @param GlobalData
 * @param StateAutomat
 * @param PointerMalloc
 * @return
 */
static void RF_StateON(DATA_QUEUE ReceiveData,
		tRfGlobalData* GlobalData, tStateRfAutomat* StateAutomat,
		void** PointerMalloc);

/**
 *
 * @param ReceiveData
 * @param GlobalData
 * @param StateAutomat
 * @param PointerMalloc
 * @return
 */
static void RF_StateStartOFF(DATA_QUEUE ReceiveData,
		tRfGlobalData* GlobalData, tStateRfAutomat* StateAutomat,
		void** PointerMalloc);

/**
 *
 * @param
 * @param
 * @param
 * @param
 * @return
 */
static void (*StateRF[])(DATA_QUEUE, tRfGlobalData*,	tStateRfAutomat*, void**) =
		{	RF_StateINIT, RF_StateOFF, RF_StateStartON , RF_StateON, RF_StateStartOFF };


/**
 *
 * @param ReceiveData
 * @param GlobalData
 * @param StateAutomat
 * @param PointerMalloc
 * @return
 */
static void RF_StateINIT(DATA_QUEUE ReceiveData,tRfGlobalData* GlobalData, tStateRfAutomat* StateAutomat,void** PointerMalloc)
{
	/*Reset and + test Rf chip communication */
	if(RU_SX1262Assign() != true)
	{
		LogError(1235);
	}

	taskENTER_CRITICAL();
	LL_EXTI_DisableIT_0_31(LL_EXTI_LINE_2);//interrupt from Semtech
	LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_2);
	taskEXIT_CRITICAL();

	StateAutomat->PreviousState=StateAutomat->ActualState;
	StateAutomat->ActualState=STATE_RF_OFF;

}

/**
 *
 * @param ReceiveData
 * @param GlobalData
 * @param StateAutomat
 * @param PointerMalloc
 * @return
 */
static void RF_StateOFF(DATA_QUEUE ReceiveData,tRfGlobalData* GlobalData, tStateRfAutomat* StateAutomat,void** PointerMalloc)
{
	DATA_QUEUE SendData;
	SendData.pointer=NULL;
	eStateCoreSystem	state=StateAutomat->ActualState;

	if(ReceiveData.Address==ADDR_TO_RF_CHANGE_STATE)
	{
		SendData.Address=ADDR_TO_CORE_TASK_STATE_CHANGED;

		if(ReceiveData.Data==DATA_TO_RF_START_ON)
		{
			RadioInit();

			taskENTER_CRITICAL();
			LL_EXTI_DisableIT_0_31(LL_EXTI_LINE_2);//interrupt from Semtech - only one IRQ
			LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_2);
			LL_EXTI_EnableIT_0_31(LL_EXTI_LINE_2);	//interrupt from Semtech - only one IRQ
			taskEXIT_CRITICAL();

			SendData.Data=DATA_TO_CORE_RF_IS_ON;
			state=STATE_RF_ON;
			xQueueSend(QueueCoreHandle,&SendData,portMAX_DELAY);

			/* Init Variables. */

			RadioParam.TxConfig.bw=RC_RadioGetTxBw();
			RadioParam.TxConfig.iq=RC_RadioGetTxIq();
			RadioParam.TxConfig.sf=RC_RadioGetTxSf();

			RadioParam.RxConfig.bw=RC_RadioGetRxBw();
			RadioParam.RxConfig.iq=RC_RadioGetRxIq();
			RadioParam.RxConfig.sf=RC_RadioGetRxSf();

			RadioParam.Power = RC_RadioGetTxPower();
			if(RadioParam.Power>22)	RadioParam.Power = 22;
			else if (RadioParam.Power<(-72)) RadioParam.Power = -72;

			PRT_SetAtten1To(0);
			PRT_SetAtten2To(0);

			RadioCleanAndStandby();
			//RU_LoRaConfigAndStartRX(RC_RadioGetRxFreq(),RadioParam.RxConfig,true,portMAX_DELAY);

		}
		else if(ReceiveData.Data==DATA_TO_RF_START_OFF)
		{
			RadioDeinit();
			taskENTER_CRITICAL();
			LL_EXTI_DisableIT_0_31(LL_EXTI_LINE_2); //interrupt from Semtech - only one IRQ
			LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_2);
			taskEXIT_CRITICAL();

			SendData.Data=DATA_TO_CORE_RF_IS_OFF;
			state=STATE_RF_OFF;
			xQueueSend(QueueCoreHandle,&SendData,portMAX_DELAY);
		}

		StateAutomat->PreviousState=StateAutomat->ActualState;
		StateAutomat->ActualState=state;
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
static void RF_StateStartON(DATA_QUEUE ReceiveData,tRfGlobalData* GlobalData, tStateRfAutomat* StateAutomat,void** PointerMalloc)
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
static void RF_StateON(DATA_QUEUE ReceiveData,tRfGlobalData* GlobalData, tStateRfAutomat* StateAutomat,void** PointerMalloc)
{
	DATA_QUEUE SendData;
	SendData.pointer = NULL;
	p_tStaticStructure_RF LocalStaticStructure;

	/* Get RF state*/
	GlobalData->RF_State = RadioGetStatus();

	switch (ReceiveData.Address)
	{
		case ADDR_TO_RF_CHANGE_STATE:
			if(ReceiveData.Data==DATA_TO_RF_START_OFF)
			{
				RadioDeinit();

				taskENTER_CRITICAL();
				LL_EXTI_DisableIT_0_31(LL_EXTI_LINE_2);//interrupt from Semtech
				LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_2);
				taskEXIT_CRITICAL();

				SendData.Address=ADDR_TO_CORE_TASK_STATE_CHANGED;
				SendData.Data=DATA_TO_CORE_RF_IS_OFF;
				xQueueSend(QueueCoreHandle,&SendData,portMAX_DELAY);

				StateAutomat->PreviousState=StateAutomat->ActualState;
				StateAutomat->ActualState=STATE_RF_OFF;

				/* Deinit Variables. */

			}
			else if(ReceiveData.Data==DATA_TO_RF_START_ON)
			{
				SendData.Address=ADDR_TO_CORE_TASK_STATE_CHANGED;
				SendData.Data=DATA_TO_CORE_RF_IS_ON;

				xQueueSend(QueueCoreHandle,&SendData,portMAX_DELAY);

				StateAutomat->PreviousState=StateAutomat->ActualState;
				StateAutomat->ActualState=STATE_RF_ON;

				if (GlobalData->RF_State != RF_IDLE)	RadioCleanAndStandby();
			}
			break;

		case ADDR_TO_RF_CMD:
			RU_CommandProcess((RfCommands)ReceiveData.Data,GlobalData, &ReceiveData);
			break;

		case ADDR_TO_RF_SX1262_IRQ:
			RU_IRQProcess(GlobalData);
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
static void RF_StateStartOFF(DATA_QUEUE ReceiveData,tRfGlobalData* GlobalData, tStateRfAutomat* StateAutomat,void** PointerMalloc)
{

}

/**
 *
 * @param argument
 */
void StartTaskRF(void const * argument)
{
	DATA_QUEUE ReceiveData;

	static tRfGlobalData DataTaskRf;
	static tStateRfAutomat StateAutomat = {STATE_RF_INIT,STATE_RF_INIT};
	static void* PointerToMalloc = NULL;
	static portBASE_TYPE ReturnValue;

	RF_StateINIT(ReceiveData,&DataTaskRf, &StateAutomat,&PointerToMalloc);

	for(;;)
	{
		ReturnValue = xQueueReceive(QueueRFHandle, &ReceiveData, portMAX_DELAY);
		if (ReturnValue == pdPASS)
		{
			StateRF[StateAutomat.ActualState](ReceiveData, &DataTaskRf, &StateAutomat,&PointerToMalloc);

			/* Clear malloc */
			vPortFree(ReceiveData.pointer);
			ReceiveData.pointer=NULL;
		}
	}
}

