/*
 * TemplateRadioUser.c
 *
 *  Created on: 7. 2. 2020
 *      Author: jan.ropek
 */

#include "main.h"
#include "TaskRF.h"
#include "ProcessCoreTask.h"
#include "radio_general.h"
#include "TemplateRadioUser.h"
#include "LoRa_Codec.h"
#include "sx126x.h"
#include "radio.h"



/**
 *
 */
__attribute__(( weak )) bool RU_SX1262Assign(void)
{

	return false;
}

/*
 * @brief : Serve IRQ from Semtech
 */
__attribute__(( weak )) uint8_t RU_IRQProcess(tRfGlobalData* GlobalData)
{

	return 0;
}

/*
 * @brief: Command recieved to RF task
 */
__attribute__(( weak ))void RU_CommandProcess(RfCommands cmd,tRfGlobalData* GlobalData, DATA_QUEUE *ReceiveData)
{


}


/*
 * brief: Set RX config
 */
__attribute__(( weak )) void RU_LoRaConfigAndStartRX (tRadioParam 	radioParam, bool Rx,uint8_t payloadSize, uint32_t rxTimeout)
{
	bool rxContin=false;

	PRT_SetAtten1To(0);
	PRT_SetAtten2To(0);

	if(rxTimeout==portMAX_DELAY)	rxContin=true;

	RadioSetRxConfig(radioParam.RxConfig.freq,MODEM_LORA,radioParam.RxConfig.bw,radioParam.RxConfig.sf+5,
			radioParam.RxConfig.cr+1/*cr*/,0/*AFC*/,8/*preamble*/,0/*symbtimeout to lock*/,
			 (radioParam.RxConfig.headerMode==true) ? false:true/*fixlen*/,payloadSize/*length*/,
			(bool)radioParam.RxConfig.crcCheck/*crc on*/,0,0,(bool)radioParam.RxConfig.iq,rxContin/*rx continous*/);

	if (Rx == true)   RadioRxBoosted (rxTimeout);
}



/*
 * brief: set default parametter for UP packets
 */
__attribute__(( weak )) void RU_RFSetTXUp(tRadioParam 	radioParam)
{
	/* Radio TX configuration for "Up" way */

	RadioSetTxConfig(radioParam.TxConfig.freq, MODEM_LORA /* modem*/,(int8_t)radioParam.Power /*power*/, 0/*fdev*/,
			radioParam.TxConfig.bw/*BW*/,radioParam.TxConfig.sf+5 /*SF*/,radioParam.TxConfig.cr /*coderate*/,
			8 /*preamble*/,  (radioParam.TxConfig.headerMode==true) ? false:true /*fixlen*/,
			(bool)radioParam.TxConfig.crcCheck/*crc on*/, 0 /*freqhop*/, 0 /* hopeperiod*/, radioParam.TxConfig.iq /* IQ inverted*/, 0xff /*Timeout*/);
}
