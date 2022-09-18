/*
 * OS_program.c
 *
 *  Created on: Aug 22, 2022
 *      Author: ELHOSSENI
 */

#include <SERVICE/RTOS/RTOS_interface.h>
#include <SERVICE/RTOS/RTOS_private.h>
#include <SERVICE/RTOS/RTOS_configuration.h>


#include "../Inc/MCAL/STK/STK_interface.h"

static TASK_TCB_t  TASKS[NUM_OF_TASKS];
static u16 Ticks;

void RTOS_voidSchadular (void)
{
	u8 i = 0;
	for (i = 0 ; i < NUM_OF_TASKS ; i++)
	{
		if (TASKS [i].u16_firstDelay  == 0)
		//if (( Ticks % TASKS [i].u16_periodicty ) == 0)

		{
			/*call [i] task */
			TASKS[i].Copy_PF();
			TASKS[i].u16_firstDelay = TASKS[i].u16_periodicty -1;
		}
		else
		{
			TASKS[i].u16_firstDelay --;
		}
	}
	Ticks++;
}
/*void RTOS_voidSchadular (void)
{
	u8 i = 0;
	for (i = 0 ; i < NUM_OF_TASKS ; i++)
	{
		if (( Ticks % TASKS [i].periodicty ) == 0)
		{
			TASKS[i].Copy_PF();
		}
	}
	Ticks++;
}*/
void SRTOS_voidInit (void)
{
	/*SysTick*/
	MSTK_voidInit();

	/*SysTick to be 1 msesc with interval */
	MSTK_voidSetIntervalPeriodic(1000, RTOS_voidSchadular); /*tick time  = 1 ms */
}

void SRTOS_voidCreatTask (u8 Copy_u8Priotity, u16 copy_u16periodicity , void (*Copy_PF)(void), u16 Copy_u16FirstDelay)
{
	TASKS[Copy_u8Priotity].u16_periodicty = copy_u16periodicity;
	TASKS[Copy_u8Priotity].Copy_PF    = Copy_PF;
	TASKS[Copy_u8Priotity].u16_firstDelay = Copy_u16FirstDelay;
}
