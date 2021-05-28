/*
 * RadioConfig.h
 *
 *  Created on: 7. 2. 2020
 *      Author: jan.ropek
 */

#ifndef SX1262_RADIOCONFIG_H_
#define SX1262_RADIOCONFIG_H_

/* Set definition according to your system*/
#define radioEND_DEVICE							(1)
#define radioGATEWAY							(2)


#define radioDEV_TYPE							radioGATEWAY
#define radioUSE_SPI_BY_DMA						0

#define radioPACKET_MAX_SIZE					250

#if ((radioDEV_TYPE != radioEND_DEVICE) && (radioDEV_TYPE != radioGATEWAY))
	#error ("definuj typ systemu")
#endif



#endif /* SX1262_RADIOCONFIG_H_ */
