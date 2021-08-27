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

/*
 *
 */
typedef enum
{
	DR_IQ_FALSE=0,
	DR_IQ_TRUE=1,
	DR_IQ_SIZE,
}eDR_IQ;


/**
 *
 */
typedef enum
{
	DR_HEADER_FALSE=0,//!< DR_HEADER_FALSE
	DR_HEADER_TRUE=1, //!< DR_HEADER_TRUE
	DR_HEADER_SIZE,   //!< DR_HEADER_SIZE
}eDR_HEADER;

/**
 *
 */
typedef enum
{
	DR_CRC_FALSE=0,
	DR_CRC_TRUE=1,
	DR_CRC_SIZE,
}eDR_CRC;



/*
 *
 */
typedef enum
{
	DR_BW_7_81_KHZ=0,
	DR_BW_10_42_KHZ=1,
	DR_BW_15_63_KHZ=2,
	DR_BW_20_83_KHZ=3,
	DR_BW_31_25_KHZ=4,
	DR_BW_41_67_KHZ=5,
	DR_BW_62_5_KHZ=6,
	DR_BW_125_KHZ=7,
	DR_BW_250_KHZ=8,
	DR_BW_500_KHZ=9,
	DR_BW_SIZE,

}eDR_BW;

/*
 *
 */
typedef enum
{
	DR_SF5=0,
	DR_SF6=1,
	DR_SF7=2,
	DR_SF8=3,
	DR_SF9=4,
	DR_SF10=5,
	DR_SF11=6,
	DR_SF12=7,
	DR_SF_SIZE,

}eDR_SF;

/**
 *
 */
typedef enum
{
	DR_CR_45=0,//!< DR_CR_45
	DR_CR_46=1,//!< DR_CR_46
	DR_CR_47=2,//!< DR_CR_47
	DR_CR_48=3,//!< DR_CR_48
	DR_CR_SIZE,

}eDR_CR;



typedef union
{
	uint8_t  			DataArray[PACKET_MAX_SIZE];

}tGeneralPacket;


#endif /* RF_SEMTECH_JUSTLORAPROCESS_H_ */
