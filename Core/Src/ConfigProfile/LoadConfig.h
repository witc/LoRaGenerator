/*
 * LoadConfig.h
 *
 *  Created on: 25. 5. 2020
 *      Author: jan.ropek
 */

#ifndef SRC_LOADCONFIG_H_
#define SRC_LOADCONFIG_H_


#define USE_INTERNAL_EEPROM		1

#define radioSF_MAX		12
#define radioSF_MIN		5
#define radioBW_MAX		500000
#define radioBW_MIN		7810
#define radioCR_MAX		48
#define radioCR_MIN		45


#ifdef USE_INTERNAL_EEPROM

/**
 *
 */
void inline EepromStart(bool write)
{
    osThreadSuspendAll();
    if(write == true)
    {
    	HAL_FLASHEx_DATAEEPROM_Unlock();
    }

}

/**
 *
 */
void inline EepromStop(void)
{
    HAL_FLASHEx_DATAEEPROM_Lock();
    osThreadResumeAll();
}


#define EE_RADIO_TX_POWER		DATA_EEPROM_BASE
#define EE_RF_DATA_PACKET		(EE_RADIO_TX_POWER+sizeof(uint32_t))
#define EE_RF_PACKET_SIZE		(EE_RF_DATA_PACKET+50*sizeof(uint32_t))	//uint8_t
#define EE_RADIO_TX_FREQ		(EE_RF_PACKET_SIZE+sizeof(uint32_t))
#define EE_RADIO_RX_FREQ		(EE_RADIO_TX_FREQ+sizeof(uint32_t))
#define EE_RADIO_TX_SF			(EE_RADIO_RX_FREQ+sizeof(uint32_t))
#define EE_RADIO_RX_SF			(EE_RADIO_TX_SF+sizeof(uint32_t))
#define EE_RADIO_TX_BW			(EE_RADIO_RX_SF+sizeof(uint32_t))
#define EE_RADIO_RX_BW			(EE_RADIO_TX_BW+sizeof(uint32_t))
#define EE_RADIO_TX_INVERT		(EE_RADIO_RX_BW+sizeof(uint32_t))
#define EE_RADIO_RX_INVERT		(EE_RADIO_TX_INVERT+sizeof(uint32_t))
#define EE_RADIO_TX_CR			(EE_RADIO_RX_INVERT+sizeof(uint32_t))
#define EE_RADIO_RX_CR			(EE_RADIO_TX_CR+sizeof(uint32_t))
#define EE_RADIO_TX_HEADER		(EE_RADIO_RX_CR+sizeof(uint32_t))
#define EE_RADIO_RX_HEADER		(EE_RADIO_TX_HEADER+sizeof(uint32_t))
#define EE_RADIO_TX_CRC			(EE_RADIO_RX_HEADER+sizeof(uint32_t))
#define EE_RADIO_RX_CRC			(EE_RADIO_TX_CRC+sizeof(uint32_t))


#endif

typedef struct
{
	uint32_t	TimeLedOn;
	uint32_t	TimeShowAlive;
	uint32_t	TxPeriod;
	uint32_t	DurationRxWindow;
	uint32_t	TimeToEnTrigger;
	uint32_t	TimeoutPair;
	uint32_t	TimeoutRfTx;
	uint32_t  	TimeoutRepeatePacket;
	uint32_t	TimeoutEnSendNextPacket;
	uint32_t	TimeoutTrySendNextPacket;
}__packed tGLobalConfig;



uint8_t LC_GetSystemIfo(uint8_t *data);
void LC_SaveTXFreq(uint32_t freq);
void LC_SaveRXFreq(uint32_t freq);
void LC_SaveTXSF(uint8_t sf);
void LC_SaveRXSF(uint8_t sf);
void LC_SaveTXBW(uint8_t bw);
void LC_SaveRXBW(uint8_t bw);
void LC_SaveTXIQ(uint8_t iq);
void LC_SaveRXIQ(uint8_t iq);
void LC_SaveTXCR(uint8_t cr);
void LC_SaveRXCR(uint8_t cr);
void LC_SaveTXHeader(uint8_t header);
void LC_SaveRXHeader(uint8_t header);
void LC_SaveTXCRC(uint8_t crc);
void LC_SaveRXCRC(uint8_t crc);
void LC_SaveTXPower(uint8_t power);

#endif /* SRC_LOADCONFIG_H_ */
