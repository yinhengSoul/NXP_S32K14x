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

#ifndef ComTo4G_TP_H
#define ComTo4G_TP_H
//****************************************************************************
// @Project Includes
//****************************************************************************
#include "Std_Types.h"
#include "ComTo4G_Link.h"

//****************************************************************************
// @Macros
//****************************************************************************
#define StartSymbolNum 		2
#define StartSymbolValue1 	0x23
#define StartSymbolValue2   0x23 

#define RecOverTimeSet	  10 //  overtime 30ms  waiting Nwxt Byte

#define TxWaitingOverTimeSet	  60 //  overtime 60 ms  for 2ms Period


#define TpSendDataStreamLenMax	120
#define TpRecDataStreamLenMax	120

#define TpRecPDUNumMax			5
#define TpRecDataFieldNum		120	

 
//****************************************************************************
// @Typedefs emum
//****************************************************************************
typedef enum
{
	 No_Encrypt = 1,
	 RSA   	= 2,
	 AES128 = 3,
	 DES =  4
}DataEncryptType;



//****************************************************************************
// @Typedefs Structures
//****************************************************************************
//typedef uint8 	ColourType;

typedef struct
{
	uint8 	StartSymbol[StartSymbolNum];
	uint8 	RollingCounter;
	uint8   FrameType;//5a:dataFrame  aa:response frame
	uint8 	DataEncrptFormat;
	uint16 	DataFiledLen;
	uint8* 	Datafield;
	uint8	CheckSum;
}PDU_FrameType;



/******************************************************************************/
/*-------------------------Global Variable Prototypes-------------------------*/
/******************************************************************************/ 
 

/******************************************************************************/
/*-------------------------Global Function Prototypes-------------------------*/
/******************************************************************************/
// Ref Manage
void ComTo4GTp_GetVersionInfo(Std_VersionInfoType *VersionInfo);
void ComTo4GTpModuleInit();

void ComTo4GTranTpMain();
void ComTo4GRecTpMain();


//for APP
void ComTo4GTpRestart();
uint8 ComTo4GTpGetTranSts();
void ComTo4GTpClearTranSts();

uint8 ComTo4GTpTranReq(uint8* data,uint16 len);
uint8 ComTo4GTpRecDataStream(uint8* recData,uint16* len);





#endif
