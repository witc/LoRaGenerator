/*
 * ProcessRFTask.c
 *
 *  Created on: 24. 3. 2020
 *      Author: jan.ropek
 */

#include "main.h"
#include "TaskRF.h"
#include "ProcessRFTask.h"
#include "SignalProcessing.h"


extern SPI_HandleTypeDef hspi2;

/**
 *
 * @param atten
 */
void PRT_SetAtten1To(double atten)
{
	uint8_t temp;

	if(atten > 31.75)	atten = 31.75;
	else if(atten < 0)  atten = 0;

	temp = (uint8_t ) (atten*4);

	HAL_SPI_Transmit(&hspi2,&temp,1,10000);
	HAL_GPIO_WritePin(ATTEN_LE1_GPIO_Port,ATTEN_LE1_Pin,1);
	HAL_GPIO_WritePin(ATTEN_LE1_GPIO_Port,ATTEN_LE1_Pin,0);
}

/**
 *
 * @param atten
 */
void PRT_SetAtten2To(double atten)
{
	uint8_t temp;

	atten=SP_ConstrainDouble(atten,0,31.75);
	temp = (uint8_t ) (atten*4);

	HAL_SPI_Transmit(&hspi2,&temp,1,10000);
	HAL_GPIO_WritePin(ATTEN_LE2_GPIO_Port,ATTEN_LE2_Pin,1);
	HAL_GPIO_WritePin(ATTEN_LE2_GPIO_Port,ATTEN_LE2_Pin,0);
}

/**
 *
 * @param power
 */
void PRT_SetOutputPower(double power)
{
	SP_ConstrainDouble(power,-17,22);
}

/**
 * @param out_power
 * @param atten1
 * @param atten2
 */
void PRT_PowerDistribution(double wantedPowerdBm, int8_t *outPowerdB, double *atten1dB, double *atten2dB)
{
	double tempPower;
	double tempResiduePower=0;
	double tempAtten;
	double tempAttenResid=0;

	wantedPowerdBm+=6;	//6 = utlum na cestach

	tempPower = SP_ConstrainDouble(wantedPowerdBm,-80,22);

	tempResiduePower = fmod(tempPower,1);
	//tempResiduePower = (double)(tempPower%1);

	if(tempPower >= (-9) )
	{
		if(tempResiduePower == 0)
		{
			*atten1dB = *atten2dB = 0;
		}
		else
		{
			*atten1dB = (1-tempResiduePower);
			if(tempPower>=0) tempPower +=1;
		}

		*outPowerdB = (uint8_t)tempPower;
		*atten2dB = 0;
	}
	else
	{	//napr (-18,75)
		*outPowerdB = (-9);
		tempAtten = -(tempPower+9); // zbyde pouze utlum napr (46,75)

		tempAtten*=2;	// napr 93,5
		tempAttenResid=fmod(tempAtten,1);
		*atten1dB =(uint8_t)tempAtten;	// napr 93

		if(tempAttenResid)	*atten2dB =(*atten1dB)+1;
		else				*atten2dB =(*atten1dB);

		/* na dB */
		*atten1dB/=4;
		*atten2dB/=4;
	}


}

