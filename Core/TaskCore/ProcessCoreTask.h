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
#define UART_CIRCLE_MAX_BUFFER_SIZE		(200)	//musi byt > 2= MAximal RX msg ?!?
#define TIME_TO_CHECK_UART_RX_BUFFER	(1)// // pri 9600 Baud => 1200B/sec
#define MAX_SIZE_FOR_PAYLOAD			(100)
#define UART_CHECK_FREQUENCY			(3600)

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
	UART_MSG_SET_RX_SF=5,
	UART_MSG_SET_TX_BW=6,
	UART_MSG_SET_RX_BW=7,
	UART_MSG_SET_TX_IQ=8,
	UART_MSG_SET_RX_IQ=9,
	UART_MSG_SET_TX_CR=10,
	UART_MSG_SET_RX_CR=11,
	UART_MSG_SET_STANDBY=12,
	UART_MSG_SET_TX_CW=13,
	UART_MSG_PREP_PACKET=14,
	UART_MSG_SET_AUTO_REPEAT=15,
	UART_MSG_SET_REPEATING_PERIOD=16,
	UART_MSG_RX_CRC_CHECK=17,
	UART_MSG_SET_HEADER_MODE=18,
	UART_MSG_SEND_PACKET=19,
	UART_MSG_SEND_LAST_PAKET_AGAIN=20,
	UART_MSG_START_RX=21,

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
	eUART_MSG_OK,
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


uint8_t 		PCT_CalcCRC(uint8_t *data, uint8_t size);
bool 			PCT_FindSyncWord(uint8_t *data, uint8_t sizeToSearch, uint8_t *headerStarts);
eUARTBufferMasg PCT_FindAnyMsg(uint8_t **rxPacket);
void 			PCT_DecodeUartRxMsg(uint8_t *rxBuffer);
void 			PCT_SendRfPacket();

#endif /* TASKCORE_PROCESSCORETASK_H_ */
