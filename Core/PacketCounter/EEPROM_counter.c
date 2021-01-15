/*
 * EEPROM_counter.c
 *
 *  Created on: 25. 1. 2019
 *      Author: Radek Smejkal
 */

#include "cmsis_os.h"
#include <stdbool.h>
#include "FreeRTOS.h"
#include "main.h"
#include "EEPROM_counter.h"

uint32_t addresCM[4][3] = { {COUNTER_POINTER1, COUNTER_DATA11, COUNTER_DATA12},
							{COUNTER_POINTER2, COUNTER_DATA21, COUNTER_DATA22},
							{COUNTER_POINTER3, COUNTER_DATA31, COUNTER_DATA32},
							{COUNTER_POINTER_TX, COUNTER_DATA1_TX, COUNTER_DATA2_TX}};

uint8_t eeprom_read_8bit(uint32_t addres)
{
	return (*(volatile uint8_t *) (EEPROM_BASE + addres));
}

uint16_t eeprom_read_16bit(uint32_t addres)
{
	return (*(volatile uint16_t *) (EEPROM_BASE + addres));
}

uint32_t eeprom_read_32bit(uint32_t addres)
{
	return (*(volatile uint32_t *) (EEPROM_BASE + addres));
}

void eeprom_write_8bit(uint32_t addres, uint8_t data)
{
	HAL_FLASHEx_DATAEEPROM_Unlock();
	HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_BYTE, EEPROM_BASE + addres, data);
	HAL_FLASHEx_DATAEEPROM_Lock();
}

void eeprom_write_16bit(uint32_t addres, uint16_t data)
{
	HAL_FLASHEx_DATAEEPROM_Unlock();
	HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_HALFWORD, EEPROM_BASE + addres, data);
	HAL_FLASHEx_DATAEEPROM_Lock();
}

void eeprom_write_32bit(uint32_t addres, uint32_t data)
{
	HAL_FLASHEx_DATAEEPROM_Unlock();
	HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_WORD, EEPROM_BASE + addres, data);
	HAL_FLASHEx_DATAEEPROM_Lock();
}

/*
 * return 0: eeprom fail
 * return 1: init ok
 * return 2: has already been initialized
 */
int CounterMemoryInit()
{
	volatile uint32_t temp = 0;
	uint8_t test[3];
	uint8_t  tempCount=0;

	EepromStart(false);
	memcpy(&test[0],(uint8_t*)(EEPROM_BASE+COUNTER_STATE),1);
	memcpy(&test[1],(uint8_t*)EEPROM_COUNTER_INIT_DONE_1,1);
	memcpy(&test[2],(uint8_t*)EEPROM_COUNTER_INIT_DONE_2,1);
	EepromStop();

	for(uint8_t count=0;count<3;count++)
	{
		if(test[count]==TEST_VALUE_INIT_EEPROM) tempCount+=1;
	}

	//temp = eeprom_read_8bit(COUNTER_STATE);
	if (tempCount<2)
	{
		for (uint32_t i = 0; i < (EEPROM_SIZE_FOR_COUNTER/4); i++)
		{
			eeprom_write_32bit(i*4, 0xffffffff);
			temp = eeprom_read_32bit(i*4);
			if (temp != 0xffffffff) return 0;

			eeprom_write_32bit(i*4, 0x0);
			temp = eeprom_read_32bit(i*4);
			if (temp != 0x0) return 0;

			RefreshWatchDog();
		}

		EepromStart(true);
		eeprom_write_8bit(COUNTER_STATE, TEST_VALUE_INIT_EEPROM);
		EepromStop();
		RefreshWatchDog();
		EepromStart(true);
		HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_BYTE,EEPROM_COUNTER_INIT_DONE_1,TEST_VALUE_INIT_EEPROM);
		HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_BYTE,EEPROM_COUNTER_INIT_DONE_2,TEST_VALUE_INIT_EEPROM);
		RefreshWatchDog();
		EepromStop();

		return 1;
	}

	EepromStart(true);
	eeprom_write_8bit(COUNTER_STATE, TEST_VALUE_INIT_EEPROM);
	EepromStop();

	EepromStart(true);
	HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_BYTE,EEPROM_COUNTER_INIT_DONE_1,TEST_VALUE_INIT_EEPROM);
	HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_BYTE,EEPROM_COUNTER_INIT_DONE_2,TEST_VALUE_INIT_EEPROM);
	EepromStop();

	return 2;
}

/*
 * return: Returns the current counter for the selected position
 * position: 	0 = RX counter for device 1
 * 				1 = RX counter for device 2
 * 				2 = RX counter for device 3
 * 				3(TX_COUNTER) = TX counter
 */
uint32_t CounterMemoryReadAndIncrement(const uint8_t position)
{
	uint32_t temp1;
	uint32_t temp2;
	uint16_t pointer;
	uint32_t deposit1;
	uint32_t deposit2;
	uint32_t pointer_to_active_data1_block;
	uint32_t pointer_to_active_data2_block;

	if (position >= NUMBER_OF_COUNTERS) return 0;

	pointer = eeprom_read_16bit(addresCM[position][POINTER]);

	pointer_to_active_data1_block = addresCM[position][DATA_1] + (COUNTER_DATA_NUMBER_OF_BYTES*pointer);
	pointer_to_active_data2_block = addresCM[position][DATA_2] + (COUNTER_DATA_NUMBER_OF_BYTES*pointer);

	deposit1 = eeprom_read_32bit(pointer_to_active_data1_block);
	deposit2 = eeprom_read_32bit(pointer_to_active_data2_block);

	if (deposit1 < (((pointer + 1)*MAX_WRITE)-1)) // in one block max 250000 records
	{
		eeprom_write_32bit(pointer_to_active_data1_block, deposit1 + 1);
		eeprom_write_32bit(pointer_to_active_data2_block, deposit2 + 1);

		temp1 = eeprom_read_32bit(pointer_to_active_data1_block);
		temp2 = eeprom_read_32bit(pointer_to_active_data2_block);

		if ((temp1 != deposit1 + 1) || (temp2 != deposit2 + 1))
		{
#if REPEAT_COUNTER == 1
			temp1 = pointer + 1;
			if (temp1 >= COUNTER_MAX_POINTER)
			{
				eeprom_write_32bit(addresCM[position][DATA_1], 0);
				eeprom_write_32bit(addresCM[position][DATA_2], deposit2 + 1);
				eeprom_write_16bit(addresCM[position][POINTER], 0);

				temp1 = eeprom_read_8bit(COUNTER_STATE);
				eeprom_write_8bit(COUNTER_STATE, temp1+2);
			}
			else
			{
				eeprom_write_32bit(pointer_to_active_data1_block + COUNTER_DATA_NUMBER_OF_BYTES, (temp1 * MAX_WRITE));
				eeprom_write_32bit(pointer_to_active_data2_block + COUNTER_DATA_NUMBER_OF_BYTES, deposit2 + 1);
				eeprom_write_16bit(addresCM[position][POINTER], temp1);
			}
#else
			eeprom_write_32bit(pointer_to_active_data1_block + COUNTER_DATA_NUMBER_OF_BYTES, (temp1 * MAX_WRITE));
			eeprom_write_32bit(pointer_to_active_data2_block + COUNTER_DATA_NUMBER_OF_BYTES, deposit2 + 1);
			eeprom_write_16bit(addresCM[position][POINTER], pointer + 1);
#endif
		}
	}
	else
	{
		temp1 = pointer + 1;
#if REPEAT_COUNTER == 1
		if (temp1 >= COUNTER_MAX_POINTER)
		{
			eeprom_write_32bit(addresCM[position][DATA_1], 0);
			eeprom_write_32bit(addresCM[position][DATA_2], deposit2 + 1);
			eeprom_write_16bit(addresCM[position][POINTER], 0);

			temp1 = eeprom_read_8bit(COUNTER_STATE);
			eeprom_write_8bit(COUNTER_STATE, temp1+2);
		}
		else
		{
			eeprom_write_32bit(pointer_to_active_data1_block + COUNTER_DATA_NUMBER_OF_BYTES, (temp1 * MAX_WRITE));
			eeprom_write_32bit(pointer_to_active_data2_block + COUNTER_DATA_NUMBER_OF_BYTES, deposit2 + 1);
			eeprom_write_16bit(addresCM[position][POINTER], temp1);
		}
#else
		eeprom_write_32bit(pointer_to_active_data1_block + COUNTER_DATA_NUMBER_OF_BYTES, (temp1 * MAX_WRITE));
		eeprom_write_32bit(pointer_to_active_data2_block + COUNTER_DATA_NUMBER_OF_BYTES, deposit2 + 1);
		eeprom_write_16bit(addresCM[position][POINTER], temp1);
#endif
	}

	return deposit2;
}

/*
 *
 */
uint32_t CounterMemoryGetNumberOfRecords(const uint8_t position)
{
	if (position >= NUMBER_OF_COUNTERS) return 0;

	uint16_t pointer=0;

	pointer = eeprom_read_16bit(addresCM[position][POINTER]);
	uint32_t pointer_to_active_data1_block = addresCM[position][DATA_1] + (COUNTER_DATA_NUMBER_OF_BYTES*pointer);

	uint32_t records=0;
	records = eeprom_read_32bit(pointer_to_active_data1_block);

	return records;
}

/*
 * return: Returns the current counter for the selected position
 */
uint32_t CounterMemoryGetActualCounter(const uint8_t position)
{
	if (position >= NUMBER_OF_COUNTERS) return 0;

	uint16_t pointer=0;

	pointer = eeprom_read_16bit(addresCM[position][POINTER]);
	uint32_t pointer_to_active_data2_block = addresCM[position][DATA_2] + (COUNTER_DATA_NUMBER_OF_BYTES*pointer);

	uint32_t counter=0;
	counter = eeprom_read_32bit(pointer_to_active_data2_block);

	return counter;
}

/*
 * Sets the new counter number for the selected position.
 */
int CounterMemorySetActualCounter(const uint8_t position, uint32_t counter)
{
	uint32_t temp1;
	uint32_t temp2;
	uint16_t pointer;
	uint32_t deposit1;
	uint32_t pointer_to_active_data1_block;
	uint32_t pointer_to_active_data2_block;

	if (position >= NUMBER_OF_COUNTERS) return 0;

	pointer = eeprom_read_16bit(addresCM[position][POINTER]);

	pointer_to_active_data1_block = addresCM[position][DATA_1] + (COUNTER_DATA_NUMBER_OF_BYTES*pointer);
	pointer_to_active_data2_block = addresCM[position][DATA_2] + (COUNTER_DATA_NUMBER_OF_BYTES*pointer);

	deposit1 = eeprom_read_32bit(pointer_to_active_data1_block);

	if (deposit1 < (((pointer + 1)*MAX_WRITE)-1)) // in one block max 250000 records
	{
		eeprom_write_32bit(pointer_to_active_data1_block, deposit1 + 1);
		eeprom_write_32bit(pointer_to_active_data2_block, counter);

		temp1 = eeprom_read_32bit(pointer_to_active_data1_block);
		temp2 = eeprom_read_32bit(pointer_to_active_data2_block);

		if ((temp1 != deposit1 + 1) || (temp2 != counter))
		{
#if REPEAT_COUNTER == 1
			temp1 = pointer + 1;
			if (temp1 >= COUNTER_MAX_POINTER)
			{
				eeprom_write_32bit(addresCM[position][DATA_1], 0);
				eeprom_write_32bit(addresCM[position][DATA_2], counter);
				eeprom_write_16bit(addresCM[position][POINTER], 0);

				temp1 = eeprom_read_8bit(COUNTER_STATE);
				eeprom_write_8bit(COUNTER_STATE, temp1+2);
			}
			else
			{
				eeprom_write_32bit(pointer_to_active_data1_block + COUNTER_DATA_NUMBER_OF_BYTES, (temp1 * MAX_WRITE));
				eeprom_write_32bit(pointer_to_active_data2_block + COUNTER_DATA_NUMBER_OF_BYTES, counter);
				eeprom_write_16bit(addresCM[position][POINTER], temp1);
			}
#else
			eeprom_write_32bit(pointer_to_active_data1_block + COUNTER_DATA_NUMBER_OF_BYTES, (temp1 * MAX_WRITE));
			eeprom_write_32bit(pointer_to_active_data2_block + COUNTER_DATA_NUMBER_OF_BYTES, counter);
			eeprom_write_16bit(addresCM[position][POINTER], pointer + 1);
#endif
		}
	}
	else
	{
		temp1 = pointer + 1;
#if REPEAT_COUNTER == 1
		if (temp1 >= COUNTER_MAX_POINTER)
		{
			eeprom_write_32bit(addresCM[position][DATA_1], 0);
			eeprom_write_32bit(addresCM[position][DATA_2], counter);
			eeprom_write_16bit(addresCM[position][POINTER], 0);

			temp1 = eeprom_read_8bit(COUNTER_STATE);
			eeprom_write_8bit(COUNTER_STATE, temp1+2);
		}
		else
		{
			eeprom_write_32bit(pointer_to_active_data1_block + COUNTER_DATA_NUMBER_OF_BYTES, (temp1 * MAX_WRITE));
			eeprom_write_32bit(pointer_to_active_data2_block + COUNTER_DATA_NUMBER_OF_BYTES, counter);
			eeprom_write_16bit(addresCM[position][POINTER], temp1);
		}
#else
		eeprom_write_32bit(pointer_to_active_data1_block + COUNTER_DATA_NUMBER_OF_BYTES, (temp1 * MAX_WRITE));
		eeprom_write_32bit(pointer_to_active_data2_block + COUNTER_DATA_NUMBER_OF_BYTES, counter);
		eeprom_write_16bit(addresCM[position][POINTER], temp1);
#endif
	}
	return 1;
}

