/*
 * SignalProcessing.c
 *
 *  Created on: 30. 3. 2020
 *      Author: jan.ropek
 */


#include "main.h"
#include "SignalProcessing.h"

/*impulse  kV*/

/* ADC Batt*/
#define CONST_MV_TO_PACKET_MULTIPLE		(254/5)

/**
 *
 * @param adc
 * @return
 */
void SP_SMA_ImpulseFilter(uint16_t adc, tCoreGlobalData *GlobalData)
{
//	static uint16_t filterBuff[LENGTH_IMPULSE_FILTER-1];
//	uint32_t sumFilter=0;
//
//
//	for(uint8_t i=(LENGTH_IMPULSE_FILTER-1);i>1;i--)
//	{
//		filterBuff[i-1]=filterBuff[i-2];
//	}
//
//	filterBuff[0]=adc;
//
//	for(uint8_t i=0;i<(LENGTH_IMPULSE_FILTER-1);i++)
//	{
//		sumFilter+=filterBuff[i];
//	}
//
//	GlobalData->LastFilteredImpulsemV= (uint16_t)(sumFilter/(LENGTH_IMPULSE_FILTER-1));

	/* odstraneni filtrace pro zachyceni prustrelu n ohrade */
	GlobalData->LastFilteredImpulsemV = adc;

}

/**
 *
 * @param value
 * @param min
 * @param max
 * @return
 */
double	SP_ConstrainDouble(double value, double min, double max)
{
	if(value>max)		return max;
	else if(value<min)	return min;

	return value;
}

/**
 *
 * @param value
 * @param min
 * @param max
 * @return
 */
uint8_t	SP_ConstrainU8(uint8_t value, uint8_t min, uint8_t max)
{
	if(value>max)		return max;
	else if(value<min)	return min;

	return value;
}

/**
 *
 * @param value
 * @param min
 * @param max
 * @return
 */
uint32_t	SP_ConstrainU32(uint32_t value, uint32_t min, uint32_t max)
{
	if(value>max)		return max;
	else if(value<min)	return min;

	return value;
}


/**
 *
 * @param coefA
 */
void SP_SaveImpulsCoefA(int32_t coefA)
{
	EepromStart(true);
	HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_WORD,EEPROM_PULSE_COEF_A,(uint32_t)(coefA));
	EepromStop();
}

/**
 *
 * @param intercept
 */
void SP_SaveImpulsIntercept(int32_t intercept)
{
	EepromStart(true);
	HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_WORD,EEPROM_PULSE_INTERCEPT,(uint32_t)(intercept));
	EepromStop();
}

/**
 *
 * @param coefA
 * @param intercept
 */
void SP_PulseGetCoefAIntercept(int32_t * coefA, int32_t *intercept)
{
	EepromStart(false);
	memcpy(coefA,(uint8_t*)EEPROM_PULSE_COEF_A,4);
	memcpy(intercept,(uint8_t*)EEPROM_PULSE_INTERCEPT,4);
	EepromStop();

}
