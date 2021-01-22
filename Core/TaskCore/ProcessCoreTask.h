/*
 * ProcessCoreTask.h
 *
 *  Created on: 17. 3. 2020
 *      Author: jan.ropek
 */

#ifndef TASKCORE_PROCESSCORETASK_H_
#define TASKCORE_PROCESSCORETASK_H_

#include "main.h"
#include "TaskCore.h"

#define UART_BUFF_CRC_SIZE				(1)
#define UART_BUFF_HEADER_SIZE			(1)
#define MINIMAL_SIZE_USART_RX_MSG		(sizeof(SyncUartMsg)+UART_BUFF_HEADER_SIZE+1/*payload*/+UART_BUFF_CRC_SIZE/*crc*/)
#define MAXIMAL_SIZE_USART_RX_MSG		(sizeof(SyncUartMsg)+UART_BUFF_HEADER_SIZE+60/*payload*/+UART_BUFF_CRC_SIZE/*crc*/)
#define UART_CIRCLE_MAX_BUFFER_SIZE		(50)	//musi byt > 2= MAximal RX msg
#define TIME_TO_CHECK_UART_RX_BUFFER	(5)// // pri 9600 Baud => 1200B/sec
#define MAX_SIZE_FOR_PAYLOAD			(30)

extern uint8_t GlUartRxBugger[UART_CIRCLE_MAX_BUFFER_SIZE];

/**
 *
 */
typedef enum
{
	SOUND_NONE=0,     //!< SOUND_NONE
	SOUND_SHORT_PRESS,//!< SOUND_SHORT_PRESS
	SOUND_LONG_PRESS, //!< SOUND_LONG_PRESS
	SOUND_START_ON,
	SOUND_START_OFF,
	SOUND_START_PAIR

}eSoundSType;

/*
 *
 */
typedef enum
{
	UART_MSG_NONE=0,
	UART_MSG_SET_TX_FREQ=1,
	UART_MSG_SET_RX_FREQ=2,
	UART_MSG_SET_TX_POWER=3,
	UART_MSG_SET_TX_SF=4,
	UART_MSG_SET_TX_BW=5,
	UART_MSG_SET_TX_IQ=6,
	UART_MSG_SET_TX_CR=7,
	UART_MSG_SET_STANDBY=8,
	UART_MSG_SET_TX_CW=9,

}eUartMsgCmds;

/**
 *
 */
typedef enum
{
	COLOR_NONE=0,//!< COLOR_NONE
	COLOR_GREEN, //!< COLOR_GREEN
	COLOR_RED,   //!< COLOR_RED
	COLOR_ORANGE,//!< COLOR_ORANGE
	COLOR_SWAP,  //!< COLOR_SWAP


}eColorLed;


typedef enum
{
	eUART_MSG_EMPTY=0,
	eUART_MSG_TOO_SHORT,
	eUART_MSG_WRONG_HEADER,
	eUART_MSG_WRONG_CRC,
}eUARTBufferMasg;


/**
 *
 */
typedef union
{
	uint8_t		payload[250];

}eUartMsgs;


uint8_t PCT_CalcCRC(uint8_t *data, uint8_t size);
bool 	PCT_FindSyncWord(uint8_t *data, uint8_t sizeToSearch, uint8_t *headerStarts);
bool	PCT_RadioSetTxFreq(uint32_t freq);
bool 	PCT_RadioSetRxFreq(uint32_t freq);
bool 	PCT_RadioSetTxPower(int8_t power);
bool	PCT_RadioSetTxSF(uint8_t sf);
bool	PCT_RadioSetTxBW(uint16_t bw);
bool 	PCT_RadioSetTxIQ(bool *iqInvert);
bool	PCT_RadioSetTxCR(uint8_t *cr);
bool	PCT_RadioSetStandby();
bool    PCT_RadioSetTxCW();
void 	PCT_DecodeUartRxMsg(uint8_t *rxBuffer);

#endif /* TASKCORE_PROCESSCORETASK_H_ */
