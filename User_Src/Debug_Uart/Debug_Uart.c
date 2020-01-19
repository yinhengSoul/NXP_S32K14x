/**
* \file    c
* \brief   functionality
*
* \version V1.0
* \copyright Copyright (c) 2016 YOGOMO Motor. All rights reserved.
* \by  yinheng 
*/
/* 
 	notice:
*/
/*****************************************************************************
 Includes   <System Includes> , "Project Includes"
 *****************************************************************************/
#include "Debug_Uart.h"
#include "uart_pal.h"
#include "uart_pal2.h"

//****************************************************************************
// @Macros
//****************************************************************************
#define  VendorID   0
#define  ModuleID 	0
	
#define  Sw_major_version	0 
#define  Sw_minor_version   0
#define  Sw_path_version	1	

/*****************************************************************************
 Typedef definitions
*****************************************************************************/


/*****************************************************************************
 Imported global variables and functions (from other files)
*****************************************************************************/


/*****************************************************************************
 Exported global variables and functions (to be accessed by other files)
*****************************************************************************/


/*****************************************************************************
 Private variables and functions
*****************************************************************************/



/*
*********************************************************************************************************
*                                      
* Description: 
*
* Arguments  : 
*             
* Returns    : 
*
* Notes:
*             
*********************************************************************************************************
*/	
void Debug_Uart_GetVersionInfo(Std_VersionInfoType *VersionInfo)
{
	VersionInfo->vendorID = VendorID; 
	VersionInfo->moduleID = ModuleID; 
	VersionInfo->sw_major_version = Sw_major_version; 
	VersionInfo->sw_minor_version = Sw_minor_version; 
	VersionInfo->sw_path_version  = Sw_path_version; 
}




/*
*********************************************************************************************************
*                                      
* Description: Send a string to user via UART
*
* Arguments  : sourceStr: pointer to the array of characters
*              		that you wish to send.
* Returns    :  None
*
* Notes:
*             
*********************************************************************************************************
*/	
void print(const char *sourceStr)
{
	uint32 bytesRemaining;

	//Add Your Own Code
    /* Send data via LPUART */
	UART_SendData(&uart_pal2_instance, (uint8 *) sourceStr, strlen(sourceStr));
    /* Wait for transmission to be successful */
    while (UART_GetTransmitStatus(&uart_pal2_instance, &bytesRemaining) != STATUS_SUCCESS){};
	//end code

}


/*
*********************************************************************************************************
*                                      
* Description: Rec a string from user via UART
*
* Arguments  : sourceStr: pointer to the array of characters
*              		that you wish to send.
* Returns    :  None
*
* Notes:
*             
*********************************************************************************************************
*/
status_t StartReceive(uint8* buffer,uint32 len, uint32_t timeout)
{
	status_t Ret;
	Ret = UART_ReceiveDataBlocking(&uart_pal2_instance,buffer, len,timeout);
	return Ret;
}

void DataProceess(uint8 *data,uint8* OutOpion)
{

	//Add Your Own Code
  /* Because the terminal appends new line to user data,
         * receive and store data into a buffer until it is received
         */
	
    /* Check for invalid options */
    if (data[0] < '1' || data[0] > '6')
    {
        print((const char *)"Invalid option!\r\n");
        *OutOpion = 0;
    }
    else
    {
        *OutOpion = data[0];
    }
	//end code
}








