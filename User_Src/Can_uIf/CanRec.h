/**
 * \file CanAPP.h
 * \brief PORT  basic functionality
 * \ingroup IO_Driver
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
 * \defgroup IO_Driver_PORT PORT
 * \ingroup IO_Driver
 * \defgroup IO_Driver_Port_Enum Enumerations
 * \ingroup IO_Driver_PORT
 * \defgroup IO_Driver_Port_DataStructures Data structures
 * \ingroup IO_Driver_PORT
 * \defgroup IO_Driver_Port_PinFunctions Single Pin Functions
 * \ingroup IO_Driver_PORT
 */

#ifndef CANREC_H
#define CANREC_H

//****************************************************************************
// @Project Includes
//****************************************************************************
#include "Std_Types.h"


//****************************************************************************
// @Macros
//****************************************************************************


//****************************************************************************
// @Typedefs enum
//****************************************************************************




//****************************************************************************
// @Typedefs Structures
//****************************************************************************
typedef struct
{
	uint8 	VehicleRunSt;
	uint8	VehiclePowerMode;
	uint8 	VehicleGearLeverPos;
	uint8	VehicleDCDCSt;
	uint32	IPC_Odometer;
	uint8	BMS_IsolationR_Sts;
	uint16	BMS_IsolationR_Neg;
	uint16	BMS_IsolationR_Pos;
}Msg1_Type;


typedef struct
{
	uint8 	BMS_ChargingSts;
	uint16	BMS_BattVolt;
	uint16 	BMS_BattCurr;
	uint8	BMS_SOC;
	uint16	BMS_RemainChrgTime;
	uint8	OBC_CCLineSts;
	uint8	VehicleCrashSts;
}Msg2_Type;

typedef struct
{
	uint8 	ABS_VehicleSpdValid;
	uint16	ABS_VehicleSpd;
}Msg3_Type;

typedef struct
{
	uint8 	BCM_DrvDoorSts;
	uint8   BCM_PassDoorSts;
	uint8 	BCM_RearLeftDoorSts;
	uint8   BCM_RearRightDoorSts;
	uint8 	BCM_TrunkDoorSts;
	uint8   BCM_VehAntitheftSts;
	uint8	BCM_DoorLockSts;
}Msg4_Type;

typedef struct
{
	uint8 	AC_WorkSts;
	uint8   AC_TempSetPoint;
	uint8 	AC_BlowerSpd;
	uint8   AC_AirDistribMode;
	uint8 	AC_AirCircMode;
}Msg5_Type;

typedef struct
{
	uint8 	PEPS_PowerMode;
	uint8	PEPS_EnginStartupInhibitSts;
	uint8 	PEPS_EnginStartupInhibitFeedBack;
}PEPS1_Type;


typedef struct
{
	uint8 	TBOX_CheckSum;
	uint8   TBOX_RemtVehSearchCmd;
	uint8 	TBOX_ACRemtCtrlCmd;
	uint8   TBOX_StartReq_T;
	uint8 	TBOX_StopReq_T;
	uint8 	TBOX_AppointmentChargeSts;
	uint8 	TBOX_ACMode;
	uint8 	TBOX_ACParameter;
	uint8 	TBOX_RunMode;
	uint8   TBOX_EngineStartupInhibitSts;
	uint8 	TBOX_RollingCounter;
}TboxMsg_Type;

typedef struct
{
	uint8  changeFlag;	
    uint32 id;       /*!< ID of the message */
    uint8 data[8];  /*!< Data bytes of the CAN message*/
    uint8 length;    /*!< Length of payload in bytes */
}StCanMsg_Type;



/******************************************************************************/
/*-------------------------Global Function Prototypes-------------------------*/
/******************************************************************************/
extern Msg1_Type Msg1_SI;
extern Msg2_Type Msg2_SI;
extern Msg3_Type Msg3_SI;
extern Msg4_Type Msg4_SI;
extern Msg5_Type Msg5_SI;
extern PEPS1_Type PEPS1_SI;

extern StCanMsg_Type Msg1_Frame;
extern StCanMsg_Type Msg2_Frame;
extern StCanMsg_Type Msg3_Frame;
extern StCanMsg_Type Msg4_Frame;
extern StCanMsg_Type Msg5_Frame;
extern StCanMsg_Type PEPS1_Frame;

extern StCanMsg_Type FreeMaster_RecFrame;



extern TboxMsg_Type TboxMsgSI;
extern uint8 OptFlash ;

extern uint8 BussOffFlag_CAN[2];
extern uint8 BussNoAck_CAN[2];



uint8 JudgeChange(uint8* HisData,uint8* NewData,uint16 len);

/******************************************************************************/
/*-------------------------for os-------------------------*/
/******************************************************************************/



#endif
