/*
 * TemplateRadioUser.h
 *
 *  Created on: 7. 2. 2020
 *      Author: jan.ropek
 */

#ifndef SX1262_TEMPLATERADIOUSER_H_
#define SX1262_TEMPLATERADIOUSER_H_

#include "main.h"
#include "TaskRF.h"
#include "radio.h"
#include "sx126x.h"
#include "RadioCommands.h"

/* Calibrate frequency of the your Radio*/
#define RF_MAX_ENEBALE_FREQ		869525000+500000
#define RF_MIN_ENEBALE_FREQ		869525000-500000

extern RadioPar GlDefaultDrTX;
extern RadioPar GlDefaultDrRX;
extern RadioPar USB_RfLink;
/*
 * @ Commands for radio
 */
typedef enum
{
	RF_CMD_NONE=0,
	RF_CMD_INIT_ON,
	RF_CMD_INIT_OFF,
	RF_INIT_START_RX,
	RF_INIT_START_TX,
	RF_CMD_PAUSE_RX,
	RF_CMD_ENABLE_RX,
	RF_CMD_SEND_UNIVERSAL_PAYLOAD,
	RF_CMD_SEND_UNIVERSAL_PAYLOAD_NOW,
	RF_CMD_SEND_KEY_TO_GW,
	RF_CMD_HEARTBEAT_STOPPED,
	RF_CMD_START_PAIRING_PROCESS,
	RF_CMD_START_RX,
	RF_CMD_TX_CW,
	RF_CMD_GIVE_ME_RN,	//return random number
	RF_CMD_LBT_TIMEOUT,
	RF_CMD_STOP_TX_AND_DISCARD,
	RF_CMD_STANDBY,
	RF_CMD_SLEEP

}RfCommands;


/* Functions */
bool		RU_SX1262Assign				(void);
void		RU_GetTrueRandom32bit		(uint32_t RandomData[], uint8_t CountOfRandomData);
void		RU_SX1262Init				(void);
void		RU_RadioInit				(void);
void		RU_RadioDeinit				(void);
uint8_t 	RU_IRQProcess				(tRfGlobalData* GlobalData);
void 		RU_CommandProcess			(RfCommands cmd,tRfGlobalData* GlobalData, DATA_QUEUE *ReceiveData);
void 		RU_RadioStandby				(void);
void 		RU_RadioSleep				(void);
void 		RU_LoRaConfigAndStartRX 	(uint32_t freq,RadioPar DR, bool Rx, uint32_t rxTimeout);
void 		RU_RFSetTXUp				(int8_t power, uint32_t freq, RadioPar DR);
void 		RU_RFSetTXDown				(void);
void 		RU_GetRfFrequency			(uint32_t *frequency);
void 		RU_SetRfFrequency			(uint32_t frequency);
void 		RU_UpdatetMyAESKey			(void);
void 		RU_UpdatetMyTxEUI			(void);
uint64_t	RU_GetMyMasterMac			(void);
RadioPar 	RU_GetDrRX					(void);
RadioPar 	RU_GetDrTX					(void);
void		RU_SetDrTX					(RadioPar newDrTx);
void		RU_SetDrRX					(RadioPar newDrRx);
void 		LoRaScanCAD					(RadioLoRaCadSymbols_t symbols);
bool 		RU_RunLBTProcedure			(RadioPar DrData);


#endif /* SX1262_TEMPLATERADIOUSER_H_ */
