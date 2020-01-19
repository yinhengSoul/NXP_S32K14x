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
#include "ComTo4G_APP.h"
#include "CanRec.h"


//****************************************************************************
// @Macros
//****************************************************************************
#define  VendorID   0
#define  ModuleID 	0
	
#define  Sw_major_version	0
#define  Sw_minor_version   0
#define  Sw_path_version	1

#define ComAppUseMailBox   11

#define WarningLedFlashNum  5
#define WorkingLedFlashNum  5



/*****************************************************************************
 Typedef definitions
*****************************************************************************/
typedef struct
{
	uint8 Row;//行坐标表示第几字节
	uint8 Column;//列坐标表示帝即位
}BitPosition;


/*****************************************************************************
 Imported global variables and functions (from other files)
*****************************************************************************/


/*****************************************************************************
 Exported global variables and functions (to be accessed by other files)
*****************************************************************************/
uint8 CrashEventFlag = 0;
uint8 ECallBtnEventFlag = 0;

extern uint8 LED_Flash;



/*****************************************************************************
 Private variables and functions
*****************************************************************************/
static void GetApuFromDataStream(uint8* data,uint16 len,APU_FrameType* Apu);
static uint8 CheckApuLen(uint8* data,uint16 len);
static uint8 ComTo4GAPPTranReq(APU_FrameType APU);
static BitPosition GetBitPosition_Moto(uint8 BitNum);
static uint8 GetMotoCalcStartBit(uint8 StartBit);
static uint8 JudgeMaskCode_Moto(uint8 StartBit,uint8 len,uint8* maskcode);// return : 0:do not care 1:need judge

static void ThrouCanFrameToCom(StCanMsg_Type RecCanFrame);
static void DID5501TranProc();
static void DID5503TranProc();


const static uint8 ByteMask[8] = {0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};

static uint8 TranDataStream[120];

static uint8 His_WarningledAction;
static uint8 His_WorkingledAction;



static LED_ActionHashTableType WarningLed[WarningLedFlashNum];// 0: off 1: slow flsh 2: normal flash 3:quick f;ash 4: on
static LED_ActionHashTableType WorkingLed[WorkingLedFlashNum];// 0: off 1: slow flsh 2: normal flash 3:quick f;ash 4: on


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
void ComTo4GAPP_GetVersionInfo(Std_VersionInfoType *VersionInfo)
{
	VersionInfo->vendorID = VendorID;
	VersionInfo->moduleID = ModuleID;
	VersionInfo->sw_major_version = Sw_major_version;
	VersionInfo->sw_minor_version = Sw_minor_version;
	VersionInfo->sw_path_version  = Sw_path_version;
}

static BitPosition GetBitPosition_Moto(uint8 BitNum)
{
	BitPosition Temp;
	Temp.Row = BitNum / 8;
	Temp.Column = BitNum % 8;
	return Temp;
}

static uint8 GetMotoCalcStartBit(uint8 StartBit)
{
	uint8 row = StartBit / 8;
	uint8 col = StartBit % 8;
	return(row*8 + (7-col));
}


// return : 0:do not care 1:need judge
static uint8 JudgeMaskCode_Moto(uint8 StartBit,uint8 len,uint8* maskcode)
{
	BitPosition Temp;
	uint8 CalcStartBit;
	int i;
	CalcStartBit = GetMotoCalcStartBit(StartBit);
	for(i=0;i<len;i++)
	{
		Temp = GetBitPosition_Moto(CalcStartBit);
		if(maskcode[Temp.Row]&ByteMask[Temp.Column])
		{
			CalcStartBit --;
		}
		else
			return 0;
	}
	return 1;
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
static void GetApuFromDataStream(uint8* data,uint16 len,APU_FrameType* Apu)
{
	if(CheckApuLen(data,len) == 1)
	{
		Apu->Apu_len = len;
		Apu->Apu_Did = data[2];
		Apu->Apu_Did = ((Apu->Apu_Did)<<8)+data[3];
		Apu->Apu_Data = data+4;
	}
	else
	{
		Apu->Apu_len = 0;
		Apu->Apu_Did = 0xffff;
	}
	
}

static uint8 CheckApuLen(uint8* data,uint16 len)
{
	uint16 tempLen;
	tempLen = data[0];
	tempLen = (tempLen<<8)+data[1];
	if(tempLen == len)
		return 1;
	else
		return 0;

}

static void GetDataStream(APU_FrameType APU)
{
	uint16 i;
	TranDataStream[0] = (APU.Apu_len>>8);
	TranDataStream[1] = (APU.Apu_len);
	TranDataStream[2] = (APU.Apu_Did>>8);
	TranDataStream[3] = (APU.Apu_Did);
	for(i=0;i<(APU.Apu_len-4);i++)
		TranDataStream[4+i] = APU.Apu_Data[i];
}


/*
*********************************************************************************************************
*
* Description:
*
* Arguments  :
*
* Returns    :0:Req Success 1: busy   2:WaitingForResp  3:Response OverTime 
*
* Notes:
*
*********************************************************************************************************
*/
static uint8 ComTo4GAPPTranReq(APU_FrameType APU)
{
	uint8 Ret;
	Ret = ComTo4GTpGetTranSts();
	if(Ret == 0)
	{
		GetDataStream(APU);
		Ret = ComTo4GTpTranReq(TranDataStream,APU.Apu_len);
		return Ret;
	}
	else
		return Ret;

}


/*
*********************************************************************************************************
*
* Description:
*
* Arguments  :
*
* Returns    :0:Req Success 1: busy   2:WaitingForResp  3:Response OverTime 
*
* Notes:
*
*********************************************************************************************************
*/
static uint8 JudgeCurFlash(LED_ActionHashTableType* Prepare,uint8 PrepareNum)
{
	uint8 i=0;
	uint8 LedAction =0;
	for(i=0;i<PrepareNum;i++)
	{
		if(Prepare[i].ValidFlag)
		{
			LedAction = i;
		}
	}
	return LedAction;
}

static void ClearHashTable(LED_ActionHashTableType* Prepare,uint8 PrepareNum) 
{
	uint8 i;
	for(i=0;i<PrepareNum;i++)
	{
		Prepare[i].ValidFlag = 0;
		Prepare[i].DurationTime = 0;
	}
}


void WarningLEDIndProc()
{
	uint8 i;
	static uint16 ProcCout=0;
	
	uint8  WarningledAction = 0;
	WarningledAction = JudgeCurFlash(WarningLed,WarningLedFlashNum);
	if(His_WarningledAction !=WarningledAction )
	{
		His_WarningledAction = WarningledAction;
		LED_Red_Off;
		ProcCout = 0;
	}
	
	switch(WarningledAction)
	{
		case 0: //off
			LED_Red_Off;
			break;
		case 1: // 3s - on  3s -off
			if(ProcCout==3000)
			{
				LED_Red_Toggle;
			}	
			break;
		case 2: // 1s - on  1s - off
			if(ProcCout%1000 == 0)
			{
				LED_Red_Toggle;
			}
			break;
		case 3: //200ms - on 200ms - off
			if(ProcCout%200==0)
			{
				LED_Red_Toggle;
			}
			break;
		case 4:
			LED_Red_On;
			break;
		default:
			break;
	}

	for(i=0;i<WarningLedFlashNum;i++)
	{
		if((WarningLed[i].ValidFlag)&&(WarningLed[i].DurationTime<0xffffffff))
		{
			if(WarningLed[i].DurationTime)
				WarningLed[i].DurationTime--;
			else
				WarningLed[i].ValidFlag = 0;
		}
	}

	if(ProcCout<3000)
	{
		ProcCout++;
	}
	else
		ProcCout = 1;
	
}

void WorkingLEDIndProc()
{
	uint8 i;
	static uint16 ProcCout=0;
	
	uint8  WorkingledAction = 0;
	WorkingledAction = JudgeCurFlash(WorkingLed,WorkingLedFlashNum);
	if(His_WorkingledAction !=WorkingledAction )
	{
		His_WorkingledAction = WorkingledAction;
		LED_Green_Off;
		ProcCout = 1;
	}
	
	switch(WorkingledAction)
	{
		case 0: //off
			LED_Green_Off;
			break;
		case 1: // 3s - on  3s -off
			if(ProcCout==3000)
			{
				LED_Green_Toggle;
			}	
			break;
		case 2: // 1s - on  1s - off
			if(ProcCout%1000==0)
			{
				LED_Green_Toggle;
			}
			break;
		case 3: //200ms - on 200ms - off
			if(ProcCout%200==0)
			{
				LED_Green_Toggle;
			}
			break;
		case 4:
			LED_Green_On;
			break;
		default:
			break;
	}

	for(i=0;i<WorkingLedFlashNum;i++)
	{
		if((WorkingLed[i].ValidFlag)&&(WorkingLed[i].DurationTime<0xffffffff))
		{
			if(WorkingLed[i].DurationTime)
				WorkingLed[i].DurationTime--;
			else
				WorkingLed[i].ValidFlag = 0;
		}
	}

	if(ProcCout<3000)
	{
		ProcCout++;
	}
	else
		ProcCout = 1;

	if((His_WarningledAction == 0)&&(His_WorkingledAction==0))
		LED_Flash = 0;
	else
		LED_Flash = 1;
	
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
void ComTo4GAPP_RecTask()
{
	uint8	RecRet;
	uint8	Rec_Data[200];
	uint16	Reclen;
	uint16  RecAPUDataLen;
	uint8 i;
	can_message_t sendcan;
	APU_FrameType Apu_temp;

	uint32 Can_id;
	uint8 DLC;

	uint8 LedType;
	uint8 LedAction;
	uint32 LedDuration;

	RecRet = ComTo4GTpRecDataStream(Rec_Data,&Reclen);
	if(RecRet ==1)
	{
		GetApuFromDataStream(Rec_Data,Reclen,&Apu_temp);
		if(Apu_temp.Apu_len<4)
			return;
		RecAPUDataLen = Apu_temp.Apu_len-4;

		switch(Apu_temp.Apu_Did)
		{
			case 0x5501:
				if(RecAPUDataLen<3)
					return;
				sendcan.id = (Apu_temp.Apu_Data[0]<<8) + Apu_temp.Apu_Data[1];
				sendcan.length = Apu_temp.Apu_Data[2];
				if(sendcan.length<=8)
				{
					for(i=0;i<sendcan.length;i++)
						sendcan.data[i] = Apu_temp.Apu_Data[3+i];

					if(EnableTranCan0)
					{
						status_t retValue; 
						retValue = Canif_Write(&can_pal1_instance,ComAppUseMailBox,CAN_MSG_ID_STD,sendcan);
						if(retValue == STATUS_BUSY)
							Canif_AbortTranMailbox(&can_pal1_instance,ComAppUseMailBox);
					}
				}
				break;
			case 0x5502:
				if(RecAPUDataLen<3)
					return;
				Can_id = Apu_temp.Apu_Data[0];
				RecAPUDataLen--;
				Can_id = (Can_id<<8) + Apu_temp.Apu_Data[1];
				RecAPUDataLen--;
				DLC = Apu_temp.Apu_Data[2];
				RecAPUDataLen--;
				if((Can_id == 0x3e0)&&(DLC==8))
				{
					uint8	CanData[8];
					uint8	CanDataMsk[8];
					for(i = 0;i<8;i++)
						CanData[i] = Apu_temp.Apu_Data[3+i];
					for(i = 0;i<8;i++)
						CanDataMsk[i] = Apu_temp.Apu_Data[11+i];

					if(JudgeMaskCode_Moto(7,1,CanDataMsk))
						TboxMsgSI.TBOX_RemtVehSearchCmd =  CanData[0]>>7;
					if(JudgeMaskCode_Moto(8,2,CanDataMsk))
						TboxMsgSI.TBOX_ACRemtCtrlCmd = CanData[1]&0x3;
					if(JudgeMaskCode_Moto(12,2,CanDataMsk))
						TboxMsgSI.TBOX_StartReq_T = (CanData[1]>>4)&0X3;
					if(JudgeMaskCode_Moto(14,2,CanDataMsk))
						TboxMsgSI.TBOX_StopReq_T = (CanData[1]>>6)&0x3;
					if(JudgeMaskCode_Moto(16,1,CanDataMsk))
						TboxMsgSI.TBOX_AppointmentChargeSts = (CanData[2]&0x1);
					if(JudgeMaskCode_Moto(18,2,CanDataMsk))
						TboxMsgSI.TBOX_ACMode = (CanData[2]>>2)&0x3;
					if(JudgeMaskCode_Moto(20,4,CanDataMsk))
						TboxMsgSI.TBOX_ACParameter = (CanData[2]>>4)&0xf;
					if(JudgeMaskCode_Moto(24,5,CanDataMsk))
						TboxMsgSI.TBOX_RunMode = (CanData[3])&0x1f;
					if(JudgeMaskCode_Moto(5,2,CanDataMsk))
						TboxMsgSI.TBOX_EngineStartupInhibitSts = (CanData[0]>>5)&0x3;
				}
				break;
			case 0x5504:
				if(RecAPUDataLen<2)
					return;
				LedType = Apu_temp.Apu_Data[0];
				RecAPUDataLen--;
				LedAction =  Apu_temp.Apu_Data[1];
				RecAPUDataLen--;
				if(RecAPUDataLen)
				{	
					if(Apu_temp.Apu_Data[2]!=0)
						LedDuration= (uint32)(Apu_temp.Apu_Data[2]*1000);
					else
						LedDuration = 0xffffffff;
				}
				else
					LedDuration = 0xffffffff;
				if(LedType == 0)//warning led
				{
					if(LedAction==0) //Req off
					{
						ClearHashTable(WarningLed,WarningLedFlashNum);
						WarningLed[LedAction].ValidFlag = 1;
						WarningLed[LedAction].DurationTime = LedDuration;
						
					}
					else if(LedAction<WarningLedFlashNum)
					{
						WarningLed[LedAction].ValidFlag = 1;
						WarningLed[LedAction].DurationTime = LedDuration;
					}
				}
				else if(LedType == 1)//working led
				{
					if(LedAction==0) //Req off
					{
						ClearHashTable(WorkingLed,WorkingLedFlashNum);
						WorkingLed[LedAction].ValidFlag = 1;
						WorkingLed[LedAction].DurationTime = LedDuration;
						
					}
					else if(LedAction<WorkingLedFlashNum)
					{
						WorkingLed[LedAction].ValidFlag = 1;
						WorkingLed[LedAction].DurationTime = LedDuration;
					}
				}
				break;
			default:
					break;
		}
			
	}

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
void ComTo4GAPP_TranTask()
{
	uint8 Ret;
	Ret = ComTo4GTpGetTranSts();
	if(Ret == 0)
	{
		DID5501TranProc();
		DID5503TranProc();
	}
}

static void DID5501TranProc()
{
	if(Msg1_Frame.changeFlag)
	{
		Msg1_Frame.changeFlag = 0;
		ThrouCanFrameToCom(Msg1_Frame);
	}
	else if(Msg2_Frame.changeFlag)
	{
		Msg2_Frame.changeFlag = 0;
		ThrouCanFrameToCom(Msg2_Frame);
	}
	else if(Msg3_Frame.changeFlag)
	{
		Msg3_Frame.changeFlag = 0;
		ThrouCanFrameToCom(Msg3_Frame);
	}
	else if(Msg4_Frame.changeFlag)
	{
		Msg4_Frame.changeFlag = 0;
		ThrouCanFrameToCom(Msg4_Frame);
	}
	else if(Msg5_Frame.changeFlag)
	{
		Msg5_Frame.changeFlag = 0;
		ThrouCanFrameToCom(Msg5_Frame);
	}
	else if(PEPS1_Frame.changeFlag)
	{
		PEPS1_Frame.changeFlag = 0;
		ThrouCanFrameToCom(PEPS1_Frame);
	}
}

static void DID5503TranProc()
{
	static uint8 HisCrashSI =0;
	uint8 flag = JudgeChange(&HisCrashSI,&Msg2_SI.VehicleCrashSts,1);
	if(flag)
	{
		HisCrashSI = Msg2_SI.VehicleCrashSts;
		if( Msg2_SI.VehicleCrashSts == 2)
			CrashEventFlag = 1;
	}
	if((CrashEventFlag)||(ECallBtnEventFlag))
	{
		uint8 SendStream[1];
		APU_FrameType APU_Frame;
		APU_Frame.Apu_Did = 0x5503;
		APU_Frame.Apu_len = 4+1;
		APU_Frame.Apu_Data = SendStream;
		SendStream[0] = (CrashEventFlag&0x1) +((ECallBtnEventFlag&0x1)<<1);
		ComTo4GAPPTranReq(APU_Frame);
		CrashEventFlag = 0;
		ECallBtnEventFlag = 0;
	}
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
static void ThrouCanFrameToCom(StCanMsg_Type RecCanFrame)
{
	uint8 SendStream[20];
	uint8 i;
	APU_FrameType APU_Frame;
	APU_Frame.Apu_Did = 0x5501;
	APU_Frame.Apu_len = RecCanFrame.length + 7;
	APU_Frame.Apu_Data = SendStream;
	SendStream[0] = (RecCanFrame.id)>>8;
	SendStream[1] = (RecCanFrame.id);
	SendStream[2] = RecCanFrame.length;
	for(i=0;i<RecCanFrame.length;i++)
	{
		SendStream[3+i] = RecCanFrame.data[i];
	}
	ComTo4GAPPTranReq(APU_Frame);
}









