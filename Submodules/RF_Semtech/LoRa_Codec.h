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





uint16_t LC_RadioCRC(uint16_t crcValue, uint8_t newByte);

#endif /* RF_SEMTECH_JUSTLORAPROCESS_H_ */
