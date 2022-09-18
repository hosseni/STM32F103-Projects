/*
 * TFT_program.c
 *
 *  Created on: Sep 14, 2022
 *      Author: ELKHOLY
 */



#include "../Inc/LIB/STD_TYPES.h"

#include "../Inc/HAL/TFT/TFT_interface.h"
#include "../Inc/HAL/TFT/TFT_private.h"
#include "../Inc/HAL/TFT/TFT_configuration.h"

#include "../Inc/MCAL/GPIO/GPIO_interface.h"
#include "../Inc/MCAL/SPI/SPI_interfce.h"
#include "../inc/MCAL/STK/STK_interface.h"


void HTFT_voidInit (void)
{
	/* Configure pins for TFT */
		MGPIO_voidSetPinDirection(TFT_RST_PIN  , GPIO_OUTPUT_PP_10MHZ);
		MGPIO_voidSetPinDirection(TFT_A0_PIN  , GPIO_OUTPUT_PP_10MHZ);

		/* Reset pulse */
		MGPIO_voidSetPinValue (TFT_RST_PIN , GPIO_HIGH);
		 MSTK_voidSetBusyWait( 100 );
		 MGPIO_voidSetPinValue (TFT_RST_PIN , GPIO_LOW);
		 MSTK_voidSetBusyWait( 2 );
		 MGPIO_voidSetPinValue (TFT_RST_PIN , GPIO_HIGH);
		 MSTK_voidSetBusyWait( 100 );
		 MGPIO_voidSetPinValue (TFT_RST_PIN , GPIO_LOW);
		 MSTK_voidSetBusyWait( 100 );
		 MGPIO_voidSetPinValue (TFT_RST_PIN , GPIO_HIGH);
		 MSTK_voidSetBusyWait( 120000 );

		/* Sleep out */
		TFT_voidWriteCommand(0x11);

		/* Wait 150 ms */
		 MSTK_voidSetBusyWait( 150 );

		/* Colour mode command */
		TFT_voidWriteCommand(0x3A);
		TFT_voidWriteData(0x05);

		/* Display on */
		TFT_voidWriteCommand(0x29);
}
void HTFT_voidDispalyImage ( const u16* Copy_Image)
{
	/* Set address range for the whole display */
	TFT_voidSetAddress(0  , TFT_MAX_X , 0 , TFT_MAX_Y);

	u16 local_u16Counter ;
	for (local_u16Counter = 0 ; local_u16Counter < (TFT_MAX_X * TFT_MAX_Y); local_u16Counter++){
		TFT_voidSetColour(Copy_Image[local_u16Counter]);
	}
}

void HTFT_voidDrawRectangle (u16 copy_u16X ,u16 copy_u16Y , u16 copy_u16Width , u16 copy_u16Hight , u16 copy_u16Color )
{
	u16 loacal_u16EndX = copy_u16X + copy_u16Width - 1;
	u16 loacal_u16EndY = copy_u16Y + copy_u16Hight - 1;

	/* Set area of addresses */
	TFT_voidSetAddress(copy_u16X ,loacal_u16EndX,copy_u16Y ,loacal_u16EndY );

	/* Draw the Rectangle*/
	for (u16 i = 0 ; i < (copy_u16Width *copy_u16Hight ) ; i++){
		TFT_voidSetColour(copy_u16Color);
	}
}

void HTFT_voidPrintChar(s8 copy_s8Char , u16 copy_u16X , u16 copy_u16Y, u8 copy_u8Size , u16 copy_u16Color , u16 copy_u16BackColor)
{
	/* Get array index */
	u8 local_u8CharIndex = 0 ;
	if (( copy_s8Char >= ' ' ))
	{/* Get array index */
		u8 local_u8CharIndex = 0 ;
		if (( copy_s8Char >= ' ' )){
			local_u8CharIndex = copy_s8Char - 32 ;
		}

		/* Background */
		HTFT_voidDrawRectangle( copy_u16X, copy_u16Y ,copy_u8Size*TFT_CHARACTER_WIDTH ,copy_u8Size*TFT_CHARACTER_HIGHT ,copy_u16BackColor);

		for (u8 i = 0; i <TFT_CHARACTER_WIDTH ; i++ ){
			for (u8 j = 0 ; j <TFT_CHARACTER_HIGHT ; j++){
				if (TFT_font[local_u8CharIndex][i] & (1 << j)){
					if (copy_u8Size == 1){
						TFT_voidDrawPixel(copy_u16X+i , copy_u16Y+j , copy_u16Color);
					}
					else {
						u16 local_x = copy_u16X+(i*copy_u8Size) ;
						u16 local_y = copy_u16Y+(j*copy_u8Size) ;
						HTFT_voidDrawRectangle( local_x, local_y ,copy_u8Size ,copy_u8Size ,copy_u16Color);
					}
				}
			}
		}
}
}

void HTFT_voidPrintText(s8 *copy_s8Text , u16 copy_u16X , u16 copy_u16Y, u8 copy_u8Size , u16 copy_u16Color, u16 copy_u16BackColor)
{
	while (*copy_s8Text)
	{
		HTFT_voidPrintChar(*copy_s8Text , copy_u16X ,copy_u16Y ,copy_u8Size ,copy_u16Color,copy_u16BackColor );
		copy_u16X += (copy_u8Size *TFT_CHARACTER_WIDTH)  ;
		copy_s8Text++;
	}
}


void HTFT_voidFillDisplay (u16 copy_u16Colour)
{
	HTFT_voidDrawRectangle(0 , 0 ,TFT_MAX_X+1 , TFT_MAX_Y+1 , copy_u16Colour);
}

 void TFT_voidWriteCommand ( u8 Copy_u8Command )
{
	/*set A0 low*/
	MGPIO_voidSetPinValue(TFT_A0_PIN, GPIO_LOW);

	/*Send command over spi*/
	SPI_voidSendRecSynch(SPI1, Copy_u8Command);

}
 void TFT_voidWriteData ( u8 Copy_u8Data )
{
	/*set A0 high*/
	MGPIO_voidSetPinValue(TFT_A0_PIN, GPIO_HIGH);

	/*Send data over spi*/
	SPI_voidSendRecSynch(SPI1, Copy_u8Data);
}

 void TFT_voidSetAddress (u16 copy_u16StartX ,u16 copy_u16EndX , u16 copy_u16StartY , u16 copy_u16EndY)
{
	if ( (copy_u16StartX < TFT_MAX_X ) && (copy_u16StartY < TFT_MAX_Y )){
		/* Set x Address */
		TFT_voidWriteCommand(0x2A);
		/* Start byte */
		TFT_voidWriteData((copy_u16StartX >> 8));  // MS byte
		TFT_voidWriteData(copy_u16StartX);

		/* Stop byte */
		TFT_voidWriteData((copy_u16EndX >> 8));
		TFT_voidWriteData(copy_u16EndX);

		/* Set y Address */
		TFT_voidWriteCommand(0x2B);
		/* Start byte */
		TFT_voidWriteData((copy_u16StartY >> 8));  // MS byte
		TFT_voidWriteData(copy_u16StartY);
		/* Stop byte */
		TFT_voidWriteData((copy_u16EndY >> 8));
		TFT_voidWriteData(copy_u16EndY);

		/* RAM write */
		TFT_voidWriteCommand(0x2C);
	}
}

 void TFT_voidSetColour(u16 copy_u16Colour)
{
	/* Write pixel */
	u8 high_byte = (copy_u16Colour >> 8);
	u8 low_byte = (copy_u16Colour & 0xff);
	TFT_voidWriteData(high_byte);
	TFT_voidWriteData(low_byte);
}

 void TFT_voidDrawPixel (u16 copy_u16X , u16 copy_u16Y , u16 copy_u16Colour)
{
	if ( (copy_u16X < TFT_MAX_X ) && (copy_u16Y < TFT_MAX_Y ))
	{
		/* Set address of the pixel */
		TFT_voidSetAddress(copy_u16X  , copy_u16X+1 , copy_u16Y , copy_u16Y+1);

		/* Write pixel colour */
		TFT_voidSetColour(copy_u16Colour);
	}
}


