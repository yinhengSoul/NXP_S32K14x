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
#include "Fls.h"

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
void XX_GetVersionInfo(Std_VersionInfoType *VersionInfo)
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


