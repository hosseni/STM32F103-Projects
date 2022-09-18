/*
 * TFT_configuration.h
 *
 *  Created on: Sep 14, 2022
 *      Author: ELKHOLY
 */

#ifndef HAL_TFT_TFT_CONFIGURATION_H_
#define HAL_TFT_TFT_CONFIGURATION_H_

#include "../Inc/MCAL/GPIO/GPIO_interface.h"

#define TFT_A0_PIN  GPIO_U8_PORTA,GPIO_U8_PIN2
#define TFT_RST_PIN GPIO_U8_PORTA,GPIO_U8_PIN1



/* Configure TFT size */
#define    TFT_MAX_X       127
#define    TFT_MAX_Y       159


#endif /* HAL_TFT_TFT_CONFIGURATION_H_ */
