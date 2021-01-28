/*
 * RadioCommands.h
 *
 *  Created on: 28. 1. 2021
 *      Author: jan.ropek
 */

#ifndef SRC_RADIOCOMMANDS_H_
#define SRC_RADIOCOMMANDS_H_

#define MAX_ALLOWEDPACKET_SIZE			100

bool RC_RadioSetTxFreq(uint32_t freq);
bool RC_RadioSetRxFreq(uint32_t freq);
bool RC_RadioSetTxPower(int8_t power);
bool RC_RadioSetStandby();
bool RC__RadioSetTxCW();
bool RC_SavePacket(uint8_t *data);


#endif /* SRC_RADIOCOMMANDS_H_ */
