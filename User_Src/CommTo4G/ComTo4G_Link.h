/**
 * \file .h
 * \brief  functionality
 * \ingroup 
 *
 * \version V1.0
 * \copyright Copyright (c) 2016 YOGOMO Motor. All rights reserved.
 *
 *
 *                                 IMPORTANT NOTICE
 *
 *
 * THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 * OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 * INFINEON SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
 * OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 *
 */

#ifndef ComTo4G_Link_H
#define ComTo4G_Link_H
//****************************************************************************
// @Project Includes
//****************************************************************************
#include "Std_Types.h"
#include "uart_pal.h"
#include "uart_pal1.h"
#include "Usr_Queue.h"



//****************************************************************************
// @Macros
//****************************************************************************
#define LinkRecDataLenMax	200

#define  COM_Asc  1
//****************************************************************************
// @Typedefs emum
//****************************************************************************




//****************************************************************************
// @Typedefs Structures
//****************************************************************************
//typedef uint8 	ColourType;







/******************************************************************************/
/*-------------------------Global Variable Prototypes-------------------------*/
/******************************************************************************/ 



/******************************************************************************/
/*-------------------------Global Function Prototypes-------------------------*/
/******************************************************************************/
// Ref Manage
void ComTo4GLink_GetVersionInfo(Std_VersionInfoType *VersionInfo);

void ComTo4GLinkModuleInit();
void SetComFormatASCII();
void SetComFormatHex();
//For Tp Layer
uint8 ComTo4GLinkSendData(uint8* tranData ,uint16 len);
uint16 ComTo4GLinkRecData(uint8* recData,uint16 len);
void ComTo4GLinkClearRecBuffer();
void ComTo4GLinkAbortTraning();
uint8 ComTo4GLinkGetTranSts(uint32* bytesRemaining);



//For Phsical Driver
uint8 ComTo4GDriverWriteData(uint8* data ,uint16 len);


#endif
