/*
 * RadioCommands.h
 *
 *  Created on: 28. 1. 2021
 *      Author: jan.ropek
 */

#ifndef SRC_RADIOCOMMANDS_H_
#define SRC_RADIOCOMMANDS_H_

#define MAX_ALLOWEDPACKET_SIZE			100


typedef struct
{
	DrConfig_t	SfBqIq;
	uint32_t	freq;
	uint8_t		cr;
	bool		headerMode;
	bool		crcCheck;

}RadioPar;

/**
 *
 */
typedef struct
{
	RadioPar		RxConfig;
	RadioPar		TxConfig;
	int8_t				Power;


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


extern tRadioParam RadioParam;
extern tPacketParam	PacketParam;

bool RC_RadioSetTxFreq(uint32_t freq);
bool RC_RadioSetRxFreq(uint32_t freq);
bool RC_RadioSetTxPower(int8_t power);
bool RC_RadioSetStandby();
bool RC__RadioSetTxCW();
bool RC_SavePacket(uint8_t *data);
uint8_t RC_GetSizeOfSavedPacket(void);


#endif /* SRC_RADIOCOMMANDS_H_ */
