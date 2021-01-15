/*
 * TaskCore.h
 *
 *  Created on: 12. 3. 2020
 *      Author: jan.ropek
 */

#ifndef TASKCORE_TASKCORE_H_
#define TASKCORE_TASKCORE_H_

#include "main.h"


#define KEYBOARD_TIMER						TIM7
#define PWM_TIMER							TIM2
#define SYSTEM_ADC							ADC1
#define SYSTEM_DMA_FOR_ADC					DMA1

extern uint16_t	GlAdcBattMeas[ADC_BATT_BUFFER_SIZE];

typedef enum
{
	PARENT_TYPE_NONE=0,
	PARENT_TYPE_GW=1,
	PARENT_TYPE_ENERGY=2

}eParentType;

typedef enum
{
	CALIB_STEP_NONE=0,
	CALIB_STEP_RF_TX_RUN=1,
	CALIB_STEP_PULSE_RUN=2,
	CALIB_STEP_RF_RX_RUN
}eCalibSteps;

/**
 *
 */
typedef struct
{
	uint32_t		 ShortPressLowLevelMask;
	uint8_t			 KeyBoardGoToPair;
	bool			 PairInProcess;
	bool			 ImpulseMeasureIsEnabled;
	uint64_t		 tempMasterMac;
	uint16_t		 CountImpulse;
	uint16_t		 ImpulseValuemV;
	uint16_t 		 LastFilteredImpulsemV;
	uint8_t		 	 KvImpulseValue;	/*100 ~ 10 kV*/
	uint8_t			 KvImpulseLastValue;
	uint8_t			 flagUnderPulse;
	uint8_t			 flagAbovePulse;
	uint8_t			 LastBattLevelPacket;		/*254 ~ 5V, 0=0V*/
	uint16_t		 BattValuemV;
	uint8_t			 BattPercent;
	uint32_t		 FrameCounterWait;
	uint8_t			 MaxAttemptToSendData;
	bool			 AlarmKvSend;
	bool			 AlarmBattSend;
	bool 			 FirstImpulseSent;
	bool			 CanSendNextPacket;
	bool 		     FactoryTestActive;
	uint8_t			 RequestsToSendRF;
	eCalibSteps		 FactoryCalibSteps;
	uint32_t		 USBLinkSessionKey;
	uint8_t			 PairFirstStepBeIn;
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
