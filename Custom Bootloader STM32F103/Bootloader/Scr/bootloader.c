/**********************************************************************************************************************
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file  FileName.c
 *        \brief
 *
 *      \details
 *
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "../Inc/Bootloader/bootloader.h"

/**********************************************************************************************************************
*  LOCAL MACROS CONSTANT\FUNCTION
*********************************************************************************************************************/
/**********************************************************************************************************************
 *  LOCAL DATA
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/
static u8 BL_Host_Buffer [BL_HOST_BUFFER_RX_LENGTH];
static u8 Bootloader_Support_CMD [12] =
{
		CBL_GET_VER_CMD,
		CBL_GET_HELP_CMD,
		CBL_GET_CID_CMD,
		CBL_GET_RDP_STATUS_CMD,
		CBL_GO_TO_ADDR_CMD,
		CBL_FLASH_ERASE_CMD,
		CBL_MEM_WRITE_CMD,
		CBL_EN_R_W_PROTECT_CMD,
		CBL_MEM_READ_CMD,
		CBL_READ_SECTOR_STATUS_CMD,
		CBL_OTP_READ_CMD,
		CBL_DIS_R_W_PROJECT_CMD
};


u8  INVALID_PAGE_NUMBER   = 0x01;
u8  VALID_PAGE_NUMBER     = 0x02;
u8  UNSUCCESSFUL_ERASE    = 0x03;
u8  SUCCESSFUL_ERASE      = 0x04;

u8 Flashing_Flag          = 0;

/**********************************************************************************************************************
 *  Static FUNCTION PROTOTYPES
 *********************************************************************************************************************/



static CRC_VerifyType 		 Bootloader_CRC_Verify 								  ( u8 *pData, u32 Data_Len, u32 Host_CRC);
static void           		 Bootloader_Send_ACK						  			( u8 Replay_len);
static void           	   Bootloader_Send_NACK									  ( );
static void           		 Bootloader_Send_Data_To_Host 					( u8 *Data_Buffer, u8 Data_Len);
static Address_VerifyType  Bootloader_Address_Verify   						( u32 Address);
static u8							     Bootloader_Preforn_Flash_Erase 				( u8 pageNum , u8 NumOfPage);
static u8 								 Bootloader_Flash_Write_Payload         ( u8 *Host_Payload, u32 Payload_Start_Address, u32 Payload_Length);

static void           Bootloader_Get_Version 					  					( u8 *Host_Buffer);
static void           Bootloader_Get_Help 					      				( u8 *Host_Buffer);
static void           Bootloader_Get_Chip_Identification_Number   ( u8 *Host_Buffer);
static void           Bootloader_Read_Protection_Level      	  	( u8 *Host_Buffer);
static void           Bootloader_Jumb_To_Address         	      	( u8 *Host_Buffer);
static void           Bootloader_Erase_Flash 					  					( u8 *Host_Buffer);
static void           Bootloader_Memory_Write 				      			( u8 *Host_Buffer);
static void           Bootloader_Enable_RW_Protection 		      	( u8 *Host_Buffer);
static void           Bootloader_Memory_Read 					  					( u8 *Host_Buffer);
static void           Bootloader_Get_Sector_Protection 		  	  	( u8 *Host_Buffer);
static void           Bootloader_Read_OTP 			              		( u8 *Host_Buffer);
static void           Bootloader_Disable_RW_Protectio			  			( u8 *Host_Buffer);
/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/


/******************************************************************************
* \Syntax          : void BL_Print_Message (char *format, ...)
* \Description     : Describe this service
*
* \Sync\Async      : Synchronous
* \Reentrancy      : Non Reentrant
* \Parameters (in) : parameterName   Parameter Describtion
* \Parameters (out): None
* \Return value:   : Std_ReturnType  E_OK
*                                    E_NOT_OK
*******************************************************************************/
void BL_Print_Message (char *format, ...)
{
	char message [100] = {0};
	va_list args;
	va_start (args, format);
	vsprintf (message, format, args);
#if (DEBUG_METHOD  == ENABLE_UART_DEBUG_MESSAGE)
	MUART_voidTransmitSynch(BL_DEBUG_UART,(u8 *) message, 1);
#elif(DEBUG_METHOD  == ENABLE_SPI_DEBUG_MESSAGE)
#elif(DEBUG_METHOD  == ENABLE_CAN_DEBUG_MESSAGE)
#endif
	va_end (args);
}





/******************************************************************************
* \Syntax          : BL_StatusType BL_UART_Featch_Host_Command (void)
* \Description     : Describe this service
*
* \Sync\Async      : Synchronous
* \Reentrancy      : Non Reentrant
* \Parameters (in) : parameterName   Parameter Describtion
* \Parameters (out): None
* \Return value:   : Std_ReturnType  E_OK
*                                    E_NOT_OK
*******************************************************************************/
BL_StatusType BL_UART_Featch_Host_Command (void)
{
	BL_StatusType BL_status = BL_NACK;
	u8 data_length = 0;
	ReturnStatus  UART_State = NOT_OK;
	/*To clear BL host buffer*/
	memset(BL_Host_Buffer, 0, BL_HOST_BUFFER_RX_LENGTH);

	/*TODO : Receive Packet length*/
	UART_State = MUART_u8ReceiveByteSynch(BL_HOST_COMMUNICATION_UART, BL_Host_Buffer, 1);
	if (UART_State != OK)
	{
		BL_status = BL_NACK;
	}
	else
	{
	data_length = BL_Host_Buffer[0];
	/*TODO : Receive CMD_ID(1Byte) - CRC(4Byte)*/
	UART_State = MUART_u8ReceiveByteSynch(BL_HOST_COMMUNICATION_UART, &BL_Host_Buffer[1], data_length);
	BL_status = BL_OK;
		if (UART_State != OK)
		{
			BL_status = BL_NACK;
		}
		else
		{
				switch (BL_Host_Buffer[1])
			{
			case  CBL_GET_VER_CMD:
				Bootloader_Get_Version(BL_Host_Buffer);
				BL_status = BL_OK;
		    memset(BL_Host_Buffer, 0, BL_HOST_BUFFER_RX_LENGTH);
				break;

			case  CBL_GET_HELP_CMD:
				Bootloader_Get_Help(BL_Host_Buffer);
				BL_status = BL_OK;

				break;

			case  CBL_GET_CID_CMD :
				Bootloader_Get_Chip_Identification_Number(BL_Host_Buffer);
				BL_status = BL_OK;
				break;

			case  CBL_GET_RDP_STATUS_CMD    :
				BL_Print_Message ("Read the flash read protection level ...\n");
				BL_status = BL_OK;
				break;

			case  CBL_GO_TO_ADDR_CMD:
				Bootloader_Jumb_To_Address(BL_Host_Buffer);
				BL_status = BL_OK;
				break;

			case  CBL_FLASH_ERASE_CMD       :
				Bootloader_Erase_Flash(BL_Host_Buffer);
				BL_status = BL_OK;
				break;

			case  CBL_MEM_WRITE_CMD :
				Bootloader_Memory_Write(BL_Host_Buffer);
				BL_status = BL_OK;
			break;

			case  CBL_EN_R_W_PROTECT_CMD    :
				BL_Print_Message ("Enable read write protect on different sectors of the user flash ....\n");
				BL_status = BL_OK;
				break;

			case  CBL_MEM_READ_CMD          :
				BL_Print_Message ("Read all data fromdifferent memories of MCU...\n");
				BL_status = BL_OK;
				break;

			case  CBL_READ_SECTOR_STATUS_CMD:
				BL_Print_Message ("Read all sector protection status...\n");
				BL_status = BL_OK;
				break;

			case  CBL_OTP_READ_CMD          :
				BL_Print_Message ("Read the OTP connects....\n");
				BL_status = BL_OK;
				break;

			case  CBL_DIS_R_W_PROJECT_CMD   :
				BL_Print_Message ("Disable read write protection on different sectors of the user flash...\n");
				BL_status = BL_OK;
				break;

			default :
				BL_Print_Message("Invalid command code received from host ... \n");
			
				break;
			}
		}
	}
	return BL_status;
}
static void Bootloader_Send_Data_To_Host (u8 *Data_Buffer, u8 Data_Len)
{
	MUART_voidTransmitSynch(BL_HOST_COMMUNICATION_UART, Data_Buffer, Data_Len);
}

static void           Bootloader_Send_ACK						  (u8 Replay_len)
{
	u8 ACK_Val [2] = {0};
	ACK_Val [0] = CBL_SEND_ACK;
	ACK_Val [1] = Replay_len;
	MUART_voidTransmitSynch(BL_HOST_COMMUNICATION_UART,(u8 *) ACK_Val, 1);
#ifdef BL_DEBUG_INFO
	BL_Print_Message ("ACK is : %x and Replay Len: %d sent..  \n", ACK_Val [0],  ACK_Val [1]);
#endif
}
static void           Bootloader_Send_NACK						  ()
{
	u8 NACK_Val = CBL_SEND_NACK;
	 MUART_voidTransmitSynch(BL_HOST_COMMUNICATION_UART, &NACK_Val, 1);
#ifdef BL_DEBUG_INFO
	BL_Print_Message ("NACK is : %x sent..  \n", NACK_Val);
#endif

}

static CRC_VerifyType Bootloader_CRC_Verify ( u8 *pData, u32 Data_Len, u32 Host_CRC)
{
#ifdef BL_DEBUG_INFO
	BL_Print_Message ("CRC Verifying..  \n");
#endif

	CRC_VerifyType CRC_State = CRC_FAILD;
	u32 MCU_CRC_Calculated = 0;
		u8 Data_Counter = 0;
		u32 Data_Buffer = 0;
		/* Calculate CRC32 */
		for(Data_Counter = 0; Data_Counter < Data_Len; Data_Counter++){
			Data_Buffer = (u32)pData[Data_Counter];
			MCU_CRC_Calculated = MCRC_u32CalculateCrcHw( &Data_Buffer, 1);
		}
		/*Reset CRC*/
		MCRC_voidReset();
		if (MCU_CRC_Calculated == Host_CRC)
		{
			CRC_State = CRC_PASSED;
#ifdef BL_DEBUG_INFO
	BL_Print_Message ("CRC_PASSED..  \n");
#endif
		}
		else
		{
			CRC_State = CRC_FAILD;
#ifdef BL_DEBUG_INFO
	BL_Print_Message ("CRC_FAILD..  \n");
#endif
		}

	return CRC_State;
}

static Address_VerifyType  Bootloader_Address_Verify   ( u32 Address)
{
	Address_VerifyType Address_Status = ADDRESS_IS_INVALID;
	
	if ((Address >= FLASH_BASE)&&(Address <= FLASH_END_ADDRESS))
	{
#ifdef BL_DEBUG_INFO
	BL_Print_Message ("Address Accepted, Flash boundary ..  \n");
#endif
		Address_Status = ADDRESS_IS_VALID;
	}
	else if ((Address >= SRAM_BASE)&&(Address <= SRAM_END_ADDRESS))
	{
#ifdef BL_DEBUG_INFO
	BL_Print_Message ("Address Accepted, Ram boundary ..  \n");
#endif
		Address_Status = ADDRESS_IS_VALID;
	}
	else 
	{
#ifdef BL_DEBUG_INFO
	BL_Print_Message ("Address Out of range ..  \n");
#endif
		Address_Status = ADDRESS_IS_INVALID;
	}
		
	return Address_Status;
}

static u8 Bootloader_Preforn_Flash_Erase (u8 pageNum , u8 NumOfPage)
{
	

	/*local var Address_Status to get Erase status */
	u8 Erase_Status = UNSUCCESSFUL_ERASE;
	ReturnStatus Status = NOT_OK;
	if (FLASH_MAX_PAGE_NUM >= NumOfPage)
	{
		
		/*Num of page is in range*/
		if ((FLASH_MAX_PAGE_NUM > pageNum) || (MASS_ERASE == pageNum))
		{
			/*Unlock Falsh*/
			if (MASS_ERASE == pageNum)
			{
#ifdef BL_DEBUG_INFO
BL_Print_Message("Mass erase running....\n");						
#endif		
				/*mass erase*/
				MFLASH_massErase();
				Erase_Status = SUCCESSFUL_ERASE;
			}
			else 
			{
				/*sector erase*/
				u8 PageCounter = 0;
				u8 PageMAxCounter = (pageNum + NumOfPage);
				for ( PageCounter = pageNum; PageCounter < PageMAxCounter ; PageCounter ++)
				{
					if ( pageNum < FLASH_MAX_PAGE_NUM  )
					{
						/*Unlock Flash*/
						MFLASH_unlock();
					  Status	= MFLASH_erasePage(PAGE_ARRAY[PageCounter]);	
						if (OK == Status)
						{
#ifdef BL_DEBUG_INFO
BL_Print_Message(" Erasing Page %d with address 0x%X done successfully....\n",PageCounter,  PAGE_ARRAY[PageCounter] );						
#endif					
						Erase_Status = SUCCESSFUL_ERASE;
						}
						else	
						{
#ifdef BL_DEBUG_INFO
BL_Print_Message("Error Erasing  Page %d with address 0x%X ....\n",PageCounter,  PAGE_ARRAY[PageCounter] );						
#endif					
						Erase_Status = UNSUCCESSFUL_ERASE;
						}
							
					}/*if*/
					else 
					{
#ifdef BL_DEBUG_INFO
BL_Print_Message(" Page %d not available....\n", PageCounter);						
#endif
				Erase_Status = INVALID_PAGE_NUMBER;
					}/*else*/
				}/*	for*/
			}/*else*/
			
		}/*		if ((FLASH_MAX_PAGE_NUM > pageNum) || (MASS_ERASE == pageNum))*/
		else 
		{
		Erase_Status = INVALID_PAGE_NUMBER;
#ifdef BL_DEBUG_INFO
BL_Print_Message(" Page Number not in range ....\n" );						
#endif
		}
	}/*	if (FLASH_MAX_PAGE_NUM >= NumOfPage)*/
	else 
	{
#ifdef BL_DEBUG_INFO
BL_Print_Message(" Number of pages is not allowed ....\n" );						
#endif
		/*Num of page is out of range*/
	 Erase_Status = UNSUCCESSFUL_ERASE;
	}
	MFLASH_lock();
	return Erase_Status;
}

static u8 Bootloader_Flash_Write_Payload ( u8 *Host_Payload, u32 Payload_Start_Address, u32 Payload_Length)
{
	u8 	Flash_Payload_Write_Status = FLASH_PAYLOAD_WRITE_FAILED;
	u32 Payload_Counter 					 = 0;
	ReturnStatus Return_Status     = NOT_OK;
	u32 Current_Payload_Address    = 0;
	MFLASH_unlock();

	for (Payload_Counter = 0; Payload_Counter <= Payload_Length; Payload_Counter +=1)
	{

		Current_Payload_Address = (Payload_Start_Address + (Payload_Counter*2));

		Return_Status = MFLASH_programPage(Current_Payload_Address ,(Host_Payload[Payload_Counter]) );
		if (NOT_OK == Return_Status)
		{
			Flash_Payload_Write_Status = FLASH_PAYLOAD_WRITE_FAILED;
#ifdef BL_DEBUG_INFO
BL_Print_Message(" Error while flashing to 0x%x \n", Current_Payload_Address );						
#endif
			break;
		}
		else 
		{
#ifdef BL_DEBUG_INFO
BL_Print_Message(" current Payload flashed to 0x%x \n", Current_Payload_Address );						
#endif
			Flash_Payload_Write_Status = FLASH_PAYLOAD_WRITE_PASSED;
		}
	}
	if (Flash_Payload_Write_Status == FLASH_PAYLOAD_WRITE_PASSED)
	{
		
		MFLASH_lock();
	}
	return Flash_Payload_Write_Status;
}
/**************************************************************************************************/
static void Bootloader_Get_Version 					  ( u8 * Host_Buffer)
{
	/*********************************************************
	 ***************BOOTLOADER PACKET (6Byte)*****************
	 *******|    1     |   2  | 3 | 4 | 5 | 6 |***************
	 *******|Packet_Len|CMD_ID|CRC|CRC|CRC|CRC|***************
	 *********************************************************/
#ifdef BL_DEBUG_INFO
	BL_Print_Message ("Read the bootloader version from MCU..  \n");
#endif

	u8 BL_VERSION[4] = {CBL_VENDOR_ID, CBL_SW_MAJOR_VERSION, CBL_SW_MINOR_VERSION, CBL_SW_PATCH_VERSION};
	u16 Host_CMD_Packet_Len = 0;
	u32 Host_CRC32 = 0;

	/*Extract CRC - Packet length sent by host */
	Host_CMD_Packet_Len = Host_Buffer[0] + 1;
	Host_CRC32 = *((u32* )((Host_Buffer + (u32)Host_CMD_Packet_Len) - CRC_TYPE_SIZE_BYTE));

	/*CRC Verification */
	if (CRC_PASSED == Bootloader_CRC_Verify( &Host_Buffer[0], (Host_CMD_Packet_Len - 4), Host_CRC32))
	{
		Bootloader_Send_ACK(4);
		Bootloader_Send_Data_To_Host(BL_VERSION, 1);
	}
	else
	{
		Bootloader_Send_NACK();
	}

}


static void Bootloader_Get_Help 					  ( u8 Host_Buffer[])
{
	/*********************************************************
	 ***************BOOTLOADER PACKET (6Byte)*****************
	 *******|    1     |   2  | 3 | 4 | 5 | 6 |***************
	 *******|Packet_Len|CMD_ID|CRC|CRC|CRC|CRC|***************
	 *********************************************************/
#ifdef BL_DEBUG_INFO
	BL_Print_Message ("Read the command supported by the bootloader.\n");
#endif

	u16 Host_CMD_Packet_Len = 0;
	u32 Host_CRC32 = 0;

	/*Extract CRC - Packet length sent by host */
	Host_CMD_Packet_Len = Host_Buffer[0] + 1;
	Host_CRC32 = *((u32 *) ((Host_Buffer + Host_CMD_Packet_Len) - CRC_TYPE_SIZE_BYTE));

	/*CRC Verification */
	if (CRC_PASSED == Bootloader_CRC_Verify( (u8 *)Host_Buffer, (Host_CMD_Packet_Len - 4), Host_CRC32))
	{
		Bootloader_Send_ACK(12);
		Bootloader_Send_Data_To_Host(Bootloader_Support_CMD, 12);
	}
	else
	{
		Bootloader_Send_NACK();
	}
}

static void Bootloader_Get_Chip_Identification_Number ( u8 *Host_Buffer)
{
#ifdef BL_DEBUG_INFO
	BL_Print_Message ("Read the MCU chip identification number ..\n");
#endif
	u16 Host_CMD_Packet_Len = 0;
	u32 Host_CRC32 = 0;
	u16 Mcu_Identification_Num = 0;

	/*Extract CRC - Packet length sent by host */
	Host_CMD_Packet_Len = Host_Buffer[0] + 1;
	Host_CRC32 = *((u32 *) ((Host_Buffer + Host_CMD_Packet_Len) - CRC_TYPE_SIZE_BYTE));

#define DBGMCU_IDCODE   *((volatile u32 *) (0xE0042000))

	Mcu_Identification_Num = (u16)((DBGMCU_IDCODE) & (0x00000FFF));

	/*CRC Verification */
	if (CRC_PASSED == Bootloader_CRC_Verify( (u8 *)Host_Buffer, (Host_CMD_Packet_Len - 4), Host_CRC32))
	{
		/*Get MCU chip identification number*/
		/*Send MCU chip identification number To Host*/
		Bootloader_Send_ACK(2);
		Bootloader_Send_Data_To_Host((u8 *)&Mcu_Identification_Num, 2);
	}
	else
	{
		Bootloader_Send_NACK();
	}


}

static void Bootloader_Jumb_To_Address         	      ( u8 *Host_Buffer)
{
#ifdef BL_DEBUG_INFO
	BL_Print_Message ("Jumb bootloader to specific address...\n");
#endif

	u16 Host_CMD_Packet_Len = 0;
	u32 Host_CRC32 = 0;
	u32 Host_Jumb_Address = 0;
	Address_VerifyType Host_Address_Status = ADDRESS_IS_INVALID;

	/*Extract CRC - Packet length sent by host */
	Host_CMD_Packet_Len = Host_Buffer[0] + 1;
	Host_CRC32 = *((u32 *) ((Host_Buffer + Host_CMD_Packet_Len) - CRC_TYPE_SIZE_BYTE));


	/*CRC Verification */
	if (CRC_PASSED == Bootloader_CRC_Verify( (u8 *)Host_Buffer, (Host_CMD_Packet_Len - 4), Host_CRC32))
	{
		/*ACK For CRC Passed*/
		Bootloader_Send_ACK(1);	
		/*Get Host_Jumb_Address From Packet "Host_Buffer"*/
		Host_Jumb_Address = *((u32 *)&Host_Buffer[2]);
		
		/*Verify  Host Jumb Address*/
		Host_Address_Status = Bootloader_Address_Verify( Host_Jumb_Address );
		if (ADDRESS_IS_VALID == Host_Address_Status)
		{
			/*report host Verifying Address_Status */
			Bootloader_Send_Data_To_Host((u8 *)Host_Address_Status, 1);
			void (*MainApp) (void) = (void *) (Host_Jumb_Address +1 );
#ifdef BL_DEBUG_INFO
	BL_Print_Message ("Jumb to 0x%X\n", MainApp);
#endif
			MainApp();
		}
		else
		{
			Bootloader_Send_Data_To_Host((u8 *)Host_Address_Status, 1);			
		}
		
	}
	else
	{
		Bootloader_Send_NACK();
		Bootloader_Send_Data_To_Host((u8 *)Host_Address_Status, 1);			
	}


	
}

static void   Bootloader_Erase_Flash ( u8 *Host_Buffer)
{
	/*************************************************************************
	 ****************************BOOTLOADER PACKET (8Byte)***********************
	 *******|    1     |   2  | 	3 	|    4     | 5 | 6 | 7 | 8 |***************
	 *******|Packet_Len|CMD_ID|PageNum|NumOfPages|CRC|CRC|CRC|CRC|***************
	 ***************************************************************************/
#ifdef BL_DEBUG_INFO
			BL_Print_Message ("Mass erase or sectors erase of the user flash...\n");
#endif

	u16 Host_CMD_Packet_Len = 0;
	u32 Host_CRC32 = 0;
	u8 Erase_Status = INVALID_PAGE_NUMBER ;

	/*Extract CRC - Packet length sent by host */
	Host_CMD_Packet_Len = Host_Buffer[0] + 1;
	Host_CRC32 = *((u32 *) ((Host_Buffer + Host_CMD_Packet_Len) - CRC_TYPE_SIZE_BYTE));
	
	/*CRC Verification */
	if (CRC_PASSED == Bootloader_CRC_Verify( (u8 *)Host_Buffer, (Host_CMD_Packet_Len - 4), Host_CRC32))
	{
		/*ACK For CRC Passed*/
		Bootloader_Send_ACK(1);	
		
		
		Erase_Status =	Bootloader_Preforn_Flash_Erase(Host_Buffer[2], Host_Buffer[3]);
		if (SUCCESSFUL_ERASE == Erase_Status)
		{
	//		report host Erasing page Status 
			Bootloader_Send_Data_To_Host( &Erase_Status, 1);			
#ifdef BL_DEBUG_INFO
			BL_Print_Message ("Successful Erase...\n");
#endif
		
		}
		else 
		{
//			report host Erasing page Status 
				Bootloader_Send_Data_To_Host(&Erase_Status, 1);			

		}
	}
	else 
	{
	Bootloader_Send_NACK();		
	}
}

static void Bootloader_Memory_Write ( u8 *Host_Buffer)
{
	
#ifdef BL_DEBUG_INFO
			BL_Print_Message ("Write data on memory of the MCU...\n");		
#endif

	u16 							 Host_CMD_Packet_Len  = 0;
	u32 						   Host_CRC32           = 0;
	u32 							 Host_Address 				= 0;
	u16  							 Payload_len					= 0;
	Address_VerifyType Address_State 				= ADDRESS_IS_INVALID;
	u8 	Flash_Payload_Write_Status          = FLASH_PAYLOAD_WRITE_FAILED;
	u32 							 Start_Address				= 0;
	/*Extract CRC - Packet length sent by host */
	Host_CMD_Packet_Len = Host_Buffer[0] + 1;
	Host_CRC32 = *((u32 *) ((Host_Buffer + Host_CMD_Packet_Len) - CRC_TYPE_SIZE_BYTE));
	
	/*CRC Verification */
	if (CRC_PASSED == Bootloader_CRC_Verify( (u8 *)Host_Buffer, (Host_CMD_Packet_Len - 4), Host_CRC32))
	{
		/*ACK For CRC Passed*/
		Bootloader_Send_ACK(1);
		/*Extract the start address (4byte) from the Host Packet*/
		Host_Address = *((u32*)(&Host_Buffer[2]));
		Address_State = Bootloader_Address_Verify(Host_Address);
		if (Address_State == ADDRESS_IS_VALID)
		{
			Payload_len = Host_Buffer[6];
			Start_Address = Host_Address +(Flashing_Flag*(Payload_len *2))  ;

			Flash_Payload_Write_Status = Bootloader_Flash_Write_Payload ((u8 *)&Host_Buffer[7],Start_Address, Payload_len);
	    Flashing_Flag++;
			if (Flash_Payload_Write_Status == FLASH_PAYLOAD_WRITE_PASSED)
			{
			Bootloader_Send_Data_To_Host((u8 *)&Flash_Payload_Write_Status , 1);		
			}
			else 
			{
				Bootloader_Send_Data_To_Host((u8 *)&Flash_Payload_Write_Status , 1);	
			}
		}
		else 
		{
				Bootloader_Send_Data_To_Host((u8 *)&Address_State , 1);	
		}
			
	}
	else
	{
	Bootloader_Send_NACK();		
	Bootloader_Send_Data_To_Host((u8 *)&Address_State , 1);		
	}		
		
	
}

/**********************************************************************************************************************
 *  END OF FILE: FileName.c
 *********************************************************************************************************************/
