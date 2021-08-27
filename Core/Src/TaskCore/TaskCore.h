/*
 * TaskCore.h
 *
 *  Created on: 12. 3. 2020
 *      Author: jan.ropek
 */

#ifndef TASKCORE_TASKCORE_H_
#define TASKCORE_TASKCORE_H_

#include "main.h"




/**
 *
 */
typedef struct
{
	uint32_t		 ShortPressLowLevelMask;
	uint8_t			 KeyBoardGoToPair;

}__packed tCoreGlobalData;

/**
 *
 */
typedef struct
{
	uint32_t ShortPressLowLevelMask;
	void* PointerMalloc;

}tStaticStructure_CORE, *p_tStaticStructure_CORE;

/**
 *
 */
typedef struct
{
	uint32_t BitMask;
	uint32_t ProhibitionBitMask;
	uint8_t Counter;
}tKeyBoard;

/**
 *
 */
typedef enum
{
	STATE_CORE_INIT,
	STATE_CORE_OFF,
	STATE_CORE_START_ON,
	STATE_CORE_ON,
	STATE_CORE_START_OFF,
	STATE_CORE_ERROR			// pokud zabral watchdog
}eStateCoreSystem;


/**
 * Core task
 */
typedef struct
{
	eStateCoreSystem	ActualState;
	eStateCoreSystem	PreviousState;

} tStateCoreAutomat;

/**
 *
 */
typedef enum
{
	PACKET_BORN_NONE=0,//!< PACKET_BORN_NONE
	PACKET_BORN_TRUE=1,//!< PACKET_BORN_TRUE
	PACKET_BORN_FALSE=0//!< PACKET_BORN_FALSE

}ePacketBorn;


/**
 *
 */
typedef enum
{
	PACKET_SEND_NONE=0,
	PACKET_SEND_NOW=1,
	PACKET_SEND_IF_FREE=2
}ePacketSendNow;


void 		CallbackButtonIRQ			(bool init);
void		KeyboardTimElapsed			(TIM_HandleTypeDef *htim);
void 		CallbackWatchDogRefresh		(void);
#endif /* TASKCORE_TASKCORE_H_ */
