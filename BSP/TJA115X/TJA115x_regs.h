/*
 * Copyright (c) 2007 - 2018, NXP Semiconductors B.V.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * o Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in
 *   the documentation and/or other materials provided with the
 *   distribution.
 *
 * o Neither the name of NXP Semiconductors B.V. nor the names of its
 *   contributors may be used to endorse or promote products derived
 *   from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * @file TJA115x_regs.h.h
 */

#ifndef _TJA115X_REGS_H_
#define _TJA115X_REGS_H_

#include <stdint.h>
		
/* TJA115x Definitions
 */
		
/* Maximum number of filter elements */
#define TJA115x_ELEMENTS_COUNT 16

/* CMD10 Register Masks
 * Configuration of spoofing protection (command byte = 0x10)
 */
		
/* Filter Element Type */
#define TJA115x_CMD10_FET_RMASK	0x80000000u
#define TJA115x_CMD10_FET_SHIFT	31u
#define TJA115x_CMD10_FET_WIDTH	1u
#define	TJA115x_CMD10_FET(x_)	(((uint32_t)(((uint32_t)(x_))<<TJA115x_CMD10_FET_SHIFT))&TJA115x_CMD10_FET_RMASK)
  
/* Standard Filter Configuration */
#define TJA115x_CMD10_SFC_RMASK	0x60000000u
#define TJA115x_CMD10_SFC_SHIFT	29u
#define TJA115x_CMD10_SFC_WIDTH	2u
#define	TJA115x_CMD10_SFC(x_)	(((uint32_t)(((uint32_t)(x_))<<TJA115x_CMD10_SFC_SHIFT))&TJA115x_CMD10_SFC_RMASK)
  
/* Transmission White List for SFID1 */
#define TJA115x_CMD10_TWL1_RMASK	0x10000000u
#define TJA115x_CMD10_TWL1_SHIFT	28u
#define TJA115x_CMD10_TWL1_WIDTH	1u
#define	TJA115x_CMD10_TWL1(x_)	(((uint32_t)(((uint32_t)(x_))<<TJA115x_CMD10_TWL1_SHIFT))&TJA115x_CMD10_TWL1_RMASK)
  
/* Bus Black List for SFID1 */
#define TJA115x_CMD10_BBL1_RMASK	0x8000000u
#define TJA115x_CMD10_BBL1_SHIFT	27u
#define TJA115x_CMD10_BBL1_WIDTH	1u
#define	TJA115x_CMD10_BBL1(x_)	(((uint32_t)(((uint32_t)(x_))<<TJA115x_CMD10_BBL1_SHIFT))&TJA115x_CMD10_BBL1_RMASK)
  
/* The first Standard Identifier */
#define TJA115x_CMD10_SFID1_RMASK	0x7FF0000u
#define TJA115x_CMD10_SFID1_SHIFT	16u
#define TJA115x_CMD10_SFID1_WIDTH	11u
#define	TJA115x_CMD10_SFID1(x_)	(((uint32_t)(((uint32_t)(x_))<<TJA115x_CMD10_SFID1_SHIFT))&TJA115x_CMD10_SFID1_RMASK)
  
/* Transmission White List for SFID2 */
#define TJA115x_CMD10_TWL2_RMASK	0x1000u
#define TJA115x_CMD10_TWL2_SHIFT	12u
#define TJA115x_CMD10_TWL2_WIDTH	1u
#define	TJA115x_CMD10_TWL2(x_)	(((uint32_t)(((uint32_t)(x_))<<TJA115x_CMD10_TWL2_SHIFT))&TJA115x_CMD10_TWL2_RMASK)
  
/* Bus Black List for SFID2 */
#define TJA115x_CMD10_BBL2_RMASK	0x800u
#define TJA115x_CMD10_BBL2_SHIFT	11u
#define TJA115x_CMD10_BBL2_WIDTH	1u
#define	TJA115x_CMD10_BBL2(x_)	(((uint32_t)(((uint32_t)(x_))<<TJA115x_CMD10_BBL2_SHIFT))&TJA115x_CMD10_BBL2_RMASK)
  
/* The first Standard Identifier */
#define TJA115x_CMD10_SFID2_RMASK	0x7FFu
#define TJA115x_CMD10_SFID2_SHIFT	0u
#define TJA115x_CMD10_SFID2_WIDTH	11u
#define	TJA115x_CMD10_SFID2(x_)	(((uint32_t)(((uint32_t)(x_))<<TJA115x_CMD10_SFID2_SHIFT))&TJA115x_CMD10_SFID2_RMASK)
  
/* Extended Filter Configuration */
#define TJA115x_CMD10_EFC_RMASK	0x60000000u
#define TJA115x_CMD10_EFC_SHIFT	29u
#define TJA115x_CMD10_EFC_WIDTH	2u
#define	TJA115x_CMD10_EFC(x_)	(((uint32_t)(((uint32_t)(x_))<<TJA115x_CMD10_EFC_SHIFT))&TJA115x_CMD10_EFC_RMASK)
  
/* The Extended Identifier */
#define TJA115x_CMD10_EFID_RMASK	0x1FFFFFFFu
#define TJA115x_CMD10_EFID_SHIFT	0u
#define TJA115x_CMD10_EFID_WIDTH	29u
#define	TJA115x_CMD10_EFID(x_)	(((uint32_t)(((uint32_t)(x_))<<TJA115x_CMD10_EFID_SHIFT))&TJA115x_CMD10_EFID_RMASK)
  

/* CMD10 Register Values
 * Configuration of spoofing protection (command byte = 0x10)
 */
		

/* Filter Element Type */
#define TJA115x_CMD10_FET_STD  0 /*! Standard Identifier Filter Type */
#define TJA115x_CMD10_FET_EXT  1 /*! Extended Identifier Filter Type */
#define TJA115x_CMD10_FET_DEFAULT	TJA115x_CMD10_FET_STD

/* Standard Filter Configuration */
#define TJA115x_CMD10_SFC_OFF  0b00u /*! Filter element is disabled */
#define TJA115x_CMD10_SFC_DUAL  0b01u /*! Dual filter ID for SFID1 or SFID2 */
#define TJA115x_CMD10_SFC_CLASSIC  0b10u /*! Classic filter: SFID1 = filter, SFID2 = mask */
#define TJA115x_CMD10_SFC_RFU  0b11u /*! Reserved */
#define TJA115x_CMD10_SFC_DEFAULT	TJA115x_CMD10_SFC_OFF

/* Transmission White List for SFID1 */
#define TJA115x_CMD10_TWL1_DIS  0 /*! Do not include SFID1 in the Transmission White list */
#define TJA115x_CMD10_TWL1_EN  1 /*! Include SFID1 in the Transmission White list */
#define TJA115x_CMD10_TWL1_DEFAULT	TJA115x_CMD10_TWL1_DIS

/* Bus Black List for SFID1 */
#define TJA115x_CMD10_BBL1_DIS  0 /*! Do not include SFID1 in the Bus Black list */
#define TJA115x_CMD10_BBL1_EN  1 /*! Include SFID1 in the Bus Black list */
#define TJA115x_CMD10_BBL1_DEFAULT	TJA115x_CMD10_BBL1_DIS

/* The first Standard Identifier */
#define TJA115x_CMD10_SFID1_DEFAULT  0x000 /*! 0x000 */

/* Transmission White List for SFID2 */
#define TJA115x_CMD10_TWL2_DIS  0 /*! Do not include SFID2 in the Transmission White list */
#define TJA115x_CMD10_TWL2_EN  1 /*! Include SFID2 in the Transmission White list */
#define TJA115x_CMD10_TWL2_DEFAULT	TJA115x_CMD10_TWL2_DIS

/* Bus Black List for SFID2 */
#define TJA115x_CMD10_BBL2_DIS  0 /*! Do not include SFID2 in the Bus Black list */
#define TJA115x_CMD10_BBL2_EN  1 /*! Include SFID2 in the Bus Black list */
#define TJA115x_CMD10_BBL2_DEFAULT	TJA115x_CMD10_BBL2_DIS

/* The first Standard Identifier */
#define TJA115x_CMD10_SFID2_DEFAULT  0x000 /*! 0x000 */

/* Extended Filter Configuration */
#define TJA115x_CMD10_EFC_MASK  0b00u /*! Interpret EFID2 as a mask for EFID1 in the next Filter Element */
#define TJA115x_CMD10_EFC_BBL  0b01u /*! Bus Black list enabled */
#define TJA115x_CMD10_EFC_TWL  0b10u /*! Transmission White List enabled */
#define TJA115x_CMD10_EFC_BOTH  0b11u /*! Transmission White List and Spoofing protection enabled */

/* The Extended Identifier */
#define TJA115x_CMD10_EFID_DEFAULT  0x00000000

/* CMD40 Register Masks
 * Configuration of flooding and tamper protection (command byte=0x40)
 */
		
/* Tamper protection enabled */
#define TJA115x_CMD40_TMPE_RMASK	0x40u
#define TJA115x_CMD40_TMPE_SHIFT	6u
#define TJA115x_CMD40_TMPE_WIDTH	1u
#define	TJA115x_CMD40_TMPE(x_)	(((uint32_t)(((uint32_t)(x_))<<TJA115x_CMD40_TMPE_SHIFT))&TJA115x_CMD40_TMPE_RMASK)
  
/* Flooding threshold limit */
#define TJA115x_CMD40_FLDTH_RMASK	0x20u
#define TJA115x_CMD40_FLDTH_SHIFT	5u
#define TJA115x_CMD40_FLDTH_WIDTH	1u
#define	TJA115x_CMD40_FLDTH(x_)	(((uint32_t)(((uint32_t)(x_))<<TJA115x_CMD40_FLDTH_SHIFT))&TJA115x_CMD40_FLDTH_RMASK)
  
/* Max flooding bucket limit */
#define TJA115x_CMD40_FLDMAX_RMASK	0x1Cu
#define TJA115x_CMD40_FLDMAX_SHIFT	2u
#define TJA115x_CMD40_FLDMAX_WIDTH	3u
#define	TJA115x_CMD40_FLDMAX(x_)	(((uint32_t)(((uint32_t)(x_))<<TJA115x_CMD40_FLDMAX_SHIFT))&TJA115x_CMD40_FLDMAX_RMASK)
  
/* Fooding load */
#define TJA115x_CMD40_FLDL_RMASK	0x3u
#define TJA115x_CMD40_FLDL_SHIFT	0u
#define TJA115x_CMD40_FLDL_WIDTH	2u
#define	TJA115x_CMD40_FLDL(x_)	(((uint32_t)(((uint32_t)(x_))<<TJA115x_CMD40_FLDL_SHIFT))&TJA115x_CMD40_FLDL_RMASK)
  

/* CMD40 Register Values
 * Configuration of flooding and tamper protection (command byte=0x40)
 */
		

/* Tamper protection enabled */
#define TJA115x_CMD40_TMPE_DIS  0
#define TJA115x_CMD40_TMPE_EN  1

/* Flooding threshold limit */
#define TJA115x_CMD40_FLDTH_600  0 /*! Standard ID 0x600, Extended ID 0x18000000 */
#define TJA115x_CMD40_FLDTH_500  1 /*! Standard ID 0x500, Extended ID 0x14000000 */
#define TJA115x_CMD40_FLDTH_DEFAULT	TJA115x_CMD40_FLDTH_600

/* Max flooding bucket limit */
#define TJA115x_CMD40_FLDMAX_OFF  0b000u /*! Flooding prevention turned off */
#define TJA115x_CMD40_FLDMAX_004  0b001u /*! 0,04096/(1-FLDL) */
#define TJA115x_CMD40_FLDMAX_008  0b010u /*! 0,08192/(1-FLDL) */
#define TJA115x_CMD40_FLDMAX_016  0b011u /*! 0,16384/(1-FLDL) */
#define TJA115x_CMD40_FLDMAX_032  0b100u /*! 0,32768/(1-FLDL) */
#define TJA115x_CMD40_FLDMAX_065  0b101u /*! 0,65536s/(1-FLDL) */
#define TJA115x_CMD40_FLDMAX_098  0b110u /*! 0,98304s/(1-FLDL) */
#define TJA115x_CMD40_FLDMAX_131  0b111u /*! 1,31072s/(1-FLDL) */
#define TJA115x_CMD40_FLDMAX_DEFAULT	TJA115x_CMD40_FLDMAX_OFF

/* Fooding load */
#define TJA115x_CMD40_FLDL_3  0b00u /*! 3,125% */
#define TJA115x_CMD40_FLDL_6  0b01u /*! 6,25% */
#define TJA115x_CMD40_FLDL_12  0b10u /*! 12,5 */
#define TJA115x_CMD40_FLDL_25  0b11u /*! 25% */
#define TJA115x_CMD40_FLDL_DEFAULT	TJA115x_CMD40_FLDL_25

/* CMD50 Register Masks
 * Configuration of CAN FD bit timing, Secure Mode timeout, length of error frame and blacklist behavior (command byte = 0x50)
 */
		
/* Secure Mode timeout */
#define TJA115x_CMD50_SEC_TO_RMASK	0x300u
#define TJA115x_CMD50_SEC_TO_SHIFT	8u
#define TJA115x_CMD50_SEC_TO_WIDTH	2u
#define	TJA115x_CMD50_SEC_TO(x_)	(((uint32_t)(((uint32_t)(x_))<<TJA115x_CMD50_SEC_TO_SHIFT))&TJA115x_CMD50_SEC_TO_RMASK)
  
/* Defines sample point Classical CAN / CAN FD arbitration */
#define TJA115x_CMD50_CAN_SP_RMASK	0xC0u
#define TJA115x_CMD50_CAN_SP_SHIFT	6u
#define TJA115x_CMD50_CAN_SP_WIDTH	2u
#define	TJA115x_CMD50_CAN_SP(x_)	(((uint32_t)(((uint32_t)(x_))<<TJA115x_CMD50_CAN_SP_SHIFT))&TJA115x_CMD50_CAN_SP_RMASK)
  
/* Defines sample point DATA PHASE */
#define TJA115x_CMD50_CANFD_SP_RMASK	0x30u
#define TJA115x_CMD50_CANFD_SP_SHIFT	4u
#define TJA115x_CMD50_CANFD_SP_WIDTH	2u
#define	TJA115x_CMD50_CANFD_SP(x_)	(((uint32_t)(((uint32_t)(x_))<<TJA115x_CMD50_CANFD_SP_SHIFT))&TJA115x_CMD50_CANFD_SP_RMASK)
  
/* Config enable time out (for local re-configuration after power on) */
#define TJA115x_CMD50_CONFIG_EN_TO_RMASK	0x3000u
#define TJA115x_CMD50_CONFIG_EN_TO_SHIFT	12u
#define TJA115x_CMD50_CONFIG_EN_TO_WIDTH	2u
#define	TJA115x_CMD50_CONFIG_EN_TO(x_)	(((uint32_t)(((uint32_t)(x_))<<TJA115x_CMD50_CONFIG_EN_TO_SHIFT))&TJA115x_CMD50_CONFIG_EN_TO_RMASK)
  
/* Defines if the local CAN node receives the original blacklisted ID */
#define TJA115x_CMD50_BBLM_RMASK	0x800u
#define TJA115x_CMD50_BBLM_SHIFT	11u
#define TJA115x_CMD50_BBLM_WIDTH	1u
#define	TJA115x_CMD50_BBLM(x_)	(((uint32_t)(((uint32_t)(x_))<<TJA115x_CMD50_BBLM_SHIFT))&TJA115x_CMD50_BBLM_RMASK)
  
/* Defines the Error Frame length for Stinger functions TWL, BBL, Tampering and Flooding */
#define TJA115x_CMD50_LGEF_RMASK	0x400u
#define TJA115x_CMD50_LGEF_SHIFT	10u
#define TJA115x_CMD50_LGEF_WIDTH	1u
#define	TJA115x_CMD50_LGEF(x_)	(((uint32_t)(((uint32_t)(x_))<<TJA115x_CMD50_LGEF_SHIFT))&TJA115x_CMD50_LGEF_RMASK)
  
/* Defines CAN/CANFD bitrate */
#define TJA115x_CMD50_CANCANFD_BR_RMASK	0xFu
#define TJA115x_CMD50_CANCANFD_BR_SHIFT	0u
#define TJA115x_CMD50_CANCANFD_BR_WIDTH	4u
#define	TJA115x_CMD50_CANCANFD_BR(x_)	(((uint32_t)(((uint32_t)(x_))<<TJA115x_CMD50_CANCANFD_BR_SHIFT))&TJA115x_CMD50_CANCANFD_BR_RMASK)
  

/* CMD50 Register Values
 * Configuration of CAN FD bit timing, Secure Mode timeout, length of error frame and blacklist behavior (command byte = 0x50)
 */
		

/* Secure Mode timeout */
#define TJA115x_CMD50_SEC_TO_500  0b00u /*! 500 ms */
#define TJA115x_CMD50_SEC_TO_2  0b01u /*! 2 seconds */
#define TJA115x_CMD50_SEC_TO_8  0b10u /*! 8 seconds */
#define TJA115x_CMD50_SEC_TO_AUTO  0b11u /*! Auto (1st time: 0.5s, 2nd time: 2s, 3rd time: 8s, >3rd time: 8s) */
#define TJA115x_CMD50_SEC_TO_DEFAULT	TJA115x_CMD50_SEC_TO_2

/* Defines sample point Classical CAN / CAN FD arbitration */
#define TJA115x_CMD50_CAN_SP_70  0b00u /*! 70% */
#define TJA115x_CMD50_CAN_SP_75  0b01u /*! 75% */
#define TJA115x_CMD50_CAN_SP_80  0b10u /*! 80% */
#define TJA115x_CMD50_CAN_SP_RFU  0b11u /*! Reserved, do not use */
#define TJA115x_CMD50_CAN_SP_DEFAULT	TJA115x_CMD50_CAN_SP_80

/* Defines sample point DATA PHASE */
#define TJA115x_CMD50_CANFD_SP_70  0b00u /*! 70% */
#define TJA115x_CMD50_CANFD_SP_75  0b01u /*! 75% */
#define TJA115x_CMD50_CANFD_SP_80  0b10u /*! 80% */
#define TJA115x_CMD50_CANFD_SP_RFU  0b11u /*! Reserved, do not use */
#define TJA115x_CMD50_CANFD_SP_DEFAULT	TJA115x_CMD50_CANFD_SP_80

/* Config enable time out (for local re-configuration after power on) */
#define TJA115x_CMD50_CONFIG_EN_TO_1  0b00u /*! 1 second */
#define TJA115x_CMD50_CONFIG_EN_TO_2  0b01u /*! 2 seconds */
#define TJA115x_CMD50_CONFIG_EN_TO_4  0b10u /*! 4 seconds */
#define TJA115x_CMD50_CONFIG_EN_TO_8  0b11u /*! 8 seconds */
#define TJA115x_CMD50_CONFIG_EN_TO_DEFAULT	TJA115x_CMD50_CONFIG_EN_TO_2

/* Defines if the local CAN node receives the original blacklisted ID */
#define TJA115x_CMD50_BBLM_DIS  0 /*! local node does not receive the blacklisted frame */
#define TJA115x_CMD50_BBLM_EN  1 /*! local node does receive the blacklisted frame */
#define TJA115x_CMD50_BBLM_DEFAULT	TJA115x_CMD50_BBLM_DIS

/* Defines the Error Frame length for Stinger functions TWL, BBL, Tampering and Flooding */
#define TJA115x_CMD50_LGEF_NORM  0 /*! Normal length of error flag (6 bit) */
#define TJA115x_CMD50_LGEF_LONG  1 /*! Long error frame (11 bits) for logging purposes */
#define TJA115x_CMD50_LGEF_DEFAULT	TJA115x_CMD50_LGEF_NORM

/* Defines CAN/CANFD bitrate */
#define TJA115x_CMD50_CANCANFD_BR_125  0b0000u /*! 125 kbps (Classical CAN Only) */
#define TJA115x_CMD50_CANCANFD_BR_250_500  0b0101u /*! 250/500 kbps */
#define TJA115x_CMD50_CANCANFD_BR_250_1000  0b0110u /*! 250/1000 kbps */
#define TJA115x_CMD50_CANCANFD_BR_500_1000  0b1010u /*! 500/1000 kbps */
#define TJA115x_CMD50_CANCANFD_BR_500_2000  0b1011u /*! 500/2000 kbps */
#define TJA115x_CMD50_CANCANFD_BR_DEFAULT	TJA115x_CMD50_CANCANFD_BR_500_2000

/* CMD60 Register Masks
 * Configuration of command message ID (command byte = 0x60)
 */
		
/* Defines Toplevel FSM state transition */
#define TJA115x_CMD60_LCLREMn_RMASK	0x80000000u
#define TJA115x_CMD60_LCLREMn_SHIFT	31u
#define TJA115x_CMD60_LCLREMn_WIDTH	1u
#define	TJA115x_CMD60_LCLREMn(x_)	(((uint32_t)(((uint32_t)(x_))<<TJA115x_CMD60_LCLREMn_SHIFT))&TJA115x_CMD60_LCLREMn_RMASK)
  
/* Defines new CONFIG Identifier to be used */
#define TJA115x_CMD60_CONFIG_ID_RMASK	0x1FFFFFFFu
#define TJA115x_CMD60_CONFIG_ID_SHIFT	0u
#define TJA115x_CMD60_CONFIG_ID_WIDTH	29u
#define	TJA115x_CMD60_CONFIG_ID(x_)	(((uint32_t)(((uint32_t)(x_))<<TJA115x_CMD60_CONFIG_ID_SHIFT))&TJA115x_CMD60_CONFIG_ID_RMASK)
  

/* CMD60 Register Values
 * Configuration of command message ID (command byte = 0x60)
 */
		

/* Defines Toplevel FSM state transition */
#define TJA115x_CMD60_LCLREMn_CONFIG_REM  0 /*! From NORMAL state to CONFIG_REM state */
#define TJA115x_CMD60_LCLREMn_CONFIG_LOC  1 /*! From NORMAL state to CONFIG_LOC state */
#define TJA115x_CMD60_LCLREMn_DEFAULT	TJA115x_CMD60_LCLREMn_CONFIG_REM

/* Defines new CONFIG Identifier to be used */
#define TJA115x_CMD60_CONFIG_ID_CANID  0x18DA00F1
#define TJA115x_CMD60_CONFIG_ID_DEFAULT	TJA115x_CMD60_CONFIG_ID_CANID

#endif /* _TJA115X_REGS_H_ */
  