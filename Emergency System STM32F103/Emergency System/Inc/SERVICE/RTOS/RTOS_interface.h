/*
 * OS_interface.h
 *
 *  Created on: Aug 22, 2022
 *      Author: ELKHOLY
 */

#ifndef SERVICE_OS_INTERFACE_H_
#define SERVICE_OS_INTERFACE_H_

#include "../Inc/LIB/STD_TYPES.h"

void SRTOS_voidInit (void);
void SRTOS_voidCreatTask (u8 Copy_u8Priotity, u16 copy_u16periodicity , void (*Copy_PF)(void), u16 Copy_u16FirstDelay);

#endif /* SERVICE_OS_INTERFACE_H_ */
