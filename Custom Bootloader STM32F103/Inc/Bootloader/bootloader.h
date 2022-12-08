/**********************************************************************************************************************

 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *         File:  <bootloader.h>
 *       Module:  bootloader
 *
 *  Description:  <Write File DESCRIPTION here>
 *
 *********************************************************************************************************************/
#ifndef BOOTLOADER_BOOTLOADER_H_
#define BOOTLOADER_BOOTLOADER_H_

/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "../Inc/MCAL/UART/UART_interface.h"
#include "../Inc/MCAL/CRC/CRC_interface.h"
#include "../Inc/MCAL/FLASH/flash_interface.h"

extern u32 PAGE_ARRAY[];


/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 ******************************	***************************************************************************************/
#define BL_DEBUG_UART 			   		  UART_2
#define BL_HOST_COMMUNICATION_UART 		  UART_1
#define BL_DEBUG_INFO              		  1


#define ENABLE_UART_DEBUG_MESSAGE 0x01
#define ENABLE_SPI_DEBUG_MESSAGE  0x02
#define ENABLE_CAN_DEBUG_MESSAGE  0x03


#define CBL_GET_VER_CMD			      	0x10
#define CBL_GET_HELP_CMD            0x11
#define CBL_GET_CID_CMD             0x12
#define CBL_GET_RDP_STATUS_CMD      0x13
#define CBL_GO_TO_ADDR_CMD          0x14
#define CBL_FLASH_ERASE_CMD         0x15
#define CBL_MEM_WRITE_CMD           0x16
#define CBL_EN_R_W_PROTECT_CMD      0x17
#define CBL_MEM_READ_CMD            0x18
#define CBL_READ_SECTOR_STATUS_CMD  0x19
#define CBL_OTP_READ_CMD            0x20
#define CBL_DIS_R_W_PROJECT_CMD     0x21


#define CBL_VENDOR_ID                 100
#define CBL_SW_MAJOR_VERSION          2
#define CBL_SW_MINOR_VERSION          1
#define CBL_SW_PATCH_VERSION          1

#define CRC_TYPE_SIZE_BYTE 			  4

#define CBL_SEND_NACK                 0xAB
#define CBL_SEND_ACK                  0xCD

#define APPLICATION_START_ADDRESS     0x08004400


#define DEBUG_METHOD    ENABLE_UART_DEBUG_MESSAGE

#define BL_HOST_BUFFER_RX_LENGTH 200

/*Memory boundary*/
#define FLASH_BASE            ((u32)0x08000000) /*!< FLASH base address in the alias region */
#define SRAM_BASE             ((u32)0x20000000) /*!< SRAM base address in the alias region */

#define FLASH_SIZE   				(64 * 1024)     /*FLASH SIZE : 64Kbyte*/       
#define SRAM_SIZE 	 				(20 * 1024)		 /*SRAM  SIZE : 20Kbyte*/

#define FLASH_END_ADDRESS    (FLASH_BASE + FLASH_SIZE)
#define SRAM_END_ADDRESS    (SRAM_BASE + SRAM_SIZE)


#define FLASH_MAX_PAGE_NUM     32
#define MASS_ERASE						0xFF
/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
typedef enum
{
	BL_NACK,
	BL_OK
}BL_StatusType;

typedef enum
{
	CRC_FAILD,
	CRC_PASSED
}CRC_VerifyType;

typedef enum
{
	ADDRESS_IS_INVALID,
	ADDRESS_IS_VALID
	
}Address_VerifyType;

typedef u8 Erase_VerifyType;

	




/**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
void BL_Print_Message (char *format, ...);
BL_StatusType BL_UART_Featch_Host_Command (void);


#endif  /* BOOTLOADER_BOOTLOADER_H_ */

/**********************************************************************************************************************
 *  END OF FILE: bootloader.h
 *********************************************************************************************************************/
