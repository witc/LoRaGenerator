/*
 * LoRa_Codec.h
 *
 *  Created on: 25. 4. 2018
 *      Author: ropek
 */

#ifndef __LORA_CODEC_H_
#define __LORA_CODEC_H_

#include "RadioConfig.h"
#include "radio_general.h"
#include "AES_SW.h"

#define PACKET_MAX_SIZE							250

/*
 *
 */
typedef enum
{
	DR_IQ_FALSE=0,
	DR_IQ_TRUE=1
}ENUM_DR_IQ;



/*
 *
 */
typedef enum
{
	DR_BW_62_5_KHZ=0,
	DR_BW_125_KHZ=1,
	DR_BW_250_KHZ=2,
	DR_BW_500_KHZ=3,

}ENUM_DR_BW;

/*
 *
 */
typedef enum
{
	DR_SF6=0,
	DR_SF7=1,
	DR_SF8=2,
	DR_SF9=3,
	DR_SF10=4,
	DR_SF11=5,
	DR_SF12=6,

}ENUM_DR_SF;


typedef union
{
	uint8_t Value;

	struct sDrBits{

	ENUM_DR_SF SF: 3;

	ENUM_DR_BW BW:2;

	ENUM_DR_IQ IqInvert:1;

	}Bits;

}DrConfig_t;

typedef union
{
	uint8_t  						DataArray[PACKET_MAX_SIZE];

}GeneralPacketsUpOrDown_t;



uint16_t crc16(uint16_t crcValue, uint8_t newByte);

#endif /* RF_SEMTECH_JUSTLORAPROCESS_H_ */
