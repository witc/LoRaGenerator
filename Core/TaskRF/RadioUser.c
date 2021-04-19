/*
 * RadioUser.c
 *
 *  Created on: 12. 3. 2020
 *      Author: jan.ropek
 */

#include "main.h"
#include "TaskRF.h"
#include "TaskCore.h"
#include "TemplateRadioUser.h"
#include "AES_SW.h"
#include "ProcessRFTask.h"
#include "LoRa_Codec.h"
#include "RadioUser.h"
#include "RadioCommands.h"

/*!
 *
 */

extern osMessageQId QueueCoreHandle;
extern osTimerId TimerRepeateTXHandle;
extern  SPI_HandleTypeDef			hspi1;

tRadioUserConfig					RadioUserConfig;
Radio_Configuration_Struct 			spiDevice;


/**
 *
 */
bool RU_SX1262Assign(void)
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

#if (RF_USE_DMA==1)

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

	return true;
}


/**
 *
 * @param cmd
 * @param GlobalData
 * @param ReceiveData
 */
void RU_CommandProcess(RfCommands cmd,tRfGlobalData* GlobalData, DATA_QUEUE *ReceiveData)
{
	DATA_QUEUE			SendData;
	SendData.pointer = NULL;
	uint8_t				*TxPacket;
	uint32_t			rfFreq=0;
	int8_t				tempPower;
	double				atten1,atten2;

	switch (cmd)
	{
		case RF_CMD_INIT_OFF:
			RadioDeinit();
			break;

		case RF_CMD_INIT_ON:
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

		case RF_CMD_TX_CW:
			//RadioCleanAndStandby();
			RadioStandby();
			//PRT_SetAtten1To(0);
			//PRT_SetAtten2To(0);
			RadioSetTxContinuousWave(RadioParam.TxConfig.freq,RadioParam.Power,0);
			//osDelay(1);
			break;

		case RF_CMD_STOP_TX_AND_DISCARD:
			if (GlobalData->RF_State == RF_TX_RUNNING)
			{
				RadioCleanAndStandby();
				SendData.Address=ADDR_TO_CORE_TX_PACKET_DONE;
				xQueueSend(QueueCoreHandle, &SendData, portMAX_DELAY);
			}

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

/**
 *
 * @param GlobalData
 * @return
 */
uint8_t RU_IRQProcess(tRfGlobalData* GlobalData)
{
	DATA_QUEUE SendData;
	SendData.pointer=NULL;
	PacketStatus_t RadioPktStatus;
	uint16_t irqRegs =0;
	uint8_t size=0;

	GeneralPacketsUpOrDown_t RxBuffer;
	GeneralPacketsUpOrDown_t *RxBufferToCore;

	RadioStandby();
	irqRegs = SX126xGetIrqStatus();
	SX126xClearIrqStatus(IRQ_RADIO_ALL);

	switch (GlobalData->RF_State)
	{
	    case RF_RX_RUNNING:

            if (((irqRegs & IRQ_RX_DONE) == IRQ_RX_DONE)&& ((irqRegs & IRQ_CRC_ERROR) != IRQ_CRC_ERROR)) //Data recieved
            {
                if (SX126xGetPayload(RxBuffer.DataArray, &size, PACKET_MAX_SIZE)== 0)
                {
                    SX126xGetPacketStatus(&RadioPktStatus);	//RSSI, SNR, Freq Error,..

                   	RxBufferToCore = pvPortMalloc(sizeof(GeneralPacketsUpOrDown_t));
					if (RxBufferToCore == NULL)
					{
						osDelay(500);
						RxBufferToCore = pvPortMalloc(sizeof(GeneralPacketsUpOrDown_t));
						if (RxBufferToCore == NULL)  LogError(51334);
					}
					memset(RxBufferToCore, 0, sizeof(GeneralPacketsUpOrDown_t));
					memcpy(RxBufferToCore,&RxBuffer,sizeof(GeneralPacketsUpOrDown_t));

					SendData.Address = ADDR_TO_CORE_RF_DATA_RECEIVED;
					SendData.RFU=RadioPktStatus.Params.LoRa.SignalRssiPkt;
					SendData.temp=size;
					SendData.pointer = RxBufferToCore;
					xQueueSend(QueueCoreHandle, &SendData, portMAX_DELAY);

					if(GlobalData->rxSingle == true)
					{
						RadioCleanAndStandby();
						break;
					}
                 }
		    }

			RU_LoRaConfigAndStartRX(RadioParam.RxConfig.freq,RadioParam.RxConfig,true,GlobalData->payloadSize,portMAX_DELAY);

	        break;

        case RF_TX_RUNNING:

        	if(GlobalData->rxSingle == true)
			{
				RadioCleanAndStandby();
			}
        	else
        	{
        		RU_LoRaConfigAndStartRX(RadioParam.RxConfig.freq,RadioParam.RxConfig,true,GlobalData->payloadSize, portMAX_DELAY);
        	}

           	SendData.Address=ADDR_TO_CORE_TX_PACKET_DONE;
           	xQueueSend(QueueCoreHandle, &SendData, portMAX_DELAY);



            break;

	    default:
		    break;
	}

	return 0;

}



