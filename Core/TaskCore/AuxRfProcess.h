/*
 * AuxRfProcess.h
 *
 *  Created on: 19. 3. 2020
 *      Author: jan.ropek
 */

#ifndef TASKCORE_AUXRFPROCESS_H_
#define TASKCORE_AUXRFPROCESS_H_

void 	ARP_PairTreatment		(DATA_QUEUE ReceiveData,tCoreGlobalData* GlobalData,tStateCoreAutomat* StateAutomat);
void 	ARP_RfStandardTreatment	(DATA_QUEUE ReceiveData,tCoreGlobalData* GlobalData);
void 	ARP_PairStart			(tCoreGlobalData *GlobalData);
void 	ARP_RfTestFactoryRSSI	(DATA_QUEUE ReceiveData,tCoreGlobalData* GlobalData);
void 	ARP_PairStop			(tCoreGlobalData *GlobalData,bool pairsucceeded);
void 	ARP_SendMonitorInfo		(tCoreGlobalData *GlobalData,GeneralPacketsUpOrDown_t *pBuffer,uint8_t size, bool sendNow);
void 	ARP_USBSendHereIam		(tCoreGlobalData *GlobalData);
void 	ARP_USBSendCoefSaved	(tCoreGlobalData *GlobalData);
void 	ARP_USBSendPulsemV		(tCoreGlobalData *GlobalData, uint16_t pulsemV);
bool 	ARP_USBLinkCheckPair	(DATA_QUEUE ReceiveData,tCoreGlobalData* GlobalData);
void 	ARP_SendMyName			(tCoreGlobalData *GlobalData);
void 	ARP_SendDummyPacket		(void);
void 	ARP_SetCorrespondingDr	(void);
void 	ARP_SetRxCounter		(uint32_t counter);
uint32_t ARP_ReadRxCounter		(void);
uint8_t ARP_CreateMonitorInfo	(tCoreGlobalData *GlobalData,uint8_t kVoltage,uint8_t battVoltage, bool checkAlarm, GeneralPacketsUpOrDown_t  *TxPacket);
#endif /* TASKCORE_AUXRFPROCESS_H_ */
