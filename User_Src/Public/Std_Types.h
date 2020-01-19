/****************************************************************************
* 						Copyright (C) Yogomo Motor Corp, 2016               *                     
*****************************************************************************
* File name:       Std_Types.h                                              
* Discreption:                                                              
* Current version: V1.0                                                     
* Data:            16th July, 2016                                          
* Comment:                                                                  
*****************************************************************************
*                            Change Record                                 
*---------------------------------------------------------------------------
*  No.   Author      Data             Inf.                                    
*---------------------------------------------------------------------------
*  1.0   yinheng    13th July, 2016   Initial Release.    
****************************************************************************/

#ifndef STD_TYPES_H
#define STD_TYPES_H

//****************************************************************************
// @Project Includes
//****************************************************************************
#include "Platform_Types.h"

//****************************************************************************
// @Typedefs
//****************************************************************************
typedef uint8 Std_ReturnType;

#ifndef STATUSTYPEDEFINED
#define STATUSTYPEDEFINED
#define E_OK   		0x00u
typedef uint8 StatusType;  //OSEK Compliance
#endif
#define E_NOT_OK   	0x01u

#define STD_HIGH  	0x01u
#define STD_LOW		0x00u

typedef struct
{
	uint16 vendorID;
	uint16 moduleID;
	uint8  sw_major_version;
	uint8  sw_minor_version;
	uint8  sw_path_version;
}Std_VersionInfoType;

typedef enum
{
	STD_OFF = 0, 
	STD_ON  = 1
}Std_EnableType;



#endif /* STD_TYPES_H */
