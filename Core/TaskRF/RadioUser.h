/*
 * RadioUser.h
 *
 *  Created on: 12. 3. 2020
 *      Author: jan.ropek
 */

#ifndef TASKRF_RADIOUSER_H_
#define TASKRF_RADIOUSER_H_

#define BOARD_USB_RF_LINK

#ifdef BOARD_LORA_GENERATOR
	#define SX1262_BUSY_GPIO_Port		RF_BUSY_GPIO_Port
	#define SX1262_BUSY_Pin				RF_BUSY_Pin
	#define SX1262_NRESET_GPIO_Port		RF_NRESET_GPIO_Port
	#define	SX1262_NRESET_Pin			RF_NRESET_Pin
	#define SX1262_DIO_GPIO_Port		RF_DIO_GPIO_Port
	#define	SX1262_DIO_Pin				RF_DIO_Pin
	#define SX1262_NSS_GPIO_Port		RF_NSS_GPIO_Port
	#define SX1262_NSS_Pin				RF_NSS_Pin
	#define SX1262_TX_ENABLE_GPIO_Port	RF_TX_ENABLE_GPIO_Port
	#define SX1262_TX_ENABLE_Pin		RF_TX_ENABLE_Pin

#elif defined(BOARD_USB_RF_LINK)
	#define SX1262_BUSY_GPIO_Port		GPIOB
	#define SX1262_BUSY_Pin				GPIO_PIN_10
	#define SX1262_NRESET_GPIO_Port		GPIOB
	#define	SX1262_NRESET_Pin			GPIO_PIN_1
	#define SX1262_DIO_GPIO_Port		GPIOB
	#define	SX1262_DIO_Pin				GPIO_PIN_2
	#define SX1262_NSS_GPIO_Port		GPIOA
	#define SX1262_NSS_Pin				GPIO_PIN_4
	#define SX1262_TX_ENABLE_GPIO_Port	GPIOA
	#define SX1262_TX_ENABLE_Pin		GPIO_PIN_8

#endif

#endif /* TASKRF_RADIOUSER_H_ */
