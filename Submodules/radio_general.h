/*
 * spi_module.h
 *
 *  Created on: 7. 10. 2019
 *      Author: jan.ropek
 */

#ifndef __RADIO_GENERAL_H_
#define __RADIO_GENERAL_H_

#include "main.h"
#include "LoRa_Codec.h"

/*
 *
 */
typedef enum
{
	SWITCH_RX=0,
	SWITCH_TX=!SWITCH_RX

}eRF_switch;


/*
 *
 */
typedef struct
{
	uint32_t 		pin;
	GPIO_TypeDef	*port;

}tRadioPins;

typedef struct {

	tRadioPins 			pin_NSS;				/* SPI Select pin */
	tRadioPins			pin_BUSY;				/* BUSY pin */
	tRadioPins			pin_RESET;				/* Reset pin */
	tRadioPins			pin_DIO1;				/* DIO1 - IRQ 1 pin */
	tRadioPins			pin_RF_SWITCH;			/* RF switch */
	SPI_HandleTypeDef	*target;				/* SPI target */
#if (RF_USE_DMA==1)
	osSemaphoreId		RFBinarySPISemaphore;
	uint8_t				Gl_RF_RX_SPI_DMA_Buffer[100];
	uint8_t				Gl_RF_TX_SPI_DMA_Buffer[100];
	void (*RF_TX_DMATransferComplete(void));
	void (*RF_RX_DMATransferComplete(void));
#endif
	void (*AtomicActionEnter)(void);				/* pointer na funkci - vstup do Atomicke oblasti  */
	void (*AtomicActionExit)(void);					/* pointer na funkci - vystup do Atomicke oblasti */
//	void (*RadioRFSwitch) (Enum_RF_switch state);

}tRadioConfiguration;


void	RG_SX126xWaitOnBusy			(void);
void	RG_SX126xWakeup				(void);
void	RG_SX126xWriteCommand		(uint8_t command, uint8_t *buffer, uint16_t size);
void	RG_SX126xReadCommand		(uint8_t command, uint8_t *buffer, uint16_t size);
void	RG_SX126xWriteRegisters		(uint16_t address, uint8_t *buffer, uint16_t size);
void	RG_SX126xWriteRegister		(uint16_t address,  uint8_t value);
void	RG_SX126xReadRegisters		(uint16_t address, uint8_t *buffer, uint16_t size);
uint8_t	RG_SX126xReadRegister		(uint16_t address);
void	RG_SX126xWriteBuffer		(uint8_t offset, uint8_t *buffer, uint8_t size);
void	RG_SX126xReadBuffer			(uint8_t offset, uint8_t *buffer, uint8_t size);
uint8_t RG_SX126xGetPaSelect		(uint32_t channel);
void 	RG_SX126xReset				(void);
void 	RG_SX1262WriteSpi			(uint8_t *data,uint8_t size);
void 	RG_SX1262ReadSpi			(uint8_t *buffer,uint8_t size);
void 	RG_SX1262WriteSpi			(uint8_t *data,uint8_t size);
void 	RG_RFSwitch					(eRF_switch state);

extern tRadioConfiguration 			spiDevice;

#endif /* SRC_SUBMODULES_SPI_GENERAL_H_ */
