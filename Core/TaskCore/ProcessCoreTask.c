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

extern UART_HandleTypeDef huart1;
extern osMessageQId QueueCoreHandle;
extern osMessageQId QueueRFHandle;

uint8_t SyncUartMsg[1] = {0xaa};

/* CRC8 from CRSF crossfire */
static unsigned char crc8tab[256] = {
    0x00, 0xD5, 0x7F, 0xAA, 0xFE, 0x2B, 0x81, 0x54, 0x29, 0xFC, 0x56, 0x83, 0xD7, 0x02, 0xA8, 0x7D,
    0x52, 0x87, 0x2D, 0xF8, 0xAC, 0x79, 0xD3, 0x06, 0x7B, 0xAE, 0x04, 0xD1, 0x85, 0x50, 0xFA, 0x2F,
    0xA4, 0x71, 0xDB, 0x0E, 0x5A, 0x8F, 0x25, 0xF0, 0x8D, 0x58, 0xF2, 0x27, 0x73, 0xA6, 0x0C, 0xD9,
    0xF6, 0x23, 0x89, 0x5C, 0x08, 0xDD, 0x77, 0xA2, 0xDF, 0x0A, 0xA0, 0x75, 0x21, 0xF4, 0x5E, 0x8B,
    0x9D, 0x48, 0xE2, 0x37, 0x63, 0xB6, 0x1C, 0xC9, 0xB4, 0x61, 0xCB, 0x1E, 0x4A, 0x9F, 0x35, 0xE0,
    0xCF, 0x1A, 0xB0, 0x65, 0x31, 0xE4, 0x4E, 0x9B, 0xE6, 0x33, 0x99, 0x4C, 0x18, 0xCD, 0x67, 0xB2,
    0x39, 0xEC, 0x46, 0x93, 0xC7, 0x12, 0xB8, 0x6D, 0x10, 0xC5, 0x6F, 0xBA, 0xEE, 0x3B, 0x91, 0x44,
    0x6B, 0xBE, 0x14, 0xC1, 0x95, 0x40, 0xEA, 0x3F, 0x42, 0x97, 0x3D, 0xE8, 0xBC, 0x69, 0xC3, 0x16,
    0xEF, 0x3A, 0x90, 0x45, 0x11, 0xC4, 0x6E, 0xBB, 0xC6, 0x13, 0xB9, 0x6C, 0x38, 0xED, 0x47, 0x92,
    0xBD, 0x68, 0xC2, 0x17, 0x43, 0x96, 0x3C, 0xE9, 0x94, 0x41, 0xEB, 0x3E, 0x6A, 0xBF, 0x15, 0xC0,
    0x4B, 0x9E, 0x34, 0xE1, 0xB5, 0x60, 0xCA, 0x1F, 0x62, 0xB7, 0x1D, 0xC8, 0x9C, 0x49, 0xE3, 0x36,
    0x19, 0xCC, 0x66, 0xB3, 0xE7, 0x32, 0x98, 0x4D, 0x30, 0xE5, 0x4F, 0x9A, 0xCE, 0x1B, 0xB1, 0x64,
    0x72, 0xA7, 0x0D, 0xD8, 0x8C, 0x59, 0xF3, 0x26, 0x5B, 0x8E, 0x24, 0xF1, 0xA5, 0x70, 0xDA, 0x0F,
    0x20, 0xF5, 0x5F, 0x8A, 0xDE, 0x0B, 0xA1, 0x74, 0x09, 0xDC, 0x76, 0xA3, 0xF7, 0x22, 0x88, 0x5D,
    0xD6, 0x03, 0xA9, 0x7C, 0x28, 0xFD, 0x57, 0x82, 0xFF, 0x2A, 0x80, 0x55, 0x01, 0xD4, 0x7E, 0xAB,
    0x84, 0x51, 0xFB, 0x2E, 0x7A, 0xAF, 0x05, 0xD0, 0xAD, 0x78, 0xD2, 0x07, 0x53, 0x86, 0x2C, 0xF9};


volatile uint32_t		dmaOverRun=0;



/**
 *
 * @param buff
 * @param maxRange
 * @param sizeOfDetectedMsg
 * @return
 */
eUARTBufferMasg PCT_FindAnyMsg()
{
	DATA_QUEUE SendData;
	SendData.pointer=NULL;

	static uint8_t 	startToFind=0;
	static uint8_t  lastRxed=0;
	uint8_t			actualArrivalSize=0;
	uint8_t 		startHeader;
	uint8_t			startPayload;
	uint16_t 		payloadSizeFromHeader;
	uint8_t 		workingBuffer[UART_CIRCLE_MAX_BUFFER_SIZE];
	eUartMsgs		UartPayload;
	uint8_t			*TxPacket;
	uint8_t 		tempCntDma;

	static uint8_t	remainsToZero=UART_CIRCLE_MAX_BUFFER_SIZE;
	static uint8_t  sumArrivalSize=0;
	uint8_t			tempSizeOfRxMsg;

	static uint8_t	sizeToCheck;
	LL_GPIO_TogglePin(LED_GREEN_GPIO_Port,LED_GREEN_Pin);
	memset(workingBuffer,0,UART_CIRCLE_MAX_BUFFER_SIZE);

	/* read DMA status */
	//dmaOverRun = LL_DMA_IsActiveFlag_TC3(DMA1);
	//if (dmaOverRun) 	LL_DMA_ClearFlag_TC3(DMA1);
	tempCntDma = LL_DMA_GetDataLength(DMA1,LL_DMA_CHANNEL_3);

//	if(tempCntDma<3)
//	{
//		osDelay(1);
//	}
	dmaOverRun=0;
	if(remainsToZero<tempCntDma)	dmaOverRun=1;


	/* od minule kontroly prislo BYTU*/
	actualArrivalSize=(UART_CIRCLE_MAX_BUFFER_SIZE-tempCntDma);
	if(dmaOverRun)
	{
		actualArrivalSize+=remainsToZero; /* dma preteklo, tudiz prictu k aktualnimu poctu zbytek z minula do 0 */
	}
	else
	{
		actualArrivalSize-=lastRxed;	/* nic nepreteklo */
	}

	/* od posledni kontroly prisly data o delce actualArrivalSize*/
	sizeToCheck+=actualArrivalSize;
	lastRxed=(UART_CIRCLE_MAX_BUFFER_SIZE-tempCntDma);	//prejmenovat na pristevynechej

	sumArrivalSize+=actualArrivalSize;

	remainsToZero = tempCntDma;

	/* pokud ted nic neprislo*/
	if(actualArrivalSize == 0)	return eUART_MSG_EMPTY;	//neprislo nic

	/* data budeme hledat od startToFind az po sumArrivalSize */
/********************************************************************************/

	if(sumArrivalSize<MINIMAL_SIZE_USART_RX_MSG)	return eUART_MSG_TOO_SHORT;

	/* seradime si buffer vzdy od nuly */
	memcpy(&workingBuffer[0],&GlUartRxBugger[startToFind],UART_CIRCLE_MAX_BUFFER_SIZE-startToFind);
	if(startToFind!=0)
	{
		//if(dmaOverRun)
		{
			memcpy(&workingBuffer[UART_CIRCLE_MAX_BUFFER_SIZE-startToFind],&GlUartRxBugger[0],startToFind);
		}
	}

	/* hledame validni zpravu*/

	uint8_t	workinkgStart=0;
	do
	{
		/* 1) hledani sync wordu */
		if(PCT_FindSyncWord(&workingBuffer[workinkgStart],sumArrivalSize,&startHeader)==true)
		{
			startHeader+=workinkgStart;
			/* nasleduje mozny header zacinajici indexem startHeader */ 		//TODO kontorla crc header
			payloadSizeFromHeader = workingBuffer[startHeader];
			if((payloadSizeFromHeader>MAX_SIZE_FOR_PAYLOAD)||(payloadSizeFromHeader==0))
			{
				/* mame bulshitni zacatek takze to zahod */
				workinkgStart++;
				sumArrivalSize--;
			}
			else
			{
				/* payload */
				startPayload=startHeader+UART_BUFF_HEADER_SIZE;
				if((startPayload+payloadSizeFromHeader+UART_BUFF_CRC_SIZE) > (sumArrivalSize+workinkgStart))
				{
					/* zatim vse ok jen je zprava kratka */
					startToFind+=workinkgStart;
					/* pokud preteklo velikost bufferu  - protacime buffer zpet na zacatek*/
					startToFind%=UART_CIRCLE_MAX_BUFFER_SIZE;
					return eUART_MSG_TOO_SHORT;
				}

				uint8_t rxCrc=workingBuffer[startPayload+payloadSizeFromHeader];

				if(PCT_CalcCRC(&workingBuffer[startHeader-sizeof(SyncUartMsg)],sizeof(SyncUartMsg)+UART_BUFF_HEADER_SIZE+payloadSizeFromHeader)!=rxCrc)
				{
					/* posuneme start hledani na n+1 */
					workinkgStart+=(startHeader-sizeof(SyncUartMsg)+1);	// TODO muze byt zaporny?
					sumArrivalSize-=(startHeader-sizeof(SyncUartMsg)+1);	// TODO muze byt zaporny?
				}
				else
				{
					/* payload je pritomen */
					TxPacket=pvPortMalloc(sizeof(eUartMsgs));
					if(TxPacket==NULL)
					{
						osDelay(10);
						TxPacket=pvPortMalloc(sizeof(eUartMsgs));
						if(TxPacket==NULL)			LogError(635121);
					}

					memcpy(TxPacket,&workingBuffer[startPayload],payloadSizeFromHeader);
					//memcpy(UartPayload.payload,&workingBuffer[startPayload],payloadSizeFromHeader);
					break; // paket nalezen
				}

			}

		}
		else
		{
			/* v danem rozsahu nenalezeno sync word */
			startToFind+=workinkgStart+(sumArrivalSize-sizeof(SyncUartMsg));
			startToFind%=UART_CIRCLE_MAX_BUFFER_SIZE;

			sumArrivalSize=sizeof(SyncUartMsg);
			return eUART_MSG_TOO_SHORT;
		}
	}
	while(1);

	HAL_UART_Transmit(&huart1,&workingBuffer[startHeader-sizeof(SyncUartMsg)],sizeof(SyncUartMsg)+UART_BUFF_HEADER_SIZE+payloadSizeFromHeader+UART_BUFF_CRC_SIZE,10000);

	//LL_GPIO_TogglePin(LED_GREEN_GPIO_Port,LED_GREEN_Pin);
	tempSizeOfRxMsg = (sizeof(SyncUartMsg)+UART_BUFF_HEADER_SIZE+payloadSizeFromHeader/*payload*/+UART_BUFF_CRC_SIZE/*crc*/);
	/* crc ok posun start o prijatou zpravu*/
	startToFind+=workinkgStart+tempSizeOfRxMsg;
	/* pokud preteklo velikost bufferu  - protacime buffer zpet na zacatek*/
	startToFind%=UART_CIRCLE_MAX_BUFFER_SIZE;

	/*payload jsme nasli s velikosti tempSizeOfRxMsg - tu ted ponizime v hledacim prostoru*/
	sumArrivalSize-=tempSizeOfRxMsg;

	SendData.Address=ADDR_TO_CORE_UART_RX_NEW_PACKET;
	SendData.pointer = TxPacket;
	xQueueSend(QueueCoreHandle,&SendData,portMAX_DELAY);

}


/**
 *
 * @param data
 * @param sizeToSearch
 * @param headerStarts
 * @return
 */
bool PCT_FindSyncWord(uint8_t *data, uint8_t sizeToSearch, uint8_t *headerStarts)
{
	uint8_t cnt=0;

	if(sizeof(SyncUartMsg)>sizeToSearch)	return false;

	do
	{
		if(memcmp(&data[cnt],SyncUartMsg,sizeof(SyncUartMsg))==0) break;

		if((sizeof(SyncUartMsg)+cnt)<sizeToSearch)	cnt++;
		else
		{
			/*sync slovo neni pritomno */
			*headerStarts=0;
			return false;
		}
	}
	while(1);

	*headerStarts=cnt+sizeof(SyncUartMsg);
	return true;
}


/**
 *
 * @param rxBuffer
 */
void PCT_DecodeUartRxMsg(uint8_t *rxBuffer)
{
	//PCT_RadioSetFunc[rxBuffer[0]]();
	//return;

	switch((eUartMsgCmds)rxBuffer[0])
	{
		case UART_MSG_SET_TX_FREQ:

			RC_RadioSetTxFreq((rxBuffer[1]<<24)|(rxBuffer[2]<<16)|(rxBuffer[3]<<8)|(rxBuffer[4]));

			break;

		case UART_MSG_SET_RX_FREQ:

			break;

		case UART_MSG_SET_TX_POWER:
			RC_RadioSetTxPower((int8_t)rxBuffer[1]);
			break;

		case UART_MSG_SET_TX_SF:

			break;

		case UART_MSG_SET_TX_BW:

			break;

		case UART_MSG_SET_TX_IQ:

			break;

		case UART_MSG_SET_TX_CR:

			break;

		case UART_MSG_SET_STANDBY:
			RC_RadioSetStandby();
			break;

		case UART_MSG_SET_TX_CW:
			RC_RadioSetTxCW();
			break;

		case UART_MSG_PREP_PACKET:
			RC_SavePacket(rxBuffer);
			break;

		default:
			break;
	}
}

/**
 *
 * @param data			29
133
196
89
 * @param length
 * @return
 */
uint8_t PCT_CalcCRC(uint8_t *data, uint8_t size)
{
    uint8_t crc = 0;
    for (uint8_t i = 0; i < size; i++)
    {
        crc = crc8tab[crc ^ *data++];
    }
    return crc;
}

