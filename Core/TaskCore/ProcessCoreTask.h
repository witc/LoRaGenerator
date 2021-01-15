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
#define UART_BUFF_HEADER_SIZE			(2)
#define MINIMAL_SIZE_USART_RX_MSG		(sizeof(SyncUartMsg)+UART_BUFF_HEADER_SIZE+1/*payload*/+UART_BUFF_CRC_SIZE/*crc*/)
#define MAXIMAL_SIZE_USART_RX_MSG		(sizeof(SyncUartMsg)+UART_BUFF_HEADER_SIZE+60/*payload*/+UART_BUFF_CRC_SIZE/*crc*/)
#define UART_CIRCLE_MAX_BUFFER_SIZE		(50)	//musi byt > 2= MAximal RX msg
#define TIME_TO_CHECK_UART_RX_BUFFER	(2)// // pri 9600 Baud => 1200B/sec
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

typedef enum
{
	COLOR_NONE=0,
	COLOR_GREEN,
	COLOR_RED,
	COLOR_ORANGE,
	COLOR_SWAP,


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
bool PCT_FindSyncWord(uint8_t *data, uint8_t sizeToSearch, uint8_t *headerStarts);


#endif /* TASKCORE_PROCESSCORETASK_H_ */
