/*
 * LoadConfig.c
 *
 *  Created on: 25. 5. 2020
 *      Author: jan.ropek
 */

#include "main.h"
#include "LoadConfig.h"
#include "SignalProcessing.h"


tGLobalConfig GlMonitorProfile;


/**
 *
 * @param time
 */
void CL_SetBlinkPeriod(uint32_t time)
{
	EepromStart(true);
	HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_WORD,EEPROM_BLINK_PERIOD,(uint32_t)(time)); // random 0-3 sec
	EepromStop();
}

/**
 *
 * @return
 */
uint32_t CL_GetBlinkPeriod(void)
{
	uint32_t ret;

	EepromStart(false);
	memcpy(&ret,(uint8_t*)EEPROM_BLINK_PERIOD,4);
	EepromStop();

	return ret;
}


/**
 *
 * @param time
 */
void CL_SetTimeHeartBeatPulse(uint32_t time)
{
	EepromStart(true);
	HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_WORD,EEPROM_HEART_BEAT_RND_TIME,(uint32_t)(time)); // random 0-3 sec
	EepromStop();
}

/**
 *
 * @return
 */
uint32_t CL_GetTimeHeartBeatPulse(void)
{
	uint32_t ret;

	EepromStart(false);
	memcpy(&ret,(uint8_t*)EEPROM_HEART_BEAT_RND_TIME,4);
	EepromStop();

	ret=SP_ConstrainU32(ret,0,3000);

	return ret;
}

