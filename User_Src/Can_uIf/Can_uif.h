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
 *    Notice: source code
     	 	 "if (state->errorCallbackParam != NULL){
 				uint32_t *ESR1_Ptr = state->errorCallbackParam;
				*ESR1_Ptr = base->ESR1;}"
	   need insert to "flexcan_driver.c" "FLEXCAN_Error_IRQHandler" function

			source code
					"//add code
                    uint8_t RecFrameLen =1;
                    uint8_t flag_reg;
                    do
                    {
                    	FLEXCAN_ClearMsgBuffIntStatusFlag(base, mb_idx);
                    	flag_reg = FLEXCAN_GetMsgBuffIntStatusFlag(base, FEATURE_CAN_RXFIFO_FRAME_AVAILABLE);
                    	if(flag_reg)
                    	{
                    		flexcan_msgbuff_t* S_Addr = state->mbs[FLEXCAN_MB_HANDLE_RXFIFO].mb_message;
                    		FLEXCAN_ReadRxFifo(base, S_Addr+RecFrameLen);
                    		RecFrameLen++;
                    	}
                    }while(flag_reg);
                    if (state->callbackParam != NULL)
                    {
                    	uint32_t *Para_Ptr = state->callbackParam;
                    	*Para_Ptr = RecFrameLen;
                	}
                    //add code end"
     need insert to "flexcan_driver.c" "FLEXCAN_IRQHandlerRxFIFO" function
 */

#ifndef Can_ulf_H
#define Can_ulf_H
//****************************************************************************
// @Project Includes
//****************************************************************************
#include "Std_Types.h"
#include "Cpu.h"
//****************************************************************************
// @Macros
//****************************************************************************



//****************************************************************************
// @Typedefs emum
//****************************************************************************



//****************************************************************************
// @Typedefs Structures
//****************************************************************************
typedef uint16 RuleIndexType;

typedef struct
{
	RuleIndexType 	Can_RuleIndex;
	uint8  	Can_format;
	uint8	Can_Remot;
	uint32 	CanAccept_ID;
	uint32 	CanMask_ID;
	uint8  	MailboxId; // no use=0x80  if use RXFIFO  Mailbox0 =RecFifo
}CanRule_ConfigType;

typedef struct
{
	uint8 BOFFINT;
	uint8 TX;
	uint8 ERRINT;
	uint8 ACKERR;
}CanESR_RegType;



/******************************************************************************/
/*-------------------------Global Variable Prototypes-------------------------*/
/******************************************************************************/ 
extern uint8 BussOffFlag_CAN0;
extern uint8 BussNoAck_CAN0;
extern uint32 HookParaValue;
extern uint8 EnableTranCan0;
extern uint8 EnableTranCan1;


/******************************************************************************/
/*-------------------------Global Function Prototypes-------------------------*/
/******************************************************************************/
void Canif_GetVersionInfo(Std_VersionInfoType *VersionInfo);
status_t CANif_Init(const can_instance_t* const Hth,
        const can_user_config_t *config);

uint8 Canif_StartRecMailbox(const can_instance_t* const Hth);
status_t Canif_StartRecFifo(const can_instance_t* const Hth);

status_t Canif_Write(const can_instance_t* const Hth,uint8 mailBoxId,can_msg_id_type_t idType,
					can_message_t sendMsg);

status_t Canif_WriteBlocking(const can_instance_t* const Hth,uint8 mailBoxId,can_msg_id_type_t idType,
					can_message_t sendMsg,uint32_t timeoutMs);

status_t Canif_AbortTranMailbox(const can_instance_t* const Hth,uint8 mailBoxId);




//Call back from Int
void Canif_MailboxIntProcess(uint32_t instance,
                               can_event_t eventType,
                               uint32_t objIdx,
                               void *driverState);
void Canif_ErrorIntProcess(uint8_t instance, flexcan_event_type_t eventType,
                                         flexcan_state_t *flexcanState);

#endif
