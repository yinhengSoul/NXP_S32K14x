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


#ifndef ComTo4G_APP_H
#define ComTo4G_APP_H
//****************************************************************************
// @Project Includes
//****************************************************************************
#include "ComTo4G_TP.h"
#include "Can_uif.h"
#include "Cpu.h"

#include "Pins_driver.h"


//****************************************************************************
// @Macros
//****************************************************************************
#define APU_RecBufferNum   1
#define APU_SendBufferNum  1


#define LED_Red_On    	PINS_DRV_WritePin(PTD,15,0)
#define LED_Red_Off    	PINS_DRV_WritePin(PTD,15,1)
#define LED_Red_St		(PINS_DRV_GetPinsOutput(PTD)>>15)&0X1
#define LED_Red_Toggle	LED_Red_St?LED_Red_On:LED_Red_Off

#define LED_Blue_On    	PINS_DRV_WritePin(PTD,0,0)
#define LED_Blue_Off    PINS_DRV_WritePin(PTD,0,1)
#define LED_Blue_St		(PINS_DRV_GetPinsOutput(PTD)>>0)&0X1
#define LED_Blue_Toggle	LED_Blue_St?LED_Blue_On:LED_Blue_Off

#define LED_Green_On    PINS_DRV_WritePin(PTD,16,0)
#define LED_Green_Off   PINS_DRV_WritePin(PTD,16,1)
#define LED_Green_St	(PINS_DRV_GetPinsOutput(PTD)>>16)&0X1
#define LED_Green_Toggle	LED_Green_St?LED_Green_On:LED_Green_Off


//****************************************************************************
// @Typedefs emum
//****************************************************************************




//****************************************************************************
// @Typedefs Structures
//****************************************************************************
//typedef uint8 	ColourType;
typedef struct
{
	uint16  Apu_len;
	uint16  Apu_Did;
	uint8*  Apu_Data;
}APU_FrameType;


typedef struct
{
	uint8 ValidFlag;
	uint32 DurationTime; //unit ms
}LED_ActionHashTableType;



/******************************************************************************/
/*-------------------------Global Variable Prototypes-------------------------*/
/******************************************************************************/ 
extern uint8 CrashEventFlag;
extern uint8 ECallBtnEventFlag;


/******************************************************************************/
/*-------------------------Global Function Prototypes-------------------------*/
/******************************************************************************/
// Ref Manage
void ComTo4GAPP_GetVersionInfo(Std_VersionInfoType *VersionInfo);
void ComTo4GAPP_RecTask();
void ComTo4GAPP_TranTask();

void WarningLEDIndProc();
void WorkingLEDIndProc();



#endif
