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

#ifndef TJA115X_Cfg_H
#define TJA115X_Cfg_H
//****************************************************************************
// @Project Includes
//****************************************************************************
#include "Std_Types.h"

//****************************************************************************
// @Macros
//****************************************************************************
#define Table_Ruler_Num  6
//Max 16 rulers                        
//    FET    TWL/BBL1       CANID  (4Highbit TWL/BBL2  4LowBit:StdCan2/Mask(0:stdid2 1:MASK ) StdID/CANMASK(0:be compared 1:ignored)  											
#define WhiteAnBlakeTable \
	{0,		1,      0x300,      				1,			  	0xff},\
	{0,		2,		0x400,						1,				0xff},\
	{0,		3,		0x200,						1,				0xff},\
	{0,		3,      0x101,     					0x30,			0x102},\
	{1,		3,		0x400,						1,				0xff},\
	{1,		3,		0x300,						0,				0}

//notice must seq ruleer :StdID(Need Mask) -> StdID(No Mask) -> ExtID(NeedMask) ->ExtID(NoMask) 

//safety config
#define TMPE	1 	// 0-1 Tamper Protection Enable bit 0:disable(default) 1:enable 
#define	FloodProtectionEn	1  //0:diable(default) 1:enable
#if FloodProtectionEn 
#define	FLDTH  	0  //0-1Threshole Limit  0:std ox600 extid 0x18000000)default 1:std 0x500 extid 0x14000000
#define	FLDMAX	2  //0-7 (vALUE/(1-FLDL)    DurationTime (0=default no flood)
#define	FLDL	1 // 0-3 0:3.125%  1:6.250%  2: 12.500%  3:25.00%(default)
#endif


//setting config
#define CONFIG_EN_TO  1//0-3 config enable time out 0:1s 1:2s(default) 2:4s 3:8s
#define BBLM	1	//0-1 0:local node does not rec the blacklisted frame(default) 1:local node does rec the blacklisted frame
#define	LGEF	0   //0-1 0:normal len of error frame(6bit)(default)  1:Long error frame(11bit)
#define SEC_TO  1   // 0-3 Secure mode timeout 0:0.5s 1:2s(default) 2:8s 3:auto(1time 0.5 2 time 2 3time ..8s)
#define	CAN_SP  2   //0-2  define sample point 0:70%  1:75%  2:80%(default) 3:Reserved
#define CANFD_SP 2  // 0-2 define sample point DATA FHASE 
#define CAN_CANFD_BR 0b1011// enum(4bit) 00XX:125kbps 0101:250/500kbps 0110:500/1000kps 1011:500/2000kps(default)
#define CONFIGID	0X18DA00F1//(default)
#define LeavingConfigMode 0 //0:Development without lock ,no MTP 1:Into NVM ,But set "Open lock bit"  2:into NVM and set "full lock bit"
//****************************************************************************
// @Typedefs emum
//****************************************************************************




//****************************************************************************
// @Typedefs Structures
//****************************************************************************
typedef struct 
{
	uint8 FrameFormat;
	uint8 TWBL;
	uint32 CANID; 	
	uint8  STD2Mask;
	uint32 CanMaskID;
}TJA115X_RulerTableType;



/******************************************************************************/
/*-------------------------Global Variable Prototypes-------------------------*/
/******************************************************************************/ 
 

/******************************************************************************/
/*-------------------------Global Function Prototypes-------------------------*/
/******************************************************************************/

#endif
