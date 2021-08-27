/*
 * AuxRfProcess.c
 *
 *  Created on: 19. 3. 2020
 *      Author: jan.ropek
 */

#include "main.h"
#include "TaskCore.h"
#include "ProcessCoreTask.h"
#include "TemplateRadioUser.h"
#include "AuxRfProcess.h"
#include "EepromAddress.h"
#include "SignalProcessing.h"


extern osMessageQId QueueRFHandle;
extern osMessageQId QueueCoreHandle;
extern osTimerId TimerRfTxTimeoutHandle;
extern osTimerId TimerEnSendPacketHandle;

/**
 *
 * @param ReceiveData
 * @param GlobalData
 */
void ARP_RfStandardTreatment(DATA_QUEUE ReceiveData,tCoreGlobalData* GlobalData)
{
	DATA_QUEUE SendData;
	SendData.pointer=NULL;
	tGeneralPacket 	*RxPacket;

	RxPacket=ReceiveData.pointer;

}

/**
 *
 * @param GlobalData
 * @param kVoltage
 * @param battVoltage
 * @param sendNow
 * @param sendAlarm
 * @return
 */
uint8_t ARP_CreateMonitorInfo(tCoreGlobalData *GlobalData,uint8_t kVoltage,uint8_t battVoltage, bool checkAlarm, tGeneralPacket  *TxPacket)
{
	//GeneralPacketsUpOrDown_t 	TxPacket;
//	uint8_t size=0;
//	memset(TxPacket->DataArray,0,sizeof(UpMonitorInfoStruct_t));
//
//	osThreadSuspendAll();
//
//	TxPacket->UpMonitorInfo_V1.Payload.Voltage1 = kVoltage;
//	TxPacket->UpMonitorInfo_V1.Payload.BattVoltage = battVoltage;
//	TxPacket->UpMonitorInfo_V1.Payload.AlarmLevel = PCT_GetMyAlarmLevel();
//	TxPacket->UpMonitorInfo_V1.Payload.TxPeriod.Bits.Minutes = 4;// CL_GetTimeTXPeriod();	//TODO na minuty
//	TxPacket->UpMonitorInfo_V1.Payload.TxPeriod.Bits.Hours = 0;
//
//	if(checkAlarm==true)
//	{
//		if((kVoltage<=TxPacket->UpMonitorInfo_V1.Payload.AlarmLevel)&&(TxPacket->UpMonitorInfo_V1.Payload.AlarmLevel!=0))
//		{
//			TxPacket->UpMonitorInfo_V1.Payload.AlarmField|=(RX_ALARM_FENCE);
//		}
//		else
//		{
//			TxPacket->UpMonitorInfo_V1.Payload.AlarmField&=~(RX_ALARM_FENCE);
//		}
//	}
//
//	//ONLY Blinking Batt
//	if((GlobalData->BattPercent)<=BATT_ALARM_LEVEL_BLINK)
//	{
//		TxPacket->UpMonitorInfo_V1.Payload.AlarmField|=(RX_ALARM_BATT_ONLY_BLINK);
//	}
//	else
//	{
//		TxPacket->UpMonitorInfo_V1.Payload.AlarmField&=~(RX_ALARM_BATT_ONLY_BLINK);
//	}
//
//	// Batt Sound
//	if((GlobalData->BattPercent)<=BATT_ALARM_LEVEL_SOUND)
//	{
//		TxPacket->UpMonitorInfo_V1.Payload.AlarmField|=(RX_ALARM_BATT_SOUND);
//	}
//	else
//	{
//		TxPacket->UpMonitorInfo_V1.Payload.AlarmField&=~(RX_ALARM_BATT_SOUND);
//	}
//
//	globalSmazatBattAlarm = TxPacket->UpMonitorInfo_V1.Payload.AlarmField;
//	osThreadResumeAll();
//
//	GlobalData->FrameCounterWait=CounterMemoryGetActualCounter(3);
//	if(GlobalData->MaxAttemptToSendData > MIN_ATTEMPTS_TO_SEND_DATA) GlobalData->MaxAttemptToSendData-=1;
//
//	LD_UpMonitorInfo(TxPacket,GlDefaultDrTX,&size);
//
//	return size;
}


/**
 *
 * @param GlobalData
 * @param kVoltage
 * @param battVoltage
 */
void ARP_SendMonitorInfo(tCoreGlobalData *GlobalData,tGeneralPacket *pBuffer,uint8_t size, bool sendNow)
{
//	DATA_QUEUE	SendData;
//	SendData.pointer=NULL;
//	uint8_t		*TxBuffer;
//
//	TxBuffer=pvPortMalloc(size);
//	if(TxBuffer==NULL)
//	{
//		osDelayWatchdog(500);
//		TxBuffer=pvPortMalloc(size);
//		if(TxBuffer==NULL)		LogError(57418);
//	}
//
//	memcpy(TxBuffer,pBuffer,size);
//
//	SendData.Address = ADDR_TO_RF_CMD;
//	if(sendNow==true)
//	{
//		SendData.Data=RF_CMD_SEND_UNIVERSAL_PAYLOAD_NOW;
//	}
//	else
//	{
//		SendData.Data=RF_CMD_SEND_UNIVERSAL_PAYLOAD;
//	}
//
//	SendData.temp=size;
//	SendData.RFU=(uint64_t)RU_GetDrTX().Value;
//	SendData.pointer=TxBuffer;
//	xQueueSend(QueueRFHandle, &SendData, portMAX_DELAY);

//	PCT_DecreaseRequestsToSendPacket(GlobalData);
}




