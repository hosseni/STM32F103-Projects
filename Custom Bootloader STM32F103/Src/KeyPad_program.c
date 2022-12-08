/*
 * KeyPad_program.c
 *
 *  Created on: Aug 23, 2022
 *      Author: ELHOSSENI
 */

#include "../Inc/LIB/STD_TYPES.h"

#include "../Inc/HAL/KEYPAD/KeyPad_interface.h"
#include "../Inc/HAL/KEYPAD/KeyPad_private.h"
#include "../Inc/HAL/KEYPAD/KeyPad_configuration.h"

#include "../Inc/MCAL/GPIO/GPIO_interface.h"

void HKeypad_voidInit(void)
{

	/*set colums as output pins*/
	MGPIO_voidSetPinDirection(KeyPad_PORT, KeyPad_Col1, GPIO_OUTPUT_PP_10MHZ);
	MGPIO_voidSetPinDirection(KeyPad_PORT, KeyPad_Col2, GPIO_OUTPUT_PP_10MHZ);
	MGPIO_voidSetPinDirection(KeyPad_PORT, KeyPad_Col3, GPIO_OUTPUT_PP_10MHZ);
	MGPIO_voidSetPinDirection(KeyPad_PORT, KeyPad_Col4, GPIO_OUTPUT_PP_10MHZ);

	/*set Rows as input pins*/
	MGPIO_voidSetPinDirection(KeyPad_PORT, KeyPad_Row1, GPIO_INPUT_PULL_DOWN);
	MGPIO_voidSetPinDirection(KeyPad_PORT, KeyPad_Row2, GPIO_INPUT_PULL_DOWN);
	MGPIO_voidSetPinDirection(KeyPad_PORT, KeyPad_Row3, GPIO_INPUT_PULL_DOWN);
	MGPIO_voidSetPinDirection(KeyPad_PORT, KeyPad_Row4, GPIO_INPUT_PULL_DOWN);

	/*Rows connected as internal pull up*/
	MGPIO_voidSetPinValue(KeyPad_PORT, KeyPad_Row1, GPIO_HIGH);
	MGPIO_voidSetPinValue(KeyPad_PORT, KeyPad_Row2, GPIO_HIGH);
	MGPIO_voidSetPinValue(KeyPad_PORT, KeyPad_Row3, GPIO_HIGH);
	MGPIO_voidSetPinValue(KeyPad_PORT, KeyPad_Row4, GPIO_HIGH);

}
u8 	 HKeypad_u8GetKey(void)
{
	u8 Local_u8PressedKey = 0xff;

	/*check for first coloum*/
	MGPIO_voidSetPinValue(KeyPad_PORT, KeyPad_Col1, GPIO_LOW);
	MGPIO_voidSetPinValue(KeyPad_PORT, KeyPad_Col2, GPIO_HIGH);
	MGPIO_voidSetPinValue(KeyPad_PORT, KeyPad_Col3, GPIO_HIGH);
	MGPIO_voidSetPinValue(KeyPad_PORT, KeyPad_Col4, GPIO_HIGH);

	if (MGPIO_u8GetPinValue(KeyPad_PORT, KeyPad_Row1) == 0)
	{		Local_u8PressedKey = '~';}
	if (MGPIO_u8GetPinValue(KeyPad_PORT, KeyPad_Row2) == 0)
	{		Local_u8PressedKey = '1';}
	if (MGPIO_u8GetPinValue(KeyPad_PORT, KeyPad_Row3) == 0)
	{		Local_u8PressedKey = '4';}
	if (MGPIO_u8GetPinValue(KeyPad_PORT, KeyPad_Row4) == 0)
	{		Local_u8PressedKey = '7';}

	/*check for second coloum*/
	MGPIO_voidSetPinValue(KeyPad_PORT, KeyPad_Col1, GPIO_HIGH);
	MGPIO_voidSetPinValue(KeyPad_PORT, KeyPad_Col2, GPIO_LOW);
	MGPIO_voidSetPinValue(KeyPad_PORT, KeyPad_Col3, GPIO_HIGH);
	MGPIO_voidSetPinValue(KeyPad_PORT, KeyPad_Col4, GPIO_HIGH);

	if (MGPIO_u8GetPinValue(KeyPad_PORT, KeyPad_Row1) == 0)
	{		Local_u8PressedKey = '0';}
	if (MGPIO_u8GetPinValue(KeyPad_PORT, KeyPad_Row2) == 0)
	{		Local_u8PressedKey = '2';}
	if (MGPIO_u8GetPinValue(KeyPad_PORT, KeyPad_Row3) == 0)
	{		Local_u8PressedKey = '5';}
	if (MGPIO_u8GetPinValue(KeyPad_PORT, KeyPad_Row4) == 0)
	{		Local_u8PressedKey = '8';}

/*check for third coloum*/
	MGPIO_voidSetPinValue(KeyPad_PORT, KeyPad_Col1, GPIO_HIGH);
	MGPIO_voidSetPinValue(KeyPad_PORT, KeyPad_Col2, GPIO_HIGH);
	MGPIO_voidSetPinValue(KeyPad_PORT, KeyPad_Col3, GPIO_LOW);
	MGPIO_voidSetPinValue(KeyPad_PORT, KeyPad_Col4, GPIO_HIGH);

	if (MGPIO_u8GetPinValue(KeyPad_PORT, KeyPad_Row1) == 0)
			{Local_u8PressedKey = '=';}
	if (MGPIO_u8GetPinValue(KeyPad_PORT, KeyPad_Row2) == 0)
			{Local_u8PressedKey = '3';}
	if (MGPIO_u8GetPinValue(KeyPad_PORT, KeyPad_Row3) == 0)
			{Local_u8PressedKey = '6';}
	if (MGPIO_u8GetPinValue(KeyPad_PORT, KeyPad_Row4) == 0)
			{Local_u8PressedKey = '9';}

	//check for fourth coloum
	MGPIO_voidSetPinValue(KeyPad_PORT, KeyPad_Col1, GPIO_HIGH);
	MGPIO_voidSetPinValue(KeyPad_PORT, KeyPad_Col2, GPIO_HIGH);
	MGPIO_voidSetPinValue(KeyPad_PORT, KeyPad_Col3, GPIO_HIGH);
	MGPIO_voidSetPinValue(KeyPad_PORT, KeyPad_Col4, GPIO_LOW);

	if (MGPIO_u8GetPinValue(KeyPad_PORT, KeyPad_Row1) == 0)
	{		Local_u8PressedKey = '+';}
	if (MGPIO_u8GetPinValue(KeyPad_PORT, KeyPad_Row2) == 0)
	{		Local_u8PressedKey = '-';}
	if (MGPIO_u8GetPinValue(KeyPad_PORT, KeyPad_Row3) == 0)
	{		Local_u8PressedKey = '*';}
	if (MGPIO_u8GetPinValue(KeyPad_PORT, KeyPad_Row4) == 0)
	{		Local_u8PressedKey = '/';}


	return Local_u8PressedKey;
}
