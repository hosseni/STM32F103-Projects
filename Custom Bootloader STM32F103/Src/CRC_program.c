/*******************************************************************************
* Title                 :   CRC 
* Filename              :   CRC_program.c
* Author                :   Mohamed Hafez
* Origin Date           :   10/08/2020
* Version               :   1.0.0
* Compiler              :   TODO: COMPILER GOES HERE
* Target                :   TODO: MCU GOES HERE
* Notes                 :   None 
*
*****************************************************************************/
/*************** SOURCE REVISION LOG *****************************************
*
*    Date    Version   Author          Description 
*  10/08/20   1.0.0   Mohamed Hafez   Initial Release.
*
*******************************************************************************/
/** @file CRC_program.c
 *  @brief This is the source file for TODO: WHAT DO I DO? 
 */
/******************************************************************************
* Includes
*******************************************************************************/	

#include "../Inc/LIB/STD_TYPES.h"
#include "../Inc/LIB/BIT_MATH.h"
#include <stdlib.h>
#include "../Inc/MCAL/CRC/CRC_interface.h"
#include "../Inc/MCAL/CRC/CRC_config.h"
#include "../Inc/MCAL/CRC/CRC_private.h"

/******************************************************************************
* Module Preprocessor Constants
*******************************************************************************/

/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/

/******************************************************************************
* Module Typedefs
*******************************************************************************/

/******************************************************************************
* Module Variable Definitions
*******************************************************************************/

/******************************************************************************
* Function Prototypes
*******************************************************************************/

/******************************************************************************
* Function Definitions
*******************************************************************************/

void MCRC_voidReset ()
{
	 CRC_CR = CRC_RESRT;
}

u32 MCRC_u32CalculateCrcHw(u32 * Copy_u32WordArray, u8 Copy_u8ByteArraySize )
{
	 u32  Loc_u32PtrCrc = 0;
   if(Copy_u32WordArray != NULL)
   {
#ifndef CRC_ACUMEALTE_CALA
MCRC_voidReset();
#endif
      for(u32 Local_u32WordCounter = 0;Local_u32WordCounter < Copy_u8ByteArraySize;++Local_u32WordCounter)
      {
         CRC_DR = (u8)(Copy_u32WordArray[Local_u32WordCounter]);
      }

      Loc_u32PtrCrc = CRC_DR;
   }
   else
   {
      /*!<TODO: Null pointer detected */
   }
   return Loc_u32PtrCrc;
}
/*************** END OF FUNCTIONS ***************************************************************************/
