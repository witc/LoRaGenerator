/*
 * LoadConfig.c
 *
 *  Created on: 25. 5. 2020
 *      Author: jan.ropek
 */

#include "main.h"
#include "LoadConfig.h"
#include "SignalProcessing.h"
#include "target.h"


/**
 *
 * @param data
 * @return
 */
uint8_t LC_GetMyName(uint8_t *data)
{
	uint8_t len = strlen(TARGET_BOARD_NAME);
	memcpy(data,TARGET_BOARD_NAME,len);

	return len;
}


/**
 *
 * @param data
 * @return
 */
uint8_t LC_GetSystemIfo(uint8_t *data)
{
	uint8_t cnt = 0;
	//*data++ = 0x29;
	//*data++ = 0x63;
	//cnt+=2;

	memcpy(data,TARGET_BOARD_MCU,strlen(TARGET_BOARD_MCU));
	data+=strlen(TARGET_BOARD_MCU);
	cnt+=strlen(TARGET_BOARD_MCU);

	memcpy(data,TARGET_BOARD_RADIO,strlen(TARGET_BOARD_RADIO));
	data+=strlen(TARGET_BOARD_RADIO);
	cnt+=strlen(TARGET_BOARD_RADIO);

	memcpy(data,TARGET_MIN_POWER,strlen(TARGET_MIN_POWER));
	data+=strlen(TARGET_MIN_POWER);
	cnt+=strlen(TARGET_MIN_POWER);

	memcpy(data,TARGET_MAX_POWER,strlen(TARGET_MAX_POWER));
	data+=strlen(TARGET_MAX_POWER);
	cnt+=strlen(TARGET_MAX_POWER);

	typedef union
	{
		uint32_t Id[3];
		uint8_t array[12];
	}TempUnion;

	TempUnion tempId;
	tempId.Id[0] = HAL_GetUIDw0();
	tempId.Id[1] = HAL_GetUIDw1();
	tempId.Id[2] = HAL_GetUIDw2();

	uint8_t tempstr[4];
	for(uint8_t i=0;i<12;i++)
	{
		itoa(tempId.array[i],tempstr,16);
		memcpy(data,tempstr,2);
		data+=2;
		cnt+=2;
	}

	return cnt;
}

/*
 *
 */
void LC_SaveTXFreq(uint32_t freq)
{
#ifdef USE_INTERNAL_EEPROM
	EepromStart(true);
	HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_WORD,EE_RADIO_TX_FREQ,freq);
	EepromStop();
#endif
}


/**
 *
 */
void LC_SaveRXFreq(uint32_t freq)
{
#ifdef USE_INTERNAL_EEPROM
	EepromStart(true);
	HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_WORD,EE_RADIO_RX_FREQ,freq);
	EepromStop();
#endif
}

/*
 *
 */
void LC_SaveTXSF(uint8_t sf)
{
#ifdef USE_INTERNAL_EEPROM
	EepromStart(true);
	HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_BYTE,EE_RADIO_TX_SF,sf);
	EepromStop();
#endif
}

/**
 *
 * @param sf
 */
void LC_SaveRXSF(uint8_t sf)
{
#ifdef USE_INTERNAL_EEPROM
	EepromStart(true);
	HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_BYTE,EE_RADIO_RX_SF,sf);
	EepromStop();
#endif
}

/**
 *
 * @param bw
 */
void LC_SaveTXBW(uint8_t bw)
{
#ifdef USE_INTERNAL_EEPROM
	EepromStart(true);
	HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_BYTE,EE_RADIO_TX_BW,(uint8_t)(bw));
	EepromStop();
#endif
}

/**
 *
 * @param bw
 */
void LC_SaveRXBW(uint8_t bw)
{
#ifdef USE_INTERNAL_EEPROM
	EepromStart(true);
	HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_BYTE,EE_RADIO_RX_BW,(uint8_t)(bw));
	EepromStop();
#endif
}

/**
 *
 * @param iq
 */
void LC_SaveTXIQ(uint8_t iq)
{
#ifdef USE_INTERNAL_EEPROM
	EepromStart(true);
	HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_BYTE,EE_RADIO_TX_INVERT,(uint8_t)(iq));
	EepromStop();
#endif
}

/**
 *
 * @param iq
 */
void LC_SaveRXIQ(uint8_t iq)
{
#ifdef USE_INTERNAL_EEPROM
	EepromStart(true);
	HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_BYTE,EE_RADIO_RX_INVERT,(uint8_t)(iq));
	EepromStop();
#endif
}

/**
 *
 * @param cr
 */
void LC_SaveTXCR(uint8_t cr)
{
#ifdef USE_INTERNAL_EEPROM
	EepromStart(true);
	HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_BYTE,EE_RADIO_TX_CR,(uint8_t)(cr));
	EepromStop();
#endif
}

/**
 *
 * @param cr
 */
void LC_SaveRXCR(uint8_t cr)
{
#ifdef USE_INTERNAL_EEPROM
	EepromStart(true);
	HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_BYTE,EE_RADIO_RX_CR,(uint8_t)(cr));
	EepromStop();
#endif
}

/**
 *
 * @param header
 */
void LC_SaveTXHeader(uint8_t header)
{
#ifdef USE_INTERNAL_EEPROM
	EepromStart(true);
	HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_BYTE,EE_RADIO_TX_HEADER,(uint8_t)(header));
	EepromStop();
#endif
}

/**
 *
 * @param header
 */
void LC_SaveRXHeader(uint8_t header)
{
#ifdef USE_INTERNAL_EEPROM
	EepromStart(true);
	HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_BYTE,EE_RADIO_RX_HEADER,(uint8_t)(header));
	EepromStop();
#endif
}

/**
 *
 * @param crc
 */
void LC_SaveTXCRC(uint8_t crc)
{
#ifdef USE_INTERNAL_EEPROM
	EepromStart(true);
	HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_BYTE,EE_RADIO_TX_CRC,(uint8_t)(crc));
	EepromStop();
#endif
}

/**
 *
 * @param crc
 */
void LC_SaveRXCRC(uint8_t crc)
{
#ifdef USE_INTERNAL_EEPROM
	EepromStart(true);
	HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_BYTE,EE_RADIO_RX_CRC,(uint8_t)(crc));
	EepromStop();
#endif
}


/**
 *
 * @param power
 */
void LC_SaveTXPower(uint8_t power)
{
#ifdef USE_INTERNAL_EEPROM
	EepromStart(true);
	HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_BYTE,EE_RADIO_TX_POWER,(uint8_t)(power));
	EepromStop();
#endif
}
