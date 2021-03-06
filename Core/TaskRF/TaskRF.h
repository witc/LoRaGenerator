/*
 * TaskRF.h
 *
 *  Created on: 12. 3. 2020
 *      Author: jan.ropek
 */

#ifndef TASKRF_TASKRF_H_
#define TASKRF_TASKRF_H_

#include "radio.h"
#include "RadioCommands.h"

/**
 *
 */
typedef enum
{
	RX_CH_NONE=0,	  //!< zadny prijem
	RX_CH_1=1,        //!< RX_CH_1	// RSSI
	RX_CH_2=2,        //!< RX_CH_2	// PULSE RF
	RX_CH_3_STANDARD=3//!< RX_CH_3_STANDARD	// Pro bezny provoz

}eRxChannels;

/**
 *
 */
typedef struct
{
	RadioState_t		RF_State;

}__packed tRfGlobalData;

/**
 *
 */
typedef struct
{
	struct AES_ctx PairingCtx;
	struct AES_ctx PairingCtxDown;
	GeneralPacketsUpOrDown_t	localTxBuffer;
	uint8_t						sizeOfPacket;
	RadioPar					nextDr;

	void* PointerMalloc;

}tStaticStructure_RF, *p_tStaticStructure_RF;


#endif /* TASKRF_TASKRF_H_ */
