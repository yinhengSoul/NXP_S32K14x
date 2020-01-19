/**
* \file    c
* \brief   functionality
*
* \version V1.0
* \copyright Copyright (c) 2016 YOGOMO Motor. All rights reserved.
* \by  yinheng 
*/
/* 
 	Notice:  if RXFIFO_EN = 1 ,MAILBOX 0 only for fifo(6MBs)
*/
/*****************************************************************************
 Includes   <System Includes> , "Project Includes"
 *****************************************************************************/
#include "Can_uif.h"
#include "CanMailBox_Cfg.h"

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
 Private variables
*****************************************************************************/
uint32 HookParaValue;  // RecFifo hook: Rec Num of frame(MAX=6); RecMailbox:not use


/*****************************************************************************
 Exported global variables (to be accessed by other files)
*****************************************************************************/
uint8 EnableTranCan0 ;
uint8 EnableTranCan1 ;

/*****************************************************************************
 Private  functions
*****************************************************************************/
static void CANif_InstallCallback(const can_instance_t* const Hth);
static status_t Canif_CfgTxBOX(const can_instance_t* const Hth,uint8 mailBoxId,can_msg_id_type_t idType);





/*****************************************************************************
 global  functions
*****************************************************************************/

/*
*********************************************************************************************************
*                                      
* Description: Get the Version of Canif file
*
* Arguments  : VersionInfo: Version value
*             
* Returns    : none
*
* Notes: none
*             
*********************************************************************************************************
*/	
void Canif_GetVersionInfo(Std_VersionInfoType *VersionInfo)
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
* Description:  User Init function
*
* Arguments  : Hth: can ch 0,1,2
*              config: config as can_pal setting
*             
* Returns    : Success or error
*
* Notes: use SDK can_pal Component,No.of buffers must larger than (RxMailBoxRuleNum+TxMailBoxRuleNum+1(if RxFifoUsed))
*             
*********************************************************************************************************
*/	
#if	FlexCan0_RxFIFO_USE
static void Can0RxFifoConfig(const can_user_config_t *config)
{
	if(config->extension != NULL)
	{	
		extension_flexcan_rx_fifo_t* Rxfifo ;
		Rxfifo = (extension_flexcan_rx_fifo_t *)(config->extension);
//		if(Rxfifo->numIdFilters==FLEXCAN_RX_FIFO_ID_FILTERS_8)
		{
			static flexcan_id_table_t IDTableList[8] = {RxFifoIDFilertTable_Can0};
			Rxfifo->idFilterTable = IDTableList;
		}
	}
}
#endif

#if	FlexCan1_RxFIFO_USE
static void Can1RxFifoConfig(const can_user_config_t *config)
{

	if(config->extension != NULL)
	{	
		extension_flexcan_rx_fifo_t* Rxfifo = (extension_flexcan_rx_fifo_t*)config->extension;
		static flexcan_id_table_t IDTableList[8] = {RxFifoIDFilertTable_Can1};
		Rxfifo->idFilterTable = IDTableList;
	}
}
#endif

#if	FlexCan2_RxFIFO_USE
static void Can2RxFifoConfig(const can_user_config_t *config)
{
	if(config->extension != NULL)
	{	
		extension_flexcan_rx_fifo_t* Rxfifo = (extension_flexcan_rx_fifo_t*)config->extension;
		static flexcan_id_table_t IDTableList[8] = {RxFifoIDFilertTable_Can2};
		Rxfifo->idFilterTable = IDTableList;
	}
}
#endif

status_t CANif_Init(const can_instance_t* const Hth,
        const can_user_config_t *config)
{
	status_t Ret;
	if(Hth->instIdx == 0)
	{
#if	FlexCan0_RxFIFO_USE	
		Can0RxFifoConfig(config);
#endif
	}
	else if(Hth->instIdx == 1)
	{
#if	FlexCan1_RxFIFO_USE	
		Can1RxFifoConfig(config);
#endif
	}
	else if(Hth->instIdx == 2)
	{
#if	FlexCan1_RxFIFO_USE	
		Can2RxFifoConfig(config);
#endif
	}
	Ret = CAN_Init(Hth,config);
	CANif_InstallCallback(Hth);
	return Ret;
}

static void CANif_InstallCallback(const can_instance_t* const Hth)
{
	CAN_InstallEventCallback(Hth,Canif_MailboxIntProcess,&HookParaValue);
	FLEXCAN_DRV_InstallErrorCallback(Hth->instIdx,Canif_ErrorIntProcess,NULL);
}

static status_t Canif_CfgTxBOX(const can_instance_t* const Hth,uint8 mailBoxId,can_msg_id_type_t idType)
{
	status_t Ret = STATUS_ERROR;
	static can_buff_config_t  can_buff ;
	{
		can_buff.enableBRS = 0;
		can_buff.enableFD =0;
		can_buff.fdPadding =0;
		can_buff.idType = idType;
		can_buff.isRemote =0;
	}
	Ret = CAN_ConfigTxBuff(Hth,mailBoxId,&can_buff);
	return Ret;
}

status_t Canif_Write(const can_instance_t* const Hth,uint8 mailBoxId,can_msg_id_type_t idType,
					can_message_t sendMsg)
{
	status_t Ret = STATUS_ERROR;
	Ret = Canif_CfgTxBOX(Hth,mailBoxId,idType);
	if(Ret == STATUS_SUCCESS)
	{
		Ret = CAN_Send(Hth,mailBoxId,&sendMsg);
	}
	return Ret;
}

status_t Canif_WriteBlocking(const can_instance_t* const Hth,uint8 mailBoxId,can_msg_id_type_t idType,
					can_message_t sendMsg,uint32_t timeoutMs)
{
	status_t Ret = STATUS_ERROR;
	Ret = Canif_CfgTxBOX(Hth,mailBoxId,idType);
	if(Ret == STATUS_SUCCESS)
	{
		Ret = CAN_SendBlocking(Hth,mailBoxId,&sendMsg,timeoutMs);
	}
	return Ret;
}

status_t Canif_AbortTranMailbox(const can_instance_t* const Hth,uint8 mailBoxId)
{
	status_t Ret = STATUS_ERROR;
	Ret = CAN_AbortTransfer(Hth,mailBoxId);
	return Ret;
}






