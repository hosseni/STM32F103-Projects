/*
 * STK_program.c
 *
 *  Created on: Aug 21, 2022
 *      Author: ELHOSSENI
 */

#include <stdio.h>
#include "../Inc/LIB/BIT_MATH.h"

#include "../Inc/MCAL/STK/STK_interface.h"
#include "../Inc/MCAL/STK/STK_private.h"
#include "../Inc/MCAL/STK/STK_configuration.h"

#define STK_AHB_SRC   0x00000004
#define STK_AHB_8_SRC 0x00000000


static void (* SysTick_CallBack) (void) = NULL;
static u8 MSTK_u8ModeOfInterval;


void MSTK_voidInit 				  (void)
{
 /* CLK source */
#if MSTK_CLK_SOURCE == MSTK_SOURCE_AHB
	MSTK->CTRL = STK_AHB_SRC;
#elif MSTK_CLK_SOURCE == MSTK_SOURCE_AHB_8
	MSTK->CTRL = STK_AHB_8_SRC;
#else
#error "wrong source select"
#endif
}

void MSTK_voidSetBusyWait 		  (u32 Copy_u32Time_ms)
{
	/*load ticks to load register*/
#if MSTK_CLK_SOURCE == MSTK_SOURCE_AHB
	MSTK->LOAD = (Copy_u32Time_ms );
#elif MSTK_CLK_SOURCE == MSTK_SOURCE_AHB_8
	MSTK->LOAD = (Copy_u32Time_ms*1000);
#else
#error "wrong source select"
#endif
	/*start timer				 */
	SET_BIT(MSTK -> CTRL, 0);

	/*wait till flag is fired	 */
	/*use time out to get out of while if it */
	while((GET_BIT(MSTK -> CTRL, 16)) == 0)
	{
		asm ("NOP");
	}
	/* stop timer 				 */
	CLR_BIT( MSTK -> CTRL , 0);
	MSTK -> LOAD = 0;
	MSTK -> VAL  = 0;

}

void MSTK_voidSetIntervalSingle   (u32 Copy_u32Time_ms, void (* Copy_PF)(void))
{
	/*disable timer */
	CLR_BIT( MSTK -> CTRL , 0);
	MSTK -> VAL  = 0;

	/*load ticks to load register*/
#if MSTK_CLK_SOURCE == MSTK_SOURCE_AHB
	MSTK->LOAD = (Copy_u32Time_ms );
#elif MSTK_CLK_SOURCE == MSTK_SOURCE_AHB_8
	MSTK->LOAD = (Copy_u32Time_ms );
#else
#error "wrong source select"
#endif
	/*start timer				 */
	SET_BIT(MSTK -> CTRL, 0);

	/*save call back*/
	SysTick_CallBack = Copy_PF;

	/*Enable interrupt*/
	SET_BIT(MSTK -> CTRL, 1);

	/*set mode single interval*/
	MSTK_u8ModeOfInterval = MSTK_SINGLE_INTERVAL;

}
void MSTK_voidSetIntervalPeriodic (u32 Copy_u32Time_ms, void (* Copy_PF)(void))
{
	/*load ticks to load register*/
#if MSTK_CLK_SOURCE == MSTK_SOURCE_AHB
	MSTK->LOAD = (Copy_u32Time_ms );
#elif MSTK_CLK_SOURCE == MSTK_SOURCE_AHB_8
	MSTK->LOAD = (Copy_u32Time_ms );
#else
#error "wrong source select"
#endif

	/*start timer				 */
	SET_BIT(MSTK -> CTRL, 0);

	/*save call back*/
	SysTick_CallBack = Copy_PF;

	/*Enable interrupt*/
	SET_BIT(MSTK -> CTRL, 1);

	/*set mode period interval*/
	MSTK_u8ModeOfInterval = MSTK_PERIOD_INTERVAL;

}
void MSTK_voidStopInterval        (void)
{
	/*disable Interrupt */
		CLR_BIT( MSTK -> CTRL , 1);
	/*stop timer */
		CLR_BIT( MSTK -> CTRL , 0);
		MSTK -> LOAD = 0;
		MSTK -> VAL  = 0;
}
u32  MSTK_u32GetElapsedTime		  (void)
{
  u32 Local_u32ElapsedTimer;

  Local_u32ElapsedTimer = ((MSTK->LOAD) - (MSTK->VAL));

  return Local_u32ElapsedTimer;
}
u32  MSTK_u32GetReamianTime		  (void)
{
	u32 Local_u32RemainTime;
	Local_u32RemainTime = MSTK -> VAL ;
	return Local_u32RemainTime;
}


void SysTick_Handler (void)
{
	//u8 Local_u8Temp;
 	if (MSTK_u8ModeOfInterval == MSTK_SINGLE_INTERVAL)
	{
	/*disable timer */
	CLR_BIT( MSTK -> CTRL , 0);
	MSTK -> VAL  = 0;
	/*stop timer */
	CLR_BIT( MSTK -> CTRL , 0);
	MSTK -> LOAD = 0;
	MSTK -> VAL  = 0;
	}

	SysTick_CallBack();

	/*to clear interrupt flagLocal_u8Temp*/
	//Local_u8Temp = GET_BIT(MSTK -> CTRL, 16);
}
