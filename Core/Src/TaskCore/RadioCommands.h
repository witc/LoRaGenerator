/*
 * RadioCommands.h
 *
 *  Created on: 28. 1. 2021
 *      Author: jan.ropek
 */

#ifndef SRC_RADIOCOMMANDS_H_
#define SRC_RADIOCOMMANDS_H_

#define RF_MAX_ALLOWEDPACKET_SIZE			(200)


#define RADIO_CMD_SF_OFFSET		5
#define RADIO_CMD_CR_OFFSET		45



typedef struct
{
	eDR_SF		sf;
	eDR_BW		bw;
	eDR_IQ		iq;
	uint32_t	freq;
	eDR_CR		cr;
	eDR_HEADER	headerMode;
	eDR_CRC		crcCheck;

}RadioPar;

/**
 *
 */
typedef struct
{
	RadioPar		RxConfig;
	RadioPar		TxConfig;
	int8_t			Power;


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
uint32_t RC_RadioGetTxFreq(void);
bool RC_RadioSetRxFreq(uint32_t freq);
uint32_t RC_RadioGetRxFreq(void);
bool RC_RadioSetTxPower(int8_t power);
int8_t RC_RadioGetTxPower(void);
bool RC_RadioSetTxSf(uint8_t sf);
uint32_t RC_RadioGetTxSf(void);
bool RC_RadioSetRxSf(uint8_t sf);
uint32_t RC_RadioGetRxSf();
bool RC_RadioSetTxBw(uint32_t bw);
uint32_t RC_RadioGetTxBw();
bool RC_RadioSetRxBw(uint32_t bw);
uint32_t RC_RadioGetRxBw(void);
bool RC_RadioSetTxIq(uint8_t Iq);
uint8_t RC_RadioGetTxIq(void);
bool RC_RadioSetRxIq(uint8_t Iq);
uint8_t RC_RadioGetRxIq(void);
bool RC_RadioSetTxCr(uint8_t Cr);
uint8_t RC_RadioGetTxCr(void);
bool RC_RadioSetRxCr(uint8_t Cr);
uint8_t RC_RadioGetRxCr(void);
bool RC_RadioSetTXHeaderMode(uint8_t header);
uint8_t RC_RadioGetTXHeaderMode();
bool RC_RadioSetRXHeaderMode(uint8_t header);
uint8_t RC_RadioGetRXHeaderMode();
bool RC_RadioSetTXCRC(uint8_t crc);
uint8_t RC_RadioGetTXCRC();
bool RC_RadioSetRXCRC(uint8_t crc);
uint8_t RC_RadioGetRXCRC();
uint32_t RC_RadioGetRadioStatus();

bool RC_RadioSetStandby();
bool RC__RadioSetTxCW();
bool RC_SavePacket(uint8_t *data);
uint8_t RC_GetSizeOfSavedPacket(void);


#endif /* SRC_RADIOCOMMANDS_H_ */
