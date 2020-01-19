/*
 * CANDrv.h
 *
 *  Created on: Nov 1, 2018
 *      Author: Th. Walrant
 *
 *  CAN driver emulation - to be replaced by equivalent
 *  driver/function in customer development environment.
 */

#ifndef _TJA_CanDrv_H_
#define _TJA_CanDrv_H_

/* _____ I N C L U D E S ___________________________________________________ */

#include <stdint.h>

/* _____ G L O B A L  V A R I A B L E S ____________________________________ */


/* _____ D E F I N I T I O N S _____________________________________________ */


/* _____ P U B L I C  F U N C T I O N S ____________________________________ */

/**
 * @brief Expected function to transmit HS CAN with standard CAN identifier.
 * @param canid Standard CAN identifier
 * @param dlc Data Length Code (max 8)
 * @param payload Pointer to allocated payload data
 */
extern void TJACAN_transmit(uint32_t messageId, uint8_t dlc, uint8_t *data);

/**
 * @brief Expected function to transmit HS CAN with extended CAN identifier.
 * @param canid Extended CAN identifier
 * @param dlc Data Length Code (max 8)
 * @param payload Pointer to allocated payload data
 */
extern void	TJACAN_transmit_extended(uint32_t messageId, uint8_t dlc, uint8_t *data) ;

/**
 * @brief Expected function to check transmission ACK.
 */
extern uint8_t	CAN_acknowledged(void);

#endif /* _CANDRV_H_ */
/* End of file: CANDrv.h */
