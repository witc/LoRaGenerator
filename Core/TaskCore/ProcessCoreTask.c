/*
 * ProcessCoreTask.c
 *
 *  Created on: 17. 3. 2020
 *      Author: jan.ropek
 */

#include "ProcessCoreTask.h"
#include "TemplateRadioUser.h"
#include "SignalProcessing.h"
#include "AuxRfProcess.h"
#include "RadioCommands.h"
#include "UartProcess.h"

extern UART_HandleTypeDef huart1;
extern osMessageQId QueueCoreHandle;
extern osMessageQId QueueRFHandle;




/**
 *
 */
void PCT_SendRfPacket()
{
	DATA_QUEUE SendData;
	SendData.pointer = NULL;

	if(RC_GetSizeOfSavedPacket()!=0)
	{
		SendData.Address=ADDR_TO_RF_CMD;
		SendData.Data=RF_CMD_SEND_UNIVERSAL_PAYLOAD_NOW;
		xQueueSend(QueueRFHandle,&SendData,portMAX_DELAY);

		//osTimerStart(TimerRfTxTimeoutHandle,) TODO zvazit jak nastavit timeout  - aby byl spravne platny
	}
	else
	{
		//todo send error
	}
}


/**
 *
 * @param rxBuffer
 * @return
 */
uint8_t PCT_DecodeUartRxMsg(uint8_t *rxBuffer)
{
	if(rxBuffer[0] < 0x81)
	{
		PCT_ProcessSetCommands(rxBuffer);
	}

}

/**
 *
 * @param rxBuffer
 */
void PCT_ProcessSetCommands(uint8_t *rxBuffer)
{
	DATA_QUEUE SendData;
	SendData.pointer = NULL;
	bool retTemp=false;
	eUartMsgSetCmds cmd = rxBuffer[0];
	eActionFlags actionFlags = rxBuffer[1];

	if((actionFlags == ACTION_FLAG_SET) || (actionFlags == ACTION_FLAG_SET_GET))
	{
		switch(cmd)
		{
			case UART_MSG_TX_FREQ:
				retTemp = RC_RadioSetTxFreq((rxBuffer[2]<<24)|(rxBuffer[3]<<16)|(rxBuffer[4]<<8)|(rxBuffer[5]));
				break;

			case UART_MSG_RX_FREQ:
				retTemp = RC_RadioSetRxFreq((rxBuffer[2]<<24)|(rxBuffer[3]<<16)|(rxBuffer[4]<<8)|(rxBuffer[5]));
				break;

			case UART_MSG_TX_POWER:
				retTemp = RC_RadioSetTxPower((int8_t)rxBuffer[2]);
				break;

			case UART_MSG_TX_SF:
				retTemp = RC_RadioSetTxSf((uint8_t)rxBuffer[2]);
				break;

			case UART_MSG_RX_SF:
				retTemp = RC_RadioSetRxSf((uint8_t)rxBuffer[2]);
				break;

			case UART_MSG_TX_BW:
				retTemp = RC_RadioSetTxBw((rxBuffer[2]<<24)|(rxBuffer[3]<<16)|(rxBuffer[4]<<8)|(rxBuffer[4]));
				break;

			case UART_MSG_RX_BW:
				retTemp = RC_RadioSetRxBw((rxBuffer[1]<<24)|(rxBuffer[2]<<16)|(rxBuffer[3]<<8)|(rxBuffer[4]));
				break;

			case UART_MSG_TX_IQ:

				break;

			case UART_MSG_RX_IQ:

					break;

			case UART_MSG_TX_CR:

				break;

			case UART_MSG_RX_CR:

				break;

			case UART_MSG_SET_STANDBY:
				retTemp = RC_RadioSetStandby();
				break;

			case UART_MSG_SET_TX_CW:
				retTemp = RC_RadioSetTxCW();
				break;

			case UART_MSG_PREP_PACKET:
				retTemp = RC_SavePacket(rxBuffer);
				break;

			case UART_MSG_SEND_PACKET:
				PCT_SendRfPacket();
				break;

			default:
				break;
		}

	}

	if(retTemp == true)	/* odesilame OK status */
	{
		if(actionFlags > ACTION_FLAG_SET)
		{
			/*odesilame info o prijatem parametru */
			PCT_SendMyParam(rxBuffer);

		}
	}

}

/**
 *
 * @param rxBuffer
 */
void PCT_SendMyParam(uint8_t *rxBuffer)
{
	DATA_QUEUE SendData;
	SendData.pointer = NULL;
	uint8_t retTemp=0xff;
	uint32_t tempData;

	eUartMsgSetCmds cmd = rxBuffer[0];
	eActionFlags actionFlags = rxBuffer[1];

	switch(cmd)
	{
		case UART_MSG_TX_FREQ:
			tempData =(uint32_t) RC_RadioGetTxFreq();			//retTemp = RC_RadioSetTxFreq((rxBuffer[1]<<24)|(rxBuffer[2]<<16)|(rxBuffer[3]<<8)|(rxBuffer[4]));
			UP_UartSendData(UART_MSG_TX_FREQ,(uint8_t*)&tempData,4);
			break;

		case UART_MSG_RX_FREQ:
			tempData =(uint32_t) RC_RadioGetRxFreq();			//retTemp = RC_RadioSetTxFreq((rxBuffer[1]<<24)|(rxBuffer[2]<<16)|(rxBuffer[3]<<8)|(rxBuffer[4]));
			UP_UartSendData(UART_MSG_RX_FREQ,(uint8_t*)&tempData,4);
			break;

		case UART_MSG_TX_POWER:
			tempData =(uint32_t) RC_RadioGetTxPower();			//retTemp = RC_RadioSetTxFreq((rxBuffer[1]<<24)|(rxBuffer[2]<<16)|(rxBuffer[3]<<8)|(rxBuffer[4]));
			UP_UartSendData(UART_MSG_TX_POWER,(uint8_t*)&tempData,1);
			break;

		case UART_MSG_TX_SF:
			tempData =(uint32_t) RC_RadioGetTxSf();			//retTemp = RC_RadioSetTxFreq((rxBuffer[1]<<24)|(rxBuffer[2]<<16)|(rxBuffer[3]<<8)|(rxBuffer[4]));
			UP_UartSendData(UART_MSG_TX_SF,(uint8_t*)&tempData,1);
			break;

		case UART_MSG_RX_SF:
			tempData =(uint32_t) RC_RadioGetRxSf();			//retTemp = RC_RadioSetTxFreq((rxBuffer[1]<<24)|(rxBuffer[2]<<16)|(rxBuffer[3]<<8)|(rxBuffer[4]));
			UP_UartSendData(UART_MSG_RX_SF,(uint8_t*)&tempData,1);
			break;

		case UART_MSG_TX_BW:
			tempData =(uint32_t) RC_RadioGetTxBw();			//retTemp = RC_RadioSetTxFreq((rxBuffer[1]<<24)|(rxBuffer[2]<<16)|(rxBuffer[3]<<8)|(rxBuffer[4]));
			UP_UartSendData(UART_MSG_TX_BW,(uint8_t*)&tempData,2);
			break;

		case UART_MSG_RX_BW:
			tempData =(uint32_t) RC_RadioGetRxBw();			//retTemp = RC_RadioSetTxFreq((rxBuffer[1]<<24)|(rxBuffer[2]<<16)|(rxBuffer[3]<<8)|(rxBuffer[4]));
			UP_UartSendData(UART_MSG_RX_BW,(uint8_t*)&tempData,2);
			break;

		case UART_MSG_TX_IQ:
			tempData =(uint32_t) RC_RadioGetTxIq();			//retTemp = RC_RadioSetTxFreq((rxBuffer[1]<<24)|(rxBuffer[2]<<16)|(rxBuffer[3]<<8)|(rxBuffer[4]));
			UP_UartSendData(UART_MSG_TX_IQ,(uint8_t*)&tempData,1);
			break;

		case UART_MSG_RX_IQ:
			tempData =(uint32_t) RC_RadioGetRxIq();			//retTemp = RC_RadioSetTxFreq((rxBuffer[1]<<24)|(rxBuffer[2]<<16)|(rxBuffer[3]<<8)|(rxBuffer[4]));
			UP_UartSendData(UART_MSG_RX_IQ,(uint8_t*)&tempData,1);
			break;

		case UART_MSG_TX_CR:
			tempData =(uint32_t) RC_RadioGetTxCr();			//retTemp = RC_RadioSetTxFreq((rxBuffer[1]<<24)|(rxBuffer[2]<<16)|(rxBuffer[3]<<8)|(rxBuffer[4]));
			UP_UartSendData(UART_MSG_TX_CR,(uint8_t*)&tempData,1);
			break;

		case UART_MSG_RX_CR:
			tempData =(uint32_t) RC_RadioGetRxCr();			//retTemp = RC_RadioSetTxFreq((rxBuffer[1]<<24)|(rxBuffer[2]<<16)|(rxBuffer[3]<<8)|(rxBuffer[4]));
			UP_UartSendData(UART_MSG_RX_CR,(uint8_t*)&tempData,1);
			break;

//		case UART_MSG_PREP_PACKET:
//			tempData = RC_RadioGetTxFreq();			//retTemp = RC_RadioSetTxFreq((rxBuffer[1]<<24)|(rxBuffer[2]<<16)|(rxBuffer[3]<<8)|(rxBuffer[4]));
//			UP_UartSendData(UART_MSG_PREP_PACKET,&tempData,4);
//			break;

		default:
			break;
	}

}
