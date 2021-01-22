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
	DrConfig_t tempRxDR, tempTxDR;
	eParentType typeOfParent;
	taskENTER_CRITICAL();

	/* init */
	spiDevice.AtomicActionEnter=vPortEnterCritical;
	spiDevice.AtomicActionExit=vPortExitCritical;
	spiDevice.pin_BUSY.port=RF_BUSY_GPIO_Port;
	spiDevice.pin_BUSY.pin=RF_BUSY_Pin;
	spiDevice.pin_RESET.port=RF_NRESET_GPIO_Port;
	spiDevice.pin_RESET.pin=RF_NRESET_Pin;
	spiDevice.pin_DIO1.port=RF_DIO_GPIO_Port;
	spiDevice.pin_DIO1.pin=RF_DIO_Pin;
	spiDevice.pin_NSS.port=RF_NSS_GPIO_Port;
	spiDevice.pin_NSS.pin=RF_NSS_Pin;
	spiDevice.pin_RF_SWITCH.port=RF_TX_ENABLE_GPIO_Port;
	spiDevice.pin_RF_SWITCH.pin=RF_TX_ENABLE_Pin;

#if AES_USE_SW_VERSION
	spiDevice.AES_ECB_Encrypt=AES_ECB_encrypt;
	spiDevice.AES_ECB_Decrypt=AES_ECB_decrypt;
#elif

#endif


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

		case RF_CMD_TX_CW:
			RadioCleanAndStandby();
			PRT_SetAtten1To(0);
			PRT_SetAtten2To(0);
			RadioSetTxContinuousWave(RadioParam.TxFreq,RadioParam.Power,0);
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

			PRT_PowerDistribution((int8_t)(RadioParam.Power),&tempPower,&atten1,&atten2);
			PRT_SetAtten1To(atten1);
			PRT_SetAtten2To(atten2);

			RadioCleanAndStandby();
			RU_RFSetTXUp(tempPower,RadioParam.TxFreq,RadioParam.TxConfig);

			taskENTER_CRITICAL();
			RadioSend(TxPacket,ReceiveData->temp);
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
                 }
		    }


			RU_LoRaConfigAndStartRX(RadioParam.RxFreq,RadioParam.RxConfig,true,portMAX_DELAY);

	        break;

        case RF_TX_RUNNING:

			/*Open window for a short time*/
			RU_LoRaConfigAndStartRX(RadioParam.RxFreq,RadioParam.RxConfig,true,portMAX_DELAY);
           	SendData.Address=ADDR_TO_CORE_TX_PACKET_DONE;
           	xQueueSend(QueueCoreHandle, &SendData, portMAX_DELAY);

            break;

	    default:
		    break;
	}

	return 0;

}



