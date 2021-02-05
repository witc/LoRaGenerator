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

const uint32_t radioDefinedBw[9] ={
									7810,
									10420,
									15630,
									20830,
									31250,
									41670,
									62500,
									125000,
									500000
								  };


tRadioParam 	RadioParam;
tPacketParam	PacketParam;

extern osMessageQId QueueRFHandle;
/**
 *
 */
bool RC_RadioSetTxFreq(uint32_t freq)
{
	taskENTER_CRITICAL();
	RadioParam.TxConfig.freq = freq;
	taskEXIT_CRITICAL();

	return true;
}

/**
 *
 * @return
 */
uint32_t RC_RadioGetTxFreq()
{
	uint32_t ret;
	taskENTER_CRITICAL();
	ret = RadioParam.TxConfig.freq;
	taskEXIT_CRITICAL();

	return ret;
}

/**
 *
 */
bool RC_RadioSetRxFreq(uint32_t freq)
{
	taskENTER_CRITICAL();
	RadioParam.RxConfig.freq = freq;
	taskEXIT_CRITICAL();

	return true;
}


/**
 *
 * @return
 */
uint32_t RC_RadioGetRxFreq()
{
	uint32_t ret;

	taskENTER_CRITICAL();
	ret	= RadioParam.RxConfig.freq;
	taskEXIT_CRITICAL();

	return ret;
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
int8_t RC_RadioGetTxPower()
{
	int8_t ret;
	taskENTER_CRITICAL();
	ret = RadioParam.Power;
	taskEXIT_CRITICAL();

	return ret;
}

/**
 *
 */
bool RC_RadioSetTxSf(uint8_t sf)
{
	if((sf >= 6)&&(sf<=12))
	{
		sf-=6;
		taskENTER_CRITICAL();
		RadioParam.TxConfig.SfBqIq.Bits.SF = sf;
		taskEXIT_CRITICAL();

		return true;
	}

	return false;
}


/**
 *
 * @return
 */
uint32_t RC_RadioGetTxSf()
{
	uint32_t ret;
	taskENTER_CRITICAL();
	ret = RadioParam.TxConfig.SfBqIq.Bits.SF;
	taskEXIT_CRITICAL();

	return ret;
}
/**
 *
 */
bool RC_RadioSetRxSf(uint8_t sf)
{
	if((sf >= 6)&&(sf<=12))
	{
		sf-=6;
		taskENTER_CRITICAL();
		RadioParam.RxConfig.SfBqIq.Bits.SF = sf;
		taskEXIT_CRITICAL();

		return true;
	}

	return false;
}

/**
 *
 * @return
 */
uint32_t RC_RadioGetRxSf()
{
	uint32_t ret;

	taskENTER_CRITICAL();
	ret = RadioParam.RxConfig.SfBqIq.Bits.SF;
	taskEXIT_CRITICAL();

	return ret;
}

/**
 *
 */
bool RC_RadioSetTxBw(uint32_t bw)
{
	for(uint8_t i=0; i< (sizeof(radioDefinedBw)/4); i++)
	{
		if(bw == radioDefinedBw[i])
		{
			taskENTER_CRITICAL();
			RadioParam.TxConfig.SfBqIq.Bits.BW = i;
			taskEXIT_CRITICAL();

			return true;
		}
	}

	return false;
}

/**
 *
 * @return
 */
uint32_t RC_RadioGetTxBw()
{
	uint32_t ret;

	taskENTER_CRITICAL();
	ret = RadioParam.TxConfig.SfBqIq.Bits.BW;
	taskEXIT_CRITICAL();

	return ret;
}

/**
 *
 */
bool RC_RadioSetRxBw(uint32_t bw)
{
	for(uint8_t i=0; i< (sizeof(radioDefinedBw)/4); i++)
	{
		if(bw == radioDefinedBw[i])
		{
			taskENTER_CRITICAL();
			RadioParam.RxConfig.SfBqIq.Bits.BW = i;
			taskEXIT_CRITICAL();

			return true;
		}
	}

	return false;
}


/**
 *
 * @param bw
 * @return
 */
uint32_t RC_RadioGetRxBw()
{
	uint32_t ret;

	taskENTER_CRITICAL();
	ret = RadioParam.RxConfig.SfBqIq.Bits.BW;
	taskEXIT_CRITICAL();

	return ret;

}


/**
 *
 * @param iq
 * @return
 */
bool RC_RadioSetTxIq(uint8_t Iq)
{
	if(Iq>1)
	{
		return false;
	}

	taskENTER_CRITICAL();
	RadioParam.TxConfig.SfBqIq.Bits.IqInvert = Iq;
	taskEXIT_CRITICAL();

	return true;
}

/**
 *
 * @return
 */
uint32_t RC_RadioGetTxIq()
{
	uint32_t ret;

	taskENTER_CRITICAL();
	ret = RadioParam.TxConfig.SfBqIq.Bits.IqInvert;
	taskEXIT_CRITICAL();

	return ret;
}

/**
 *
 * @param iq
 * @return
 */
bool RC_RadioSetRxIq(uint8_t Iq)
{
	if(Iq>1)
	{
		return false;
	}

	taskENTER_CRITICAL();
	RadioParam.RxConfig.SfBqIq.Bits.IqInvert = Iq;
	taskEXIT_CRITICAL();

	return true;
}

uint32_t RC_RadioGetRxIq()
{
	uint32_t ret;

	taskENTER_CRITICAL();
	ret = RadioParam.RxConfig.SfBqIq.Bits.IqInvert;
	taskEXIT_CRITICAL();

	return ret;

}

/**
 *
 * @param iq
 * @return
 */
bool RC_RadioSetTxCr(uint8_t Cr)
{
	if((Cr>=45)&&(Cr=48))
	{
		Cr-=45;
		taskENTER_CRITICAL();
		RadioParam.TxConfig.SfBqIq.Bits.CR = Cr;
		taskEXIT_CRITICAL();

		return true;
	}

	return false;
}

/**
 *
 * @return
 */
uint32_t RC_RadioGetTxCr()
{
	uint32_t ret;

	taskENTER_CRITICAL();
	ret = RadioParam.TxConfig.SfBqIq.Bits.CR;
	taskEXIT_CRITICAL();

	return ret;
}



/**
 *
 * @param iq
 * @return
 */
bool RC_RadioSetRxCr(uint8_t Cr)
{
	if((Cr>=45)&&(Cr=48))
	{
		Cr-=45;
		taskENTER_CRITICAL();
		RadioParam.RxConfig.SfBqIq.Bits.CR = Cr;
		taskEXIT_CRITICAL();

		return true;
	}

	return false;
}

/**
 *
 * @return
 */
uint32_t RC_RadioGetRxCr()
{
	uint32_t ret;

	taskENTER_CRITICAL();
	ret = RadioParam.RxConfig.SfBqIq.Bits.CR;
	taskEXIT_CRITICAL();

	return ret;
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

/**
 *
 * @return
 */
uint8_t RC_GetSizeOfSavedPacket(void)
{
	uint8_t temp;
	EepromStart(false);
	memcpy(&temp,(uint8_t *)EEPROM_RF_PACKET_SIZE,1);
	EepromStop();

	return temp;
}
