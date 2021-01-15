/*
 * LoadConfig.h
 *
 *  Created on: 25. 5. 2020
 *      Author: jan.ropek
 */

#ifndef SRC_LOADCONFIG_H_
#define SRC_LOADCONFIG_H_

typedef struct
{
	uint32_t	TimeLedOn;
	uint32_t	TimeShowAlive;
	uint32_t	TxPeriod;
	uint32_t	DurationRxWindow;
	uint32_t	TimeToEnTrigger;
	uint32_t	TimeoutPair;
	uint32_t	TimeoutRfTx;
	uint32_t  	TimeoutRepeatePacket;
	uint32_t	TimeoutEnSendNextPacket;
	uint32_t	TimeoutTrySendNextPacket;
}__packed tGLobalConfig;



extern tGLobalConfig GlMonitorProfile;

void 			CL_SetTimeLedOn				(uint32_t time);
uint32_t		CL_GetTimeLedOn				(void);
void 			CL_SetBlinkPeriod			(uint32_t time);
uint32_t	 	CL_GetBlinkPeriod			(void);
void 			CL_SetTimeShowAlive			(uint32_t time);
uint32_t 		CL_GetTimeShowAlive			(void);
void 			CL_SetTimeHeartBeatPulse	(uint32_t time);
uint32_t 		CL_GetTimeHeartBeatPulse	(void);
void 			CL_SetTimeTXPeriod			(uint32_t time);
uint32_t 		CL_GetTimeTXPeriod			(void);
void 			CL_SetRxWindow				(uint32_t time);
uint32_t 		CL_GetTimeRxWindow			(void);
void 			CL_SetTimeToEnTrigger		(uint32_t time);
uint32_t 		CL_GetTimeToEnTrigger		(void);
void 			CL_SetTimeoutPair			(uint32_t time);
uint32_t 		CL_GetTimeoutPair			(void);
void 			CL_SetTimeoutRfTx			(uint32_t time);
void 			CL_SetTimeoutRfTx			(uint32_t time);
void 			Cl_SetTimeoutRepeatePacket	(uint32_t time);
void 			Cl_SetTimeoutEnSendNextPacket(uint32_t time);
void 			Cl_SetTimeoutTrySendNextPacket(uint32_t time);

#endif /* SRC_LOADCONFIG_H_ */
