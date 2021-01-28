/*
 * RadioCommands.c
 *
 *  Created on: 28. 1. 2021
 *      Author: jan.ropek
 */

#include "main.h"
#include "TaskRF.h"
#include "RadioCommands.h"
#include "TemplateRadioUser.h"

//bool (*PCT_RadioSetFunc[sizeof(eUartMsgCmds)])() = {&PCT_RadioSetTxFreq,
//													&PCT_RadioSetRxFreq,
//													&PCT_RadioSetTxPower,
//													&PCT_RadioSetTxSF,
//													&PCT_RadioSetTxBW,
//													&PCT_RadioSetTxIQ,
//													&PCT_RadioSetTxCR,
//													&PCT_RadioSetStandby,
//													&PCT_RadioSetTxCW,
//													};

tRadioParam RadioParam;

extern osMessageQId QueueRFHandle;
/**
 *
 */
bool RC_RadioSetTxFreq(uint32_t freq)
{
	taskENTER_CRITICAL();
	RadioParam.TxFreq = freq;
	taskEXIT_CRITICAL();

	return true;
}

/**
 *
 */
bool RC_RadioSetRxFreq(uint32_t freq)
{
	taskENTER_CRITICAL();
	RadioParam.RxFreq = freq;
	taskEXIT_CRITICAL();

	return true;
}

/**
 *
 * @param power
 * @return
 */
bool RC_RadioSetTxPower(int8_t power)
{
	taskENTER_CRITICAL();
	RadioParam.Power = power;
	taskEXIT_CRITICAL();

	EepromStart(true);
	HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_BYTE,EEPROM_RF_TX_POWER,power);
	EepromStop();

	return true;
}

/**
 *
 * @return
 */
bool RC_RadioSetStandby()
{
	DATA_QUEUE SendData;
	SendData.pointer = NULL;
	SendData.Address = ADDR_TO_RF_CMD;
	SendData.Data = RF_CMD_STANDBY;

	xQueueSend(QueueRFHandle,&SendData,portMAX_DELAY);

}

/**
 *
 * @return
 */
bool RC_RadioSetTxCW()
{
	DATA_QUEUE SendData;
	SendData.pointer = NULL;
	SendData.Address = ADDR_TO_RF_CMD;
	SendData.Data = RF_CMD_TX_CW;

	xQueueSend(QueueRFHandle,&SendData,portMAX_DELAY);
}


bool RC_SavePacket(uint8_t *data)
{
	if(data[1]<=MAX_ALLOWEDPACKET_SIZE)
	{
		EepromStart(true);

		HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_BYTE,EEPROM_RF_PACKET_SIZE,data[1]);
		for(uint8_t i=0;i<data[1];i++)
		{
			HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_BYTE,EEPROM_RF_DATA_PACKET+i*sizeof(uint8_t),data[2+i]);
		}
		EepromStop();
	}
	else
	{
		//todo poslat chybu
	}

}
