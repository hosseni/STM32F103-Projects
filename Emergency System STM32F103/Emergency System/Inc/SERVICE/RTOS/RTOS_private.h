/*
 * OS_private.h
 *
 *  Created on: Aug 22, 2022
 *      Author: ELHOSSENI
 */

#ifndef SERVICE_OS_PRIVATE_H_
#define SERVICE_OS_PRIVATE_H_

#include "../Inc/LIB/STD_TYPES.h"

typedef struct
{
	u16 u16_periodicty;
	u16 u16_firstDelay;
	void (*Copy_PF) (void);
}TASK_TCB_t;

void RTOS_voidStart ();

#endif /* SERVICE_OS_PRIVATE_H_ */
