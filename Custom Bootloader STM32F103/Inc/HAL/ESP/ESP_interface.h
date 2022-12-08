/*
 * ESP_interface.h
 *
 *  Created on: Sep 5, 2022
 *      Author: ELKHOLY
 */

#ifndef HAL_ESP_ESP_INTERFACE_H_
#define HAL_ESP_ESP_INTERFACE_H_

#include "../inc/LIB/BIT_MATH.h"
#include "../inc/LIB/STD_TYPES.h"


#include "../Inc/MCAL/UART/UART_interface.h"
#include "../Inc/MCAL/UART/UART_private.h"



void HESP_voidInit (void);
void HESP_voidConnectToWifi (const u8 *copy_u8SSD , const u8 *copy_u8Pass);
void HESP_voidConnectToServer(const u8 *copy_u8IP ,const u8 copy_u8Port);
u8 ESP_voidRecData (const u8 *copy_u8URL);



#endif /* HAL_ESP_ESP_INTERFACE_H_ */
