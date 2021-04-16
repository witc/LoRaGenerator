/*
 * target.h
 *
 *  Created on: 18. 2. 2021
 *      Author: jan.ropek
 */

#ifndef INC_TARGET_H_
#define INC_TARGET_H_

#define BOARD_USB_RF_LINK				1
#define BOARD_LORA_GENERATOR			0


#define greenLEDPort				GPIOB
#define greenLEDPin					GPIO_PIN_5
#define redLEDPort					GPIOB
#define redLEDPin					GPIO_PIN_15

//Features


#define TARGET_BOARD_NAME			"LoRaUSBStick "
#define TARGET_BOARD_MCU			"STM32L071CB "
#define TARGET_BOARD_RADIO			"SX1262 "
#define TARGET_MAX_POWER			("22 ")
#define TARGET_MIN_POWER			("-9 ")


#endif /* INC_TARGET_H_ */
