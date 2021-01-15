/*
 * EEPROM_counter.h
 *
 *  Created on: 25. 1. 2019
 *      Author: Radek Smejkal
 */

#ifndef EEPROM_COUNTER_EEPROM_COUNTER_H_
#define EEPROM_COUNTER_EEPROM_COUNTER_H_

#define TEST_VALUE_INIT_EEPROM		16

#define EEPROM_BASE 				(DATA_EEPROM_BASE + 1024)
#define EEPROM_SIZE_FOR_COUNTER 	(4*1024)

#define COUNTER_ALLDATA_NUMBER_OF_BYTES  508
#define COUNTER_MAX_POINTER				(COUNTER_ALLDATA_NUMBER_OF_BYTES/4)
#define COUNTER_POINTER_NUMBER_OF_BYTES  4
#define COUNTER_DATA_NUMBER_OF_BYTES     4

#define COUNTER_STATE           0x0
#define COUNTER_POINTER1        0x4
#define COUNTER_DATA11          (COUNTER_POINTER1 + COUNTER_POINTER_NUMBER_OF_BYTES)
#define COUNTER_DATA12          (COUNTER_DATA11 + COUNTER_ALLDATA_NUMBER_OF_BYTES)
#define COUNTER_POINTER2        (COUNTER_DATA12 + COUNTER_ALLDATA_NUMBER_OF_BYTES)
#define COUNTER_DATA21          (COUNTER_POINTER2 + COUNTER_POINTER_NUMBER_OF_BYTES)
#define COUNTER_DATA22          (COUNTER_DATA21 + COUNTER_ALLDATA_NUMBER_OF_BYTES)
#define COUNTER_POINTER3        (COUNTER_DATA22 + COUNTER_ALLDATA_NUMBER_OF_BYTES)
#define COUNTER_DATA31          (COUNTER_POINTER3 + COUNTER_POINTER_NUMBER_OF_BYTES)
#define COUNTER_DATA32          (COUNTER_DATA31 + COUNTER_ALLDATA_NUMBER_OF_BYTES)

#define COUNTER_POINTER_TX      (COUNTER_DATA32 + COUNTER_ALLDATA_NUMBER_OF_BYTES)
#define COUNTER_DATA1_TX        (COUNTER_POINTER_TX + COUNTER_POINTER_NUMBER_OF_BYTES)
#define COUNTER_DATA2_TX        (COUNTER_DATA1_TX + COUNTER_ALLDATA_NUMBER_OF_BYTES)

#define POINTER 				0
#define DATA_1  				1
#define DATA_2  				2

#define TX_COUNTER				3

#define NUMBER_OF_COUNTERS  	4
#define MAX_WRITE           	250000
#define REPEAT_COUNTER			1

int CounterMemoryInit();
uint32_t CounterMemoryReadAndIncrement(const uint8_t position);
uint32_t CounterMemoryGetNumberOfRecords(const uint8_t position);
uint32_t CounterMemoryGetActualCounter(const uint8_t position);
int CounterMemorySetActualCounter(const uint8_t position, uint32_t counter);


#endif /* EEPROM_COUNTER_EEPROM_COUNTER_H_ */
