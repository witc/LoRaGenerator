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
#include  "SignalProcessing.h"
#include "LoadConfig.h"

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



tRadioParam 	RadioParam;
tPacketParam	PacketParam;

extern osMessageQId QueueRFHandle;


/**
 *
 * @param state
 * @return
 */
bool RC_RadioSetState(uint8_t state)
{
	LC_SaveRadioState(state);
	return true;
}

/**
 *
 * @return
 */
uint8_t RC_RadioGetRadioState(void)
{
	uint8_t ret;
#if (USE_INTERNAL_EEPROM==1)
	EepromStart(false);
	memcpy(&ret,(uint8_t*)EE_RADIO_STATE,1);
	EepromStop();
#endif

	return  ret;
}


/**
 *
 */
bool RC_RadioSetTxFreq(uint32_t freq)
{
	taskENTER_CRITICAL();
	RadioParam.TxConfig.freq = freq;
	taskEXIT_CRITICAL();

	LC_SaveTXFreq(freq);
	return true;
}

/**
 *
 * @return
 */
uint32_t RC_RadioGetTxFreq()
{
	uint32_t ret;
#if (USE_INTERNAL_EEPROM==1)
	EepromStart(false);
	memcpy(&RadioParam.TxConfig.freq,(uint8_t*)EE_RADIO_TX_FREQ,4);
	ret = RadioParam.TxConfig.freq;
	EepromStop();
#endif

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
	LC_SaveRXFreq(freq);
	return true;
}


/**
 *
 * @return
 */
uint32_t RC_RadioGetRxFreq()
{
	uint32_t ret;
#if (USE_INTERNAL_EEPROM==1)
	EepromStart(false);
	memcpy(&RadioParam.RxConfig.freq,(uint8_t*)EE_RADIO_RX_FREQ,4);
	ret = RadioParam.RxConfig.freq;
	EepromStop();
#endif
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

	LC_SaveTXPower(power);

	return true;
}


/**
 *
 * @return
 */
int8_t RC_RadioGetTxPower()
{
	int8_t ret;
#if (USE_INTERNAL_EEPROM==1)
	EepromStart(false);
	memcpy(&RadioParam.Power,(uint8_t*)EE_RADIO_TX_POWER,1);
	ret = RadioParam.Power;
	EepromStop();
#endif
	return ret;

}

/**
 *
 */
bool RC_RadioSetTxSf(uint8_t sf)
{
	for(uint8_t i=0; i< (sizeof(RadioSF)); i++)
	{
		if(sf == RadioSF[i])
		{
			taskENTER_CRITICAL();
			RadioParam.TxConfig.sf = i;
			taskEXIT_CRITICAL();
			LC_SaveTXSF(i);
			return true;
		}
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
#if (USE_INTERNAL_EEPROM==1)
	EepromStart(false);
	memcpy(&RadioParam.TxConfig.sf,(uint8_t*)EE_RADIO_TX_SF,1);
	ret = RadioParam.TxConfig.sf;
	EepromStop();
#endif
	for(uint8_t i=0; i< (DR_SF_SIZE); i++)
	{
		if(RadioParam.TxConfig.sf == i)
		{
			ret = RadioSF[i];
			break;
		}
	}

	ret = SP_ConstrainU32(ret,RadioSF[DR_SF5],RadioSF[DR_SF12]);
	return ret;
}
/**
 *
 */
bool RC_RadioSetRxSf(uint8_t sf)
{
	for(uint8_t i=0; i< (sizeof(RadioSF)); i++)
	{
		if(sf == RadioSF[i])
		{
			taskENTER_CRITICAL();
			RadioParam.RxConfig.sf = i;
			taskEXIT_CRITICAL();
			LC_SaveRXSF(i);
			return true;
		}
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
#if (USE_INTERNAL_EEPROM==1)
	EepromStart(false);
	memcpy(&RadioParam.RxConfig.sf,(uint8_t*)EE_RADIO_RX_SF,1);
	ret = RadioParam.RxConfig.sf;
	EepromStop();
#endif
	for(uint8_t i=0; i< (DR_SF_SIZE); i++)
	{
		if(RadioParam.RxConfig.sf == i)
		{
			ret = RadioSF[i];
			break;
		}
	}

	ret = SP_ConstrainU32(ret,RadioSF[DR_SF5],RadioSF[DR_SF12]);
	return ret;
}

/**
 *
 */
bool RC_RadioSetTxBw(uint32_t bw)
{
	for(uint8_t i=0; i< (sizeof(RadioBW)/4); i++)
	{
		if(bw == RadioBW[i])
		{
			taskENTER_CRITICAL();
			RadioParam.TxConfig.bw = i;
			taskEXIT_CRITICAL();
			LC_SaveTXBW(i);
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
	uint8_t temp;
	uint32_t ret=0;
#if (USE_INTERNAL_EEPROM==1)
	EepromStart(false);
	memcpy(&temp,(uint8_t*)EE_RADIO_TX_BW,1);
	EepromStop();
#endif
	RadioParam.TxConfig.bw = temp;

	for(uint8_t i=0; i< (DR_BW_SIZE); i++)
	{
		if(temp == i)
		{
			ret = RadioBW[i];
			break;
		}
	}

	ret = SP_ConstrainU32(ret,RadioBW[DR_BW_7_81_KHZ],RadioBW[DR_BW_500_KHZ]);
	return ret;

}

/**
 *
 */
bool RC_RadioSetRxBw(uint32_t bw)
{
	for(uint8_t i=0; i< (sizeof(RadioBW)/4); i++)
	{
		if(bw == RadioBW[i])
		{
			taskENTER_CRITICAL();
			RadioParam.RxConfig.bw = i;
			taskEXIT_CRITICAL();
			LC_SaveRXBW(i);
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
	uint8_t temp;
	uint32_t ret=0;

#if (USE_INTERNAL_EEPROM==1)
	EepromStart(false);
	memcpy(&temp,(uint8_t*)EE_RADIO_RX_BW,1);
	EepromStop();
#endif
	RadioParam.RxConfig.bw = temp;

	for(uint8_t i=0; i< (DR_BW_SIZE); i++)
	{
		if(temp == i)
		{
			ret = RadioBW[i];
			break;
		}
	}

	ret = SP_ConstrainU32(ret,RadioBW[DR_BW_7_81_KHZ],RadioBW[DR_BW_500_KHZ]);
	return ret;

}


/**
 *
 * @param iq
 * @return
 */
bool RC_RadioSetTxIq(uint8_t Iq)
{
	for(uint8_t i=0; i< (sizeof(RadioIQ)); i++)
	{
		if(Iq == RadioIQ[i])
		{
			taskENTER_CRITICAL();
			RadioParam.TxConfig.iq = i;
			taskEXIT_CRITICAL();
			LC_SaveTXIQ(i);
			return true;
		}
	}

	return false;
}

/**
 *
 * @return
 */
uint8_t RC_RadioGetTxIq()
{
	uint8_t temp;
	uint32_t ret=0;
#if (USE_INTERNAL_EEPROM==1)
	EepromStart(false);
	memcpy(&temp,(uint8_t*)EE_RADIO_TX_INVERT,1);
	EepromStop();
#endif
	RadioParam.TxConfig.iq = temp;

	for(uint8_t i=0; i< (DR_IQ_SIZE); i++)
	{
		if(temp == i)
		{
			ret = RadioIQ[i];
			break;
		}
	}

	ret = SP_ConstrainU32(ret,RadioIQ[DR_IQ_FALSE],RadioIQ[DR_IQ_TRUE]);
	return ret;
}

/**
 *
 * @param iq
 * @return
 */
bool RC_RadioSetRxIq(uint8_t Iq)
{
	for(uint8_t i=0; i< (sizeof(RadioIQ)); i++)
	{
		if(Iq == RadioIQ[i])
		{
			taskENTER_CRITICAL();
			RadioParam.RxConfig.iq = i;
			taskEXIT_CRITICAL();
			LC_SaveRXIQ(i);
			return true;
		}
	}

	return false;
}

uint8_t RC_RadioGetRxIq()
{
	uint8_t temp;
	uint32_t ret=0;
#if (USE_INTERNAL_EEPROM==1)
	EepromStart(false);
	memcpy(&temp,(uint8_t*)EE_RADIO_RX_INVERT,1);
	EepromStop();
#endif
	RadioParam.RxConfig.iq = temp;

	for(uint8_t i=0; i< (DR_IQ_SIZE); i++)
	{
		if(temp == i)
		{
			ret = RadioIQ[i];
			break;
		}
	}

	ret = SP_ConstrainU32(ret,RadioIQ[DR_IQ_FALSE],RadioIQ[DR_IQ_TRUE]);
	return ret;
}

/**
 *
 * @param iq
 * @return
 */
bool RC_RadioSetTxCr(uint8_t Cr)
{
	for(uint8_t i=0; i< (sizeof(RadioCR)); i++)
	{
		if(Cr == RadioCR[i])
		{
			taskENTER_CRITICAL();
			RadioParam.TxConfig.cr = i;
			taskEXIT_CRITICAL();
			LC_SaveTXCR(i);
			return true;
		}
	}

	return false;

}

/**
 *
 * @return
 */
uint8_t RC_RadioGetTxCr()
{
	uint8_t temp;
	uint8_t ret=0;
#if (USE_INTERNAL_EEPROM==1)
	EepromStart(false);
	memcpy(&temp,(uint8_t*)EE_RADIO_TX_CR,1);
	EepromStop();
#endif
	RadioParam.TxConfig.cr = temp;

	for(uint8_t i=0; i< (DR_CR_SIZE); i++)
	{
		if(temp == i)
		{
			ret = RadioCR[i];
			break;
		}
	}

	ret = SP_ConstrainU32(ret,RadioCR[DR_CR_45],RadioCR[DR_CR_48]);
	return ret;
}



/**
 *
 * @param cr
 * @return
 */
bool RC_RadioSetRxCr(uint8_t Cr)
{
	for(uint8_t i=0; i< (sizeof(RadioCR)); i++)
	{
		if(Cr == RadioCR[i])
		{
			taskENTER_CRITICAL();
			RadioParam.RxConfig.cr = i;
			taskEXIT_CRITICAL();
			LC_SaveRXCR(i);
			return true;
		}
	}

	return false;

}

/**
 *
 * @return
 */
uint8_t RC_RadioGetRxCr()
{
	uint8_t temp;
	uint8_t ret=0;
#if (USE_INTERNAL_EEPROM==1)
	EepromStart(false);
	memcpy(&temp,(uint8_t*)EE_RADIO_RX_CR,1);
	EepromStop();
#endif
	RadioParam.RxConfig.cr = temp;

	for(uint8_t i=0; i< (DR_CR_SIZE); i++)
	{
		if(temp == i)
		{
			ret = RadioCR[i];
			break;
		}
	}

	ret = SP_ConstrainU32(ret,RadioCR[DR_CR_45],RadioCR[DR_CR_48]);
	return ret;
}


/**
 *
 * @return
 */
bool RC_RadioSetTXHeaderMode(uint8_t header)
{
	if(header<DR_HEADER_SIZE)
	{
		taskENTER_CRITICAL();
		RadioParam.TxConfig.headerMode=(ENUM_DR_HEADER)header;
		taskEXIT_CRITICAL();
		LC_SaveTXHeader(header);
		return true;
	}

	return false;

}


/**
 *
 * @return
 */
uint8_t RC_RadioGetTXHeaderMode()
{
	uint8_t temp;
	uint32_t ret=0;
#if (USE_INTERNAL_EEPROM==1)
	EepromStart(false);
	memcpy(&temp,(uint8_t*)EE_RADIO_TX_HEADER,1);
	EepromStop();
#endif
	RadioParam.TxConfig.headerMode = temp;

	for(uint8_t i=0; i< (DR_HEADER_SIZE); i++)
	{
		if(temp == i)
		{
			ret = RadioHeader[i];
			break;
		}
	}

	ret = SP_ConstrainU32(ret,RadioHeader[DR_HEADER_FALSE],RadioIQ[DR_HEADER_TRUE]);
	return ret;
}


/**
 *
 * @return
 */
bool RC_RadioSetRXHeaderMode(uint8_t header)
{

	if(header<DR_HEADER_SIZE)
	{
		taskENTER_CRITICAL();
		RadioParam.RxConfig.headerMode=(ENUM_DR_HEADER)header;
		taskEXIT_CRITICAL();
		LC_SaveRXHeader(header);
		return true;
	}

	return false;

}
/**
 *
 * @return
 */
uint8_t RC_RadioGetRXHeaderMode()
{
	uint8_t temp;
	uint32_t ret=0;
#if (USE_INTERNAL_EEPROM==1)
	EepromStart(false);
	memcpy(&temp,(uint8_t*)EE_RADIO_RX_HEADER,1);
	EepromStop();
#endif
	RadioParam.RxConfig.headerMode = temp;

	for(uint8_t i=0; i< (DR_HEADER_SIZE); i++)
	{
		if(temp == i)
		{
			ret = RadioHeader[i];
			break;
		}
	}

	ret = SP_ConstrainU32(ret,RadioHeader[DR_HEADER_FALSE],RadioIQ[DR_HEADER_TRUE]);
	return ret;
}


/**
 *
 * @return
 */
bool RC_RadioSetTXCRC(uint8_t crc)
{

	if(crc<DR_CRC_SIZE)
	{
		taskENTER_CRITICAL();
		RadioParam.TxConfig.crcCheck=(ENUM_DR_CRC)crc;
		taskEXIT_CRITICAL();
		LC_SaveTXCRC(crc);
		return true;
	}

	return false;

}
/**
 *
 * @return
 */
uint8_t RC_RadioGetTXCRC()
{
	uint8_t temp;
	uint32_t ret=0;
#if (USE_INTERNAL_EEPROM==1)
	EepromStart(false);
	memcpy(&temp,(uint8_t*)EE_RADIO_TX_CRC,1);
	EepromStop();
#endif
	RadioParam.TxConfig.crcCheck = temp;

	for(uint8_t i=0; i< (DR_CRC_SIZE); i++)
	{
		if(temp == i)
		{
			ret = RadioCrc[i];
			break;
		}
	}

	ret = SP_ConstrainU32(ret,RadioCrc[DR_CRC_FALSE],RadioCrc[DR_CRC_TRUE]);
	return ret;
}


/**
 *
 * @return
 */
bool RC_RadioSetRXCRC(uint8_t crc)
{
	if(crc<DR_CRC_SIZE)
	{
		taskENTER_CRITICAL();
		RadioParam.RxConfig.crcCheck=(ENUM_DR_CRC)crc;
		taskEXIT_CRITICAL();
		LC_SaveRXCRC(crc);
		return true;
	}

	return false;

}
/**
 *
 * @return
 */
uint8_t RC_RadioGetRXCRC()
{
	uint8_t temp;
	uint32_t ret=0;
#if (USE_INTERNAL_EEPROM==1)
	EepromStart(false);
	memcpy(&temp,(uint8_t*)EE_RADIO_RX_CRC,1);
	EepromStop();
#endif
	RadioParam.RxConfig.crcCheck = temp;

	for(uint8_t i=0; i< (DR_CRC_SIZE); i++)
	{
		if(temp == i)
		{
			ret = RadioCrc[i];
			break;
		}
	}

	ret = SP_ConstrainU32(ret,RadioCrc[DR_CRC_FALSE],RadioCrc[DR_CRC_TRUE]);
	return ret;
}


/**
 *
 * @return
 */
uint32_t RC_RadioGetRadioStatus()
{
	uint32_t ret;

	taskENTER_CRITICAL();
	//ret = RadioParam.TxConfig.crcCheck;	TODO
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
	if(data[2]<=MAX_ALLOWEDPACKET_SIZE)
	{

#if (USE_INTERNAL_EEPROM==1)
		EepromStart(true);

		HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_BYTE,EE_RF_PACKET_SIZE,data[2]);
		for(uint8_t i=0;i<data[2];i++)
		{
			HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_BYTE,EE_RF_DATA_PACKET+i*sizeof(uint8_t),data[3+i]);
		}
		EepromStop();
#endif
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
#if (USE_INTERNAL_EEPROM==1)
	EepromStart(false);
	memcpy(&temp,(uint8_t *)EE_RF_PACKET_SIZE,1);
	EepromStop();
#endif

	return temp;
}

/**
 *
 * @return
 */
bool RC_RadioSetRXPayloadSize(uint8_t payloadSize)
{

	LC_SaveRxPayloadSize(payloadSize);
	return true;
}


/**
 *
 * @param payloadSize
 * @return
 */
uint8_t RC_RadioGetRXPayloadSize()
{
	uint8_t temp;
#if (USE_INTERNAL_EEPROM==1)
	EepromStart(false);
	memcpy(&temp,(uint8_t *)EE_RADIO_RX_SIZE,1);
	EepromStop();
#endif

	return temp;
}
