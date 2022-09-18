/*
 * UART_interface.h
 *
 *  Created on: Aug 30, 2022
 *      Author: ELHOSSENI
 */

#ifndef MCAL_UART_UART_INTERFACE_H_
#define MCAL_UART_UART_INTERFACE_H_




#include "../Inc/LIB/STD_TYPES.h"
#include "UART_private.h"

typedef void (*CallBack)(void);
static volatile CallBack CallBackArray[UART_NUMBER_OF_CHANNELS][UART_NUMBER_OF_INTERRUPT];


void 	MUART_voidInit 		(UART_Num_t UART_NUM);
void    MUART_voidTransmitChar (UART_Num_t UART_NUM, u8 Copy_u8Data);
void 	MUART_voidTransmitSTR  (UART_Num_t UART_NUM, u8 Copy_u8Arr[]);
u8		MUART_u8Recive         (UART_Num_t UART_NUM);
void MUART_voidSetCallBack 	   (UART_Num_t UART_NUM, CallBack CallBackFunc, Interrupt_t Copy_Interrupt_t);


#endif /* MCAL_UART_UART_INTERFACE_H_ */
