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

#ifndef Can_uif_Cfg_H
#define Can_uif_Cfg__H
//****************************************************************************
// @Project Includes
//****************************************************************************
#include "Std_Types.h"
#include "freemaster_defcfg.h"

//****************************************************************************
// @Macros
//****************************************************************************
#define FlexCan0_Use			1
#define FlexCan0_RxFIFO_USE  	1

#define FlexCan1_Use			1
#define FlexCan1_RxFIFO_USE     0

#define FlexCan2_Use			0
#define FlexCan2_RxFIFO_USE     0


#define APPTranMailBoxCan0  23

#define FreeMasterRecCanID    FMSTR_CAN_CMDID
#define FreeMasterRespCanID   FMSTR_CAN_RESPID
#define FreeMasterRecMailBox  FMSTR_FLEXCAN_RXMB
#define FreeMasterTranMailBox FMSTR_FLEXCAN_TXMB

/*for can0 Specify */
#if FlexCan0_Use
	#define RecMaibox_ContiueMode_Can0    	1  //if enable ,auto Restart Rec routine,else must call Canif_StartRecMailbox every time
	//if Rxfifo used need config para for fifo
	#if FlexCan0_RxFIFO_USE
		#define RecFifo_ContiueMode_Can0    	1  //if enable ,auto Restart Rec routine,else must use Canif_StartRecFifo every time	
		/*isRemoteFrame   isExtendedFrame   FiltId*/
		#define RxFifoIDFilertTable_Can0 \
		{ 0,				0,				0x100},\
		{ 0,				0,				0X101},\
		{ 0,				0,				0X102},\
		{ 0,				0,				0X103},\
		{ 0,				0,				0X104},\
		{ 0,				0,				0X105},\
		{ 0,				0,				0X106},\
		{ 0,				0,				0X107}
	#endif	

	// RecMailBox table
	// note :this can_if driver can not support Can_Mask_ID so there is no use to set this value
	#define RxMailBoxRuleNum_Can0    9
	/*Can_Rec_Index  FrameFomat  IsRemot    CAN_ID    CAN_Mask_ID MailboxNum(MAX 64-FIFO) (no use=0x80  if use RXFIFO  Mailbox0 =RecFifo)   */
	#define  RxMailBoxTable_Can0       \
			{0,			0,         	0,     0x290,		0x7ff,	 	0x03},\
			{1,			0,         	0,     0x0b0,		0x7ff,	 	0x04},\
			{2,			0,         	0,     0x171,		0x7ff,	 	0x05},\
			{3,			0,    		0,     0x30a,		0x7ff,	 	0x06},\
			{4,			0,     		0,     0x320,		0x7ff,	 	0x07},\
			{5,			0,     		0,     0x6F1,		0x7ff,	 	0x08},\
			{6,			1,     		0,     0x123,		0x1fffffff, 0x09},\
			{7,			0,     		0,     0x110,		0x7ff,	 	0x0a},\
			{8,			0,     		0,     FreeMasterRecCanID, 0x7ff,FreeMasterRecMailBox}

#endif

/*for can1 Specify*/
#if FlexCan1_Use
	#define RecMaibox_ContiueMode_Can1    	1  //if enable ,auto Restart Rec routine,else must use Canif_StartRecMailbox every time
//if Rxfifo used need config para for fifo
	#if FlexCan1_RxFIFO_USE
		#define RecFifo_ContiueMode_Can1    	1  //if enable ,auto Restart Rec routine,else must use Canif_StartRecFifo every time
		/*isRemoteFrame   isExtendedFrame   FiltId*/
		#define RxFifoIDFilertTable_Can1 \
		{ 0,				0,				0x100},\
		{ 0,				0,				0X101},\
		{ 0,				0,				0X102},\
		{ 0,				0,				0X103},\
		{ 0,				0,				0X104},\
		{ 0,				0,				0X105},\
		{ 0,				0,				0X106},\
		{ 0,				0,				0X107}
	#endif

	// RecMailBox table
	// note :this can_if driver can not support Can_Mask_ID so there is no use to set this value
	#define RxMailBoxRuleNum_Can1    2
		/*Can_Rec_Index  FrameFomat  IsRemot    CAN_ID                 CAN_Mask_ID        MailboxNum(MAX 64-FIFO) (no use=0x80  if use RXFIFO  Mailbox0 =RecFifo)   */
	#define  RxMailBoxTable_Can1       \
			{0,			0,     0,     0x6f1,			  0x7ff,		    0x01},\
			{1,			1,     0,     0x123,			0x1fffffff,		    0x02}

#endif

/*for can2 Specify*/
#if FlexCan2_Use
	#define RecMaibox_ContiueMode_Can2    	1  //if enable ,auto Restart Rec routine,else must use Canif_StartRecMailbox every time
	//if Rxfifo used need config para for fifo
	#if FlexCan2_RxFIFO_USE
		#define RecFifo_ContiueMode_Can2    	1  //if enable ,auto Restart Rec routine,else must use Canif_StartRecFifo every time
		#define ID_Filter_elementNum_Can2  		(1*8) //must 8 integral multiple,max = 16*8 = 128
		/*isRemoteFrame   isExtendedFrame   FiltId*/
		#define RxFifoIDFilertTable_Can2 \
		{ 0,				0,				0x100},\
		{ 0,				0,				0X101},\
		{ 0,				0,				0X102},\
		{ 0,				0,				0X103},\
		{ 0,				0,				0X104},\
		{ 0,				0,				0X105},\
		{ 0,				0,				0X106},\
		{ 0,				0,				0X107}
	#endif

	// RecMailBox table
	// note :this can_if driver can not support Can_Mask_ID so there is no use to set this value
	#define RxMailBoxRuleNum_Can2    2
		/*Can_Rec_Index  FrameFomat  IsRemot    CAN_ID                 CAN_Mask_ID        MailboxNum(MAX 64-FIFO) (no use=0x80  if use RXFIFO  Mailbox0 =RecFifo)   */
	#define  RxMailBoxTable_Can2       \
			{0,			0,     0,     0x6f1,			  0x7ff,		    0x01},\
			{1,			1,     0,     0x123,			0x1fffffff,		    0x02}

#endif


//****************************************************************************
// @Typedefs emum
//****************************************************************************

//****************************************************************************
// @Typedefs Structures
//****************************************************************************

/******************************************************************************/
/*-------------------------Global Variable Prototypes-------------------------*/
/******************************************************************************/ 
 
/******************************************************************************/
/*-------------------------Global Function Prototypes-------------------------*/
/******************************************************************************/

#endif
