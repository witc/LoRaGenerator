/*
 * RadioConfig.h
 *
 *  Created on: 7. 2. 2020
 *      Author: jan.ropek
 */

#ifndef SX1262_RADIOCONFIG_H_
#define SX1262_RADIOCONFIG_H_

/* Set definition according to your system*/
#define I_AM_RF_END_DEVICE				1
#define I_AM_RF_GATEWAY					0
#define RF_USE_DMA						0


#if (I_AM_RF_END_DEVICE==I_AM_RF_GATEWAY)
	#error ("definuj typ systemu")
#endif


#if (I_AM_RF_END_DEVICE==1)
	#define	I_AM_NODE_MONITOR				1
	#define	I_AM_NODE_ENERGY				0
#if (I_AM_NODE_MONITOR==I_AM_NODE_ENERGY)
	#error("definuj typ koncoveho zarizeni")
#endif

#endif

#endif /* SX1262_RADIOCONFIG_H_ */
