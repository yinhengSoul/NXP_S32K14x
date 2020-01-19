/*
 * TJA_CANDrv.c - CAN abstraction for TJA115x "driver" - based on S32DS SDK
 *
 *  Created on: Jan 6, 2020
 *      Author: YINHENG
 */
#include "Can_uif.h"

/* Used mailbox */
#define	TX_MAILBOX	(10UL)


/* Local variable with last transmission status */
static uint8_t	ack = false;

/* Required function to test the last transmission status */
uint8_t	CAN_acknowledged(void)
{
	return ack;
}

/* Transmit a message with Standard ID */
void TJACAN_transmit(uint32_t messageId, uint8_t dlc, uint8_t *data)
{
	status_t Rel;
	uint16 i;
	can_message_t sendMsg;
	sendMsg.id = messageId;
	sendMsg.length = dlc;
	for(i=0;i<sendMsg.length;i++)
		sendMsg.data[i] = data[i];

	Rel = Canif_WriteBlocking(&can_pal2_instance,TX_MAILBOX,CAN_MSG_ID_STD,sendMsg,500);

    /* Execute send non-blocking */
    if (STATUS_SUCCESS == Rel)
	{
    	ack = 1U;
    }
    else
    {
    	ack = 0U;
    }
}

/* Transmit a message with Extended ID */
void	TJACAN_transmit_extended(uint32_t messageId, uint8_t dlc, uint8_t *data)
{
	status_t Rel;
	uint16 i;
	can_message_t sendMsg;
	sendMsg.id = messageId;
	sendMsg.length = dlc;
	for(i=0;i<sendMsg.length;i++)
		sendMsg.data[i] = data[i];

	Rel = Canif_WriteBlocking(&can_pal2_instance,TX_MAILBOX,CAN_MSG_ID_EXT,sendMsg,1000);

    /* Execute send non-blocking */
    if (STATUS_SUCCESS == Rel)
	{
    	ack = 1U;
    }
    else
    {
    	ack = 0U;
    }
}

/* End of file: CANDrv.c */
