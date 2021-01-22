/*
 * EepromAddress.h
 *
 *  Created on: 12. 3. 2020
 *      Author: jan.ropek
 */

#ifndef INC_EEPROMADDRESS_H_
#define INC_EEPROMADDRESS_H_

#include "cmsis_os.h"
#include "stm32l071xx.h"
#include "stdint.h"

#define EEPROM_MACHINE_STATE			(DATA_EEPROM_BASE)
#define EEPROM_RF_TX_POWER				(EEPROM_MACHINE_STATE)	//uint8_t

#define ADDR_ARRAY_COUNT_PAIRING		(DATA_EEPROM_BASE+1024)
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

#endif /* INC_EEPROMADDRESS_H_ */
