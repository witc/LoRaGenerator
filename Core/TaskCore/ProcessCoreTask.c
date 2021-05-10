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
#include "LoadConfig.h"

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

	//if(RC_GetSizeOfSavedPacket()!=0)
	{
		SendData.Address=ADDR_TO_RF_CMD;
		SendData.Data=RF_CMD_SEND_UNIVERSAL_PAYLOAD_NOW;
		xQueueSend(QueueRFHandle,&SendData,portMAX_DELAY);

		//osTimerStart(TimerRfTxTimeoutHandle,) TODO zvazit jak nastavit timeout  - aby byl spravne platny
	}
//	else
//	{
//		//todo send error
//	}
}



/*
 *
 */
void PCT_SetRadioRX(bool single, uint8_t payloadSize)
{
	DATA_QUEUE SendData;
	SendData.pointer = NULL;

	SendData.Address=ADDR_TO_RF_CMD;
	SendData.Data=RF_CMD_START_RX;
	SendData.temp = single;
	SendData.temp2 = payloadSize;
	xQueueSend(QueueRFHandle,&SendData,portMAX_DELAY);

	RC_RadioSetRXPayloadSize(payloadSize);
}


/**
 *
 * @param rxBuffer
 * @return
 */
uint8_t PCT_DecodeUartRxMsg(uint8_t *rxBuffer)
{
	if(rxBuffer[0] <= UART_MSG_PREP_PACKET)
	{
		PCT_ProcessSetCommands(rxBuffer);
	}
	else if(rxBuffer[0] <= UART_MSG_GET_PREP_PACKET)
	{
		PCT_SendMyParam(rxBuffer[0]-40);
	}
	else
	{
		PCT_ProcessSystemCommands();
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
				retTemp = RC_RadioSetTxBw((rxBuffer[2]<<24)|(rxBuffer[3]<<16)|(rxBuffer[4]<<8)|(rxBuffer[5]));
				break;

			case UART_MSG_RX_BW:
				retTemp = RC_RadioSetRxBw((rxBuffer[2]<<24)|(rxBuffer[3]<<16)|(rxBuffer[4]<<8)|(rxBuffer[5]));
				break;

			case UART_MSG_TX_IQ:
				retTemp = RC_RadioSetTxIq((uint8_t)rxBuffer[2]);
				break;

			case UART_MSG_RX_IQ:
				retTemp = RC_RadioSetRxIq((uint8_t)rxBuffer[2]);
					break;

			case UART_MSG_TX_CR:
				retTemp = RC_RadioSetTxCr((uint8_t)rxBuffer[2]);
				break;

			case UART_MSG_RX_CR:
				retTemp = RC_RadioSetRxCr((uint8_t)rxBuffer[2]);
				break;

			case UART_MSG_HEADER_MODE_TX:
				retTemp = RC_RadioSetTXHeaderMode((uint8_t)rxBuffer[2]);
				break;

			case UART_MSG_HEADER_MODE_RX:
				retTemp = RC_RadioSetRXHeaderMode((uint8_t)rxBuffer[2]);
				break;

			case UART_MSG_TX_CRC:
				retTemp = RC_RadioSetTXCRC((uint8_t)rxBuffer[2]);
				break;

			case UART_MSG_RX_CRC:
				retTemp = RC_RadioSetRXCRC((uint8_t)rxBuffer[2]);
				break;


			case UART_MSG_PREP_PACKET:
				retTemp = RC_SavePacket(rxBuffer);
				break;

			default:
				break;
		}

	}
	else if(actionFlags == ACTION_FLAG_GET)
	{
		retTemp = true;
	}


	if(retTemp == true)	/* odesilame OK status */
	{
		if(actionFlags > ACTION_FLAG_SET)
		{
			/*odesilame info o prijatem parametru */
			PCT_SendMyParam(rxBuffer[0]);
		}
	}
}


void PCT_ProcessSystemCommands(uint8_t *rxBuffer)
{
	DATA_QUEUE SendData;
	SendData.pointer = NULL;
	uint8_t retTemp=false;
	uint8_t tempBuffer[100];

	eUartMsgSetCmds cmd = rxBuffer[0];

	switch(cmd)
	{
		case UART_MSG_START_RX:
			PCT_SetRadioRX(rxBuffer[1] /* single */, rxBuffer[2] /* payload Size*/);
			osThreadYield();
			break;

		case UART_MSG_SET_STANDBY:
			retTemp = RC_RadioSetStandby();
			osThreadYield();
			break;

		case UART_MSG_SET_TX_CW:
			retTemp = RC_RadioSetTxCW();
			osThreadYield();
			break;

		case UART_MSG_SEND_PACKET:
			PCT_SendRfPacket();
			osThreadYield();
			break;

		case UART_MSG_WHAT_IS_YOUR_NAME:
			retTemp = LC_GetMyName(tempBuffer);
			UP_UartSendData(UART_MSG_WHAT_IS_YOUR_NAME,tempBuffer,retTemp);
			break;

		case UART_MSG_WHO_ARE_YOU:
			retTemp = LC_GetSystemIfo(tempBuffer);
			UP_UartSendData(UART_MSG_WHO_ARE_YOU,tempBuffer,retTemp);
			break;

		default:
			break;
	}


	RC_RadioSetState(cmd);

}

/**
 *
 * @param rxBuffer
 */
void PCT_SendMyParam(eUartMsgSetCmds cmd)
{
	DATA_QUEUE SendData;
	SendData.pointer = NULL;
	uint32_t tempData;

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
			UP_UartSendData(UART_MSG_TX_BW,(uint8_t*)&tempData,4);
			break;

		case UART_MSG_RX_BW:
			tempData =(uint32_t) RC_RadioGetRxBw();			//retTemp = RC_RadioSetTxFreq((rxBuffer[1]<<24)|(rxBuffer[2]<<16)|(rxBuffer[3]<<8)|(rxBuffer[4]));
			UP_UartSendData(UART_MSG_RX_BW,(uint8_t*)&tempData,4);
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

		case UART_MSG_HEADER_MODE_TX:
			tempData =(uint32_t) RC_RadioGetTXHeaderMode();			//retTemp = RC_RadioSetTxFreq((rxBuffer[1]<<24)|(rxBuffer[2]<<16)|(rxBuffer[3]<<8)|(rxBuffer[4]));
			UP_UartSendData(UART_MSG_HEADER_MODE_TX,(uint8_t*)&tempData,1);
			break;

		case UART_MSG_HEADER_MODE_RX:
			tempData =(uint32_t) RC_RadioGetRXHeaderMode();			//retTemp = RC_RadioSetTxFreq((rxBuffer[1]<<24)|(rxBuffer[2]<<16)|(rxBuffer[3]<<8)|(rxBuffer[4]));
			UP_UartSendData(UART_MSG_HEADER_MODE_RX,(uint8_t*)&tempData,1);
			break;

		case UART_MSG_TX_CRC:
			tempData =(uint32_t) RC_RadioGetTXCRC();			//retTemp = RC_RadioSetTxFreq((rxBuffer[1]<<24)|(rxBuffer[2]<<16)|(rxBuffer[3]<<8)|(rxBuffer[4]));
			UP_UartSendData(UART_MSG_TX_CRC,(uint8_t*)&tempData,1);
			break;

		case UART_MSG_RX_CRC:
			tempData =(uint32_t) RC_RadioGetRXCRC();			//retTemp = RC_RadioSetTxFreq((rxBuffer[1]<<24)|(rxBuffer[2]<<16)|(rxBuffer[3]<<8)|(rxBuffer[4]));
			UP_UartSendData(UART_MSG_RX_CRC,(uint8_t*)&tempData,1);
			break;

		case UART_MSG_SET_STANDBY:
		case UART_MSG_SET_TX_CW:
		case UART_MSG_SEND_PACKET:
		case UART_MSG_START_RX:
			tempData =(uint32_t) RC_RadioGetRadioStatus();			//retTemp = RC_RadioSetTxFreq((rxBuffer[1]<<24)|(rxBuffer[2]<<16)|(rxBuffer[3]<<8)|(rxBuffer[4]));
			//UP_UartSendData(UART_MSG_RX_CR,(uint8_t*)&tempData,1);
			break;

//		case UART_MSG_PREP_PACKET:
//			tempData = RC_RadioGetTxFreq();			//retTemp = RC_RadioSetTxFreq((rxBuffer[1]<<24)|(rxBuffer[2]<<16)|(rxBuffer[3]<<8)|(rxBuffer[4]));
//			UP_UartSendData(UART_MSG_PREP_PACKET,&tempData,4);
//			break;

		default:
			break;
	}

}
