/*
 * ProcessRFTask.h
 *
 *  Created on: 24. 3. 2020
 *      Author: jan.ropek
 */

#ifndef TASKRF_PROCESSRFTASK_H_
#define TASKRF_PROCESSRFTASK_H_

void		PRT_MeasureInternalBatt						(void);
void 		Callback_DMA1_Channel1_IRQHandler			( DMA_HandleTypeDef * _hdma);
void 		PRT_DummyADCMeasure							(void);
void 		PRT_RFRefreschWatchdog						(void);
void 		PRT_SetAtten1To								(double atten);
void 		PRT_SetAtten2To								(double atten);
void 		PRT_PowerDistribution						(double wantedPowerdBm, int8_t *outPowerdB, double *atten1dB, double *atten2dB);
#endif /* TASKRF_PROCESSRFTASK_H_ */
