/*
 * JustLoRa.c
 *
 *  Created on: 25. 4. 2018
 *      Author: ropek
 *
 *      __			  __    __       _	   _ _ _ _ _ _ _
 *      \ \			 / /   |  \     | |   |_ _ _ _ _ _ _|
 *       \ \    	/ /    | \ \    | |         | |
 *		  \	\  	   / /     | |\ \   | |         | |
 *         \ \    / /	   | | \ \  | |         | |
 *          \ \  / /	   | |  \ \ | |         | |
 *           \ \/ /		   | |   \ \| |         | |
 *            \__/         |_|    \___|         |_|			s.r.o
 *
 *        	  2018
 *
 */
#include "RadioConfig.h"
//#include "radio_general.h"
//#include "main.h"


/**
 *brief: Calculate  crc - 16/MODBUS - init 0xffff
 */
uint16_t LC_RadioCRC(uint16_t crcValue, uint8_t newByte)
{
	int i;

	crcValue ^= (uint16_t)newByte;
	for (i = 0; i < 8; ++i) {
		if (crcValue & 1)
			crcValue = (crcValue >> 1) ^ 0xA001;
		else
			crcValue = (crcValue >> 1);
	}

	return crcValue;
}

