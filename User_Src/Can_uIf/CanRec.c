/**
 * \file CanApp.c
 * \brief Can  	Communication functionality
 *
 * \version V1.0
 * \copyright Copyright (c) 2016 YOGOMO Motor. All rights reserved.
 *
 */
 
 
/*****************************************************************************
 Includes   <System Includes> , "Project Includes"
 *****************************************************************************/
#include "Can_uif.h"
#include "CanRec.h"
#include "CanMailBox_Cfg.h"
#include "ComTo4G_APP.h"

/*****************************************************************************
 Typedef definitions
 *****************************************************************************/



/*****************************************************************************
 Macro definitions
 *****************************************************************************/       
#if FlexCan0_Use
	#if	FlexCan0_RxFIFO_USE
		static can_message_t  	RecMsgFifo_Can0[6];
	#endif
static can_message_t	RecMsg_Can0;
#endif

#if FlexCan1_Use
	#if	FlexCan1_RxFIFO_USE
		static can_message_t  	RecMsgFifo_Can1[6];
	#endif
static can_message_t	RecMsg_Can1;
#endif

#if FlexCan2_Use
	#if	FlexCan2_RxFIFO_USE
		static can_message_t  	RecMsgFifo_Can2[6];
	#endif
static can_message_t	RecMsg_Can2;
#endif


 /*****************************************************************************
 Imported global variables and functions (from other files)
 *****************************************************************************/
  
 
/*****************************************************************************
 Exported global variables and functions (to be accessed by other files)
*****************************************************************************/ 
Msg1_Type Msg1_SI;
Msg2_Type Msg2_SI;
Msg3_Type Msg3_SI;
Msg4_Type Msg4_SI;
Msg5_Type Msg5_SI;
PEPS1_Type PEPS1_SI;

StCanMsg_Type Msg1_Frame;
StCanMsg_Type Msg2_Frame;
StCanMsg_Type Msg3_Frame;
StCanMsg_Type Msg4_Frame;
StCanMsg_Type Msg5_Frame;
StCanMsg_Type PEPS1_Frame;

StCanMsg_Type FreeMaster_RecFrame;

TboxMsg_Type TboxMsgSI;
uint8 OptFlash ;

uint8 C4G_FirstSucFlag ;
#define Msg1Mask 0x1
#define Msg2Mask 0x2
#define Msg3Mask 0x4
#define Msg4Mask 0x8
#define Msg5Mask 0x10
#define PEPS1Mask 0x20


uint8 BussOffFlag_CAN[2];
uint8 BussNoAck_CAN[2];

/* For Rec */



/*****************************************************************************
 Private variables and functions
 *****************************************************************************/
//static CAN_Type * const g_canBase[] = CAN_BASE_PTRS;
static void MailboxRecProcess_Can0();


/*
*********************************************************************************************************
*
* Description:  Start recMailbox as RuletableIndex element
*
* Arguments  : 	Hth: can_module 0-2
*				RuleIndexType: RuleTable_Index in Can_uif_cfg.h

* Returns    :  Success or error
*
* Notes: use  SDK can_pal Component
*
*********************************************************************************************************
*/
static status_t Canif_CfgRecMailBox(const can_instance_t* const Hth,CanRule_ConfigType CanRule)
{
	status_t Ret;
	can_buff_config_t RecBufferCfg = {
		.enableFD = 0,
		.enableBRS = 0,
		.fdPadding = 0,
	};
	RecBufferCfg.idType = CanRule.Can_format;
	RecBufferCfg.isRemote = CanRule.Can_Remot;
	Ret = CAN_ConfigRxBuff(Hth,CanRule.MailboxId,&RecBufferCfg,CanRule.CanAccept_ID);
	if(Ret == STATUS_SUCCESS )
	{
		Ret = CAN_SetRxFilter(Hth,CanRule.Can_format,CanRule.MailboxId,CanRule.CanMask_ID);
	}
	return Ret;
}

/*
*********************************************************************************************************
*
* Description:  Start recMailbox as RuletableIndex element
*
* Arguments  : 	Hth: can_module 0-2
*				RuleIndexType: RuleTable_Index in Can_uif_cfg.h

* Returns    :  Success Num
*
* Notes: use  SDK can_pal Component
*
*********************************************************************************************************
*/
uint8 Canif_StartRecMailbox(const can_instance_t* const Hth)
{
	uint8 i;
	uint8 RtValue = 0;
	status_t Ret;
	if(Hth->instIdx ==0)
	{
#if FlexCan0_Use
		CanRule_ConfigType CanRxMailBox_Can0[RxMailBoxRuleNum_Can0]={RxMailBoxTable_Can0};
		for(i=0;i<RxMailBoxRuleNum_Can0;i++)
		{
		 	Ret = Canif_CfgRecMailBox(Hth,CanRxMailBox_Can0[i]);
			if(Ret == STATUS_SUCCESS)
			{
				Ret = CAN_Receive(Hth,CanRxMailBox_Can0[i].MailboxId,&RecMsg_Can0);
				if(Ret == STATUS_SUCCESS)
					RtValue++;
			}
		}
#endif
	}
	else if(Hth->instIdx == 1)
	{
#if FlexCan1_Use
		CanRule_ConfigType CanRxMailBox_Can1[RxMailBoxRuleNum_Can1]={RxMailBoxTable_Can1};
		for(i=0;i<RxMailBoxRuleNum_Can1;i++)
		{
			Ret = Canif_CfgRecMailBox(Hth,CanRxMailBox_Can1[i]);
			if(Ret == STATUS_SUCCESS)
			{
				Ret = CAN_Receive(Hth,CanRxMailBox_Can1[i].MailboxId,&RecMsg_Can1);
				if(Ret == STATUS_SUCCESS)
					RtValue++;
			}
		}
#endif
	}
	else if(Hth->instIdx == 2)
	{
#if FlexCan2_Use
		CanRule_ConfigType CanRxMailBox_Can2[RxMailBoxRuleNum_Can2]={RxMailBoxTable_Can2};
		for(i=0;i<RxMailBoxRuleNum_Can2;i++)
		{
			Ret = Canif_CfgRecMailBox(Hth,CanRxMailBox_Can2[i]);
			if(Ret == STATUS_SUCCESS)
			{
				Ret = CAN_Receive(Hth,CanRxMailBox_Can2[i].MailboxId,&RecMsg_Can2);
				if(Ret == STATUS_SUCCESS)
					RtValue++;
			}
		}
#endif
	}
	return(RtValue);
}


/*
*********************************************************************************************************
*
* Description:  Start RxFifo Rec
*
* Arguments  : 	Hth: can_module 0-2
*
* Returns    :  Success or error
*
* Notes: use  SDK can_pal Component
*
*********************************************************************************************************
*/
status_t Canif_StartRecFifo(const can_instance_t* const Hth)
{
	status_t Ret = STATUS_ERROR;
	if(Hth->instIdx ==0)
	{
#if	FlexCan0_RxFIFO_USE
		Ret = CAN_Receive(Hth,0,RecMsgFifo_Can0); //bufferid must 0
#endif
	}
	else if(Hth->instIdx ==1)
	{
#if	FlexCan1_RxFIFO_USE
		Ret = CAN_Receive(Hth,0,RecMsgFifo_Can1); //bufferid must 0
#endif
	}
	else if(Hth->instIdx ==2)
	{
#if	FlexCan2_RxFIFO_USE
		Ret = CAN_Receive(Hth,0,RecMsgFifo_Can2); //bufferid must 0
#endif
	}
	return Ret;
}





/*
*********************************************************************************************************
*                                     CAN_RXBUFFERx_Handle
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

void Canif_MailboxIntProcess(uint32_t instance,can_event_t eventType,
                               uint32_t objIdx,void *driverState)
{
	if(eventType == CAN_EVENT_TX_COMPLETE)
	{
		BussOffFlag_CAN[instance] = 0;
		BussNoAck_CAN[instance] = 0;
	}
	else if(eventType == CAN_EVENT_RX_COMPLETE)
	{
		if(instance==0)
		{
			if(objIdx == 0)
			{
				for(int i =0;i<HookParaValue;i++)
				{

				}
				#if	RecFifo_ContiueMode_Can0
						Canif_StartRecFifo(&can_pal1_instance);
				#endif				
			}
			else
			{
				MailboxRecProcess_Can0();
				#if	RecMaibox_ContiueMode_Can0
					#if FlexCan0_Use
						CAN_Receive(&can_pal1_instance,objIdx,&RecMsg_Can0);
					#endif	
				#endif
			}
		}
	}
}


void Canif_ErrorIntProcess(uint8_t instance, flexcan_event_type_t eventType,
                                         flexcan_state_t *flexcanState)
{
	uint32 ESR1_VALUE;
	ESR1_VALUE = FLEXCAN_DRV_GetErrorStatus(instance);
	if(ESR1_VALUE&CAN_ESR1_BOFFINT_MASK)
	{
		BussOffFlag_CAN[instance] = 1;
	}
	if(ESR1_VALUE&CAN_ESR1_ACKERR_MASK)
	{
		BussNoAck_CAN[instance] = 1;
	}

}

/*
*********************************************************************************************************
*                                     CAN_DoProcess_Handle
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
uint8 JudgeChange(uint8* HisData,uint8* NewData,uint16 len)
{
	uint8 i;
	for(i=0;i<len;i++)
	{
		if(HisData[i] != NewData[i])
		{
			return 1;
		}
	}
	return 0 ;
}


static void MailboxRecProcess_Can0()
{	
	//Add own code RecMsgTemp_Can0
	uint8 i;
	switch(RecMsg_Can0.id)
	{
		case 0x290:
			Msg1_SI.VehicleRunSt = RecMsg_Can0.data[0] & 0x7;
			Msg1_SI.VehiclePowerMode = (RecMsg_Can0.data[0]>>3)&0x3;
			Msg1_SI.VehicleGearLeverPos = (RecMsg_Can0.data[1])&0xf;
			Msg1_SI.VehicleDCDCSt = (RecMsg_Can0.data[1]>>4)&0x3;
			Msg1_SI.BMS_IsolationR_Sts = (RecMsg_Can0.data[1]>>6)&0x3;
			Msg1_SI.IPC_Odometer= (RecMsg_Can0.data[2]);
			Msg1_SI.IPC_Odometer= (Msg1_SI.IPC_Odometer<<8)+RecMsg_Can0.data[3];
			Msg1_SI.IPC_Odometer= (Msg1_SI.IPC_Odometer<<4)+(RecMsg_Can0.data[4]>>4);

			Msg1_Frame.id = RecMsg_Can0.id;
				Msg1_Frame.length = RecMsg_Can0.length;

			if(JudgeChange(Msg1_Frame.data,RecMsg_Can0.data,8))
			{
				for(i=0;i<Msg1_Frame.length;i++)
					Msg1_Frame.data[i] = RecMsg_Can0.data[i];

				Msg1_Frame.changeFlag = 1;
			}
			if(C4G_FirstSucFlag & Msg1Mask)
			{
				for(i=0;i<Msg1_Frame.length;i++)
					Msg1_Frame.data[i] = RecMsg_Can0.data[i];

				C4G_FirstSucFlag &=(~Msg1Mask);
				Msg1_Frame.changeFlag = 1;
			}
			break;
		case 0xb0:
			Msg2_SI.BMS_SOC = RecMsg_Can0.data[0];
			Msg2_SI.OBC_CCLineSts = RecMsg_Can0.data[1]>>7;
			Msg2_SI.VehicleCrashSts = (RecMsg_Can0.data[1]>>5)&0x3;
			Msg2_SI.BMS_BattVolt = RecMsg_Can0.data[1]&0x1f;
			Msg2_SI.BMS_BattVolt = (Msg2_SI.BMS_BattVolt<<8)+RecMsg_Can0.data[2];
			Msg2_SI.BMS_ChargingSts = (RecMsg_Can0.data[3]>>6);
			Msg2_SI.BMS_BattCurr = RecMsg_Can0.data[3]&0x3f;
			Msg2_SI.BMS_BattCurr = (Msg2_SI.BMS_BattCurr<<8)+ RecMsg_Can0.data[4];
			Msg2_SI.BMS_RemainChrgTime = RecMsg_Can0.data[5];
			Msg2_SI.BMS_RemainChrgTime = (Msg2_SI.BMS_RemainChrgTime<<8)+RecMsg_Can0.data[6];

			Msg2_Frame.id = RecMsg_Can0.id;
			Msg2_Frame.length = RecMsg_Can0.length;
			if(JudgeChange(Msg2_Frame.data,RecMsg_Can0.data,8))
			{
				for(i=0;i<Msg2_Frame.length;i++)
					Msg2_Frame.data[i] = RecMsg_Can0.data[i];
				Msg2_Frame.changeFlag = 1;
			}	

			if(C4G_FirstSucFlag & Msg2Mask)
			{
				for(i=0;i<Msg2_Frame.length;i++)
					Msg2_Frame.data[i] = RecMsg_Can0.data[i];

				C4G_FirstSucFlag &=(~Msg2Mask);
				Msg2_Frame.changeFlag = 1;
			}
			break;
			
		case 0x171:
			Msg3_SI.ABS_VehicleSpdValid = RecMsg_Can0.data[0]>>7;
			Msg3_SI.ABS_VehicleSpd = RecMsg_Can0.data[0]&0x1f;
			Msg3_SI.ABS_VehicleSpd = (Msg3_SI.ABS_VehicleSpd<<8)+RecMsg_Can0.data[1];

			Msg3_Frame.id = RecMsg_Can0.id;
			Msg3_Frame.length = RecMsg_Can0.length;
			if(JudgeChange(Msg3_Frame.data,RecMsg_Can0.data,8))
			{
				for(i=0;i<Msg3_Frame.length;i++)
					Msg3_Frame.data[i] = RecMsg_Can0.data[i];

				Msg3_Frame.changeFlag = 1;
			}

			if(C4G_FirstSucFlag & Msg3Mask)
			{
				for(i=0;i<Msg3_Frame.length;i++)
					Msg3_Frame.data[i] = RecMsg_Can0.data[i];

				C4G_FirstSucFlag &=(~Msg3Mask);
				Msg3_Frame.changeFlag = 1;
			}
			break;
		case 0x30a:
			Msg4_SI.BCM_DoorLockSts = RecMsg_Can0.data[1]&0x3;
			Msg4_SI.BCM_DrvDoorSts = (RecMsg_Can0.data[1]>>2)&0x1;
			Msg4_SI.BCM_PassDoorSts= (RecMsg_Can0.data[1]>>3)&0x1;
			Msg4_SI.BCM_RearLeftDoorSts= (RecMsg_Can0.data[1]>>4)&0x1;
			Msg4_SI.BCM_RearRightDoorSts= (RecMsg_Can0.data[1]>>5)&0x1;
			Msg4_SI.BCM_TrunkDoorSts = (RecMsg_Can0.data[1]>>6)&0x1;
			Msg4_SI.BCM_VehAntitheftSts = (RecMsg_Can0.data[3]>>1)&0x3;
			

			Msg4_Frame.id = RecMsg_Can0.id;
			Msg4_Frame.length = RecMsg_Can0.length;
			if(JudgeChange(Msg4_Frame.data,RecMsg_Can0.data,8))
			{
				for(i=0;i<Msg4_Frame.length;i++)
					Msg4_Frame.data[i] = RecMsg_Can0.data[i];

				Msg4_Frame.changeFlag = 1;
			}	

			if(C4G_FirstSucFlag & Msg4Mask)
			{
				for(i=0;i<Msg4_Frame.length;i++)
					Msg4_Frame.data[i] = RecMsg_Can0.data[i];

				C4G_FirstSucFlag &=(~Msg4Mask);
				Msg4_Frame.changeFlag = 1;
			}
			break;	
		case 0x320:
			Msg5_SI.AC_WorkSts = RecMsg_Can0.data[0]&0x7;
			Msg5_SI.AC_TempSetPoint = (RecMsg_Can0.data[1])&0x1f;
			Msg5_SI.AC_BlowerSpd = (RecMsg_Can0.data[1]>>5)&0x7;
			Msg5_SI.AC_AirDistribMode= (RecMsg_Can0.data[2])&0x7;
			Msg5_SI.AC_AirCircMode= (RecMsg_Can0.data[2]>>3)&0x3;
			
			Msg5_Frame.id = RecMsg_Can0.id;
			Msg5_Frame.length = RecMsg_Can0.length;
			if(JudgeChange(Msg5_Frame.data,RecMsg_Can0.data,8))
			{
				for(i=0;i<Msg5_Frame.length;i++)
					Msg5_Frame.data[i] = RecMsg_Can0.data[i];

				Msg5_Frame.changeFlag = 1;
			}

			if(C4G_FirstSucFlag & Msg5Mask)
			{
				for(i=0;i<Msg5_Frame.length;i++)
					Msg5_Frame.data[i] = RecMsg_Can0.data[i];

				C4G_FirstSucFlag &=(~Msg5Mask);
				Msg5_Frame.changeFlag = 1;
			}
			break;	
		case 0x110:
			PEPS1_SI.PEPS_PowerMode = (RecMsg_Can0.data[0]>>3)&0x7;
			PEPS1_SI.PEPS_EnginStartupInhibitSts = (RecMsg_Can0.data[0]>>6)&0x1;
			PEPS1_SI.PEPS_EnginStartupInhibitFeedBack = (RecMsg_Can0.data[0]>>7)&0x1;;

			PEPS1_Frame.id = RecMsg_Can0.id;
			PEPS1_Frame.length = RecMsg_Can0.length;
			if(JudgeChange(PEPS1_Frame.data,RecMsg_Can0.data,8))
			{
				for(i=0;i<PEPS1_Frame.length;i++)
					PEPS1_Frame.data[i] = RecMsg_Can0.data[i];

				PEPS1_Frame.changeFlag = 1;
			}	

			if(C4G_FirstSucFlag & PEPS1Mask)
			{
				for(i=0;i<PEPS1_Frame.length;i++)
					PEPS1_Frame.data[i] = RecMsg_Can0.data[i];

				C4G_FirstSucFlag &=(~PEPS1Mask);
				PEPS1_Frame.changeFlag = 1;
			}

			break;
		case 0x7aa:
			FreeMaster_RecFrame.changeFlag = 1;
			FreeMaster_RecFrame.id = RecMsg_Can0.id;
			FreeMaster_RecFrame.length = RecMsg_Can0.length;
			for(i=0;i<FreeMaster_RecFrame.length;i++)
					FreeMaster_RecFrame.data[i] = RecMsg_Can0.data[i];
			break;
/*		case 0x6f1:
			if( RecMsg_Can0.data[0] == 1)
			{
				SetComFormatASCII();
			}
			else if( RecMsg_Can0.data[0] == 0)
			{
				SetComFormatHex();
			}
			if(RecMsg_Can0.data[1] == 1)
			{
				OptFlash = 0x80;
			}
			else if(RecMsg_Can0.data[1] == 0)
			{
				OptFlash = 0;
			}
			break;
*/			
		default:
			break;
	}
	//#end own code

}






