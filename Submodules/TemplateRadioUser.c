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


static const RadioPar  rfProfileGPSCollar = {	.bw = DR_BW_125_KHZ,
												.cr = DR_CR_48,
												.crcCheck = DR_CRC_TRUE,
												.freq = 869525000,
												.headerMode = DR_HEADER_FALSE,
												.iq = DR_IQ_FALSE,
												.sf = DR_SF8
									  	  	  };


tRadioUserConfig			RadioUserConfig;
tRadioConfiguration 		spiDevice;

/**
 *
 */
__attribute__(( weak )) bool RU_SX1262Assign(void)
{
	RadioPar tempRxDR, tempTxDR;
	taskENTER_CRITICAL();

	/* init */
	spiDevice.AtomicActionEnter=vPortEnterCritical;
	spiDevice.AtomicActionExit=vPortExitCritical;
	spiDevice.pin_BUSY.port=SX1262_BUSY_GPIO_Port;
	spiDevice.pin_BUSY.pin=SX1262_BUSY_Pin;
	spiDevice.pin_RESET.port=SX1262_NRESET_GPIO_Port;
	spiDevice.pin_RESET.pin=SX1262_NRESET_Pin;
	spiDevice.pin_DIO1.port=SX1262_DIO_GPIO_Port;
	spiDevice.pin_DIO1.pin=SX1262_DIO_Pin;
	spiDevice.pin_NSS.port=SX1262_NSS_GPIO_Port;
	spiDevice.pin_NSS.pin=SX1262_NSS_Pin;
	spiDevice.pin_RF_SWITCH.port=SX1262_TX_ENABLE_GPIO_Port;
	spiDevice.pin_RF_SWITCH.pin=SX1262_TX_ENABLE_Pin;

#if (radioUSE_SPI_BY_DMA==1)
	//TODO JR
#endif

	spiDevice.target=&hspi1;
	taskEXIT_CRITICAL();

	/* Select to high*/
	LL_GPIO_SetOutputPin(spiDevice.pin_NSS.port,spiDevice.pin_NSS.pin);

	/* radio init */
	RadioInit();

	/* zkouska SPI komunikace*/
	if(RG_SX126xReadRegister(0x08AC)!=0x94 )
	{
		LogError(0x5134);
		return false;
	}

	RadioStandby();
	RadioSleep();

	RadioUserConfig.RadioParam.RxConfig = rfProfileGPSCollar;
	RadioUserConfig.RadioParam.TxConfig = rfProfileGPSCollar;
//	RadioUserConfig.RadioParam.RxConfig.bw = DR_BW_125_KHZ;
//	RadioUserConfig.RadioParam.RxConfig.cr = DR_CR_48;
//	RadioUserConfig.RadioParam.RxConfig.crcCheck = DR_CRC_TRUE;
//	RadioUserConfig.RadioParam.RxConfig.freq = 869525000;
//	RadioUserConfig.RadioParam.RxConfig.headerMode = DR_HEADER_FALSE;
//	RadioUserConfig.RadioParam.RxConfig.iq = DR_IQ_FALSE;
//	RadioUserConfig.RadioParam.RxConfig.sf = DR_SF8;

	return true;

}

/*
 * @brief : Serve IRQ from Semtech
 */
__attribute__(( weak )) uint8_t RU_IRQProcess()
{
	uint16_t irqRegs =0;
	uint8_t packetSize=0;
	tGeneralPacket RxBuffer;

	RadioStandby();
	irqRegs = SX126xGetIrqStatus();
	SX126xClearIrqStatus(IRQ_RADIO_ALL);

	switch (RadioGetStatus())
	{
		case RF_RX_RUNNING:

			if (((irqRegs & IRQ_RX_DONE) == IRQ_RX_DONE)&& ((irqRegs & IRQ_CRC_ERROR) != IRQ_CRC_ERROR)) //Data rxed
			{
				if (SX126xGetPayload(RxBuffer.DataArray, &packetSize, radioPACKET_MAX_SIZE) == 0)
				{
					SX126xGetPacketStatus(&RadioPktStatus);	//RSSI, SNR, Freq Error,..

					RU_CallbackDataRecieved(RxBuffer,packetSize,RadioPktStatus);

					if(RadioUserConfig.rxSingle == true)
					{
						RadioCleanAndStandby();
						break;
					}
				 }
			}

			RU_LoRaConfigAndStartRX(RadioParam.RxConfig.freq,RadioParam.RxConfig,true,GlobalData->payloadSize,portMAX_DELAY);

			break;

		case RF_TX_RUNNING:

			RU_CallbackTxDone();

			if(RadioUserConfig.rxSingle == true)
			{
				RadioCleanAndStandby();
			}
			else
			{
				RU_LoRaConfigAndStartRX(RadioParam.RxConfig.freq,RadioParam.RxConfig,true,GlobalData->payloadSize, portMAX_DELAY);
			}

			break;

		default:
			break;
	}

	return 0;

}

/*
 * @brief: Command recieved to RF task
 */
__attribute__(( weak ))void RU_CommandProcess(RfCommands cmd,tRfGlobalData* GlobalData, DATA_QUEUE *ReceiveData)
{
	DATA_QUEUE			SendData;
	SendData.pointer = NULL;
	uint8_t				*TxPacket;
	uint32_t			rfFreq=0;
	int8_t				tempPower;
	double				atten1,atten2;

	RadioState_t radioState = RadioGetStatus();

	switch (cmd)
	{
		case RF_CMD_DEINIT:
			RadioDeinit();
			break;

		case RF_CMD_INIT:
			RadioInit();

			break;

		case RF_CMD_STANDBY:
			RadioCleanAndStandby();
			break;

		case RF_CMD_START_RX:
			GlobalData->rxSingle = (bool)ReceiveData->temp;
			GlobalData->payloadSize = ReceiveData->temp2;
			RU_LoRaConfigAndStartRX(RadioParam.RxConfig.freq,RadioParam.RxConfig,true,GlobalData->payloadSize,portMAX_DELAY);
			break;

		case RF_CMD_STOP_RX:
			if (radioState == RF_RX_RUNNING)
			{
				RadioCleanAndStandby();
			}
			break;

		case RF_CMD_RESTART_RX:
			RadioCleanAndStandby();
			RU_LoRaConfigAndStartRX(RadioParam.RxConfig.freq,RadioParam.RxConfig,true,GlobalData->payloadSize,portMAX_DELAY);
			break;

		case RF_CMD_TX_CW:
			//RadioCleanAndStandby();
			RadioStandby();
			RadioSetTxContinuousWave(RadioParam.TxConfig.freq,RadioParam.Power,0);
			//osDelay(1);
			break;

		case RF_CMD_STOP_TX:
			if(radioState == RF_TX_RUNNING)
			{
				RadioCleanAndStandby();
			}
			break;

		case RF_CMD_SEND_UNIVERSAL_PAYLOAD_LBT:
			//TODO JR
			break;

		case RF_CMD_SEND_UNIVERSAL_PAYLOAD_NOW:

			break;

		case RF_CMD_SEND_UNIVERSAL_PAYLOAD_NOW:
			TxPacket=ReceiveData->pointer;
			//RadioCleanAndStandby();
			RadioStandby();

			PRT_PowerDistribution((int8_t)(RadioParam.Power),&tempPower,&atten1,&atten2);
			PRT_SetAtten1To(atten1);
			PRT_SetAtten2To(atten2);

			RU_RFSetTXUp(tempPower,RadioParam.TxConfig.freq,RadioParam.TxConfig);

			taskENTER_CRITICAL();
			RadioSend((uint8_t *)EE_RF_DATA_PACKET,RC_GetSizeOfSavedPacket());
		//	RadioSend((uin8_t*)"Pokus zda to bude vubec fungovat",RC_GetSizeOfSavedPacket());
			taskEXIT_CRITICAL();

			break;

		default:
			break;
	}

}


/*
 * brief: Set RX config
 */
__attribute__(( weak )) void RU_LoRaConfigAndStartRX (uint32_t freq,RadioPar DR, bool Rx,uint8_t payloadSize, uint32_t rxTimeout)
{
	bool rxContin=false;

	if(rxTimeout==portMAX_DELAY)	rxContin=true;

	RadioSetRxConfig(freq,MODEM_LORA,DR.bw,DR.sf+5,DR.cr+1/*cr*/,0/*AFC*/,8/*preamble*/,0/*symbtimeout to lock*/,
					 (DR.headerMode==true) ? false:true/*fixlen*/,payloadSize/*length*/,(bool)DR.crcCheck/*crc on*/,0,0,(bool)DR.iq,rxContin/*rx continous*/);

	if (Rx == true)   RadioRxBoosted (rxTimeout);
}



/*
 * brief: set default parametter for UP packets
 */
__attribute__(( weak )) void RU_RFSetTXUp(int8_t power, uint32_t freq, RadioPar DR)
{
	/* Radio TX configuration for "Up" way */

	RadioSetTxConfig(freq, MODEM_LORA /* modem*/,(int8_t)power /*power*/, 0/*fdev*/, DR.bw/*BW*/,DR.sf+5 /*SF*/,
					 	 	 	 1 /*coderate*/, 8 /*preamble*/,  (DR.headerMode==true) ? false:true /*fixlen*/, (bool)DR.crcCheck/*crc on*/,
								 0 /*freqhop*/, 0 /* hopeperiod*/, DR.iq /* IQ inverted*/, 0xff /*Timeout*/);
}



/**
 *
 * @param data
 * @param size
 * @param signalInfo
 */
__attribute__(( weak )) void RU_CallbackDataRecieved(uint8_t *data, uint8_t size, PacketStatus_t signalInfo)
{

}

/**
 *
 */
__attribute__(( weak )) RU_CallbackTxDone()
{

}
