/*
 * SignalProcessing.c
 *
 *  Created on: 30. 3. 2020
 *      Author: jan.ropek
 */


#include "main.h"
#include "SignalProcessing.h"

/*impulse  kV*/

/* ADC Batt*/
#define CONST_MV_TO_PACKET_MULTIPLE		(254/5)


/**
 *
 * @param value
 * @param min
 * @param max
 * @return
 */
double	SP_ConstrainDouble(double value, double min, double max)
{
	if(value>max)		return max;
	else if(value<min)	return min;

	return value;
}

/**
 *
 * @param value
 * @param min
 * @param max
 * @return
 */
uint8_t	SP_ConstrainU8(uint8_t value, uint8_t min, uint8_t max)
{
	if(value>max)		return max;
	else if(value<min)	return min;

	return value;
}

/**
 *
 * @param value
 * @param min
 * @param max
 * @return
 */
uint32_t	SP_ConstrainU32(uint32_t value, uint32_t min, uint32_t max)
{
	if(value>max)		return max;
	else if(value<min)	return min;

	return value;
}


