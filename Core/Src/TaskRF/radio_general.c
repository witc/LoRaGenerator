/*
 * spi_general.c
 *
 *  Created on: 7. 10. 2019
 *      Author: jan.ropek
 */

#include "radio_general.h"
#include "main.h"
#include "sx126x.h"

/*
 *
 */
void RG_SX126xReset( void  )
{
    osDelay( 10 );
    HAL_GPIO_WritePin(spiDevice.pin_RESET.port,spiDevice.pin_RESET.pin,0);
    osDelay( 1 );
    HAL_GPIO_WritePin(spiDevice.pin_RESET.port,spiDevice.pin_RESET.pin,1);
    osDelay( 10 );
}


/**
 *
 */
void RG_SX126xIoIrqInit( DioIrqHandler dioIrq )
{

}


/**
 *
 */
void RG_SX1262IrqEnable(void)
{

}

/**
 *
 */
void RG_SX126xWaitOnBusy(void )
{
    while(HAL_GPIO_ReadPin(spiDevice.pin_BUSY.port,spiDevice.pin_BUSY.pin) == 1 );
}

/**
 *
 */

void RG_SX126xWakeup(void )
{
	uint8_t temp=RADIO_GET_STATUS;

//	spiDevice.AtomicActionEnter();
	HAL_GPIO_WritePin(spiDevice.pin_NSS.port,spiDevice.pin_NSS.pin,0);
	osDelay(1);
    RG_SX1262WriteSpi(&temp,1);
    temp=0;
    RG_SX1262WriteSpi(&temp,1);

    HAL_GPIO_WritePin(spiDevice.pin_NSS.port,spiDevice.pin_NSS.pin,1);

//    spiDevice.AtomicActionExit();

    // Wait for chip to be ready.
    RG_SX126xWaitOnBusy();

}

/**
 *
 */
void RG_SX126xWriteCommand( uint8_t command, uint8_t *buffer, uint16_t size )
{
    SX126xCheckDeviceReady();

//    spiDevice.AtomicActionEnter();
    HAL_GPIO_WritePin(spiDevice.pin_NSS.port,spiDevice.pin_NSS.pin,0);

    RG_SX1262WriteSpi((uint8_t*)&command,1);
    RG_SX1262WriteSpi(buffer,size);

    HAL_GPIO_WritePin(spiDevice.pin_NSS.port,spiDevice.pin_NSS.pin,1);
//    spiDevice.AtomicActionExit();

    if( command != RADIO_SET_SLEEP )
    {
        RG_SX126xWaitOnBusy();
    }
}

/**
 *
 */

void RG_SX126xReadCommand( uint8_t command, uint8_t *buffer, uint16_t size  )
{
	uint8_t temp;
    SX126xCheckDeviceReady();

//    spiDevice.AtomicActionEnter();
    HAL_GPIO_WritePin(spiDevice.pin_NSS.port,spiDevice.pin_NSS.pin,0);

    RG_SX1262WriteSpi(&command,1);

    temp=0;
    RG_SX1262WriteSpi(&temp,1);
    RG_SX1262ReadSpi(buffer,size);

    HAL_GPIO_WritePin(spiDevice.pin_NSS.port,spiDevice.pin_NSS.pin,1);
//    spiDevice.AtomicActionExit();

    RG_SX126xWaitOnBusy();
}


/**
 *
 */
void RG_SX126xWriteRegisters( uint16_t address, uint8_t *buffer, uint16_t size)
{
	uint8_t temp=RADIO_WRITE_REGISTER;

	SX126xCheckDeviceReady();

	//spiDevice.AtomicActionEnter();
	HAL_GPIO_WritePin(spiDevice.pin_NSS.port,spiDevice.pin_NSS.pin,0);

	RG_SX1262WriteSpi(&temp,1);
	temp=(( address & 0xFF00 )>>8);
	RG_SX1262WriteSpi(&temp,1);
	temp=( address & 0x00FF );
	RG_SX1262WriteSpi(&temp,1);

	RG_SX1262WriteSpi(buffer,size);

	HAL_GPIO_WritePin(spiDevice.pin_NSS.port,spiDevice.pin_NSS.pin,1);
	//spiDevice.AtomicActionExit();

	RG_SX126xWaitOnBusy();

}

/**
 *
 */
void RG_SX126xWriteRegister( uint16_t address,  uint8_t value)
{
    RG_SX126xWriteRegisters( address, &value, 1);
}

/**
 *
 */
void RG_SX126xReadRegisters( uint16_t address, uint8_t *buffer, uint16_t size )
{
	uint8_t temp=RADIO_READ_REGISTER;
    SX126xCheckDeviceReady();

    //spiDevice.AtomicActionEnter();
	HAL_GPIO_WritePin(spiDevice.pin_NSS.port,spiDevice.pin_NSS.pin,0);

    RG_SX1262WriteSpi(&temp,1);
    temp=(( address & 0xFF00 )>>8);
    RG_SX1262WriteSpi(&temp,1);
    temp=( address & 0x00FF );
    RG_SX1262WriteSpi(&temp,1);

    temp=0;
    RG_SX1262WriteSpi(&temp,1);

    RG_SX1262ReadSpi(buffer,size);

	HAL_GPIO_WritePin(spiDevice.pin_NSS.port,spiDevice.pin_NSS.pin,1);
	//spiDevice.AtomicActionExit();

    RG_SX126xWaitOnBusy();
}

/**
 *
 */
uint8_t RG_SX126xReadRegister( uint16_t address )
{
    uint8_t data;
    RG_SX126xReadRegisters( address, &data, 1);
    return data;
}

/**
 *
 */

void RG_SX126xWriteBuffer( uint8_t offset, uint8_t *buffer, uint8_t size )
{
	uint8_t temp=RADIO_WRITE_BUFFER;

	SX126xCheckDeviceReady();

	//spiDevice.AtomicActionEnter();
	HAL_GPIO_WritePin(spiDevice.pin_NSS.port,spiDevice.pin_NSS.pin,0);

	RG_SX1262WriteSpi(&temp,1);
	RG_SX1262WriteSpi(&offset,1);
	RG_SX1262WriteSpi(buffer,size);

	HAL_GPIO_WritePin(spiDevice.pin_NSS.port,spiDevice.pin_NSS.pin,1);
	//spiDevice.AtomicActionExit();

	RG_SX126xWaitOnBusy();
}

/**
 *
 */

void RG_SX126xReadBuffer( uint8_t offset, uint8_t *buffer, uint8_t size )
{
	uint8_t temp=RADIO_READ_BUFFER;

	SX126xCheckDeviceReady();

	//spiDevice.AtomicActionEnter();
	HAL_GPIO_WritePin(spiDevice.pin_NSS.port,spiDevice.pin_NSS.pin,0);

	RG_SX1262WriteSpi(&temp,1);
	RG_SX1262WriteSpi(&offset,1);
	temp=0;
	RG_SX1262WriteSpi(&temp,1);
	RG_SX1262ReadSpi(buffer,size);

	HAL_GPIO_WritePin(spiDevice.pin_NSS.port,spiDevice.pin_NSS.pin,1);
	//spiDevice.AtomicActionExit();

	RG_SX126xWaitOnBusy();

}


uint8_t RG_SX126xGetPaSelect( uint32_t channel)
{
	return SX1262;
}

void RG_RFSwitch(eRF_switch state)
{

	switch (state)
	{
		case SWITCH_RX:
			HAL_GPIO_WritePin(spiDevice.pin_RF_SWITCH.port,spiDevice.pin_RF_SWITCH.pin,1);
			break;

		case SWITCH_TX:
			HAL_GPIO_WritePin(spiDevice.pin_RF_SWITCH.port,spiDevice.pin_RF_SWITCH.pin,0);

		default:
			break;
	}
}

/**
 *
 */
void RG_SX1262WriteSpi(uint8_t *data,uint8_t size)
{
#if (RF_USE_DMA==1)
	/*
	spiDevice2.Gl_RF_TX_SPI_DMA_Buffer=data;
	LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_7);
	LL_DMA_SetMemoryAddress(DMA1,LL_DMA_CHANNEL_7,(uint32_t) &Gl_SPI_TX_DMA_Buffer[0]);
	LL_DMA_SetDataLength(DMA1,LL_DMA_CHANNEL_7,size + 2);
	LL_GPIO_ResetOutputPin(SX1262_NSS_GPIO_Port,SX1262_NSS_Pin);

	LL_DMA_ClearFlag_TC7(DMA1);
	LL_DMA_EnableIT_TC(DMA1,LL_DMA_CHANNEL_7);
	LL_DMA_EnableChannel(DMA1,LL_DMA_CHANNEL_7);

	osSemaphoreWait(BinarySemSPI_RFHandle,osWaitForever);
	*/

#else
	HAL_SPI_Transmit(spiDevice.target,data,size,0xffff);
#endif
}

/**
 *
 */
void RG_SX1262ReadSpi(uint8_t *buffer,uint8_t size)
{
#if (RF_USE_DMA==1)
	Gl_SPI_TX_DMA_Buffer[0] = RADIO_READ_BUFFER;
	Gl_SPI_TX_DMA_Buffer[1] = offset;
	memset(&Gl_SPI_TX_DMA_Buffer[2], 0, size + 1);

	//Set DMA channel - SPI TX
	LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_7);
	LL_DMA_SetMemoryAddress(DMA1,LL_DMA_CHANNEL_7,(uint32_t) &Gl_SPI_TX_DMA_Buffer[0]);
	LL_DMA_SetDataLength(DMA1,LL_DMA_CHANNEL_7,size + 3);

	//Set DMA channel - SPI RX
	LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_6);
	LL_DMA_SetMemoryAddress(DMA1,LL_DMA_CHANNEL_6,(uint32_t) &Gl_SPI_RX_DMA_Buffer[0]);
	LL_DMA_SetDataLength(DMA1,LL_DMA_CHANNEL_6,size + 3);

	LL_SPI_ReceiveData8(SPI2);

	LL_GPIO_ResetOutputPin(SX1262_NSS_GPIO_Port,SX1262_NSS_Pin);
	LL_DMA_ClearFlag_TC7(DMA1);
	LL_DMA_ClearFlag_TC6(DMA1);
	LL_DMA_EnableIT_TC(DMA1,LL_DMA_CHANNEL_6);

	taskENTER_CRITICAL();
	LL_DMA_EnableChannel(DMA1,LL_DMA_CHANNEL_6);
	LL_DMA_EnableChannel(DMA1,LL_DMA_CHANNEL_7);
	taskEXIT_CRITICAL();

	osSemaphoreWait(BinarySemSPI_RFHandle,osWaitForever);
	LL_GPIO_SetOutputPin(SX1262_NSS_GPIO_Port,SX1262_NSS_Pin);

#else
	HAL_SPI_Receive(spiDevice.target,buffer,size,0xffff);
#endif
}
