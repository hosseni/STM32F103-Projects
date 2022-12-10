/*
 * UART_program.c
 *
 *  Created on: Aug 30, 2022
 *      Author: ELHOSSENI
 */

#include "../Inc/LIB/BIT_MATH.h"
#include "../Inc/LIB/STD_TYPES.h"

#include "../Inc/MCAL/UART/UART_interface.h"
#include "../Inc/MCAL/UART/UART_private.h"
#include "../Inc/MCAL/UART/UART_configuration.h"

#define WAIT_TO_TRANSMIT   (GET_BIT(UARTChannel[Copy_u8UartNumber]->SR, TC)   == 0)
#define WAIT_TO_RECEIVE    (GET_BIT(UARTChannel[Copy_u8UartNumber]->SR, RXNE) == 0)


void 	MUART_voidInit 		(UART_Num_t UART_NUM)
{
	/*clear CR1 - CR2 - CR3 - SR */
	UARTChannel[UART_NUM] -> CR [0] = CLR_REGISTER;
	UARTChannel[UART_NUM] -> CR [1] = CLR_REGISTER;
	UARTChannel[UART_NUM] -> CR [2] = CLR_REGISTER;
	UARTChannel[UART_NUM] -> SR     = CLR_REGISTER;

	UARTChannel[UART_NUM] -> CR[0]     |=    ( (SEND_BREAK      << SPK)  | (RECEIVER_WAKEUP     <<RWU)| (PARITY_SELECTION << PS)
										     | (RECEIVER_ENABLE << RE)   | (TRANSMITTER_ENABLE  <<TE) | (PARITY_CONTROL << PCE)
										     | (WAKEUP_METHOD   << WAKE) | (WORD_LENGTH         <<M));

	UARTChannel[UART_NUM] -> CR[1]     |=    ( (NODE_ADDRESS 		 << ADD)  | (LIN_BREAK_DETEC_LENGTH << LBDL)| (LIN_BREAK_DETEC_INTERRUPT << LBDIE)
										     | (LAST_BIT_CLOCK_PULSE << LBCL) | (CLOCK_PHASE     		<< CPHA)| (CLOCK_POLARITY 			 << CPOL)
										     | (CLOCK_PIN_ENABLE 	 << CLKEN)| (STOP_BIT_NUMBER        << STOP)| (LIN_MODE 				 << LINEN));

	UARTChannel[UART_NUM] -> CR[2]     |=    ( (IRDA_MODE 	    << IREN) | (IRDA_LOW_POWER << IRLP)  | (HALF_DUPLEX  << HDSEL)
										     | (SMARTCARD_NACK  << NACK) | (SMARTCARD_MODE << SCEN)  | (DMA_RECEIVER <<  DMAR)
										     | (DMA_TRANSMITTER << DMAT) | (RTS_MODE       << RTSE)  | (CTS_MODE     <<  CTSE));

	/*set boudrate*/
	UARTChannel[UART_NUM] -> BRR  = UART_BAUDRATE;
	/*enable UART*/
	UARTChannel[UART_NUM] -> CR[0] |= (ENABLE_UART << UE);

}
void    MUART_voidTransmitChar (UART_Num_t UART_NUM, u8 Copy_u8Data)
{

	/*load data from user to Data Reg*/
	UARTChannel[UART_NUM] ->DR = Copy_u8Data;
	/*wait to transmit*/
	while ( (GET_BIT(UARTChannel[UART_NUM] -> SR, TC)) == 0);
if ((GET_BIT(UARTChannel[UART_NUM] -> SR, TC)) == 1)
	{
#if	TRANSMITTER_COMPLETE_INTERRUPT  == ENABLE_INTERRUPT
	SET_BIT(UARTChannel[UART_NUM] ->CR[0], TCIE);
#endif
	}

}
ReturnStatus MUART_voidTransmitSynch (u8 Copy_u8UartNumber, u8 Copy_u8DataArray[], u8 sizeOfTransmittedData)
{
	if ( (Copy_u8DataArray == NULL) || (sizeOfTransmittedData == 0))
		return NOT_OK;

	else
	{
		u8 counter = 0;
		u8 index = 0;
		while (counter < sizeOfTransmittedData)
			{


			while(Copy_u8DataArray[index] != '\0')
			{
			// Put data in DR register to be transmitted.
		     UARTChannel[Copy_u8UartNumber]->DR = Copy_u8DataArray[index];
		     index++;
			// Wait untill transmition finished.
			 while(WAIT_TO_TRANSMIT);
			}
			index = 0;
			counter ++;
		}
	}
	return OK;
}


ReturnStatus MUART_u8ReceiveByteSynch(u8 Copy_u8UartNumber, u8  ReceivedData[], u8 sizeOfReceivedData)
{

	if ((ReceivedData == NULL) || (sizeOfReceivedData == 0))
		return NOT_OK;

	else
	{
		u8 counter = 0;

		while (counter < sizeOfReceivedData)
		{
			// Wait untill receiving finished.
			while(WAIT_TO_RECEIVE);
			// Getting the first byte of the register to be returned.
			ReceivedData[counter] = ( (UARTChannel[Copy_u8UartNumber]->DR));
			counter ++;
		}

	}
   return OK;
}
