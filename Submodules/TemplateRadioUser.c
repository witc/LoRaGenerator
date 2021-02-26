/*
 * TemplateRadioUser.c
 *
 *  Created on: 7. 2. 2020
 *      Author: jan.ropek
 */

#include "main.h"
#include "TaskRF.h"
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
__attribute__(( weak )) void RU_LoRaConfigAndStartRX (uint32_t freq,RadioPar DR, bool Rx, uint32_t rxTimeout)
{
	bool rxContin=false;

	if(rxTimeout==portMAX_DELAY)	rxContin=true;

	RadioSetRxConfig(freq,MODEM_LORA,DR.bw,DR.sf+5,DR.cr+1/*cr*/,0/*AFC*/,8/*preamble*/,0/*symbtimeout to lock*/,
					 (bool)DR.headerMode/*fixlen*/,10/*length*/,(bool)DR.crcCheck/*crc on*/,0,0,(bool)DR.iq,rxContin/*rx continous*/);

	if (Rx == true)   RadioRxBoosted (rxTimeout);
}



/*
 * brief: set default parametter for UP packets
 */
__attribute__(( weak )) void RU_RFSetTXUp(int8_t power, uint32_t freq, RadioPar DR)
{
	/* Radio TX configuration for "Up" way */

	RadioSetTxConfig(freq, MODEM_LORA /* modem*/,(int8_t)power /*power*/, 0/*fdev*/, DR.bw/*BW*/,DR.sf+5 /*SF*/,
					 	 	 	 1 /*coderate*/, 8 /*preamble*/, (bool)DR.headerMode /*fixlen*/, (bool)DR.crcCheck/*crc on*/,
								 0 /*freqhop*/, 0 /* hopeperiod*/, DR.iq /* IQ inverted*/, 0xff /*Timeout*/);
}
