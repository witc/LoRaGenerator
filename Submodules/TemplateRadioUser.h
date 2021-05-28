/*
 * TemplateRadioUser.h
 *
 *  Created on: 7. 2. 2020
 *      Author: jan.ropek
 */

#ifndef SX1262_TEMPLATERADIOUSER_H_
#define SX1262_TEMPLATERADIOUSER_H_

#include "main.h"
#include "TaskRF.h"
#include "radio.h"
#include "sx126x.h"
#include "RadioCommands.h"




/*
 *
 */
typedef enum
{
	DR_IQ_FALSE=0,
	DR_IQ_TRUE=1,
	DR_IQ_SIZE,
}ENUM_DR_IQ;


/**
 *
 */
typedef enum
{
	DR_HEADER_FALSE=0,//!< DR_HEADER_FALSE
	DR_HEADER_TRUE=1, //!< DR_HEADER_TRUE
	DR_HEADER_SIZE,   //!< DR_HEADER_SIZE
}ENUM_DR_HEADER;

/**
 *
 */
typedef enum
{
	DR_CRC_FALSE=0,
	DR_CRC_TRUE=1,
	DR_CRC_SIZE,
}ENUM_DR_CRC;



/*
 *
 */
typedef enum
{
	DR_BW_7_81_KHZ=0,
	DR_BW_10_42_KHZ=1,
	DR_BW_15_63_KHZ=2,
	DR_BW_20_83_KHZ=3,
	DR_BW_31_25_KHZ=4,
	DR_BW_41_67_KHZ=5,
	DR_BW_62_5_KHZ=6,
	DR_BW_125_KHZ=7,
	DR_BW_250_KHZ=8,
	DR_BW_500_KHZ=9,
	DR_BW_SIZE,

}ENUM_DR_BW;

/*
 *
 */
typedef enum
{
	DR_SF5=0,
	DR_SF6=1,
	DR_SF7=2,
	DR_SF8=3,
	DR_SF9=4,
	DR_SF10=5,
	DR_SF11=6,
	DR_SF12=7,
	DR_SF_SIZE,

}ENUM_DR_SF;

/**
 *
 */
typedef enum
{
	DR_CR_45=0,//!< DR_CR_45
	DR_CR_46=1,//!< DR_CR_46
	DR_CR_47=2,//!< DR_CR_47
	DR_CR_48=3,//!< DR_CR_48
	DR_CR_SIZE,

}ENUM_DR_CR;


typedef struct
{
	ENUM_DR_SF		sf;
	ENUM_DR_BW		bw;
	ENUM_DR_IQ		iq;
	uint32_t		freq;
	ENUM_DR_CR		cr;
	ENUM_DR_HEADER	headerMode;
	ENUM_DR_CRC		crcCheck;

}__packed RadioPar;

/**
 *
 */
typedef struct
{
	RadioPar		RxConfig;
	RadioPar		TxConfig;
	int8_t			TxPower;

}tRadioParam;

/**
 *
 */
typedef struct
{
	bool		autoRepeat;
	uint32_t	repeatPeriod;
	uint32_t	rxTimeout;

}tPacketParam;


/*
 *
 */
typedef struct
{
	tRadioParam 	RadioParam;
	tPacketParam	PacketParam;
	bool			rxSingle;

}tRadioUserConfig;


typedef union
{
	uint8_t  				DataArray[radioPACKET_MAX_SIZE];

}tGeneralPacket;


/*
 * @ Commands for radio
 */
typedef enum
{
	RF_CMD_NONE=0,
	RF_CMD_INIT,
	RF_CMD_DEINIT,
	RF_CMD_START_RX,
	RF_CMD_STOP_RX,
	RF_CMD_RESTART_RX,
	RF_CMD_TX_CW,
	RF_CMD_STOP_TX,
	RF_CMD_SEND_UNIVERSAL_PAYLOAD_LBT,
	RF_CMD_SEND_UNIVERSAL_PAYLOAD_NOW,
	RF_CMD_HEARTBEAT_STOPPED,
	RF_CMD_GIVE_ME_RN,	//return random number
	RF_CMD_LBT_TIMEOUT,
	RF_CMD_STANDBY,
	RF_CMD_SLEEP

}RfCommands;


/* Functions */
bool		RU_SX1262Assign				(void);
void		RU_GetTrueRandom32bit		(uint32_t RandomData[], uint8_t CountOfRandomData);
void		RU_SX1262Init				(void);
void		RU_RadioInit				(void);
void		RU_RadioDeinit				(void);
uint8_t 	RU_IRQProcess				(tRfGlobalData* GlobalData);
void 		RU_CommandProcess			(RfCommands cmd,tRfGlobalData* GlobalData, DATA_QUEUE *ReceiveData);
void 		RU_RadioStandby				(void);
void 		RU_RadioSleep				(void);
void 		RU_LoRaConfigAndStartRX 	(uint32_t freq,RadioPar DR, bool Rx,uint8_t payloadSize, uint32_t rxTimeout);
void 		RU_RFSetTXUp				(int8_t power, uint32_t freq, RadioPar DR);
void 		RU_RFSetTXDown				(void);
void 		RU_GetRfFrequency			(uint32_t *frequency);
void 		RU_SetRfFrequency			(uint32_t frequency);
void 		RU_UpdatetMyAESKey			(void);
void 		RU_UpdatetMyTxEUI			(void);
uint64_t	RU_GetMyMasterMac			(void);
RadioPar 	RU_GetDrRX					(void);
RadioPar 	RU_GetDrTX					(void);
void		RU_SetDrTX					(RadioPar newDrTx);
void		RU_SetDrRX					(RadioPar newDrRx);
void 		LoRaScanCAD					(RadioLoRaCadSymbols_t symbols);
bool 		RU_RunLBTProcedure			(RadioPar DrData);

extern tRadioUserConfig			RadioUserConfig;

#endif /* SX1262_TEMPLATERADIOUSER_H_ */
