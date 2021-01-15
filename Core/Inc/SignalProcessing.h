/*
 * SignalProcessing.h
 *
 *  Created on: 30. 3. 2020
 *      Author: jan.ropek
 */

#ifndef INC_SIGNALPROCESSING_H_
#define INC_SIGNALPROCESSING_H_

#include "TaskCore.h"

#define CONST_MV_TO_KV_MULTIPLE		62687//60764
#define CONST_MV_TO_KV_OFFSET		(-11573)//(-10009)

double		SP_ConstrainDouble						(double value, double min, double max);
uint8_t		SP_ConstrainU8							(uint8_t value, uint8_t min, uint8_t max);
uint32_t	SP_ConstrainU32							(uint32_t value, uint32_t min, uint32_t max);
uint8_t		SP_ConvertmVToKv						(uint16_t adcmV);
uint8_t 	SP_ConvertBattmVToPacketRange			(uint16_t adcmV);
uint8_t 	SP_ConvertBattPacketToPercent			(uint16_t adcmV);
uint16_t 	SP_ConvertBattToTruemV					(uint16_t value);
void		SP_SMA_ImpulseFilter					(uint16_t adc, tCoreGlobalData *GlobalData);
void 		SP_SaveImpulsCoefA						(int32_t coefA);
void 		SP_SaveImpulsIntercept					(int32_t intercept);
void 		SP_PulseGetCoefAIntercept				(int32_t * coefA, int32_t *intercept);

#endif /* INC_SIGNALPROCESSING_H_ */
