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
#define EEPROM_MY_TX_EUI				(EEPROM_MACHINE_STATE+sizeof(uint32_t))
#define EEPROM_RF_FREQUENCY				(EEPROM_MY_TX_EUI+sizeof(uint64_t))
#define EEPROM_MY_AES_KEY				(EEPROM_RF_FREQUENCY+sizeof(uint32_t))
#define EEPROM_NEW_AES_ADEPT			(EEPROM_MY_AES_KEY+2*sizeof(uint64_t))
#define EEPROM_MAC_OF_MY_MASTER			(EEPROM_NEW_AES_ADEPT+2*sizeof(uint64_t))	//MAC GW/EN
#define EEPROM_ALARM_LEVEL				(EEPROM_MAC_OF_MY_MASTER+sizeof(uint64_t))
#define EEPROM_TYPE_OF_PARRENT			(EEPROM_ALARM_LEVEL+sizeof(uint32_t))
#define EEPROM_PULSE_COEF_A				(EEPROM_TYPE_OF_PARRENT+sizeof(uint32_t))
#define EEPROM_PULSE_INTERCEPT			(EEPROM_PULSE_COEF_A+sizeof(uint32_t))
#define	EEPROM_MY_NAME_IS				(EEPROM_PULSE_INTERCEPT+sizeof(uint32_t))
#define EEPROM_HEART_BEAT_RND_TIME		(EEPROM_MY_NAME_IS+sizeof(uint32_t))
#define EEPROM_BLINK_PERIOD				(EEPROM_HEART_BEAT_RND_TIME+sizeof(uint32_t))
#define EEPROM_PULSE_CALIB_OK			(EEPROM_BLINK_PERIOD+sizeof(uint32_t))
#define EEPROM_PULSE_RSSI_OK			(EEPROM_PULSE_CALIB_OK+sizeof(uint32_t))
#define EEPROM_RF_TX_OK					(EEPROM_PULSE_RSSI_OK+sizeof(uint32_t))
#define EEPROM_SW_VERSION				(EEPROM_RF_TX_OK+sizeof(uint32_t))
#define EEPROM_MAC_OF_MY_MASTER_2		(EEPROM_SW_VERSION+sizeof(uint32_t))
#define EEPROM_MAC_OF_MY_MASTER_3		(EEPROM_MAC_OF_MY_MASTER_2+sizeof(uint64_t))
#define EEPROM_COUNTER_INIT_DONE_1		(EEPROM_MAC_OF_MY_MASTER_3+sizeof(uint64_t))
#define EEPROM_COUNTER_INIT_DONE_2		(EEPROM_COUNTER_INIT_DONE_1+sizeof(uint32_t))



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
